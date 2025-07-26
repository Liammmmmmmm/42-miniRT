/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movements_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 15:22:33 by lilefebv          #+#    #+#             */
/*   Updated: 2025/07/24 16:55:25 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "camera.h"
#include "maths.h"

t_vec3	get_right_vector(t_minirt *minirt)
{
	t_vec3	up;

	up = (t_vec3){0, 1, 0};
	if (minirt->scene.camera.orientation.y > 0.999 || minirt->scene.camera.orientation.y < -0.999)
		up = (t_vec3){0, 0, 1};
	return (vec3_cross(minirt->scene.camera.orientation, up));
}

t_vec3	get_up_vector(t_minirt *minirt)
{
    t_vec3	up_ref;
    t_vec3	right_vec;
    t_vec3	camera_orientation;

	camera_orientation = minirt->scene.camera.orientation;
	up_ref = (t_vec3){0, 1, 0};
	if (camera_orientation.y > 0.999 || camera_orientation.y < -0.999)
		up_ref = (t_vec3){0, 0, 1};
	right_vec = vec3_cross(camera_orientation, up_ref);
	return (vec3_cross(right_vec, camera_orientation));
}

t_bool	is_cam_moving(t_minirt *minirt)
{
	return (minirt->controls.movements.forward
		|| minirt->controls.movements.back
		|| minirt->controls.movements.right
		|| minirt->controls.movements.left
		|| minirt->controls.movements.up
		|| minirt->controls.movements.down
		|| minirt->controls.movements.mouse);
}
