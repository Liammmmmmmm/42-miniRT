/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   material_reflection.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 15:04:00 by madelvin          #+#    #+#             */
/*   Updated: 2025/03/25 21:57:04 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "minirt.h"
#include "maths.h"

t_vec3  reflection_vec(const t_vec3 uv, const t_vec3 n)
{
	t_vec3	direction;

	direction = vec3_subtract(uv, vec3_multiply_scalar(n, 2 * vec3_dot(uv, n)));
    return (direction);
}

t_vec3  refracted_vec(const t_vec3 uv, const t_vec3 n, double ri)
{
    double  cos_theta;
    t_vec3 r_out_perp;
    t_vec3 r_out_parallel;

    cos_theta = fmin(vec3_dot(vec3_negate(uv), n), 1.0);
    r_out_perp = vec3_multiply_scalar(vec3_add(uv, vec3_multiply_scalar(n, cos_theta)), ri);
    double r_out_perp_length_squared = vec3_length_squared(r_out_perp);
    double parallel_factor = -sqrt(fabs(1.0 - r_out_perp_length_squared));
    r_out_parallel = vec3_multiply_scalar(n, parallel_factor);
    return (vec3_add(r_out_parallel, r_out_perp));
}

double	reflectance(double cosine, double refraction_index)
{
	double	r0;

	r0 = (1 - refraction_index) / (1 + refraction_index);
	r0 = r0 * r0;
	return (r0 + (1 - r0) * pow((1 - cosine), 5));
}

char	mix_mat(t_hit_record hit_record, t_ray ray_in, t_ray *scattered, t_mat material, t_color *attenuation, t_minirt *minirt)
{
	t_vec3	unit_direction;
	double	etai_over_etat;
	double	cos_theta; 
	double	sin_theta;
	t_vec3	direction;

	*attenuation = material.color_value;

	if (material.metallic_value > 0.0)
	{
		direction = reflection_vec(ray_in.dir, hit_record.normal);
		attenuation->r *= material.metallic_value;
		attenuation->g *= material.metallic_value;
		attenuation->b *= material.metallic_value;
	} 
	else if (material.ior > 0)
	{
		unit_direction = vec3_unit(ray_in.dir);
		etai_over_etat = hit_record.front_face ? (minirt->scene.ior_all / material.ior) : material.ior;
		cos_theta = fmin(vec3_dot(vec3_negate(unit_direction), hit_record.normal), 1.0);
		sin_theta = sqrt(1.0 - cos_theta * cos_theta);
		if (etai_over_etat * sin_theta > 1.0 || reflectance(cos_theta, etai_over_etat) > random_double())
			direction = reflection_vec(unit_direction, hit_record.normal);
		else
			direction = refracted_vec(unit_direction, hit_record.normal, etai_over_etat);
	} 
	else
	{
		direction = vec3_add(hit_record.normal, random_vec3_unit());
		if (fabs(direction.x) < 1e-8 && fabs(direction.y) < 1e-8 && fabs(direction.z) < 1e-8)
			direction = hit_record.normal;
	}
	if (material.roughness_value > 0.0)
		direction = vec3_add(vec3_unit(direction), vec3_multiply_scalar(random_vec3_unit(), material.roughness_value));
	scattered->orig = hit_record.point;
	scattered->dir = direction;
	return (1);
}

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

char	calc_ray_reflection(t_hit_record hit_record, t_ray ray_in, t_ray *scatted, t_color *attenuation, t_minirt *minirt)
{
	if (hit_record.mat)
	{
		return (mix_mat(hit_record, ray_in, scatted, *hit_record.mat, attenuation, minirt));
	}
	*attenuation = hit_record.color;
	return (default_texture(hit_record, scatted));
}