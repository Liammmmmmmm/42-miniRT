/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_movement.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 18:42:37 by lilefebv          #+#    #+#             */
/*   Updated: 2025/07/28 19:07:22 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "camera.h"

void	manage_movements(t_minirt *minirt)
{
	const t_bool	last_frame_is_moving_tmp
		= minirt->controls.movements.last_frame_is_moving;

	if (is_cam_moving(minirt))
		minirt->controls.movements.last_frame_is_moving = 1;
	else
		minirt->controls.movements.last_frame_is_moving = 0;
	if (is_cam_moving(minirt) && !last_frame_is_moving_tmp)
	{
		minirt->scene.render_height = minirt->scene.win_height / 10;
		minirt->scene.render_width = minirt->scene.win_width / 10;
		restart_minirt(minirt);
	}
	else if (!is_cam_moving(minirt) && last_frame_is_moving_tmp)
		restart_minirt(minirt);
}
