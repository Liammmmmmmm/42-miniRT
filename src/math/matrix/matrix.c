/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 15:07:24 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/20 17:01:57 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "minirt.h"

void	init_yaw_matrix(double matrix[3][3], t_calc_trigo trigo_calcs)
{
	matrix[0][0] = trigo_calcs.cos_yaw;
	matrix[0][1] = -trigo_calcs.sin_yaw;
	matrix[0][2] = 0;
	matrix[1][0] = trigo_calcs.sin_yaw;
	matrix[1][1] = trigo_calcs.cos_yaw;
	matrix[1][2] = 0;
	matrix[2][0] = 0;
	matrix[2][1] = 0;
	matrix[2][2] = 1;
}

void	init_roll_matrix(double matrix[3][3], t_calc_trigo trigo_calcs)
{
	matrix[0][0] = trigo_calcs.cos_roll;
	matrix[0][1] = 0;
	matrix[0][2] = trigo_calcs.sin_roll;
	matrix[1][0] = 0;
	matrix[1][1] = 1;
	matrix[1][2] = 0;
	matrix[2][0] = -trigo_calcs.sin_roll;
	matrix[2][1] = 0;
	matrix[2][2] = trigo_calcs.cos_roll;
}

void	init_pitch_matrix(double matrix[3][3], t_calc_trigo trigo_calcs)
{
	matrix[0][0] = 1;
	matrix[0][1] = 0;
	matrix[0][2] = 0;
	matrix[1][0] = 0;
	matrix[1][1] = trigo_calcs.cos_pitch;
	matrix[1][2] = -trigo_calcs.sin_pitch;
	matrix[2][0] = 0;
	matrix[2][1] = trigo_calcs.sin_pitch;
	matrix[2][2] = trigo_calcs.cos_pitch;
}

void	init_perspective_matrix(double matrix[4][4], t_minirt *minirt)
{
	const double	fov_rad = minirt->scene.camera.fov * (PI_D / 180.0);
	const double	aspect = (double)WIN_WIDTH / (double)WIN_HEIGHT;
	const double	f = 1.0 / tan(fov_rad * 0.5);
	const double	znear = 0.1;
	const double	zfar = 1000.0;

	matrix[0][0] = f / aspect;
	matrix[0][1] = 0;
	matrix[0][2] = 0;
	matrix[0][3] = 0;
	matrix[1][0] = 0;
	matrix[1][1] = f;
	matrix[1][2] = 0;
	matrix[1][3] = 0;
	matrix[2][0] = 0;
	matrix[2][1] = 0;
	matrix[2][2] = (zfar + znear) / (znear - zfar);
	matrix[2][3] = (2 * zfar * znear) / (znear - zfar);
	matrix[3][0] = 0;
	matrix[3][1] = 0;
	matrix[3][2] = -1;
	matrix[3][3] = 0;
}
