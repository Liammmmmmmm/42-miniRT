/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 18:01:54 by madelvin          #+#    #+#             */
/*   Updated: 2025/03/21 10:53:04 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "maths.h"

t_vec3	ray_at(t_ray *r, double t)
{
	return ((t_vec3){
		r->orig.x + t * r->dir.x,
		r->orig.y + t * r->dir.y,
		r->orig.z + t * r->dir.z
	});
}
