/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dropdown.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 12:23:08 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/10 15:11:15 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "ui.h"

void	draw_triangle_point(t_img *img, t_dropdown *dropdown, int width,
	t_point2 p)
{
	while (width > 0)
	{
		draw_box_2d(img, (t_point2){p.x - width / 2, p.y}, (t_point2){p.x
			+ width / 2, p.y + 1}, 0x0);
		width--;
		if (dropdown->active)
			p.y--;
		else
			p.y++;
	}
}

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
	draw_triangle_point(img, dropdown, width, (t_point2){x, y});
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

int	dropdown_common_define(t_dropdown *dropdown, int *i1, int *i2, int i)
{
	*i1 = dropdown->y + dropdown->height + 1 + (i + 1) * 25
		- dropdown->scroll_offset;
	*i2 = dropdown->y + dropdown->height + 25 + (i + 1) * 25
		- dropdown->scroll_offset;
	if (*i1 > dropdown->y + dropdown->deployed_height
		|| *i2 < dropdown->y + dropdown->height)
		return (0);
	return (1);
}

void	draw_dropdown_select_box(t_img *img, t_dropdown *dropdown)
{
	draw_horizontal_line(img, dropdown->y + dropdown->deployed_height,
		dropdown->x, dropdown->x + dropdown->width);
	draw_vertical_line(img, dropdown->x, dropdown->y + dropdown->height,
		dropdown->y + dropdown->deployed_height);
	draw_vertical_line(img, dropdown->x + dropdown->width, dropdown->y
		+ dropdown->height, dropdown->y + dropdown->deployed_height + 1);
	draw_box_2d(img, (t_point2){dropdown->x + 1, dropdown->y + dropdown->height
		+ 1}, (t_point2){dropdown->x + dropdown->width, dropdown->y
		+ dropdown->deployed_height}, UI_BACK_COLOR);
}
