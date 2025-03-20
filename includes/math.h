/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 17:55:09 by madelvin          #+#    #+#             */
/*   Updated: 2025/03/20 18:08:52 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MATH_H
# define MATH_H

# include "structs.h"

/*	Basic operations	*/
inline t_vec3	vec3_add(const t_vec3 *u, const t_vec3 *v);
inline t_vec3	vec3_subtract(const t_vec3 *u, const t_vec3 *v);
inline t_vec3	vec3_multiply(const t_vec3 *u, const t_vec3 *v);
inline t_vec3	vec3_multiply_scalar(const t_vec3 *v, double t);
inline t_vec3	vec3_divide_scalar(const t_vec3 *v, double t);

/*	Length and squared length calculation	*/
inline double	vec3_length(const t_vec3 *v);
inline double	vec3_length_squared(const t_vec3 *v);

/*	Dot product and cross product	*/
inline double	vec3_dot(const t_vec3 *u, const t_vec3 *v);
inline t_vec3	vec3_cross(const t_vec3 *u, const t_vec3 *v);

/*	Utility functions	*/
inline t_vec3	vec3_negate(const t_vec3 *v);
inline t_vec3	vec3_unit(const t_vec3 *v);

#endif