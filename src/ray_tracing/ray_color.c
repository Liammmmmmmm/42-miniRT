/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_color.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 17:36:48 by madelvin          #+#    #+#             */
/*   Updated: 2025/04/27 18:01:03 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_ray_data	ray_color(t_minirt *minirt, t_ray ray, int depth, char *hit)
{
	t_color			color;
	t_ray_data		ray_data;
	t_hit_record	hit_record;

	if (depth <= 0)
		return ((t_ray_data){(t_color){0, 0, 0}, DEFFAULT});
	if (hit_register_all(minirt, &ray, &hit_record) == 1)
	{
		color = hit_record.color;
		color = color_multiply(color, compute_light(&hit_record, minirt));
		ray_data = material_manager((t_mat_manager){hit_record, ray, minirt,
				color, depth});
		if (hit)
			*hit = 1;
		return (ray_data);
	}
	if (hit)
		*hit = 0;
	return ((t_ray_data){get_background_color(minirt, ray), DEFFAULT});
}
