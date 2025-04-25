/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_operation.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 14:14:42 by madelvin          #+#    #+#             */
/*   Updated: 2025/04/25 16:59:21 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "maths.h"

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
