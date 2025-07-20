/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   material_default.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 18:27:14 by madelvin          #+#    #+#             */
/*   Updated: 2025/07/20 14:36:25 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "minirt.h"
#include "material.h"
#include "bvh.h"
#include "importance_sampling.h"

static inline t_vec3	cos_weighted_sample_hemishphere(t_vec3 *normal)
{
	const double	r1 = random_double();
	const double	r2 = random_double();
	const double	phi = 2 * PI_D * r1;
	t_vec3			local_dir;
	t_vec3			u;

	local_dir.x = cos(phi) * sqrt(r2);
	local_dir.y = sqrt(1 - r2);
	local_dir.z = sin(phi) * sqrt(r2);
	*normal = vec3_unit(*normal);
	if (fabs(normal->y) < 0.999)
		u = vec3_unit(vec3_cross((t_vec3){0, 1, 0}, *normal));
	else
		u = vec3_unit(vec3_cross((t_vec3){1, 0, 0}, *normal));
	return (vec3_unit(vec3_add(vec3_add(vec3_multiply_scalar(u, local_dir.x),
					vec3_multiply_scalar(*normal, local_dir.y)),
				vec3_multiply_scalar(vec3_cross(*normal, u), local_dir.z))));
}

static inline void	importance_sampling(t_minirt *minirt, t_ray *ray,
	t_hit_record *hit_record, t_ray_data data)
{
	t_hit_register_data	data_tmp;
	const t_vec2		uv
		= calc_inverse_transform_sampling_uv(&minirt->scene);
	float				costheta;
	float				pdf;
	t_fcolor			radiance;

	data_tmp.is_light = 1;
	data_tmp.ray = ray;
	ray->dir = calc_inverse_transform_sampling_dir(&uv);
	if (hit_register_all(minirt, &data_tmp))
		return ;
	costheta = vec3_dot(hit_record->normal, ray->dir);
	pdf = minirt->scene.amb_light.pdf_joint[(int)(uv.y * (minirt->scene.\
amb_light.skybox_t->hdr.height - 1) * minirt->scene.amb_light.skybox_t->hdr.\
width + uv.x * (minirt->scene.amb_light.skybox_t->hdr.width - 1))];
	if (pdf <= 0)
		return ;
	radiance = get_background_color(minirt, *ray);
	if (hit_record->mat)
		radiance = multiply_scalar_fcolor(radiance, hit_record->mat->ao_value);
	radiance = multiply_fcolor(radiance, *data.power);
	radiance = multiply_scalar_fcolor(radiance, costheta / (pdf));
	*data.accumulation = add_fcolor(*data.accumulation, radiance);
	*data.power = multiply_scalar_fcolor(*data.power, 1 - costheta);
}

static float vec3_distance_sq(t_vec3 v1, t_vec3 v2)
{
    float dx = v1.x - v2.x; float dy = v1.y - v2.y;
    float dz = v1.z - v2.z;

    return (dx * dx + dy * dy + dz * dz);
}

typedef struct s_found_photon
{
    t_photon* photon;
    float dist_sq;
} t_found_photon;

typedef struct s_search_data
{
    t_vec3* target_point;
    t_found_photon* found_list;
    int max_photons;
    int photons_found;
    float max_dist_sq;
} t_search_data;

static void insert_into_found_list(t_search_data* data, t_photon* photon,
    float dist_sq)
{
    int i;
    if (data->photons_found < data->max_photons)
    {
        i = data->photons_found - 1;
        while (i >= 0 && data->found_list[i].dist_sq > dist_sq)
        {
            data->found_list[i + 1] = data->found_list[i];
            i--;
        }
        data->found_list[i + 1] = (t_found_photon){photon, dist_sq};
        data->photons_found++;
        data->max_dist_sq = data->found_list[data->photons_found - 1].dist_sq;
    } else if (dist_sq < data->max_dist_sq)
    {
        i = data->max_photons - 2;
        while (i >= 0 && data->found_list[i].dist_sq > dist_sq)
        {
            data->found_list[i + 1] = data->found_list[i];
            i--;
        }
        data->found_list[i + 1] = (t_found_photon){photon, dist_sq};
        data->max_dist_sq = data->found_list[data->max_photons - 1].dist_sq;
    }
}

static void search_recursive(t_kd_node* node, t_search_data* data) {
    if (!node)
        return;
    float dist_sq_to_node = vec3_distance_sq(node->photon->position, *data->target_point);
    insert_into_found_list(data, node->photon, dist_sq_to_node);
    float dist_to_plane = get_axis_value(*data->target_point, node->axis) - get_axis_value(node->photon->position, node->axis);
    t_kd_node* near_child = (dist_to_plane < 0) ? node->left : node->right;
    t_kd_node* far_child = (dist_to_plane < 0) ? node->right : node->left;
    search_recursive(near_child, data);
    if ((dist_to_plane * dist_to_plane) < data->max_dist_sq)
        search_recursive(far_child, data);
}

t_fcolor estimate_caustics_physically_based(t_kd_tree *caustic_kdtree, t_vec3 point, int k_nearest)
{
    t_found_photon  *found_list;
    t_search_data   search_data;
    t_fcolor        accumulated_flux;
    float           area;
    int             i;

    if (!caustic_kdtree || !caustic_kdtree->root || k_nearest <= 0)
        return ((t_fcolor){0, 0, 0});

    found_list = malloc(sizeof(t_found_photon) * k_nearest);
    if (!found_list)
        return ((t_fcolor){0, 0, 0});

    search_data = (t_search_data){&point, found_list, k_nearest, 0, 1e30f};
    search_recursive(caustic_kdtree->root, &search_data);

    if (search_data.photons_found == 0) {
        free(found_list);
        return ((t_fcolor){0, 0, 0});
    }

    accumulated_flux = (t_fcolor){0, 0, 0};
    i = 0;
    while (i < search_data.photons_found) {
        accumulated_flux = add_fcolor(accumulated_flux, found_list[i].photon->power);
        i++;
    }

    area = M_PI * search_data.max_dist_sq;
    free(found_list);
    if (area > 1e-9f)
        return (multiply_scalar_fcolor(accumulated_flux, 1.0f / area));
    return ((t_fcolor){0, 0, 0});
}

inline void default_mat(t_minirt *minirt, t_ray *ray, t_hit_record *hit_record,
    t_ray_data data)
{
    t_fcolor    direct_lighting;
    t_fcolor    caustics_lighting;
    t_fcolor    total_lighting;

    direct_lighting = compute_light_v2(hit_record, minirt);
    if (minirt->scene.photon_map.root)
        caustics_lighting = estimate_caustics_physically_based(&minirt->scene.photon_map,
            hit_record->point, 150);
    else
        caustics_lighting = (t_fcolor){0.0, 0.0, 0.0};
    total_lighting = add_fcolor(direct_lighting, caustics_lighting);
    *data.accumulation = add_fcolor(*data.accumulation,
        multiply_fcolor(multiply_fcolor(hit_record->color, *data.power), total_lighting));
    *data.power = multiply_fcolor(*data.power, hit_record->color);
    if (minirt->scene.amb_light.pdf_joint)
        importance_sampling(minirt, ray, hit_record, data);
    ray->dir = cos_weighted_sample_hemishphere(&hit_record->normal);
}

inline char default_mat_photon(t_hit_record *hit_record, t_fcolor *power)
{
    *power = multiply_fcolor(*power, hit_record->color);
    return (1);
}
