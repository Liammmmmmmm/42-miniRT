/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tesselate_bezier.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 12:24:16 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/04 12:34:17 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "font.h"

void tessellate_bezier(t_point2 *output, uint32_t *output_size, t_bezier *pts)
{
	int32_t subdiv_into = 5;
	_Float32 step_per_iter = 1.0/subdiv_into;
	int32_t out_size = 0;
	for(int i = 0; i <= subdiv_into; i++) {
		_Float32 t = i*step_per_iter;
		_Float32 t1 = (1.0 - t);
		_Float32 t2 = t*t;
		_Float32 x = t1*t1*pts->p1.x + 2*t1*t*pts->pc.x + t2*pts->p2.x;
		_Float32 y = t1*t1*pts->p1.y + 2*t1*t*pts->pc.y + t2*pts->p2.y;
		output[out_size].x = x;
		output[out_size].y = y;
		out_size++;
	}
	
	*output_size = out_size;
 }
