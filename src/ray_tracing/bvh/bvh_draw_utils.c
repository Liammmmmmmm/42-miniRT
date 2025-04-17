/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh_draw_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 18:42:13 by madelvin          #+#    #+#             */
/*   Updated: 2025/04/17 18:43:10 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "minirt.h"

uint32_t	hash32(uint32_t x)
{
	x ^= x >> 16;
	x *= 0x7feb352d;
	x ^= x >> 15;
	x *= 0x846ca68b;
	x ^= x >> 16;
	return (x);
}

int	depth_to_color_int(int depth)
{
	const uint32_t	seed = hash32((uint32_t)depth);
	const uint8_t	r = (seed >> 16) & 0xFF;
	const uint8_t	g = (seed >> 8) & 0xFF;
	const uint8_t	b = seed & 0xFF;

	return ((r << 16) | (g << 8) | b);
}

t_calc_trigo	get_cam_trigo(t_vec3 orientation)
{
	t_calc_trigo	trigo;

	trigo.cos_yaw = cos(orientation.x * PI_10D);
	trigo.sin_yaw = sin(orientation.x * PI_10D);
	trigo.cos_pitch = cos(orientation.y * PI_10D);
	trigo.sin_pitch = sin(orientation.y * PI_10D);
	trigo.cos_roll = cos(orientation.z * PI_10D);
	trigo.sin_roll = sin(orientation.z * PI_10D);
	return (trigo);
}

void	project_vertex(t_minirt *m, t_vec3 v, int *x, int *y)
{
	double			vec[4];
	double			mat[3][3];
	double			persp[4][4];
	t_calc_trigo	t;

	t = get_cam_trigo(m->scene.camera.orientation);
	vec[0] = m->scene.camera.position.x - v.x;
	vec[1] = m->scene.camera.position.y - v.y;
	vec[2] = m->scene.camera.position.z - v.z;
	vec[3] = 1;
	init_roll_matrix(mat, t);
	vector_multiply_matrix_3x3(mat, vec);
	init_yaw_matrix(mat, t);
	vector_multiply_matrix_3x3(mat, vec);
	init_pitch_matrix(mat, t);
	vector_multiply_matrix_3x3(mat, vec);
	init_perspective_matrix(persp, m);
	vector_multiply_matrix_4x4(persp, vec);
	*x = ((vec[0] / vec[3]) + 1) * 0.5 * WIN_WIDTH;
	*y = (1 - (vec[1] / vec[3])) * 0.5 * WIN_HEIGHT;
}

void	init_box_vertices(t_vec3 *v, t_vec3 min, t_vec3 max)
{
	v[0] = (t_vec3){min.x, min.y, min.z};
	v[1] = (t_vec3){max.x, min.y, min.z};
	v[2] = (t_vec3){max.x, max.y, min.z};
	v[3] = (t_vec3){min.x, max.y, min.z};
	v[4] = (t_vec3){min.x, min.y, max.z};
	v[5] = (t_vec3){max.x, min.y, max.z};
	v[6] = (t_vec3){max.x, max.y, max.z};
	v[7] = (t_vec3){min.x, max.y, max.z};
}
