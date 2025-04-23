/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_objects.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 16:56:38 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/23 12:19:11 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

const char	*get_object_name(t_objects obj)
{
	static const char	*objects[10] = {"NULL Object", "Texture", "Material",
		"Ambiant light", "Camera", "Light", "Sphere", "Plane", "Cylinder"};
	
	return (objects[(int)obj]);
}

void	draw_object(t_img *img, t_ttf *ttf, t_minirt *minirt, int i)
{
	int	i1;
	int	i2;

	i1 = 31 + i * 40 - minirt->controls.ui_infos.objects_scroll_offset;
	i2 = 70 + i * 40 - minirt->controls.ui_infos.objects_scroll_offset;
	draw_horizontal_line(img, i2, 301, 600);
	if (&minirt->scene.elements[i] == minirt->controls.ui_infos.selected_object)
		draw_box_2d(img, (t_point2){301, i1}, (t_point2){600, i2}, 0x433366);
	else
		draw_box_2d(img, (t_point2){301, i1}, (t_point2){600, i2}, 0x3D3943);
	draw_string(img, ttf, get_object_name(minirt->scene.elements[i].type), (t_point2){.x = 320, .y = i2 - 20 + ((get_height(ttf)) * 0.5)});
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
