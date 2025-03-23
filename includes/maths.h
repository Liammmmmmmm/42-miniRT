/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   maths.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 17:55:09 by madelvin          #+#    #+#             */
/*   Updated: 2025/03/23 19:16:14 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MATHS_H
# define MATHS_H

# include "structs.h"

/*	Basic operations	*/
t_vec3	vec3_subtract(const t_vec3 u, const t_vec3 v);
t_vec3	vec3_add(const t_vec3 u, const t_vec3 v);
t_vec3	vec3_multiply(const t_vec3 u, const t_vec3 v);
t_vec3	vec3_multiply_scalar(const t_vec3 v, float t);
t_vec3	vec3_divide_scalar(const t_vec3 v, float t);

/*	Length and squared length calculation	*/
float	vec3_length(const t_vec3 v);
float	vec3_length_squared(const t_vec3 v);

/*	Dot product and cross product	*/
float	vec3_dot(const t_vec3 u, const t_vec3 v);
t_vec3	vec3_cross(const t_vec3 u, const t_vec3 v);

/*	Utility functions	*/
t_vec3	vec3_unit(const t_vec3 v);
t_vec3	vec3_negate(const t_vec3 v);
t_vec3	vec3_init(float x, float y, float z);

char	hit_sphere(const t_vec3 center, float radius, const t_ray *r, \
	t_interval interval, t_hit_record *rec);
char	hit_register(t_minirt *minirt, t_ray ray, t_hit_record *hit_record, \
	t_color *obj_color);
t_vec3	set_normal_face(const t_ray *r, const t_vec3 *outward_normal);
t_vec3	ray_at(t_ray r, float t);

#endif