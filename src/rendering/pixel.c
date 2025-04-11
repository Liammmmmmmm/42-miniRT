/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pixel.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 14:30:43 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/11 10:23:58 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	put_pixel_image(t_img *img, int x, int y, int color)
{
	if (x >= (int)img->width || x < 0 || y >= (int)img->height || y < 0)
		return ;
	((int *)img->img_str + x + (img->width * y))[0] = color;
}

int	calc_gradiant_color(int color_a, int color_b, float ratio)
{
	return (((t_uchar)(((color_a >> 16) & 0xff) + ratio * (((color_b >> 16) & 0xff) - ((color_a >> 16) & 0xff))) << 16) | ((t_uchar)(((color_a >> 8) & 0xff) + ratio * (((color_b >> 8) & 0xff) - ((color_a >> 8) & 0xff))) << 8) | (t_uchar)((color_a & 0xff) + ratio * ((color_b & 0xff) - (color_a & 0xff))));
}

void	put_pixel_image_alpha(t_img *img, int x, int y, int color)
{
	if (x >= (int)img->width || x < 0 || y >= (int)img->height || y < 0)
		return ;
	((int *)img->img_str + x + (img->width * y))[0] = color;
}

void	put_sp_image(t_img *img, t_sc_point *sp)
{
	if (sp->x >= img->width || sp->y >= img->height)
		return ;
	((int *)img->img_str + (sp->x) + (img->width * sp->y))[0] = \
		(int)((sp->color.r << 16) +  (sp->color.g<<8) + (sp->color.b ));
}
