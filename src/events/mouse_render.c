/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_render.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 18:42:48 by lilefebv          #+#    #+#             */
/*   Updated: 2025/07/22 19:40:04 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "camera.h"

static void	select_object_lft(t_minirt *minirt)
{
	char	item_null;

	item_null = (minirt->controls.ui_infos.selected_object == NULL);
	minirt->controls.ui_infos.selected_object = select_object(minirt,
			minirt->controls.mlxr, minirt->controls.mlyr);
	if ((minirt->controls.ui_infos.selected_object == NULL
			|| minirt->controls.ui_infos.selected_object->type == PLANE
			|| minirt->controls.ui_infos.selected_object->type == LIGHT)
		&& item_null == 0)
	{
		copy_buff_to_image(minirt);
		mlx_put_image_to_window(minirt->mlx.mlx, minirt->mlx.render_win,
			minirt->mlx.img.img, 0, 0);
	}
	set_selected_object(minirt, minirt->controls.ui_infos.selected_object);
}

int	mousedown_render(int key, int x, int y, t_minirt *minirt)
{
	minirt->controls.mlxr = (t_uint)x;
	minirt->controls.mlyr = (t_uint)y;
	mousedown_common(key, minirt);
	if (key == LEFT_CLICK && minirt->scene.build_bvh == 0)
		select_object_lft(minirt);
	else if (key == RIGHT_CLICK)
	{
		minirt->controls.selected_x = x;
		minirt->controls.selected_y = y;
		printf("new selected : %d %d\n", minirt->controls.selected_x,
			minirt->controls.selected_y);
	}
	else if (key == SCROLL_DOWN)
	{
		minirt->controls.values.fov -= 1;
		if (minirt->controls.values.fov < 1)
			minirt->controls.values.fov = 1;
		restart_minirt(minirt);
	}
	else if (key == SCROLL_UP)
	{
		minirt->controls.values.fov += 1;
		if (minirt->controls.values.fov > 180)
			minirt->controls.values.fov = 180;
		restart_minirt(minirt);
	}
	return (0);
}

int	mouseup_render(int key, int x, int y, t_minirt *minirt)
{
	minirt->controls.mlxr = (t_uint)x;
	minirt->controls.mlyr = (t_uint)y;
	minirt->controls.movements.mouse = 0;
	mouseup_common(key, minirt);
	return (0);
}

#define MOUSE_SENSITIVITY 0.005

void	mouse_move_cam(int x, int y, t_minirt *minirt, t_vec2 delta)
{
	t_vec3	right;
	t_vec3	up;
	t_vec3	world_up;

	if (delta.x != 0 || delta.y != 0)
		minirt->controls.movements.mouse = 1;	
	world_up = (t_vec3){0, 1, 0};
	right = vec3_unit(vec3_cross(minirt->scene.camera.orientation, world_up));
	up = vec3_unit(vec3_cross(right, minirt->scene.camera.orientation));
	minirt->scene.camera.orientation = vec3_rotate(minirt->scene.camera.orientation, up, -delta.x);
	minirt->scene.camera.orientation = vec3_rotate(minirt->scene.camera.orientation, right, -delta.y);
	minirt->scene.camera.orientation = vec3_unit(minirt->scene.camera.orientation);
	restart_minirt(minirt);
}

int	mouse_move_render(int x, int y, t_minirt *minirt)
{
	t_vec2	delta;

	delta.x = (float)(x - (int)minirt->controls.mlxc) * MOUSE_SENSITIVITY;
	delta.y = (float)(y - (int)minirt->controls.mlyc) * MOUSE_SENSITIVITY;
	if (!(delta.x != 0 || delta.y != 0))
		minirt->controls.movements.mouse = 0;
	if (minirt->controls.keydown.lmb)
		mouse_move_cam(x, y, minirt, delta);
	minirt->controls.mlxc = (t_uint)x;
	minirt->controls.mlyc = (t_uint)y;
	return (0);
}
