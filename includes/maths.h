/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   maths.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 17:55:09 by madelvin          #+#    #+#             */
/*   Updated: 2025/03/21 12:22:44 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef MATH_H
# define MATH_H

# include "structs.h"

/*	Basic operations	*/
t_vec3	vec3_subtract(const t_vec3 u, const t_vec3 v);
t_vec3	vec3_add(const t_vec3 u, const t_vec3 v);
t_vec3	vec3_multiply(const t_vec3 u, const t_vec3 v);
t_vec3	vec3_multiply_scalar(const t_vec3 v, double t);
t_vec3	vec3_divide_scalar(const t_vec3 v, double t);

/*	Length and squared length calculation	*/
double	vec3_length(const t_vec3 v);
double	vec3_length_squared(const t_vec3 v);

/*	Dot product and cross product	*/
double	vec3_dot(const t_vec3 u, const t_vec3 v);
t_vec3	vec3_cross(const t_vec3 u, const t_vec3 v);

/*	Utility functions	*/
t_vec3	vec3_unit(const t_vec3 v);
t_vec3	vec3_negate(const t_vec3 v);

#endif