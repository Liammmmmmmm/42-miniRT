/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 16:58:01 by madelvin          #+#    #+#             */
/*   Updated: 2025/04/25 16:59:27 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "maths.h"

static inline unsigned char	clamp(uint16_t c)
{
	if (c > 255)
		return (255);
	return (c);
}

t_color	color_add_clamp(t_color c1, t_color c2)
{
	t_color	result;

	result.r = clamp((uint16_t)c1.r + c2.r);
	result.g = clamp((uint16_t)c1.g + c2.g);
	result.b = clamp((uint16_t)c1.b + c2.b);
	return (result);
}

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
