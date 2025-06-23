/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   no_upscaling.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 19:26:48 by madelvin          #+#    #+#             */
/*   Updated: 2025/06/23 10:35:38 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static void	put_render_same_size(t_minirt *minirt, double gamma_corr,
	int divide)
{
	int	i;
	int	tpx;

	tpx = minirt->scene.win_height * minirt->scene.win_width;
	i = -1;
	if (gamma_corr == 1)
	{
		while (++i < tpx)
			minirt->screen.render[i]
				= render_float_to_int(&minirt->screen.float_render[i], divide);
	}
	else
		while (++i < tpx)
			minirt->screen.render[i] = \
render_float_to_int_gamma(&minirt->screen.float_render[i], gamma_corr, divide)
				;
}

static void	put_render_default_gamma(t_minirt *minirt, double gamma_corr,
	int div)
{
	int	x;
	int	y;
	int	i;

	y = -1;
	while (++y < minirt->scene.win_height)
	{
		x = -1;
		while (++x < minirt->scene.win_width)
		{
			i = (int)((float)y / (minirt->scene.win_height - 1)
					* (minirt->scene.render_height - 1)) *\
	minirt->scene.render_width + (int)((float)x / (minirt->scene.win_width \
	- 1) * (minirt->scene.render_width - 1));
			minirt->screen.render[y * (minirt->scene.win_width) + x]
				= render_float_to_int_gamma(&minirt->screen.float_render[i],
					gamma_corr, div);
		}
	}
}

static void	put_render_default(t_minirt *minirt, double gamma_corr, int div)
{
	int	x;
	int	y;
	int	i;

	y = -1;
	if (gamma_corr == 1)
	{
		while (++y < minirt->scene.win_height)
		{
			x = -1;
			while (++x < minirt->scene.win_width)
			{
				i = (int)((float)y / (minirt->scene.win_height - 1)
						* (minirt->scene.render_height - 1)) *\
	minirt->scene.render_width + (int)((float)x / (minirt->scene.win_width \
	- 1) * (minirt->scene.render_width - 1));
				minirt->screen.render[y * (minirt->scene.win_width) + x]
					= render_float_to_int(&minirt->screen.float_render[i], div);
			}
		}
	}
	else
		put_render_default_gamma(minirt, gamma_corr, div);
}

void	put_render_to_buff(t_minirt *minirt)
{
	int		divide;
	double	gamma_corr;

	if (minirt->options.no_display)
		return ;
	divide = minirt->screen.last_sample_am;
	if (divide == 0)
		divide = 1;
	gamma_corr = 1.0 / minirt->viewport.gamma;
	if (minirt->scene.win_height == minirt->scene.render_height
		&& minirt->scene.win_width == minirt->scene.render_width)
		put_render_same_size(minirt, gamma_corr, divide);
	else
		put_render_default(minirt, gamma_corr, divide);
	copy_buff_to_image(minirt);
}

void	put_micrort_to_image(t_minirt *minirt)
{
	int	i;
	int	tpx;
	int	divide;

	divide = minirt->micrort.sample;
	if (divide == 0)
		divide = 1;
	tpx = minirt->micrort.viewport.render_w * minirt->micrort.viewport.render_h;
	i = 0;
	while (i < tpx)
	{
		put_pixel_image(&minirt->mlx.img_controls,
			i % minirt->micrort.viewport.render_w + 301,
			i / minirt->micrort.viewport.render_w + 861, ((int)(\
	((int)(clamp_double(minirt->micrort.render[i].r / divide) * 255) << 16) \
	+ ((int)(clamp_double(minirt->micrort.render[i].g / divide) * 255) << 8) \
	+ (int)(clamp_double(minirt->micrort.render[i].b / divide) * 255))));
		i++;
	}
}
