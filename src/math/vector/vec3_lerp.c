/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3_lerp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 11:37:18 by lilefebv          #+#    #+#             */
/*   Updated: 2025/05/07 10:29:03 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "maths.h"
#include <math.h>

inline t_vec3	vec3_lerp(t_vec3 a, t_vec3 b, double t)
{
	return ((t_vec3){
		(1 - t) * a.x + t * b.x,
		(1 - t) * a.y + t * b.y,
		(1 - t) * a.z + t * b.z
	});
}
