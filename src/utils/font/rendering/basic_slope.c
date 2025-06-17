/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   basic_slope.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 15:02:27 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/13 16:30:27 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	draw_line_x(t_point *point_a, t_point *point_b, t_img *img, int c)
{
	float	m;
	int		dx;
	int		dy;
	int		temp;

	dx = point_b->x - point_a->x;
	dy = point_b->y - point_a->y;
	m = (float)dy / dx;
	if (point_b->x > point_a->x)
	{
		temp = point_a->x - 1;
		while (++temp <= point_b->x)
			put_pixel_image(img, temp, m * (temp - point_a->x) + point_a->y, c);
	}
	else
	{
		temp = point_b->x - 1;
		while (++temp <= point_a->x)
			put_pixel_image(img, temp, m * (temp - point_b->x) + point_b->y, c);
	}
}

void	draw_line_y(t_point *point_a, t_point *point_b, t_img *img, int c)
{
	float	m;
	int		dx;
	int		dy;
	int		temp;

	dx = point_b->x - point_a->x;
	dy = point_b->y - point_a->y;
	m = (float)dx / dy;
	if (point_b->y > point_a->y)
	{
		temp = point_a->y - 1;
		while (++temp <= point_b->y)
			put_pixel_image(img, m * (temp - point_a->y) + point_a->x, temp, c);
	}
	else
	{
		temp = point_b->y - 1;
		while (++temp <= point_a->y)
			put_pixel_image(img, m * (temp - point_b->y) + point_b->x, temp, c);
	}
}

void	draw_line(t_point *point_a, t_point *point_b, t_img *img, int c)
{
	int	dx;
	int	dy;

	dx = point_b->x - point_a->x;
	dy = point_b->y - point_a->y;
	if (ft_abs(dy) <= ft_abs(dx))
	{
		if (dx == 0)
			return ;
		draw_line_x(point_a, point_b, img, c);
	}
	else
	{
		if (dy == 0)
			return ;
		draw_line_y(point_a, point_b, img, c);
	}
}
