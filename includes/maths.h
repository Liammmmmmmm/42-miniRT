/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   maths.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 17:55:09 by madelvin          #+#    #+#             */
/*   Updated: 2025/04/17 23:23:50 by madelvin         ###   ########.fr       */
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
t_vec3	vec3_add_scalar(t_vec3 vec, double t);
t_vec3	vec3_subtract_scalar(t_vec3 vec, double t);

/*	Length and squared length calculation	*/
double	vec3_length(const t_vec3 v);
double	vec3_length_squared(const t_vec3 v);

/*	Dot product and cross product	*/
double	vec3_dot(register t_vec3 u, register t_vec3 v);
t_vec3	vec3_cross(const t_vec3 u, const t_vec3 v);

t_vec3	vec3_fmax(t_vec3 a, t_vec3 b);
t_vec3	vec3_fmin(t_vec3 a, t_vec3 b);

/*	Utility functions	*/
t_vec3	vec3_unit(const t_vec3 v);
t_vec3	vec3_negate(const t_vec3 v);
t_vec3	vec3_init(double x, double y, double z);
t_vec3	vec3_random();

double	ft_dmin(double a, double b);

t_color	color_add(t_color c1, t_color c2);
t_color	color_sub(t_color c1, t_color c2);
t_color	color_scale(t_color c1, double ratio);
t_color	color_multiply(t_color c1, t_color c2);

t_vec3	vec3_random();
t_vec3	vec3_random_unit();
t_vec3	vec3_random_sample();
t_vec3	vec3_random_on_hemisphere(const t_vec3 normal);
t_vec3	vec3_random_in_interval(double min, double max);

double	ft_dmin(double a, double b);
double	ft_dmax(double a, double b);
double	random_double();
double	random_double_in_interval(double min, double max);

void	init_yaw_matrix(double matrix[3][3], t_calc_trigo trigo_calcs);
void	init_roll_matrix(double matrix[3][3], t_calc_trigo trigo_calcs);
void	init_pitch_matrix(double matrix[3][3], t_calc_trigo trigo_calcs);
void	init_perspective_matrix(double matrix[4][4], t_minirt *minirt);

void	vector_multiply_matrix_3x3(double matrix[3][3], double v[3]);
void	vector_multiply_matrix_4x4(double m[4][4], double v[4]);
void	multiply_matrix_3x3(double res[3][3], double a[3][3], double b[3][3]);
void	multiply_matrix_4x4(double res[4][4], double a[4][4], double b[4][4]);

void	vector_to_angles(double vec[3], double *yaw, double *pitch);
void	angles_to_vector(double yaw, double pitch, double vec[3]);

char	hit_sphere(t_sphere *sphere, t_ray *r, \
	t_interval interval, t_hit_record *rec);
char	hit_plane(const t_vec3 point_on_plane, const t_vec3 normal, \
	const t_ray *r, t_interval interval, t_hit_record *rec);
char	hit_register_all(t_minirt *minirt, t_ray *ray, t_hit_record *hit_record);
char	hit_register_bvh(t_bvh *bvh, t_bvh_node *node, t_ray *ray, t_hit_record *hit_record);
t_vec3	set_normal_face(const t_ray *r, const t_vec3 *outward_normal, t_hit_record	*hit_record);
t_vec3	ray_at(t_ray r, double t);


// MATRIX

t_vec3	matrix3_dot_vec3(double matrix[3][3], const t_vec3 v);

// FORMULES

/**
 * `F = F0​+(1−F0​)(1−cos(θ))^5`
 */
t_color	fresnel_schlick_color(double cos_theta, t_color f0);

/**
 * `F = F0​+(1−F0​)(1−cos(θ))^5`
 */
double	fresnel_schlick_f(double cos_theta, double f0);


#endif