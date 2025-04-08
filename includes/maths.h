/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   maths.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 17:55:09 by madelvin          #+#    #+#             */
/*   Updated: 2025/04/08 20:00:43 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MATHS_H
# define MATHS_H

# include "structs.h"

/*	Basic operations	*/
t_vec3	vec3_subtract(register t_vec3 u, register t_vec3 v);
t_vec3	vec3_add(const t_vec3 u, const t_vec3 v);
t_vec3	vec3_multiply(const t_vec3 u, const t_vec3 v);
t_vec3	vec3_multiply_scalar(const t_vec3 v, double t);
t_vec3	vec3_divide_scalar(const t_vec3 v, double t);

/*	Length and squared length calculation	*/
double	vec3_length(const t_vec3 v);
double	vec3_length_squared(const t_vec3 v);

/*	Dot product and cross product	*/
double	vec3_dot(register t_vec3 u, register t_vec3 v);
t_vec3	vec3_cross(const t_vec3 u, const t_vec3 v);

/*	Utility functions	*/
t_vec3	vec3_unit(const t_vec3 v);
t_vec3	vec3_negate(const t_vec3 v);
t_vec3	vec3_init(double x, double y, double z);
t_vec3	vec3_random();

double	ft_dmin(double a, double b);

t_color	color_add(t_color c1, t_color c2);
t_color	color_scale(t_color c1, double ratio);
t_color	color_multiply(t_color c1, t_color c2);

t_vec3	vec3_random();
t_vec3	vec3_random_unit();
t_vec3	vec3_random_sample();
t_vec3	vec3_random_on_hemisphere(const t_vec3 normal);
t_vec3	vec3_random_in_interval(double min, double max);

double	random_double();
double	random_double_in_interval(double min, double max);

char hit_sphere(t_sphere sphere, t_ray *r, \
	t_interval interval, t_hit_record *rec);
char	hit_plane(const t_vec3 point_on_plane, const t_vec3 normal, \
	const t_ray *r, t_interval interval, t_hit_record *rec);
char	hit_register(t_minirt *minirt, t_ray ray, t_hit_record *hit_record);

t_vec3	set_normal_face(const t_ray *r, const t_vec3 *outward_normal, t_hit_record	*hit_record);

t_vec3	ray_at(t_ray r, double t);

#endif