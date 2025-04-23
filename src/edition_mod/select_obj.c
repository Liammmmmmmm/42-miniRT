/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   select_obj.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 14:20:04 by madelvin          #+#    #+#             */
/*   Updated: 2025/04/23 10:59:50 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "structs.h"
#include "maths.h"
#include "bvh.h"

t_object	*select_object(t_minirt *minirt, int x, int y)
{
	t_ray 			ray;
	t_hit_record	hit_record;

	ray.orig = minirt->scene.camera.position;
	ray.dir = vec3_subtract(
		vec3_add(
			vec3_add(
				minirt->viewport.pixel00_loc, 
				vec3_multiply_scalar(minirt->viewport.pixel_delta_u, x)
			),
			vec3_multiply_scalar(minirt->viewport.pixel_delta_v, y)
		), 
		ray.orig
	);
	if (hit_register_all(minirt, &ray, &hit_record) == 0)
		return (NULL);
	return (hit_record.obj);
}
void	draw_selected_object(t_minirt *minirt)
{
	t_aabb	box;

	if (minirt->controls.ui_infos.selected_object == NULL || \
		minirt->controls.ui_infos.selected_object->type == NULL_OBJ || \
		minirt->controls.ui_infos.selected_object->type == PLANE)
		return ;
	box = compute_object_bounds(minirt->controls.ui_infos.selected_object);
	draw_box(minirt, box.min, box.max, 0xFFF00FF);
	mlx_put_image_to_window(minirt->mlx.mlx, minirt->mlx.render_win, minirt->mlx.img.img, 0, 0);
}
