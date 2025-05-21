/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_circles.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 14:45:43 by lilefebv          #+#    #+#             */
/*   Updated: 2025/05/21 16:04:56 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "mlx_base.h"

static void	draw_circle(t_draw_circle c, t_img *img, int color)
{
	put_pixel_image(img, c.xc + c.x, c.yc + c.y, color);
	put_pixel_image(img, c.xc - c.x, c.yc + c.y, color);
	put_pixel_image(img, c.xc + c.x, c.yc - c.y, color);
	put_pixel_image(img, c.xc - c.x, c.yc - c.y, color);
	put_pixel_image(img, c.xc + c.y, c.yc + c.x, color);
	put_pixel_image(img, c.xc - c.y, c.yc + c.x, color);
	put_pixel_image(img, c.xc + c.y, c.yc - c.x, color);
	put_pixel_image(img, c.xc - c.y, c.yc - c.x, color);
}

void	circle_bres(t_circle c, t_img *img, int color)
{
	int	x;
	int	d;

	x = 0;
	d = 3 - 2 * c.r;
	draw_circle((t_draw_circle){c.xc, c.yc, x, c.r}, img, color);
	while (c.r >= x)
	{
		if (d > 0)
		{
			c.r--;
			d = d + 4 * (x - c.r) + 10;
		}
		else
			d = d + 4 * x + 6;
		x++;
		draw_circle((t_draw_circle){c.xc, c.yc, x, c.r}, img, color);
	}
}

void	draw_filled_circle(t_circle c, t_img *img, int color)
{
	int	y;
	int	x;

	y = -c.r;
	while (y <= c.r)
	{
		x = -c.r;
		while (x <= c.r)
		{
			if (x * x + y * y <= c.r * c.r)
				put_pixel_image(img, c.xc + x, c.yc + y, color);
			x++;
		}
		y++;
	}
}
