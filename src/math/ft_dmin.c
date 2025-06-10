/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dmin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 14:15:05 by madelvin          #+#    #+#             */
/*   Updated: 2025/06/10 18:45:35 by madelvin         ###   ########.fr       */
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

t_vec3	vec3_min(t_vec3 a, t_vec3 b)
{
	return ((t_vec3){
		ft_dmin(a.x, b.x),
		ft_dmin(a.y, b.y),
		ft_dmin(a.z, b.z)
	});
}

t_vec3	vec3_max(t_vec3 a, t_vec3 b)
{
	return ((t_vec3){
		ft_dmax(a.x, b.x),
		ft_dmax(a.y, b.y),
		ft_dmax(a.z, b.z)
	});
}
