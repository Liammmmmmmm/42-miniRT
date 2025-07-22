/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movements.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 18:16:15 by lilefebv          #+#    #+#             */
/*   Updated: 2025/07/22 18:29:23 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "camera.h"
#include "minirt.h"

void	move_camera(t_minirt *minirt)
{
	if (minirt->controls.movements.forward)
	{
		minirt->scene.camera.position = vec3_add(minirt->scene.camera.position, vec3_multiply_scalar(minirt->scene.camera.orientation, MOVE_SENSI));
		restart_minirt(minirt);
	}
	if (minirt->controls.movements.back)
	{
		minirt->scene.camera.position = vec3_subtract(minirt->scene.camera.position, vec3_multiply_scalar(minirt->scene.camera.orientation, MOVE_SENSI));
		restart_minirt(minirt);
	}
	if (minirt->controls.movements.left)
	{
		minirt->scene.camera.position = vec3_subtract(minirt->scene.camera.position, get_right_vector(minirt));
		restart_minirt(minirt);
	}
	if (minirt->controls.movements.right)
	{
		minirt->scene.camera.position = vec3_add(minirt->scene.camera.position, get_right_vector(minirt));
		restart_minirt(minirt);
	}
	if (minirt->controls.movements.up)
	{
		minirt->scene.camera.position = vec3_add(minirt->scene.camera.position, get_up_vector(minirt));
		restart_minirt(minirt);
	}
	if (minirt->controls.movements.down)
	{
		minirt->scene.camera.position = vec3_subtract(minirt->scene.camera.position, get_up_vector(minirt));
		restart_minirt(minirt);
	};
}