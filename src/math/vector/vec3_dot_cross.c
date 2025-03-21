/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3_dot_cross.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 17:49:23 by madelvin          #+#    #+#             */
/*   Updated: 2025/03/21 10:52:54 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"

double	vec3_dot(const t_vec3 *u, const t_vec3 *v)
{
	return (u->x * v->x + u->y * v->y + u->z * v->z);
}

t_vec3	vec3_cross(const t_vec3 *u, const t_vec3 *v)
{
	return ((t_vec3){
		u->y * v->z - u->z * v->y,
		u->z * v->x - u->x * v->z,
		u->x * v->y - u->y * v->x
	});
}
