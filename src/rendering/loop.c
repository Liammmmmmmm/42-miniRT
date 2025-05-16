/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 15:31:03 by lilefebv          #+#    #+#             */
/*   Updated: 2025/05/16 14:17:09 by lilefebv         ###   ########lyon.fr   */
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
	// if (minirt->controls.values.upscaling_ratio == 100)
	put_render_to_buff(minirt);//no_upscaling(minirt);
	// else if (minirt->controls.values.upscaling_selected == 0)
	// 	neighbor_upscale(minirt);
	// else if (minirt->controls.values.upscaling_selected == 1)
	// 	bilinear_upscale(minirt);
	// else if (minirt->controls.values.upscaling_selected == 2)
	// 	bicubic_upscale(minirt);
}

char	*get_time_dhmsms(ssize_t time)
{
	t_uint	day;
	t_uint	hour;
	t_uint	min;
	t_uint	s;
	t_uint	ms;

	ms = time % 1000;
	s = time / 1000;
	min = s / 60;
	s = s % 60;
	hour = min / 60;
	min = min % 60;
	day = hour / 24;
	hour = hour % 24;
	if (day)
		return (ft_sprintf("%ud %uh %um %us %ums", day, hour, min, s, ms));
	if (hour)
		return (ft_sprintf("%uh %um %us %ums", hour, min, s, ms));
	if (min)
		return (ft_sprintf("%um %us %ums", min, s, ms));
	if (s)
		return (ft_sprintf("%us %ums", s, ms));
	return (ft_sprintf("%ums", ms));
}

void	no_display_infos(t_minirt *minirt)
{
	char	*txt;
	char	*tmp;
	char	*tmp2;
	ssize_t	time;

	setcolor(&minirt->mlx.img, 0x0);
	minirt->controls.font[0].size = 40;
	minirt->controls.font[0].color = 0xFFFFFF;
	txt = ft_sprintf("Sample %d/%d", minirt->screen.sample, minirt->screen.spp);
	draw_string(&minirt->mlx.img, &minirt->controls.font[0], txt, (t_point2){20, 50});
	free(txt);
	minirt->controls.font[0].size = 28;
	if (minirt->screen.sample)
	{
		time = get_cpu_time();
		tmp = get_time_dhmsms(time - minirt->screen.first_sample_time);
		tmp2 = get_time_dhmsms(((time - minirt->screen.first_sample_time) / minirt->screen.sample) * (minirt->screen.spp - minirt->screen.sample));
		txt = ft_sprintf("Elapsed time %s\nAverage: %ums/sample\nEstimated left time: %s", tmp, (t_uint)((time - minirt->screen.first_sample_time) / minirt->screen.sample), tmp2);
		free(tmp);
		free(tmp2);
		draw_string(&minirt->mlx.img, &minirt->controls.font[0], txt, (t_point2){20, 90});
		free(txt);
	}
	mlx_put_image_to_window(minirt->mlx.mlx, minirt->mlx.render_win, minirt->mlx.img.img, 0, 0);
}

void	render_frame(t_minirt *minirt)
{
	if (minirt->options.no_display)
		no_display_infos(minirt);
	render(minirt);
	if (!minirt->options.no_display)
	{
		render_bvh(minirt);
		render_ui(minirt);
		draw_selected_object(minirt);
	}
	minirt->stats.frame += 1;
}
