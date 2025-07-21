/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   normal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 16:08:12 by madelvin          #+#    #+#             */
/*   Updated: 2025/07/21 14:26:08 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "maths.h"

t_vec3	set_normal_face(const t_ray *r, const t_vec3 *outward_normal, \
	t_hit_record	*hit_record)
{
	char	front_face;

	front_face = vec3_dot(r->dir, *outward_normal) < 0;
	if (front_face)
	{
		hit_record->front_face = 1;
		return (*outward_normal);
	}
	hit_record->front_face = 0;
	return (vec3_negate(*outward_normal));
}
