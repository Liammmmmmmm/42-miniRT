/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movements_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 15:22:33 by lilefebv          #+#    #+#             */
/*   Updated: 2025/07/22 15:23:58 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "camera.h"
#include "maths.h"

t_vec3	get_right_vector(t_minirt *minirt)
{
	t_vec3	right;
	t_vec3	up;

	up = (t_vec3){0, 1, 0};
	if (minirt->scene.camera.orientation.y > 0.999)
		up = (t_vec3){0, 0, 1};
	return (vec3_multiply_scalar(vec3_cross(minirt->scene.camera.orientation, up), MOVE_SENSI));
}
