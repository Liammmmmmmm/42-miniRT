/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_register.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 18:40:21 by madelvin          #+#    #+#             */
/*   Updated: 2025/04/14 12:51:08 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_color	get_hit_register_color(t_mat *mat, t_color color, t_hit_record *hit)
{
	if (mat)
	{
		if (mat->color_tex)
		{
			if (!mat->color_tex->img.pixel_data || !mat->color_tex->img.width || !mat->color_tex->img.height)
				return (get_solid_texture(hit->point, 2));
			else
			{
				uint32_t c = mat->color_tex->img.pixel_data[mat->color_tex->img.width * (int)(hit->v * mat->color_tex->img.height) + (int)(hit->u * mat->color_tex->img.width)]; // peut etre mettre un -1 mais pas sur du tout
				return ((t_color){(c >> 16) & 0xFF, (c >> 8) & 0xFF, c & 0xFF});
			}
		}
		else
			return (mat->color_value);
	}
	return (color);
}

char	hit_register(t_minirt *minirt, t_ray ray, t_hit_record *hit_record)
{
	t_hit_record	temp_hit_record;
	t_sphere		*sphere;
	t_plane			*plane;
	t_interval		interval;
	double			closest_t;
	int				hit_anything;
	int				i;

	i = 0;
	hit_anything = 0;
	interval.min = 0.001;
	interval.max = 1000;
	closest_t = INFINITY;
	while (i < minirt->scene.el_amount)
	{
		if (minirt->scene.elements[i].type == SPHERE)
		{
			sphere = minirt->scene.elements[i].object;
			if (hit_sphere(*sphere, &ray, interval, &temp_hit_record))
			{
				if (temp_hit_record.t < closest_t)
				{
					hit_anything = 1;
					closest_t = temp_hit_record.t;
					*hit_record = temp_hit_record;
					hit_record->mat = sphere->material;
					hit_record->color = get_hit_register_color(sphere->material, sphere->color, hit_record);
				}
			}
		}
		if (minirt->scene.elements[i].type == PLANE)
		{
			plane = minirt->scene.elements[i].object;
			if (hit_plane(plane->position, plane->normal, &ray, interval, &temp_hit_record))
			{
				if (temp_hit_record.t < closest_t)
				{
					hit_anything = 1;
					closest_t = temp_hit_record.t;
					*hit_record = temp_hit_record;
					hit_record->mat = plane->material;
					hit_record->color = get_hit_register_color(plane->material, plane->color, hit_record);
				}
			}
		}
		i++;
	}
	return (hit_anything);
}
