/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_objects.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 16:56:38 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/27 18:25:06 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

const char	*get_object_name(t_objects obj)
{
	static const char	*objects[11] = {"NULL Object", "Texture", "Material",
		"Ambiant light", "Camera", "Light", "Sphere", "Plane", "Cylinder",
		"Cone"};

	return (objects[(int)obj]);
}

void	draw_object(t_img *img, t_ttf *ttf, t_minirt *minirt, int i)
{
	int	i1;
	int	i2;
	int	tmp;

	i1 = 31 + i * 40 - minirt->controls.ui_infos.objects_scroll_offset;
	i2 = 70 + i * 40 - minirt->controls.ui_infos.objects_scroll_offset;
	if (i1 > 300 || i2 < 30)
		return ;
	tmp = img->height;
	img->height = 300;
	draw_horizontal_line(img, i2, 301, 600);
	if (&minirt->scene.elements[i] == minirt->controls.ui_infos.selected_object)
		draw_box_2d(img, (t_point2){301, i1}, (t_point2){600, i2}, 0x433366);
	else
		draw_box_2d(img, (t_point2){301, i1}, (t_point2){600, i2}, 0x3D3943);
	draw_string(img, ttf, get_object_name(minirt->scene.elements[i].type),
	(t_point2){.x = 320, .y = i2 - 20 + ((get_height(ttf)) * 0.5)});
	img->height = tmp;
}

void	draw_list_objects(t_minirt *minirt)
{
	int	i;

	i = -1;
	minirt->controls.font[0].size = 30;
	minirt->controls.font[0].color = 0xFFFFFF;
	while (++i < minirt->scene.el_amount)
	{
		draw_object(&minirt->mlx.img_controls, &minirt->controls.font[0], minirt, i);
	}
}

void	get_clicked_object(t_minirt *minirt, int mouse_y)
{
	int	i;
	int	i1;
	int	i2;

	i = -1;
	put_render_to_frame(minirt);
	while (++i < minirt->scene.el_amount)
	{
		i1 = 31 + i * 40 - minirt->controls.ui_infos.objects_scroll_offset;
		i2 = 70 + i * 40 - minirt->controls.ui_infos.objects_scroll_offset;
		if (mouse_y > i1 && mouse_y < i2)
		{
			minirt->controls.ui_infos.selected_object = &minirt->scene.elements[i];
			set_selected_object(minirt, minirt->controls.ui_infos.selected_object);
			mlx_put_image_to_window(minirt->mlx.mlx, minirt->mlx.render_win, minirt->mlx.img.img, 0, 0);
			return ;
		}
	}
	minirt->controls.ui_infos.selected_object = NULL;
	mlx_put_image_to_window(minirt->mlx.mlx, minirt->mlx.render_win, minirt->mlx.img.img, 0, 0);
}

int	mouse_down_obj(t_minirt *minirt, int key, int mouse_x, int mouse_y)
{
	const int	max_scroll = imax(40 * (minirt->scene.el_amount + 1) - 270, 0);
	if (mouse_y > 300 || mouse_x < 300 || mouse_y < 30 || mouse_x > 600)
		return (0);
	if (key == SCROLL_DOWN)
	{
		minirt->controls.ui_infos.objects_scroll_offset += 20;
		if (minirt->controls.ui_infos.objects_scroll_offset > max_scroll)
			minirt->controls.ui_infos.objects_scroll_offset = max_scroll;
	}
	else if (key == SCROLL_UP)
	{
		minirt->controls.ui_infos.objects_scroll_offset -= 20;
		if (minirt->controls.ui_infos.objects_scroll_offset < 0)
			minirt->controls.ui_infos.objects_scroll_offset = 0;
	}
	else if (key == LEFT_CLICK)
		get_clicked_object(minirt, mouse_y);
	return (1);
}
