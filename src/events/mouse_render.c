/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_render.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 18:42:48 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/23 11:00:02 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	mousedown_render(int key, int x, int y, t_minirt *minirt)
{
	minirt->controls.mlxr = (t_uint)x;
	minirt->controls.mlyr = (t_uint)y;
	mousedown_common(key, minirt);
	
	if (key == LEFT_CLICK)
	{
		char item_null;
		item_null = (minirt->controls.ui_infos.selected_object == NULL);
		minirt->controls.ui_infos.selected_object = select_object(minirt, minirt->controls.mlxr, minirt->controls.mlyr);
		if (minirt->controls.ui_infos.selected_object == NULL
			&& item_null == 0)
		{
			put_render_to_frame(minirt);
			mlx_put_image_to_window(minirt->mlx.mlx, minirt->mlx.render_win, minirt->mlx.img.img, 0, 0);
		}
		else if (minirt->controls.ui_infos.selected_object != NULL && item_null == 0)
			put_render_to_frame(minirt);
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
