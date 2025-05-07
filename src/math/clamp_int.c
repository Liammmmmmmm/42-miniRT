/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clamp_int.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 13:44:30 by lilefebv          #+#    #+#             */
/*   Updated: 2025/05/07 10:02:11 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "maths.h"

int	imax(int i1, int i2)
{
	if (i1 > i2)
		return (i1);
	return (i2);
}

int	imin(int i1, int i2)
{
	if (i1 < i2)
		return (i1);
	return (i2);
}

int	iclamp(int min, int val, int max)
{
	if (min > val)
		return (min);
	else if (max < val)
		return (max);
	return (val);
}

inline double	clamp_double(double v)
{
	if (v < 0)
		return (0);
	else if (v > 1)
		return (1);
	return (v);
}
