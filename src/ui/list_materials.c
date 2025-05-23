/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_materials.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 11:11:42 by lilefebv          #+#    #+#             */
/*   Updated: 2025/05/23 11:28:10 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "ui.h"
#include "minirt.h"

void	draw_material(t_img *img, t_ttf *ttf, t_minirt *minirt, int i)
{
	int	i1;
	int	i2;
	int	tmp;

	i1 = 31 + i * 40 - minirt->controls.ui_infos.materials_scroll_offset;
	i2 = 70 + i * 40 - minirt->controls.ui_infos.materials_scroll_offset;
	if (i1 > 300 || i2 < 30)
		return ;
	tmp = img->height;
	img->height = 300;
	draw_horizontal_line(img, i2, 301, 600);
	if (&minirt->scene.materials[i] == minirt->controls.ui_infos.selected_material)
		draw_box_2d(img, (t_point2){301, i1}, (t_point2){600, i2}, 0x433366);
	else
		draw_box_2d(img, (t_point2){301, i1}, (t_point2){600, i2}, 0x3D3943);
	draw_string(img, ttf, minirt->scene.materials[i].name,
			(t_point2){.x = 320, .y = i2 - 20 + ((get_height(ttf)) * 0.5)});
	img->height = tmp;
}

void	draw_list_materials(t_minirt *minirt)
{
	int	i;

	i = -1;
	minirt->controls.font[0].size = 30;
	minirt->controls.font[0].color = 0xFFFFFF;
	while (++i < minirt->scene.mat_amount)
	{
		draw_material(&minirt->mlx.img_controls, &minirt->controls.font[0], minirt, i);
	}
}

void	get_clicked_mat(t_minirt *minirt, int mouse_y)
{
	int	i;
	int	i1;
	int	i2;

	i = -1;
	while (++i < minirt->scene.mat_amount)
	{
		i1 = 31 + i * 40 - minirt->controls.ui_infos.materials_scroll_offset;
		i2 = 70 + i * 40 - minirt->controls.ui_infos.materials_scroll_offset;
		if (mouse_y > i1 && mouse_y < i2)
		{
			minirt->controls.ui_infos.selected_material = &minirt->scene.materials[i];
			set_selected_mat(minirt, minirt->controls.ui_infos.selected_material);
			return ;
		}
	}
	minirt->controls.ui_infos.selected_material = NULL;
}

int	mouse_down_mat(t_minirt *minirt, int key, int mouse_x, int mouse_y)
{
	const int	max_scroll = imax(40 * (minirt->scene.mat_amount + 1) - 270, 0);
	if (mouse_y > 300 || mouse_x < 300 || mouse_y < 30 || mouse_x > 600)
		return (0);
	if (key == SCROLL_DOWN)
	{
		minirt->controls.ui_infos.materials_scroll_offset += 20;
		if (minirt->controls.ui_infos.materials_scroll_offset > max_scroll)
			minirt->controls.ui_infos.materials_scroll_offset = max_scroll;
	}
	else if (key == SCROLL_UP)
	{
		minirt->controls.ui_infos.materials_scroll_offset -= 20;
		if (minirt->controls.ui_infos.materials_scroll_offset < 0)
			minirt->controls.ui_infos.materials_scroll_offset = 0;
	}
	else if (key == LEFT_CLICK)
		get_clicked_mat(minirt, mouse_y);
	return (1);
}

