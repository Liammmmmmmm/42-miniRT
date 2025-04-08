/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pixel.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 14:30:43 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/08 15:07:56 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	put_pixel_image(t_img *img, int x, int y, int color)
{
	y = img->height - y -10;
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
