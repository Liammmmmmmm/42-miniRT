/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_common.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 18:29:12 by lilefebv          #+#    #+#             */
/*   Updated: 2025/05/15 14:11:31 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	keydown_common(int key, t_minirt *minirt)
{
	if (key == KEY_ESC)
		mlx_loop_end(minirt->mlx.mlx);
	else if (key == KEY_LCTRL)
		minirt->controls.keydown.lctrl = 1;
	else if (key == KEY_RCTRL)
		minirt->controls.keydown.rctrl = 1;
	else if (key == KEY_LSHIFT)
		minirt->controls.keydown.lshift = 1;
	else if (key == KEY_RSHIFT)
		minirt->controls.keydown.rshift = 1;
	else if (key == KEY_D && minirt->controls.values.debug == 0)
		minirt->controls.values.debug = 1;
	else if (key == KEY_D && minirt->controls.values.debug == 1)
	{
		minirt->controls.values.debug = 0;
		copy_buff_to_image(minirt);
		mlx_put_image_to_window(minirt->mlx.mlx, minirt->mlx.render_win,
			minirt->mlx.img.img, 0, 0);
	}
	else if (key == KEY_N)
	{
		minirt->scene.bvh.normal_mode = !minirt->scene.bvh.normal_mode;
		minirt->screen.sample = 0;
	}
	else if (key == KEY_P)
	{
		// Export
		char *filename;

		filename = ft_sprintf("minirt_export_SCENE_NAME.SAMPLES.%d.%u.ppm", minirt->screen.sample, (unsigned int)get_cpu_time());
		if (filename)
			export_ppm_p6_minirt(filename, minirt);
	}
}

void	keyup_common(int key, t_minirt *minirt)
{
	if (key == KEY_LCTRL)
		minirt->controls.keydown.lctrl = 0;
	else if (key == KEY_RCTRL)
		minirt->controls.keydown.rctrl = 0;
	else if (key == KEY_LSHIFT)
		minirt->controls.keydown.lshift = 0;
	else if (key == KEY_RSHIFT)
		minirt->controls.keydown.rshift = 0;
}
