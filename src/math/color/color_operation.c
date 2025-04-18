/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_operation.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 14:14:42 by madelvin          #+#    #+#             */
/*   Updated: 2025/04/17 15:49:05 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "maths.h"

t_color	color_scale(t_color c1, double ratio)
{
	t_color	result;

	if (ratio < 0)
		return ((t_color){0, 0, 0});
	result.r = c1.r * ratio;
	result.g = c1.g * ratio;
	result.b = c1.b * ratio;
	return (result);
}

t_color	color_add(t_color c1, t_color c2)
{
	t_color	result;

	result.r = c1.r + c2.r;
	result.g = c1.g + c2.g;
	result.b = c1.b + c2.b;
	return (result);
}

t_color	color_sub(t_color c1, t_color c2)
{
	return ((t_color){
		.r = c1.r - c2.r,
		.g = c1.g - c2.g,
		.b = c1.b - c2.b
	});
}

t_color	color_multiply(t_color c1, t_color c2)
{
	return ((t_color){
		.r = (uint16_t)c1.r * c2.r / 255,
		.g = (uint16_t)c1.g * c2.g / 255,
		.b = (uint16_t)c1.b * c2.b / 255
	});
}
