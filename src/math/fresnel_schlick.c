/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fresnel_schlick.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 12:30:27 by lilefebv          #+#    #+#             */
/*   Updated: 2025/05/14 10:42:36 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "maths.h"

/**
 * `F = F0​+(1−F0​)(1−cos(θ))^5`
 */
inline t_fcolor	fresnel_schlick_color(double cos_theta, t_fcolor f0)
{
	return (
		add_fcolor(
			f0,
			multiply_scalar_fcolor(
				sub_fcolor((t_fcolor){1.0, 1.0, 1.0}, f0),
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
