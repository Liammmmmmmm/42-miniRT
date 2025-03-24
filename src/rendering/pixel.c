/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pixel.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 14:30:43 by lilefebv          #+#    #+#             */
/*   Updated: 2025/03/24 11:04:41 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	put_pixel_image(t_img *img, t_uint x, t_uint y, int color)
{
	t_color	c;

	if (x < 0 || x >= img->width || y < 0 || y >= img->height)
		return ;
	c.r = (color >> 16) & 0xff;
	c.g = (color >> 8) & 0xff;
	c.b = color & 0xff;
	img->img_str[(x * 4) + (img->width * 4 * y)] = c.b;
	img->img_str[(x * 4) + (img->width * 4 * y) + 1] = c.g;
	img->img_str[(x * 4) + (img->width * 4 * y) + 2] = c.r;
	img->img_str[(x * 4) + (img->width * 4 * y) + 3] = 0;
}

void	put_sp_image(t_img *img, t_sc_point *sp)
{
	if (sp->x < 0 || sp->x >= img->width || sp->y < 0 || sp->y >= img->height)
		return ;
	img->img_str[(sp->x * 4) + (img->width * 4 * sp->y)] = sp->color.b;
	img->img_str[(sp->x * 4) + (img->width * 4 * sp->y) + 1] = sp->color.g;
	img->img_str[(sp->x * 4) + (img->width * 4 * sp->y) + 2] = sp->color.r;
	img->img_str[(sp->x * 4) + (img->width * 4 * sp->y) + 3] = 0;
}
