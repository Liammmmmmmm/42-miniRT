/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   no_upscaling.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 19:26:48 by madelvin          #+#    #+#             */
/*   Updated: 2025/05/15 14:09:11 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

// void	no_upscaling(t_minirt *minirt)
// {
// 	int			i;
// 	int			tpix;
// 	t_sc_point	point;
// 	int			divide;

// 	divide = minirt->screen.last_sample_am;
// 	if (divide == 0)
// 		divide = 1;
// 	i = 0;
// 	tpix = minirt->mlx.img.width * minirt->mlx.img.height;
// 	point.z = 0;
// 	while (i < tpix)
// 	{
// 		point.x = minirt->screen.render[i].x;
// 		point.y = minirt->screen.render[i].y;
// 		point.color.r = minirt->screen.render[i].color.r / divide;
// 		point.color.g = minirt->screen.render[i].color.g / divide;
// 		point.color.b = minirt->screen.render[i].color.b / divide;
// 		put_sp_image(&minirt->mlx.img, &point);
// 		i++;
// 	}
// }

void	simplified_put_image(t_minirt *minirt, int tpix, t_sc_point point, int divide)
{
	int			i;

	i = 0;
	while (i < tpix)
	{
		point.x = i % minirt->mlx.img.width;
		point.y = i / minirt->mlx.img.width;
		
		point.color.r = clamp_double(minirt->screen.float_render[i].r / divide) * 255;
		point.color.g = clamp_double(minirt->screen.float_render[i].g / divide) * 255;
		point.color.b = clamp_double(minirt->screen.float_render[i].b / divide) * 255;
		put_sp_image(&minirt->mlx.img, &point);
		i++;
	}
}

void	no_upscaling_float(t_minirt *minirt)
{
	int			i;
	int			tpix;
	t_sc_point	point;
	int			divide;
	double		gamma_corr;
	
	divide = minirt->screen.last_sample_am;
	if (divide == 0)
		divide = 1;
	i = 0;
	gamma_corr = 1.0 / (minirt->controls.values.gamma / 10.0); // 1 gamma default, la correction est deja appliquee par l'écran ._.
	tpix = minirt->mlx.img.width * minirt->mlx.img.height;
	point.z = 0;
	if (gamma_corr == 1.0)
		return (simplified_put_image(minirt, tpix, point, divide));
	while (i < tpix)
	{
		point.x = i % minirt->mlx.img.width;
		point.y = i / minirt->mlx.img.width;
		
		point.color.r = pow(clamp_double(minirt->screen.float_render[i].r / divide), gamma_corr) * 255;
		point.color.g = pow(clamp_double(minirt->screen.float_render[i].g / divide), gamma_corr) * 255;
		point.color.b = pow(clamp_double(minirt->screen.float_render[i].b / divide), gamma_corr) * 255;
		put_sp_image(&minirt->mlx.img, &point);
		i++;
	}
}

void	copy_buff_to_image(t_minirt *minirt)
{
	int	i;
	int	tpx;
	
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

	divide = minirt->screen.last_sample_am;
	if (divide == 0)
		divide = 1;
	y = 0;
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
	copy_buff_to_image(minirt);
}
