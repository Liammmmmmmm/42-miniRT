/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec4_operations.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 10:14:27 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/11 13:22:35 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "maths.h"
#include "vec4.h"

inline t_vec4	vec4_add(t_vec4 u, t_vec4 v)
{
	return ((t_vec4){_mm256_add_pd(u.data, v.data)});
}

inline t_vec4	vec4_sub(t_vec4 u, t_vec4 v)
{
	return ((t_vec4){_mm256_sub_pd(u.data, v.data)});
}

inline t_vec4	vec4_mul(t_vec4 u, t_vec4 v)
{
	return ((t_vec4){_mm256_mul_pd(u.data, v.data)});
}

inline t_vec4	vec4_div(t_vec4 u, t_vec4 v)
{
	return ((t_vec4){_mm256_div_pd(u.data, v.data)});
}
