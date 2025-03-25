/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   maths.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 17:55:09 by madelvin          #+#    #+#             */
/*   Updated: 2025/03/25 21:32:44 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MATHS_H
# define MATHS_H

# include "structs.h"

/*	Basic operations	*/
t_vec3	vec3_subtract(const t_vec3 u, const t_vec3 v) __attribute__((always_inline));
t_vec3	vec3_add(const t_vec3 u, const t_vec3 v) __attribute__((always_inline));
t_vec3	vec3_multiply(const t_vec3 u, const t_vec3 v) __attribute__((always_inline));
t_vec3	vec3_multiply_scalar(const t_vec3 v, double t) __attribute__((always_inline));
t_vec3	vec3_divide_scalar(const t_vec3 v, double t) __attribute__((always_inline));

/*	Length and squared length calculation	*/
double	vec3_length(const t_vec3 v) __attribute__((always_inline));
double	vec3_length_squared(const t_vec3 v) __attribute__((always_inline));

/*	Dot product and cross product	*/
double	vec3_dot(const t_vec3 u, const t_vec3 v) __attribute__((always_inline));
t_vec3	vec3_cross(const t_vec3 u, const t_vec3 v) __attribute__((always_inline));

/*	Utility functions	*/
t_vec3	vec3_unit(const t_vec3 v) __attribute__((always_inline));
t_vec3	vec3_negate(const t_vec3 v) __attribute__((always_inline));
t_vec3	vec3_init(double x, double y, double z) __attribute__((always_inline));

char	hit_sphere(const t_vec3 center, double radius, const t_ray *r, \
	t_interval interval, t_hit_record *rec) __attribute__((always_inline));
char	hit_plane(const t_vec3 point_on_plane, const t_vec3 normal, \
	const t_ray *r, t_interval interval, t_hit_record *rec);
char	hit_register(t_minirt *minirt, t_ray ray, t_hit_record *hit_record) __attribute__((always_inline));
t_vec3	set_normal_face(const t_ray *r, const t_vec3 *outward_normal, t_hit_record	*hit_record) __attribute__((always_inline));
t_vec3	ray_at(t_ray r, double t) __attribute__((always_inline));
char	mix_mat_sphere(t_hit_record hit_record, t_ray ray_in, t_ray *scattered, t_mat material, t_color *attenuation, t_minirt *minirt);

#endif