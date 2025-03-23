/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_register.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 18:40:21 by madelvin          #+#    #+#             */
/*   Updated: 2025/03/23 18:53:44 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "maths.h"
#include <math.h>

char	hit_register(t_minirt *minirt, t_ray ray, t_hit_record *hit_record, \
	t_color *obj_color)
{
	t_sphere	*sphere;
	t_interval	interval;
	int			i;

	i = 0;
	interval.min = 0;
	interval.max = INFINITY;
	while (i < minirt->scene.el_amount)
	{
		if (minirt->scene.elements[i].type == SPHERE)
		{
			sphere = minirt->scene.elements[i].object;
			if (hit_sphere(sphere->position, sphere->diameter / 2, &ray, interval, hit_record))
			{
				*obj_color = sphere->color;
				return (1);
			}
		}
		i++;
	}
	return (0);
}
