/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh_draw_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 18:42:13 by madelvin          #+#    #+#             */
/*   Updated: 2025/04/18 13:18:47 by madelvin         ###   ########.fr       */
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

t_calc_trigo	get_cam_trigo(t_vec3 direction)
{
	t_calc_trigo	trigo;
	t_vec3			angles;
	double			yaw;
	double			pitch;
	double			direction_d[3];

	direction_d[0] = direction.x;
	direction_d[1] = direction.y;
	direction_d[2] = direction.z;
	vector_to_angles(direction_d, &yaw, &pitch);
	angles = (t_vec3){0, -pitch, yaw + 1.570796};
	trigo.cos_yaw = cos(angles.x);
	trigo.sin_yaw = sin(angles.x);
	trigo.cos_pitch = cos(angles.y);
	trigo.sin_pitch = sin(angles.y);
	trigo.cos_roll = cos(angles.z);
	trigo.sin_roll = sin(angles.z);
	return (trigo);
}

void	project_vertex(t_minirt *minirt, t_vec3 v, int *x, int *y)
{
	double			vec[4];
	double			mat[3][3];
	double			persp[4][4];
	t_calc_trigo	calc_trigo;

	calc_trigo = get_cam_trigo(minirt->scene.camera.orientation);
	vec[0] = minirt->scene.camera.position.x - v.x;
	vec[1] = minirt->scene.camera.position.y - v.y;
	vec[2] = minirt->scene.camera.position.z - v.z;
	vec[3] = 1;
	init_roll_matrix(mat, calc_trigo);
	vector_multiply_matrix_3x3(mat, vec);
	init_yaw_matrix(mat, calc_trigo);
	vector_multiply_matrix_3x3(mat, vec);
	init_pitch_matrix(mat, calc_trigo);
	vector_multiply_matrix_3x3(mat, vec);
	init_perspective_matrix(persp, minirt);
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
