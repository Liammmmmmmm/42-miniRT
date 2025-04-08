/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   refraction_math.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 18:55:29 by madelvin          #+#    #+#             */
/*   Updated: 2025/04/08 20:21:31 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "maths.h"
#include <math.h>

t_vec3	refracted_vec(const t_vec3 uv, const t_vec3 n, double ri)
{
	t_vec3	r_out_perp;

	r_out_perp = vec3_multiply_scalar(vec3_add(uv, \
		vec3_multiply_scalar(n, fmin(vec3_dot(vec3_negate(uv), n), 1.0))), ri);
	return (vec3_add(vec3_multiply_scalar(n, \
		-sqrt(fabs(1.0 - vec3_length_squared(r_out_perp)))), r_out_perp));
}

double	reflectance(double cosine, double refraction_index)
{
	double	r0;

	r0 = (1 - refraction_index) / (1 + refraction_index);
	r0 = r0 * r0;
	return (r0 + (1 - r0) * pow((1 - cosine), 5));
}

t_color	fresnel_schlick_blend_color(double cos_theta, double refract_index, \
	t_color c1, t_color c2)
{
	double	r0;

	r0 = (1 - refract_index) / (1 + refract_index);
	r0 = r0 * r0;
	r0 = r0 + (1 - r0) * powf(1 - cos_theta, 5);
	return (
		color_add(color_scale(c1, r0), \
		color_scale(c2, 1.0 - r0))
	);
}
