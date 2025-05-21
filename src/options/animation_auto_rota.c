/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   animation_auto_rota.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 10:01:23 by lilefebv          #+#    #+#             */
/*   Updated: 2025/05/21 12:58:39 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "options.h"
#include "maths.h"

void	set_auto_orientation_points(t_obj_anim *obj)
{
	t_uint	i;

	i = (t_uint)-1;
	while (++i < obj->frames)
	{
		if (i != obj->frames - 1)
			obj->orientations[i] = vec3_unit(vec3_subtract(obj->points[i + 1],
						obj->points[i]));
		else
			obj->orientations[i] = obj->orientations[i - 1];
	}
}
