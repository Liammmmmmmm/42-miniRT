/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   put_hdr_window.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 13:06:50 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/23 13:14:50 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	put_ext_image_to_frame(t_tex_img *img, t_img *img_buff)
{
	int			i;
	int			total;
	t_sc_point	pt;
	const int	red_ratio = 2;

	i = 0;
	total = img->height * img->width;
	while (i < total)
	{
		pt.x = (i % img->width) / red_ratio;
		pt.y = (img->height - i / img->width) / red_ratio;
		pt.color.r = img->rgba[i].r;
		pt.color.g = img->rgba[i].g;
		pt.color.b = img->rgba[i].b;
		put_sp_image(img_buff, &pt);
		i++;
	}
}

static inline void	set_pt_color(t_hdr *img, double scale, int i,
	t_sc_point *pt)
{
	if (img->gamma == 1.0)
		pt->color = (t_color){iclamp(0, img->pixels[i].r * scale, 255),
			iclamp(0, img->pixels[i].g * scale, 255),
			iclamp(0, img->pixels[i].b * scale, 255)};
	else
	{
		pt->color.r = pow(img->pixels[i].r / 255.0, 1.0 / img->gamma) * 255;
		pt->color.g = pow(img->pixels[i].g / 255.0, 1.0 / img->gamma) * 255;
		pt->color.b = pow(img->pixels[i].b / 255.0, 1.0 / img->gamma) * 255;
	}
}

void	put_hdr_to_frame(t_hdr *img, t_img *img_buff, t_minirt *minirt)
{
	int			i;
	int			total;
	t_sc_point	pt;
	double		scale;

	i = 0;
	total = img->height * img->width;
	while (i < total)
	{
		if (img->exposure == 0)
			scale = ldexp(1.0, img->pixels[i].e - 128);
		else
			scale = ldexp(1.0, img->pixels[i].e - 128)
				* powf(2.0, img->exposure);
		pt.x = (i % img->width);
		pt.y = img->height - 1 - (i / img->width);
		set_pt_color(img, scale, i, &pt);
		put_sp_image(img_buff, &pt);
		i++;
	}
	mlx_put_image_to_window(minirt->mlx.mlx,
		minirt->mlx.render_win, minirt->mlx.img.img, 0, 0);
}
