/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pixel.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 14:30:43 by lilefebv          #+#    #+#             */
/*   Updated: 2025/03/26 14:36:58 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	put_pixel_image(t_img *img, t_uint x, t_uint y, int color)
{
	if (x >= img->width || y >= img->height)
		return ;
	((int *)img->img_str + (x) + (img->width * y))[0] = color;
}

void	put_sp_image(t_img *img, t_sc_point *sp)
{
	if (sp->x >= img->width || sp->y >= img->height)
		return ;
	((int *)img->img_str + (sp->x) + (img->width * sp->y))[0] = \
		(int)((sp->color.r << 16) +  (sp->color.g<<8) + (sp->color.b ));
}
