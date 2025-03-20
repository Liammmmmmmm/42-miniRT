/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 18:01:54 by madelvin          #+#    #+#             */
/*   Updated: 2025/03/20 18:12:58 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "math.h"

inline t_vec3 ray_at(t_ray *r, double t)
{
	return (t_vec3){
		r->orig.x + t * r->dir.x,
		r->orig.y + t * r->dir.y,
		r->orig.z + t * r->dir.z
	};
}
