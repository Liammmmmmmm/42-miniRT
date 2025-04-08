/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_operation.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 14:14:42 by madelvin          #+#    #+#             */
/*   Updated: 2025/04/08 14:27:39 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "maths.h"

t_color	color_scale(t_color c1, double ratio)
{
	t_color	result;

	if (ratio < 0)
		return ((t_color){0, 0, 0});
	result.r = ft_dmin(255, c1.r * ratio);
	result.g = ft_dmin(255, c1.g * ratio);
	result.b = ft_dmin(255, c1.b * ratio);
	return (result);
}

t_color	color_add(t_color c1, t_color c2)
{
	t_color	result;

	result.r = ft_dmin(255, c1.r + c2.r);
	result.g = ft_dmin(255, c1.g + c2.g);
	result.b = ft_dmin(255, c1.b + c2.b);
	return (result);
}

t_color	color_multiply(t_color c1, t_color c2)
{
	return ((t_color){
		c1.r * c2.r / 255,
		c1.g * c2.g / 255,
		c1.b * c2.b / 255
	});
}
