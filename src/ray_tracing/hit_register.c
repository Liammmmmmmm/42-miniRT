/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_register.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 18:40:21 by madelvin          #+#    #+#             */
/*   Updated: 2025/03/24 11:14:34 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "maths.h"
#include <math.h>

char	hit_register(t_minirt *minirt, t_ray ray, t_hit_record *hit_record, \
	t_color *obj_color)
{
	t_hit_record	temp_hit_record;
	t_sphere		*sphere;
	t_interval		interval;
	double			closest_t = INFINITY;
	int				hit_anything;
	int				i;

	i = 0;
	hit_anything = 0;
	interval.min = 0;
	interval.max = INFINITY;
	while (i < minirt->scene.el_amount)
	{
		if (minirt->scene.elements[i].type == SPHERE)
		{
			sphere = minirt->scene.elements[i].object;
			if (hit_sphere(sphere->position, sphere->diameter / 2, &ray, interval, &temp_hit_record))
			{
				if (temp_hit_record.t < closest_t)
				{
					hit_anything = 1;
					closest_t = temp_hit_record.t;
					*hit_record = temp_hit_record;
					*obj_color = sphere->color;
				}
			}
		}
		i++;
	}
	return (hit_anything);
}
