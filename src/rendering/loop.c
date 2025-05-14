/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 15:31:03 by lilefebv          #+#    #+#             */
/*   Updated: 2025/05/14 17:16:41 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "bvh.h"
#include "bmp_parsing.h"

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
		pt.color = img->pixel_data[i];
		put_sp_image(img_buff, &pt);
		i++;
	}
}

void	put_hdr_to_frame(t_hdr *img, t_img *img_buff, t_minirt *minirt)
{
	int			i;
	int			total;
	t_sc_point	pt;
	const int	red_ratio = 1;
	double		scale;

	i = 0;
	total = img->height * img->width;
	while (i < total)
	{
		if (img->exposure == 0)
			scale = ldexp(1.0, img->pixels[i].e - 128);
		else
			scale = ldexp(1.0, img->pixels[i].e - 128) * powf(2.0, img->exposure);

		pt.x = (i % img->width) / red_ratio;
		pt.y = img->height - 1 - (i / img->width) / red_ratio;

		if (img->gamma == 1.0)
			pt.color = (t_color){iclamp(0, img->pixels[i].r * scale, 255), iclamp(0, img->pixels[i].g * scale, 255), iclamp(0, img->pixels[i].b * scale, 255)};
		else
		{
			double gamma_corr = 1.0 / (img->gamma);
			pt.color.r = pow(img->pixels[i].r / 255.0, gamma_corr) * 255;
			pt.color.g = pow(img->pixels[i].g / 255.0, gamma_corr) * 255;
			pt.color.b = pow(img->pixels[i].b / 255.0, gamma_corr) * 255;
		}
		put_sp_image(img_buff, &pt);
		i++;
	}
	
	mlx_put_image_to_window(minirt->mlx.mlx, minirt->mlx.render_win, minirt->mlx.img.img, 0, 0);
}

void	put_render_to_frame(t_minirt *minirt)
{
	if (minirt->controls.values.upscaling_ratio == 100)
		no_upscaling_float(minirt);//no_upscaling(minirt);
	else if (minirt->controls.values.upscaling_selected == 0)
		neighbor_upscale(minirt);
	else if (minirt->controls.values.upscaling_selected == 1)
		bilinear_upscale(minirt);
	else if (minirt->controls.values.upscaling_selected == 2)
		bicubic_upscale(minirt);
}

void	render_frame(t_minirt *minirt)
{
	render(minirt);
	render_bvh(minirt);
	render_ui(minirt);
	// minirt->scene.amb_light.skybox_t->hdr.exposure = (minirt->controls.values.gamma - 25) / 10.0;
	// put_hdr_to_frame(&minirt->scene.amb_light.skybox_t->hdr, &minirt->mlx.img, minirt);

	minirt->stats.frame += 1;
}
