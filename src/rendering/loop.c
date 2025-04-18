/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 15:31:03 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/18 13:13:27 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "bvh.h"
#include "bmp_parsing.h"

void	render_controls(t_minirt *minirt)
{
	int	i;

	if (!minirt->mlx.controls_win)
		return ;
	ft_bzero(minirt->mlx.img_controls.img_str, minirt->mlx.img_controls.width * minirt->mlx.img_controls.height * 4);
	minirt->controls.font[0].size = 20;
	minirt->controls.font[0].color = 0;
	i = -1;
	while (++i < minirt->controls.nb_buttons)
		display_button(&minirt->mlx.img_controls, minirt->controls.buttons[i], &minirt->controls.font[0]);
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

void render_frame(t_minirt *minirt)
{
	render(minirt);
	render_bvh(minirt);
	render_controls(minirt);
	minirt->stats.frame += 1;
}
