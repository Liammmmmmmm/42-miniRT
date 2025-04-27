/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   no_upscaling.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 19:26:48 by madelvin          #+#    #+#             */
/*   Updated: 2025/04/27 18:30:35 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	no_upscaling(t_minirt *minirt)
{
	int			i;
	int			tpix;
	t_sc_point	point;
	int			divide;

	divide = minirt->screen.last_sample_am;
	if (divide == 0)
		divide = 1;
	i = 0;
	tpix = minirt->mlx.img.width * minirt->mlx.img.height;
	point.z = 0;
	while (i < tpix)
	{
		point.x = minirt->screen.render[i].x;
		point.y = minirt->screen.render[i].y;
		point.color.r = minirt->screen.render[i].color.r / divide;
		point.color.g = minirt->screen.render[i].color.g / divide;
		point.color.b = minirt->screen.render[i].color.b / divide;
		put_sp_image(&minirt->mlx.img, &point);
		i++;
	}
}
