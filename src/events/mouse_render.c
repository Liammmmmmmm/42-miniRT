/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_render.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 18:42:48 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/05 10:09:06 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	mousedown_render(int key, int x, int y, t_minirt *minirt)
{
	char	item_null;

	minirt->controls.mlxr = (t_uint)x;
	minirt->controls.mlyr = (t_uint)y;
	mousedown_common(key, minirt);
	if (key == LEFT_CLICK && minirt->scene.build_bvh == 0) // Ajout condition build BVH pour eviter un segfault dans le cas ou on clique sur un objet qu'on a supprimé precedamment
	{
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
