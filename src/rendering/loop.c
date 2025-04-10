/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 15:31:03 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/10 19:33:47 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "bmp_parsing.h"

void	render_buttons(t_minirt *minirt)
{
	int	i;

	if (!minirt->mlx.controls_win)
		return ;
	ft_bzero(minirt->mlx.img_controls.img_str, minirt->mlx.img_controls.width * minirt->mlx.img_controls.height * 4);
	i = -1;
	while (++i < minirt->controls.nb_buttons)
		display_button(&minirt->mlx.img_controls, minirt->controls.buttons[i], minirt->controls.font);
	i = -1;
	while (++i < minirt->controls.nb_sliders)
		display_slider_int(&minirt->mlx.img_controls, minirt->controls.sliders[i]);
	mlx_put_image_to_window(minirt->mlx.mlx, minirt->mlx.controls_win, minirt->mlx.img_controls.img, 0, 0);
}

void	put_render_to_frame(t_minirt *minirt)
{
	int			i;
	int			tpix;
	t_sc_point	point;

	if (minirt->screen.sample == 0)
		return ;
	i = 0;
	tpix = minirt->mlx.img.width * minirt->mlx.img.height;
	point.z = 0;
	while (i < tpix)
	{
		point.x = minirt->screen.render[i].x;
		point.y = minirt->screen.render[i].y;
		point.color.r = minirt->screen.render[i].color.r / minirt->screen.sample;
		point.color.g = minirt->screen.render[i].color.g / minirt->screen.sample;
		point.color.b = minirt->screen.render[i].color.b / minirt->screen.sample;
		put_sp_image(&minirt->mlx.img, &point);
		i++;
	}
}

void	render_frame(t_minirt *minirt)
{
	int i = -1;
	int tpix = minirt->mlx.img.width * minirt->mlx.img.height;
	// t_sc_point	point;
	i = -1;
	while (++i < tpix)
	{
		uint32_t x = i % minirt->bmp.info.with;
		uint32_t y = i / minirt->bmp.info.with;
		if (x < minirt->bmp.info.with && y < minirt->bmp.info.height)
		{
			// point.x = x;
			// point.y = y;
			// point.color.r = (minirt->bmp.pixel_data[y * minirt->bmp.info.with + x] >> 16) & 0xFF;
			// point.color.g = (minirt->bmp.pixel_data[y * minirt->bmp.info.with + x]  >> 8) & 0xFF;
			// point.color.b = minirt->bmp.pixel_data[y * minirt->bmp.info.with + x]  & 0xFF;
			((int *)minirt->mlx.img.img_str + (x) + (minirt->mlx.img.width * y))[0] = \
			(int)(minirt->bmp.pixel_data[y * minirt->bmp.info.with + x]);
			// put_sp_image(&minirt->mlx.img, &point);
		}
	}
	mlx_put_image_to_window(minirt->mlx.mlx, minirt->mlx.render_win, minirt->mlx.img.img, 0, 0);
	minirt->stats.frame += 1;
}

