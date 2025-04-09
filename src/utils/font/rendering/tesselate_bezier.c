/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tesselate_bezier.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 12:24:16 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/09 10:04:05 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "font.h"

void	tessellate_bezier(t_point2 *output, uint32_t *output_size, t_bezier *pts)
{
	uint32_t subdiv_into = *output_size;
	uint32_t	i;
	_Float32 step_per_iter = 1.0/subdiv_into;

	i = -1;
	while (++i <= subdiv_into)
	{
		_Float32 t = i*step_per_iter;
		_Float32 t1 = (1.0 - t);
		_Float32 t2 = t*t;
		output[i].x = t1*t1*pts->p1.x + 2*t1*t*pts->pc.x + t2*pts->p2.x;
		output[i].y = t1*t1*pts->p1.y + 2*t1*t*pts->pc.y + t2*pts->p2.y;
	}
}
