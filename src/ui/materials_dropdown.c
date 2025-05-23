/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   materials_dropdown.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 09:56:17 by lilefebv          #+#    #+#             */
/*   Updated: 2025/05/23 09:50:32 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	draw_select_triangle(t_img *img, t_dropdown *dropdown)
{
	const int	x = dropdown->x + dropdown->width - dropdown->height / 2 + 1;
	const int	weidth = 3;
	int			y;
	int			width;

	width = dropdown->height - 6;
	y = dropdown->y + 4;
	if (dropdown->active)
		y = dropdown->y + dropdown->height - 4;
	while (width > weidth)
	{
		draw_box_2d(img, (t_point2){x - width / 2, y}, (t_point2){x - width
			/ 2 + weidth, y + 1}, 0x0);
		draw_box_2d(img, (t_point2){x + width / 2 - weidth, y}, (t_point2){x
			+ width / 2, y + 1}, 0x0);
		width--;
		if (dropdown->active)
			y--;
		else
			y++;
	}
	while (width > 0)
	{
		draw_box_2d(img, (t_point2){x - width / 2, y}, (t_point2){x + width
			/ 2, y + 1}, 0x0);
		width--;
		if (dropdown->active)
			y--;
		else
			y++;
	}
}

void	draw_main_box(t_img *img, t_dropdown *dropdown)
{
	draw_horizontal_line(img, dropdown->y, dropdown->x, dropdown->x
		+ dropdown->width);
	draw_horizontal_line(img, dropdown->y + dropdown->height, dropdown->x,
		dropdown->x + dropdown->width);
	draw_vertical_line(img, dropdown->x, dropdown->y, dropdown->y
		+ dropdown->height);
	draw_vertical_line(img, dropdown->x + dropdown->width - dropdown->height,
		dropdown->y, dropdown->y + dropdown->height);
	draw_vertical_line(img, dropdown->x + dropdown->width, dropdown->y,
		dropdown->y + dropdown->height + 1);
	draw_box_2d(img, (t_point2){dropdown->x + 1, dropdown->y + 1},
		(t_point2){dropdown->x + dropdown->width - dropdown->height, dropdown->y
		+ dropdown->height}, 0x0);
	draw_box_2d(img, (t_point2){dropdown->x + dropdown->width
		- dropdown->height + 1, dropdown->y + 1}, (t_point2){dropdown->x
		+ dropdown->width, dropdown->y + dropdown->height}, 0xD9D9D9);
	draw_select_triangle(img, dropdown);
}

void	draw_dropdown_object(t_img *img, t_minirt *minirt, \
	t_dropdown *dropdown, int i)
{
	int	i1;
	int	i2;
	int	tmp;

	i1 = dropdown->y + dropdown->height + 1 + (i + 1) * 25
		- dropdown->scroll_offset;
	i2 = dropdown->y + dropdown->height + 25 + (i + 1) * 25
		- dropdown->scroll_offset;
	if (i1 > dropdown->y + dropdown->deployed_height
		|| i2 < dropdown->y + dropdown->height)
		return ;
	tmp = img->height;
	img->height = dropdown->y + dropdown->deployed_height;
	draw_horizontal_line(img, i2, dropdown->x, dropdown->x + dropdown->width);
	if (i < 0 && dropdown->selected && *dropdown->selected == NULL)
		draw_box_2d(img, (t_point2){dropdown->x + 1, i1},
			(t_point2){dropdown->x + dropdown->width, i2}, 0x353535);
	else if (dropdown->selected
		&& &minirt->scene.materials[i] == *dropdown->selected)
		draw_box_2d(img, (t_point2){dropdown->x + 1, i1},
			(t_point2){dropdown->x + dropdown->width, i2}, 0x353535);
	else
		draw_box_2d(img, (t_point2){dropdown->x + 1, i1},
			(t_point2){dropdown->x + dropdown->width, i2}, 0x0);
	if (i < 0)
		draw_string(img, &minirt->controls.font[0], "None",
			(t_point2){.x = dropdown->x + 3, .y = i2 - 13
			+ ((get_height(&minirt->controls.font[0])) * 0.5)});
	else
		draw_string(img, &minirt->controls.font[0],
			minirt->scene.materials[i].name, (t_point2){.x = dropdown->x + 3,
			.y = i2 - 13 + ((get_height(&minirt->controls.font[0])) * 0.5)});
	img->height = tmp;
}

void	draw_dropdown_select_box(t_img *img, t_dropdown *dropdown, \
	t_minirt *minirt)
{
	int	i;

	draw_horizontal_line(img, dropdown->y + dropdown->deployed_height,
		dropdown->x, dropdown->x + dropdown->width);
	draw_vertical_line(img, dropdown->x, dropdown->y + dropdown->height,
		dropdown->y + dropdown->deployed_height);
	draw_vertical_line(img, dropdown->x + dropdown->width, dropdown->y
		+ dropdown->height, dropdown->y + dropdown->deployed_height + 1);
	draw_box_2d(img, (t_point2){dropdown->x + 1, dropdown->y + dropdown->height
		+ 1}, (t_point2){dropdown->x + dropdown->width, dropdown->y
		+ dropdown->deployed_height}, UI_BACK_COLOR);
	i = -1;
	draw_dropdown_object(img, minirt, dropdown, i);
	while (++i < minirt->scene.mat_amount)
	{
		draw_dropdown_object(img, minirt, dropdown, i);
	}
}

void	display_mat_dropdown(t_minirt *minirt, t_dropdown *dropdown)
{
	minirt->controls.font[0].size = 20;
	minirt->controls.font[0].color = 0xFFFFFF;
	if (dropdown->active)
		draw_dropdown_select_box(&minirt->mlx.img_controls, dropdown, minirt);
	draw_main_box(&minirt->mlx.img_controls, dropdown);
	if (!dropdown->selected || !*dropdown->selected)
		draw_string(&minirt->mlx.img_controls, &minirt->controls.font[0],
			"None", (t_point2){dropdown->x + 3, dropdown->y
			+ dropdown->height - 3});
	else
		draw_string(&minirt->mlx.img_controls, &minirt->controls.font[0],
			((t_mat *)*dropdown->selected)->name, (t_point2){dropdown->x
			+ 3, dropdown->y + dropdown->height - 3});
}

void	get_selected_mat_dropdown(t_minirt *minirt, t_dropdown *dropdown, \
	int mouse_y)
{
	int	i;
	int	i1;
	int	i2;

	if (!dropdown->selected)
		return ;
	stop_minirt(minirt);
	i = -2;
	while (++i < minirt->scene.mat_amount)
	{
		i1 = dropdown->y + dropdown->height + 1 + (i + 1) * 25
			- dropdown->scroll_offset;
		i2 = dropdown->y + dropdown->height + 25 + (i + 1) * 25
			- dropdown->scroll_offset;
		if (mouse_y > i1 && mouse_y < i2)
		{
			if (i < 0)
				*dropdown->selected = NULL;
			else
				*dropdown->selected = &minirt->scene.materials[i];
			return ;
		}
	}
	minirt->controls.ui_infos.selected_object = NULL;
}

int	mouse_down_dropdown(t_minirt *minirt, int key, t_point2 pos, \
	t_dropdown *dropdown)
{
	const int	max_scroll = imax(25 * (minirt->scene.mat_amount + 1)
			- (dropdown->deployed_height - dropdown->height), 0);

	if (pos.x > dropdown->x && pos.y > dropdown->y && pos.x < dropdown->x
		+ dropdown->width && pos.y < dropdown->y + dropdown->height)
	{
		if (key == LEFT_CLICK)
		{
			dropdown->active = !dropdown->active;
			return (1);
		}
	}
	if (dropdown->active)
	{
		if (pos.x > dropdown->x && pos.y > dropdown->y + dropdown->height
			&& pos.x < dropdown->x + dropdown->width
			&& pos.y < dropdown->y + dropdown->deployed_height)
		{
			if (key == SCROLL_DOWN)
			{
				dropdown->scroll_offset += 15;
				if (dropdown->scroll_offset > max_scroll)
					dropdown->scroll_offset = max_scroll;
			}
			else if (key == SCROLL_UP)
			{
				dropdown->scroll_offset -= 15;
				if (dropdown->scroll_offset < 0)
					dropdown->scroll_offset = 0;
			}
			else if (key == LEFT_CLICK)
			{
				get_selected_mat_dropdown(minirt, dropdown, pos.y);
				dropdown->active = 0;
			}
		}
		else if (key == LEFT_CLICK)
			dropdown->active = 0;
		return (1);
		(void)minirt;
	}
	return (0);
}
