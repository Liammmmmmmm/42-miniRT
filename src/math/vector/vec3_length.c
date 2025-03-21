/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3_length.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 17:48:14 by madelvin          #+#    #+#             */
/*   Updated: 2025/03/21 10:52:51 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include <math.h>

double	vec3_length_squared(const t_vec3 *v)
{
	return (v->x * v->x + v->y * v->y + v->z * v->z);
}

double	vec3_length(const t_vec3 *v)
{
	return (sqrt(vec3_length_squared(v)));
}
