/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 18:01:54 by madelvin          #+#    #+#             */
/*   Updated: 2025/03/23 19:16:14 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "maths.h"

t_vec3	ray_at(t_ray r, float t)
{
	return ((t_vec3){
		r.orig.x + t * r.dir.x,
		r.orig.y + t * r.dir.y,
		r.orig.z + t * r.dir.z
	});
}
