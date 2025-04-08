/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   refraction_color.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 18:26:47 by madelvin          #+#    #+#             */
/*   Updated: 2025/04/08 20:19:44 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "minirt.h"
#include "maths.h"
#include "material.h"
#include <math.h>

static inline double	compute_eta(t_mat_manager *mat_man)
{
	if (mat_man->hit_record.front_face)
		return (mat_man->minirt->scene.ior_all / mat_man->hit_record.mat->ior);
	else
		return (mat_man->hit_record.mat->ior);
}

static inline void	compute_angles(t_vec3 unit_direction, t_vec3 normal, \
	double *cos_theta, double *sin_theta)
{
	*cos_theta = fmin(vec3_dot(vec3_negate(unit_direction), normal), 1.0);
	*sin_theta = sqrt(1.0 - (*cos_theta) * (*cos_theta));
}

static inline char	should_reflect(double etai_over_etat, double sin_theta, \
	double cos_theta)
{
	return (etai_over_etat * sin_theta > 1.0 || \
		reflectance(cos_theta, etai_over_etat) > random_double());
}

t_color	refracted_color(t_mat_manager *mat_man)
{
	t_vec3		unit_direction;
	double		etai_over_etat;
	double		cos_theta;
	double		sin_theta;
	t_color		bounce_color;

	unit_direction = vec3_unit(mat_man->ray_in.dir);
	etai_over_etat = compute_eta(mat_man);
	compute_angles(unit_direction, mat_man->hit_record.normal, \
		&cos_theta, &sin_theta);
	if (should_reflect(etai_over_etat, sin_theta, cos_theta))
		mat_man->ray_in.dir = reflection_vec(unit_direction, \
			mat_man->hit_record.normal);
	else
		mat_man->ray_in.dir = refracted_vec(unit_direction, \
			mat_man->hit_record.normal, etai_over_etat);
	if (mat_man->hit_record.mat->roughness_value > 0.0)
		mat_man->ray_in.dir = vec3_add(vec3_unit(mat_man->ray_in.orig), \
			vec3_multiply_scalar(vec3_random_unit(), \
			mat_man->hit_record.mat->roughness_value));
	mat_man->ray_in.orig = mat_man->hit_record.point;
	bounce_color = ray_color(mat_man->minirt, mat_man->ray_in, \
		mat_man->depth - 1, NULL);
	return (fresnel_schlick_blend_color(cos_theta, etai_over_etat, \
								mat_man->color, bounce_color));
}
