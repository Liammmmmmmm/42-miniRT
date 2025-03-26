/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_circles.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 14:45:43 by lilefebv          #+#    #+#             */
/*   Updated: 2025/03/26 14:47:51 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	draw_circle(int xc, int yc, int x, int y, t_img *img, int color)
{
	put_pixel_image(img, xc + x, yc + y, color);
	put_pixel_image(img, xc - x, yc + y, color);
	put_pixel_image(img, xc + x, yc - y, color);
	put_pixel_image(img, xc - x, yc - y, color);
	put_pixel_image(img, xc + y, yc + x, color);
	put_pixel_image(img, xc - y, yc + x, color);
	put_pixel_image(img, xc + y, yc - x, color);
	put_pixel_image(img, xc - y, yc - x, color);
}

void	circle_bres(int xc, int yc, int r, t_img *img, int color)
{
	int x = 0, y = r;
	int d = 3 - 2 * r;
	draw_circle(xc, yc, x, y, img, color);
	while (y >= x)
	{
		if (d > 0) {
			y--; 
			d = d + 4 * (x - y) + 10;
		}
		else
			d = d + 4 * x + 6;
		x++;
		draw_circle(xc, yc, x, y, img, color);
    }
}

void	draw_circle_comp(int xc, int yc, int x, int y, t_img *img, int color)
{
	put_pixel_image(img, xc + x, yc + y, color);
	put_pixel_image(img, xc - x, yc + y, color);
	put_pixel_image(img, xc + x, yc - y, color);
	put_pixel_image(img, xc - x, yc - y, color);
	put_pixel_image(img, xc + y, yc + x, color);
	put_pixel_image(img, xc - y, yc + x, color);
	put_pixel_image(img, xc + y, yc - x, color);
	put_pixel_image(img, xc - y, yc - x, color);
}

void	circle_bres_comp(int xc, int yc, int r, t_img *img, int color)
{
	int x = 0, y = r;
	int d = 3 - 2 * r;
	draw_circle_comp(xc, yc, x, y, img, color);
	while (y >= x)
	{
		if (d > 0) {
			y--; 
			d = d + 4 * (x - y) + 10;
		}
		else
			d = d + 4 * x + 6;
		x++;
		draw_circle_comp(xc, yc, x, y, img, color);
    }
}

void	draw_filled_circle(int xc, int yc, int r, t_img *img, int color)
{
	int	y;
	int	x;

	y = -r;
	while (y <= r)
	{
		x = -r;
		while (x <= r)
		{
			if (x * x + y * y <= r * r)
				put_pixel_image(img, xc + x, yc + y, color);
			x++;
		}
		y++;
	}
}

