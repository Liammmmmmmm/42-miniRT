/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   material_default.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 18:27:14 by madelvin          #+#    #+#             */
/*   Updated: 2025/07/01 13:56:26 by madelvin         ###   ########.fr       */
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
	const t_vec2		uv = calc_inverse_transform_sampling_uv(&minirt->scene);
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






float   vec3_distance_sq(t_vec3 v1, t_vec3 v2)
{
    float   dx;
    float   dy;
    float   dz;

    dx = v1.x - v2.x;
    dy = v1.y - v2.y;
    dz = v1.z - v2.z;
    
    return (dx * dx + dy * dy + dz * dz);
}












// Structure pour stocker un photon trouvé et sa distance (au carré)
typedef struct s_found_photon {
    t_photon* photon;
    float       dist_sq; // On stocke la distance au carré pour éviter les sqrt()
} t_found_photon;

// Structure pour gérer la recherche et ses résultats
typedef struct s_search_data {
    const t_vec3* target_point;   // Le point autour duquel on cherche
    t_found_photon* found_list;     // La liste des N meilleurs photons trouvés
    int             max_photons;    // N, le nombre de photons à trouver
    int             photons_found;  // Le nombre de photons actuellement dans la liste
    float           max_dist_sq;    // La distance (au carré) du photon le plus éloigné
} t_search_data;



static void insert_into_found_list(t_search_data* data, t_photon* photon, float dist_sq)
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
    }
    else if (dist_sq < data->max_dist_sq)
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

static void search_recursive(t_kd_node* node, t_search_data* data)
{
    float       dist_sq_to_node;
    float       dist_to_plane;
    t_kd_node* near_child;
    t_kd_node* far_child;

    if (!node)
        return;

    dist_sq_to_node = vec3_distance_sq(node->photon->position, *data->target_point);
    insert_into_found_list(data, node->photon, dist_sq_to_node);

    dist_to_plane = get_axis_value(*data->target_point, node->axis) - get_axis_value(node->photon->position, node->axis);
    
    if (dist_to_plane < 0) {
        near_child = node->left;
        far_child = node->right;
    } else {
        near_child = node->right;
        far_child = node->left;
    }

    search_recursive(near_child, data);

    if ((dist_to_plane * dist_to_plane) < data->max_dist_sq)
    {
        search_recursive(far_child, data);
    }
}

int find_n_nearest_photons(t_kd_tree *kdtree, t_vec3 point,
    int num_photons_to_find, t_photon** nearest_photons, float* search_radius_sq)
{
    t_found_photon  found_list[num_photons_to_find];
    t_search_data   search_data;

    if (!kdtree || !kdtree->root)
    {
        *search_radius_sq = 0.0f;
        return (0); // Retourne 0 si pas de photons
    }

    // Initialisation de la structure de recherche
    search_data.target_point = &point;
    search_data.found_list = found_list;
    search_data.max_photons = num_photons_to_find;
    search_data.photons_found = 0;
    search_data.max_dist_sq = 1e30f; 

    // Lancement de la recherche récursive
    search_recursive(kdtree->root, &search_data);

    // Remplissage du tableau de sortie
    for (int i = 0; i < search_data.photons_found; i++) {
        nearest_photons[i] = search_data.found_list[i].photon;
    }
    
    if (search_data.photons_found > 0) {
        *search_radius_sq = search_data.max_dist_sq;
    } else {
        *search_radius_sq = 0.0f;
    }
    
    // CORRECTION: On retourne le nombre de photons qui ont été trouvés.
    return (search_data.photons_found);
}

#define NUM_CAUSTIC_PHOTONS 50
#define MAX_CAUSTIC_RADIUS 0.05f

t_fcolor    estimate_caustics_from_map(t_kd_tree *caustic_kdtree, t_vec3 point)
{
    t_photon    *nearest_photons[NUM_CAUSTIC_PHOTONS];
    float       search_radius_sq;
    float       search_radius;
    t_fcolor    accumulated_power;
    float       area;
    int         i;
    int         photons_found;

    if (!caustic_kdtree || !caustic_kdtree->root)
        return ((t_fcolor){0.0f, 0.0f, 0.0f});

    photons_found = find_n_nearest_photons(caustic_kdtree, point,
            NUM_CAUSTIC_PHOTONS, nearest_photons, &search_radius_sq);

    if (photons_found == 0)
        return ((t_fcolor){0.0f, 0.0f, 0.0f});

    search_radius = sqrtf(search_radius_sq);

    // CORRECTION : On vérifie si le rayon de recherche est trop grand.
    // Si c'est le cas, cela signifie que les photons sont trop dispersés
    // pour former un caustic net. On ignore alors leur contribution.
    if (search_radius > MAX_CAUSTIC_RADIUS)
        return ((t_fcolor){0.0f, 0.0f, 0.0f});

    accumulated_power = (t_fcolor){0.0f, 0.0f, 0.0f};
    i = 0;
    while (i < photons_found)
    {
        accumulated_power = add_fcolor(accumulated_power, nearest_photons[i]->power);
        i++;
    }

    // On utilise le rayon réel de la recherche pour la normalisation
    area = M_PI * search_radius_sq;
    if (area > 1e-6f)
    {
        accumulated_power.r /= area;
        accumulated_power.g /= area;
        accumulated_power.b /= area;
    }

    return (accumulated_power);
}


inline void default_mat(t_minirt *minirt, t_ray *ray, t_hit_record *hit_record,
    t_ray_data data)
{
    t_fcolor    direct_lighting;
    t_fcolor    caustics_lighting;
    t_fcolor    total_lighting;

    // --- 1. ÉCLAIRAGE DIRECT (Votre code existant, inchangé) ---
    direct_lighting = compute_light_v2(hit_record, minirt);

    // --- 2. NOUVELLE ÉTAPE : AJOUT DES CAUSTICS ---
    // On estime la contribution des caustics en interrogeant la carte de photons dédiée.
    // On ne fait cela que si une carte de caustics a été construite.
    if (1) // Supposons que vous ayez un pointeur vers cet arbre
    {
        caustics_lighting = estimate_caustics_from_map(&minirt->scene.photon_map,
            hit_record->point);
    }
    else
    {
        caustics_lighting = (t_fcolor){0.0, 0.0, 0.0};
    }

    // --- 3. On combine les deux sources de lumière ---
    total_lighting = add_fcolor(direct_lighting, caustics_lighting);

    // On ajoute cette contribution lumineuse à l'accumulation finale.
    *data.accumulation = add_fcolor(
            *data.accumulation,
            multiply_fcolor(
                multiply_fcolor(
                    hit_record->color, // La couleur de l'objet (albedo)
                    *data.power      // L'énergie actuelle du rayon
                ),
                total_lighting     // La lumière directe + les caustics
            )
        );

    // --- 4. ÉCLAIRAGE INDIRECT (Votre code existant, inchangé) ---
    // On continue le trajet du rayon pour calculer le reste de l'éclairage
    // indirect via votre path tracer et votre importance sampling.
    *data.power = multiply_fcolor(
            *data.power,
            hit_record->color
            );
    if (minirt->scene.amb_light.pdf_joint)
        importance_sampling(minirt, ray, hit_record, data);
    ray->dir = cos_weighted_sample_hemishphere(&hit_record->normal);
}

inline char	default_mat_photon(t_hit_record *hit_record, t_fcolor *power)
{
	*power = multiply_fcolor(
			*power,
			hit_record->color
			);
	return (1);
}

