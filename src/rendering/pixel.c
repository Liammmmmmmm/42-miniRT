/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pixel.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 14:30:43 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/10 18:39:11 by madelvin         ###   ########.fr       */
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
	return (((t_uchar)(((color_a >> 16) & 0xff) + ratio * \
	(((color_b >> 16) & 0xff) - ((color_a >> 16) & 0xff))) << 16) \
	| ((t_uchar)(((color_a >> 8) & 0xff) + ratio * \
	(((color_b >> 8) & 0xff) - ((color_a >> 8) & 0xff))) << 8) \
	| (t_uchar)((color_a & 0xff) + ratio * \
	((color_b & 0xff) - (color_a & 0xff))));
}

int	get_pixel_color(t_img *img, int x, int y)
{
	if (x >= (int)img->width || x < 0 || y >= (int)img->height || y < 0)
		return (0);
	return (((int *)img->img_str + x + (img->width * y))[0]);
}

void	put_pixel_image_alpha(t_img *img, int x, int y, t_color_alpha color)
{
	put_pixel_image(img, x, y, calc_gradiant_color(get_pixel_color(img, x, y),
			color.color, color.alpha));
}
