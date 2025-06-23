/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clamp_double.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 15:49:44 by madelvin          #+#    #+#             */
/*   Updated: 2025/06/23 13:03:29 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "maths.h"

inline double	clamp_double_val(double v, double min, double max)
{
	if (v <= min)
		return (min);
	else if (v >= max)
		return (max);
	return (v);
}

inline float	clampf(float x, float min, float max)
{
	if (x < min)
		return (min);
	if (x > max)
		return (max);
	return (x);
}
