/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   no_upscaling.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 19:26:48 by madelvin          #+#    #+#             */
/*   Updated: 2025/06/10 18:39:03 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	copy_buff_to_image(t_minirt *minirt)
{
	int	i;
	int	tpx;
	
	if (minirt->options.no_display)
		return ;
	i = -1;
	tpx = minirt->scene.win_height * minirt->scene.win_width;
	while (++i < tpx)
		((int *)minirt->mlx.img.img_str)[i] = minirt->screen.render[i];
}

void	put_render_to_buff(t_minirt *minirt)
{
	int	x;
	int	y;
	int	i;
	int	divide;
	double		gamma_corr;

	if (minirt->options.no_display)
		return ;
	divide = minirt->screen.last_sample_am;
	if (divide == 0)
		divide = 1;
	y = 0;
	gamma_corr = 1.0 / minirt->viewport.gamma;
	if (gamma_corr == 1)
	{
		while (y < minirt->scene.win_height)
		{
			x = 0;
			while (x < minirt->scene.win_width)
			{
				i = (int)((float)y / (minirt->scene.win_height - 1) * (minirt->scene.render_height - 1)) * minirt->scene.render_width + (int)((float)x / (minirt->scene.win_width - 1) * (minirt->scene.render_width - 1));
				minirt->screen.render[y * (minirt->scene.win_width) + x] = ((int)(
					((int)(clamp_double(minirt->screen.float_render[i].r / divide) * 255) << 16) + 
					((int)(clamp_double(minirt->screen.float_render[i].g / divide) * 255) << 8) + 
					(int)(clamp_double(minirt->screen.float_render[i].b / divide) * 255)));
				x++;
			}
			y++;
		}
	}
	else
	{
		while (y < minirt->scene.win_height)
		{
			x = 0;
			while (x < minirt->scene.win_width)
			{
				i = (int)((float)y / (minirt->scene.win_height - 1) * (minirt->scene.render_height - 1)) * minirt->scene.render_width + (int)((float)x / (minirt->scene.win_width - 1) * (minirt->scene.render_width - 1));
				minirt->screen.render[y * (minirt->scene.win_width) + x] = ((int)(
					((int)(clamp_double(pow(minirt->screen.float_render[i].r / divide, gamma_corr)) * 255) << 16) + 
					((int)(clamp_double(pow(minirt->screen.float_render[i].g / divide, gamma_corr)) * 255) << 8) + 
					(int)(clamp_double(pow(minirt->screen.float_render[i].b / divide, gamma_corr)) * 255)));
				x++;
			}
			y++;
		}
	}
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
		put_pixel_image(&minirt->mlx.img_controls, i % minirt->micrort.viewport.render_w + 301, i / minirt->micrort.viewport.render_w + 861, ((int)(
			((int)(clamp_double(minirt->micrort.render[i].r / divide) * 255) << 16) + 
			((int)(clamp_double(minirt->micrort.render[i].g / divide) * 255) << 8) + 
			(int)(clamp_double(minirt->micrort.render[i].b / divide) * 255))));
		i++;
	}
	
}
