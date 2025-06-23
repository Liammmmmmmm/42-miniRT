/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   material_default.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 18:27:14 by madelvin          #+#    #+#             */
/*   Updated: 2025/06/23 19:14:39 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "minirt.h"
#include "material.h"
#include "importance_sampling.h"

static inline t_vec3	cos_weighted_sample_hemishphere(t_vec3 *normal,
	uint64_t *rand)
{
	const double	r1 = random_double(rand);
	const double	r2 = random_double(rand);
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
		= calc_inverse_transform_sampling_uv(&minirt->scene, &minirt->rand);
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

inline void	default_mat(t_minirt *minirt, t_ray *ray, t_hit_record *hit_record,
	t_ray_data data)
{
	*data.accumulation = add_fcolor(
			*data.accumulation,
			multiply_fcolor(
				multiply_fcolor(
					hit_record->color,
					*data.power
					),
				compute_light_v2(hit_record, minirt)
				)
			);
	*data.power = multiply_fcolor(
			*data.power,
			hit_record->color
			);
	if (minirt->scene.amb_light.pdf_joint)
		importance_sampling(minirt, ray, hit_record, data);
	ray->dir
		= cos_weighted_sample_hemishphere(&hit_record->normal, &minirt->rand);
}
