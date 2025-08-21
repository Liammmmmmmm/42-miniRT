/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec4.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 12:43:13 by lilefebv          #+#    #+#             */
/*   Updated: 2025/08/21 20:07:45 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VEC4_H
# define VEC4_H

# include "basic_structs.h"
# include <immintrin.h>

typedef struct s_vec4
{
	__m256d	data;
}	t_vec4;

t_vec3	vec4_to_vec3(t_vec4 v);
t_vec4	vec3_to_vec4(t_vec3 vec3);
t_vec4	vec4_div(t_vec4 u, t_vec4 v);
t_vec4	vec4_mul(t_vec4 u, t_vec4 v);
t_vec4	vec4_sub(t_vec4 u, t_vec4 v);
t_vec4	vec4_add(t_vec4 u, t_vec4 v);

t_vec4	vec4_random(void);

t_vec4	vec4_multiply_scalar(t_vec4 v, double t);
t_vec4	vec4_divide_scalar(t_vec4 v, double t);

t_vec4	vec4_unit(t_vec4 v);
double	vec4_length(t_vec4 v);
t_vec4	vec4_negate(t_vec4 v);

t_vec4	vec4_random_unit(void);
t_vec4	vec4_random_in_interval(double min, double max);

#endif