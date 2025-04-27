/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   neighbour.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 19:02:13 by madelvin          #+#    #+#             */
/*   Updated: 2025/04/26 20:12:04 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	neighbor_upscale(t_minirt *minirt)
{
	t_upscale_data		d;
	register int		i;
	const int			tpi = minirt->mlx.img.width * minirt->mlx.img.height;
	const t_lsc_point	*render;
	t_sc_point			p;
	register int		src_index;

	d.sw = minirt->viewport.render_w;
	d.sh = minirt->viewport.render_h;
	d.dw = minirt->mlx.img.width;
	d.dh = minirt->mlx.img.height;
	d.divide = minirt->screen.last_sample_am;
	if (d.divide == 0)
		d.divide = 1;
	d.scaleX = (float)(d.sw) / (d.dw);
	d.scaleY = (float)(d.sh) / (d.dh);
	render = minirt->screen.render;
	i = 0;
	p.z = 0;
	while (i < tpi)
	{
		src_index = (int)(i / d.dw * d.scaleY) * d.sw + (int)(i % d.dw * d.scaleX);
		if (src_index >= d.sw * d.sh)
			src_index = d.sw * d.sh - 1;
		p.x = render[i].x;
		p.y = render[i].y;
		p.color = (t_color){ render[src_index].color.r / d.divide, \
			render[src_index].color.g / d.divide,
			render[src_index].color.b / d.divide};
		put_sp_image(&minirt->mlx.img, &p);
		i++;
	}
}
