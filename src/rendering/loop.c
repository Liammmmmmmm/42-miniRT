/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 15:31:03 by lilefebv          #+#    #+#             */
/*   Updated: 2025/03/24 11:12:23 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

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
	render(minirt);

	// put_render_to_frame(minirt);
	// mlx_put_image_to_window(minirt->mlx.mlx, minirt->mlx.render_win, minirt->mlx.img.img, 0, 0);
	minirt->stats.frame += 1;
}

