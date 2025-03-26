/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3_length.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 17:48:14 by madelvin          #+#    #+#             */
/*   Updated: 2025/03/26 13:42:42 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "maths.h"
#include <math.h>

inline double	vec3_length_squared(const t_vec3 v)
{
	return (v.x * v.x + v.y * v.y + v.z * v.z);
}

inline double	vec3_length(const t_vec3 v)
{
	return (sqrt(vec3_length_squared(v)));
}
