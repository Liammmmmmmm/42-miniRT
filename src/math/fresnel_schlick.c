/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fresnel_schlick.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 12:30:27 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/16 12:41:33 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "maths.h"

/**
 * `F = F0​+(1−F0​)(1−cos(θ))^5`
 */
inline t_color	fresnel_schlick_color(double cos_theta, t_color f0)
{
	return (
		color_add(
			f0,
			color_scale(
				color_sub((t_color){255, 255, 255}, f0),
				powf(1 - cos_theta, 5)
			)
		)
	);
}

/**
 * `F = F0​+(1−F0​)(1−cos(θ))^5`
 */
inline double	fresnel_schlick_f(double cos_theta, double f0)
{
	return (f0 + ((1 - f0) * powf(1 - cos_theta, 5)));
}
