/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dmin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 14:15:05 by madelvin          #+#    #+#             */
/*   Updated: 2025/04/20 17:24:14 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "maths.h"
#include <math.h>

inline double	ft_dmin(double a, double b)
{
	if (a < b)
		return (a);
	return (b);
}

inline double	ft_dmax(double a, double b)
{
	if (a > b)
		return (a);
	return (b);
}

t_vec3 vec3_abs(t_vec3 v)
{
	return (t_vec3){
		fabs(v.x),
		fabs(v.y),
		fabs(v.z)
	};
}

t_vec3 vec3_min(t_vec3 a, t_vec3 b)
{
	return (t_vec3){
		ft_dmin(a.x, b.x),
		ft_dmin(a.y, b.y),
		ft_dmin(a.z, b.z)
	};
}

t_vec3 vec3_max(t_vec3 a, t_vec3 b)
{
	return (t_vec3){
		ft_dmax(a.x, b.x),
		ft_dmax(a.y, b.y),
		ft_dmax(a.z, b.z)
	};
}
