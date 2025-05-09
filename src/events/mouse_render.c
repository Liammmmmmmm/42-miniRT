/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_render.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 18:42:48 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/27 16:49:07 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	mousedown_render(int key, int x, int y, t_minirt *minirt)
{
	char	item_null;

	minirt->controls.mlxr = (t_uint)x;
	minirt->controls.mlyr = (t_uint)y;
	mousedown_common(key, minirt);
	if (key == LEFT_CLICK)
	{
		item_null = (minirt->controls.ui_infos.selected_object == NULL);
		minirt->controls.ui_infos.selected_object = select_object(minirt,
				minirt->controls.mlxr, minirt->controls.mlyr);
		if ((minirt->controls.ui_infos.selected_object == NULL
				|| minirt->controls.ui_infos.selected_object->type == PLANE
				|| minirt->controls.ui_infos.selected_object->type == LIGHT)
			&& item_null == 0)
		{
			put_render_to_frame(minirt);
			mlx_put_image_to_window(minirt->mlx.mlx, minirt->mlx.render_win,
				minirt->mlx.img.img, 0, 0);
		}
		else if (minirt->controls.ui_infos.selected_object != NULL
			&& item_null == 0)
			put_render_to_frame(minirt);
		set_selected_object(minirt, minirt->controls.ui_infos.selected_object);
	}
	else if (key == RIGHT_CLICK)
	{
		minirt->controls.selected_x = x;
		minirt->controls.selected_y = y;
		printf("new selected : %d %d\n", minirt->controls.selected_x, minirt->controls.selected_y);
	}
	return (0);
}

int	mouseup_render(int key, int x, int y, t_minirt *minirt)
{
	minirt->controls.mlxr = (t_uint)x;
	minirt->controls.mlyr = (t_uint)y;
	mouseup_common(key, minirt);
	return (0);
}
