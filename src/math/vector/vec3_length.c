/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3_length.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 17:48:14 by madelvin          #+#    #+#             */
/*   Updated: 2025/03/23 19:16:14 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include <math.h>

float	vec3_length_squared(const t_vec3 v)
{
	return (v.x * v.x + v.y * v.y + v.z * v.z);
}

float	vec3_length(const t_vec3 v)
{
	return (sqrt(vec3_length_squared(v)));
}
