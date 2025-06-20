/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   select_obj.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 14:20:04 by madelvin          #+#    #+#             */
/*   Updated: 2025/06/20 18:50:19 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "structs.h"
#include "maths.h"
#include "bvh.h"

t_object	*get_selected_custom_obj(t_minirt *minirt, t_object *selected,
	t_hit_register_data data)
{
	int	i;

	i = 0;
	while (i < minirt->scene.el_amount)
	{
		if ((minirt->scene.elements[i].type == CUSTOM) && \
			((t_custom_object *)minirt->scene.elements[i].object) == \
			((t_triangle *)data.hit_record.obj->object)->obj)
		{
			selected = &minirt->scene.elements[i];
			if (selected)
				return (&minirt->scene.elements[i]);
		}
		i++;
	}
	selected = NULL;
	return (NULL);
}

t_object	*select_object(t_minirt *minirt, int x, int y)
{
	t_ray				ray;
	t_hit_register_data	data;
	t_object			*selected;

	selected = NULL;
	x = (int)(x * ((float)minirt->scene.render_width
				/ (float)minirt->scene.win_width) + 0.5f);
	y = (int)(y * ((float)minirt->scene.render_height
				/ (float)minirt->scene.win_height) + 0.5f);
	ray.orig = minirt->scene.camera.position;
	ray.dir = vec3_subtract(vec3_add(vec3_add(minirt->viewport.pixel00_loc,
					vec3_multiply_scalar(minirt->viewport.pixel_delta_u, x)),
				vec3_multiply_scalar(minirt->viewport.pixel_delta_v, y)),
			ray.orig);
	data.hit_record.mat = NULL;
	data.is_light = 1;
	data.ray = &ray;
	if (hit_register_all(minirt, &data) == 0)
		return (NULL);
	if (data.hit_record.obj->type == TRIANGLE
		&& ((t_triangle *)data.hit_record.obj)->obj != NULL)
		if (get_selected_custom_obj(minirt, selected, data))
			return (selected);
	return (data.hit_record.obj);
}

void	draw_selected_object(t_minirt *minirt)
{
	t_aabb	box;

	if (minirt->controls.ui_infos.selected_object == NULL
		|| minirt->controls.ui_infos.selected_object->type == NULL_OBJ
		|| minirt->controls.ui_infos.selected_object->type == PLANE
		|| minirt->controls.ui_infos.selected_object->type == LIGHT
		|| minirt->controls.ui_infos.selected_object->type == DIRECTIONAL_LIGHT
		|| minirt->controls.ui_infos.selected_object->type == NULL_OBJ
	)
		return ;
	box = compute_object_bounds(minirt->controls.ui_infos.selected_object);
	copy_buff_to_image(minirt);
	draw_box(minirt, box.min, box.max, 0xFFF00FF);
	mlx_put_image_to_window(minirt->mlx.mlx, minirt->mlx.render_win,
		minirt->mlx.img.img, 0, 0);
}
