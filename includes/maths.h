/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   maths.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 17:55:09 by madelvin          #+#    #+#             */
/*   Updated: 2025/07/17 12:26:08 by madelvin         ###   ########.fr       */
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
t_vec3	vec3_divide_safe(t_vec3 a, t_vec3 b, double	min);
t_vec3	vec3_lerp(t_vec3 a, t_vec3 b, double t);

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
t_vec3	vec3_random();

t_vec3	vec3_rotate(t_vec3 v, t_vec3 axis, float angle);

double	ft_dmin(double a, double b);

t_fcolor	color_to_fcolor(t_color color);
t_fcolor	rgba_to_fcolor(t_rgba color);
t_fcolor	multiply_scalar_fcolor(t_fcolor c1, double c);
t_fcolor	multiply_fcolor(t_fcolor c1, t_fcolor c2);
t_fcolor	add_fcolor(t_fcolor c1, t_fcolor c2);
t_fcolor	sub_fcolor(t_fcolor c1, t_fcolor c2);
t_fcolor	clamp_fcolor(t_fcolor color);
t_fcolor	clamp_fcolor_val(t_fcolor color, double min, double max);
t_fcolor	get_heatmap_fcolor(int current_value, int max_value);

t_vec3	vec3_random();
t_vec3	vec3_random_neg();
t_vec3	vec3_random_unit();
t_vec3	vec3_random_in_interval(double min, double max);
t_vec3	random_in_unit_sphere(void);

double	ft_dmin(double a, double b);
double	ft_dmax(double a, double b);

double	random_double();
double	random_double_in_interval(double min, double max);
double	clamp_double_val(double v, double min, double max);
float	clampf(float x, float min, float max);

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

char	solve_quadratic(t_quadratic *q);
char	init_cylinder_quadratic(t_quadratic *q, t_cylinder *cyl, t_ray *r);
char	init_cone_quadratic(t_quadratic *q, t_cone *cone, t_ray *r);
char	init_sphere_quadratic(t_quadratic *q, t_sphere *s, t_ray *r);
char	init_hyperboloid_quadratic(t_quadratic *q, t_hyperboloid *h, t_ray *r);

t_vec3	vec3_min(t_vec3 a, t_vec3 b);
t_vec3	vec3_max(t_vec3 a, t_vec3 b);

char	hit_sphere(t_sphere *sphere, t_ray *r, t_hit_record *rec);
char	hit_plane(t_plane *plane, const t_ray *r, t_hit_record *rec);
char	hit_cylinder(t_cylinder *cylinder, t_ray *r, t_hit_record *rec);
char	hit_cone(t_cone *cone, t_ray *ray, t_hit_record *rec);
char	hit_triangle(t_triangle *t, t_ray *r, t_hit_record *rec);
char	hit_register_all(t_minirt *minirt, t_hit_register_data *data);
char	hit_hyperboloid(t_hyperboloid *hyp, t_ray *r, t_hit_record *rec,
	double interval_max);
t_vec3	set_normal_face(const t_ray *r, const t_vec3 *outward_normal, t_hit_record	*hit_record);
t_vec3	ray_at(t_ray r, double t);

void	get_cylinder_cap_uv(t_hit_record *rec, t_cylinder *cyl, int top);
void	get_cylinder_lateral_uv(t_hit_record *rec, t_cylinder *cyl);

char	valid_t(t_quadratic *q);

// MATRIX

t_vec3	matrix3_dot_vec3(double matrix[3][3], const t_vec3 v);

// FORMULES

/**
 * `F = F0​+(1−F0​)(1−cos(θ))^5`
 */
t_fcolor	fresnel_schlick_color(double cos_theta, t_fcolor f0);

/**
 * `F = F0​+(1−F0​)(1−cos(θ))^5`
 */
double	fresnel_schlick_f(double cos_theta, double f0);

int		iclamp(int min, int val, int max);
int		imin(int i1, int i2);
int		imax(int i1, int i2);
double	clamp_double(double v);


#endif