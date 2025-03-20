/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 15:55:21 by lilefebv          #+#    #+#             */
/*   Updated: 2025/03/20 16:24:41 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	basic_image(t_minirt *minirt)
{
	t_uint	tpix;
	t_uint	i;
	t_uint	y;
	t_color	color;
	
	tpix = minirt->mlx.img.width * minirt->mlx.img.height;
	i = 0;
	y = 0;
	while (i < minirt->mlx.img.height)
	{
		y = 0;
		while (y < minirt->mlx.img.width)
		{
			color.r = i * 255 / minirt->mlx.img.height;
			color.g = y * 255 / minirt->mlx.img.width;
			color.b = (minirt->mlx.img.height - i) * 255 / minirt->mlx.img.height;
			
			minirt->screen.render[minirt->mlx.img.width * i + y].color = color;
			y++;
		}
		i++;
	}

}

void	render(t_minirt *minirt)
{
	basic_image(minirt);
}
