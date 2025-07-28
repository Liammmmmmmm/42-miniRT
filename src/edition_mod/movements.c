/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movements.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 18:16:15 by lilefebv          #+#    #+#             */
/*   Updated: 2025/07/28 18:09:19 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "camera.h"
#include "minirt.h"

static float	get_time_interval(t_minirt *minirt)
{
	return (((get_cpu_time() - minirt->screen.last_sample_time) / 1000.0)
		* minirt->controls.movements.speed_modifier
		* minirt->controls.movements.speed_modifier / 100.0);
}

static void	move_sides(t_minirt *minirt, const float time_interval)
{
	if (minirt->controls.movements.left)
		minirt->scene.camera.position = vec3_subtract(
				minirt->scene.camera.position, vec3_multiply_scalar(
					get_right_vector(minirt), MOVE_SENSI * time_interval));
	if (minirt->controls.movements.right)
		minirt->scene.camera.position = vec3_add(minirt->scene.camera.position,
				vec3_multiply_scalar(get_right_vector(minirt),
					MOVE_SENSI * time_interval));
}

void	move_camera(t_minirt *minirt)
{
	const float	time_interval = get_time_interval(minirt);

	if (minirt->controls.movements.forward)
		minirt->scene.camera.position = vec3_add(minirt->scene.camera.position,
				vec3_multiply_scalar(minirt->scene.camera.orientation,
					MOVE_SENSI * time_interval));
	if (minirt->controls.movements.back)
		minirt->scene.camera.position = vec3_subtract(
				minirt->scene.camera.position, vec3_multiply_scalar(
					minirt->scene.camera.orientation,
					MOVE_SENSI * time_interval));
	if (minirt->controls.movements.up)
		minirt->scene.camera.position = vec3_add(minirt->scene.camera.position,
				vec3_multiply_scalar(get_up_vector(minirt),
					MOVE_SENSI * time_interval));
	if (minirt->controls.movements.down)
		minirt->scene.camera.position = vec3_subtract(
				minirt->scene.camera.position, vec3_multiply_scalar(
					get_up_vector(minirt), MOVE_SENSI * time_interval));
	move_sides(minirt, time_interval);
	if (minirt->controls.movements.forward || minirt->controls.movements.back
		|| minirt->controls.movements.left || minirt->controls.movements.right
		|| minirt->controls.movements.up || minirt->controls.movements.down)
		restart_minirt(minirt);
}
