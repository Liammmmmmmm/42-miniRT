/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   material_reflection.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 15:04:00 by madelvin          #+#    #+#             */
/*   Updated: 2025/03/25 15:47:02 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "minirt.h"
#include "maths.h"

// char	apply_material(t_hit_record hit_record, t_ray ray_in, t_ray *scattered, t_mat material)
// {
// 	t_vec3	reflected_dir;
// 	t_vec3	diffuse_dir;
// 	t_vec3	transmitted_dir;
// 	t_vec3	mixed_dir;

// 	/* --- Réflexion métallique --- */
// 	reflected_dir = vec3_subtract(ray_in.dir, 
// 		vec3_multiply_scalar(hit_record.normal, 2 * vec3_dot(ray_in.dir, hit_record.normal)));
// 	reflected_dir = vec3_add(reflected_dir, 
// 		vec3_multiply_scalar(random_vec3_unit(), material.roughness_value)); // Ajoute de la rugosité

// 	/* --- Diffusion classique --- */
// 	diffuse_dir = vec3_add(hit_record.normal, random_vec3_unit());
// 	if (fabs(diffuse_dir.x) < 1e-8 && fabs(diffuse_dir.y) < 1e-8 && fabs(diffuse_dir.z) < 1e-8)
// 		diffuse_dir = hit_record.normal;

// 	/* --- Transmission (Réfraction) --- */
// 	if (material.transmission > 0.0)
// 	{
// 		double	etai = 1.0; // Indice de réfraction externe (air)
// 		double	etat = material.ior; // Indice de réfraction interne (matériau)
// 		if (vec3_dot(ray_in.dir, hit_record.normal) > 0.0) // Ray sortant
// 		{
// 			double tmp = etai;
// 			etai = etat;
// 			etat = tmp;
// 			hit_record.normal = vec3_multiply_scalar(hit_record.normal, -1);
// 		}
// 		double eta = etai / etat;
// 		double cos_theta = -vec3_dot(ray_in.dir, hit_record.normal);
// 		double sin_theta2 = eta * eta * (1.0 - cos_theta * cos_theta);

// 		if (sin_theta2 > 1.0) // Réflexion totale interne
// 			transmitted_dir = reflected_dir;
// 		else
// 		{
// 			t_vec3	refracted_perp = vec3_multiply_scalar(vec3_add(ray_in.dir, 
// 				vec3_multiply_scalar(hit_record.normal, cos_theta)), eta);
// 			t_vec3	refracted_parallel = vec3_multiply_scalar(hit_record.normal, 
// 				-sqrt(fabs(1.0 - vec3_dot(refracted_perp, refracted_perp))));
// 			transmitted_dir = vec3_add(refracted_perp, refracted_parallel);
// 		}
// 	}
// 	else
// 		transmitted_dir = reflected_dir; // Si pas de transmission, utiliser réflexion

// 	/* --- Mélange des directions --- */
// 	mixed_dir = vec3_add(
// 		vec3_multiply_scalar(reflected_dir, material.metallic_value),    // Pondération métallique
// 		vec3_multiply_scalar(diffuse_dir, (1.0 - material.metallic_value) * (1.0 - material.transmission)) // Pondération diffuse
// 	);
// 	mixed_dir = vec3_add(mixed_dir, vec3_multiply_scalar(transmitted_dir, material.transmission)); // Ajoute la transmission
// 	mixed_dir = vec3_unit(mixed_dir); // Normalisation

// 	/* --- Mise à jour du rayon diffusé --- */
// 	scattered->dir = mixed_dir;
// 	scattered->orig = hit_record.point;

// 	/* Retourne 1 si le rayon sort de la surface */
// 	return (vec3_dot(scattered->dir, hit_record.normal) > 0);
// }


char	default_texture(t_hit_record hit_record, t_ray *scatted)
{
	t_vec3	direction;

	direction = vec3_add(hit_record.normal, random_vec3_unit());
	if (fabs(direction.x) < 1e-8 && fabs(direction.y) < 1e-8 && fabs(direction.z) < 1e-8)
		direction = hit_record.normal;
	scatted->dir = direction;
	scatted->orig = hit_record.point;
	return (1);
}

char	metal_texture(t_hit_record hit_record, t_ray ray_in, t_ray *scatted)
{
	t_vec3	direction;

	/*------ Reflection ------*/
	direction = vec3_subtract(ray_in.dir, vec3_multiply_scalar(hit_record.normal, 2 * vec3_dot(ray_in.dir, hit_record.normal)));
	direction = vec3_add(vec3_unit(direction), vec3_multiply_scalar(random_vec3_unit(), hit_record.mat->roughness_value)); // TODO un jour on devra mettre roughness texture et la on va plus en chier x)
	scatted->dir = direction;
	scatted->orig = hit_record.point;
	return (vec3_dot(scatted->dir, hit_record.normal) > 0);
}

t_vec3  refracted_vec(const t_vec3 uv, const t_vec3 n, double etai_over_etat)
{
    double  cos_theta;
    t_vec3 r_out_perp;
    t_vec3 r_out_parallel;

    cos_theta = fmin(vec3_dot(vec3_negate(uv), n), 1.0);
    r_out_perp = vec3_multiply_scalar(vec3_add(uv, vec3_multiply_scalar(n, cos_theta)), etai_over_etat);
    double r_out_perp_length_squared = vec3_length_squared(r_out_perp);
    double parallel_factor = -sqrt(fabs(1.0 - r_out_perp_length_squared));
    r_out_parallel = vec3_multiply_scalar(n, parallel_factor);
    return (vec3_add(r_out_parallel, r_out_perp));
}

char dielectric_texture(t_hit_record hit_record, t_ray ray_in, t_ray *scattered, double ior, t_color *attenuation)
{
    t_vec3 unit_direction;
    t_vec3 refracted;
    double etai_over_etat;

    *attenuation = (t_color){255, 255, 255};
    etai_over_etat = hit_record.front_face ? (1.0 / ior) : ior;

    unit_direction = vec3_unit(ray_in.dir);
    refracted = refracted_vec(unit_direction, hit_record.normal, etai_over_etat);

    scattered->dir = refracted;
    scattered->orig = hit_record.point;
    return (1);
}


char	calc_ray_reflection(t_hit_record hit_record, t_ray ray_in, t_ray *scatted, t_color *attenuation)
{
	if (hit_record.mat)
	{
		*attenuation = hit_record.mat->color_value;
		if (hit_record.mat->metallic_value != 0)
		{
			return (metal_texture(hit_record, ray_in, scatted));
		}
		if (hit_record.mat->ior != 0)
		{
			return (dielectric_texture(hit_record, ray_in, scatted, hit_record.mat->ior, attenuation));
		}
	}
	*attenuation = hit_record.color;
	return (default_texture(hit_record, scatted));
}