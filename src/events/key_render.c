/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_render.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 18:37:12 by lilefebv          #+#    #+#             */
/*   Updated: 2025/05/23 15:20:15 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	keydown_render(int key, t_minirt *minirt)
{
	if (key == KEY_D && minirt->controls.values.debug == 0)
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

		filename = ft_sprintf("%sminirt_export_SCENE_NAME.SAMPLES.%d.%u.ppm", minirt->options.output_dir, minirt->screen.sample, (unsigned int)get_cpu_time());
		if (filename)
			export_ppm_p6_minirt(filename, minirt);
	}
	keydown_common(key, minirt);
	if (key == minirt->controls.open_controls)
	{
		if (!minirt->mlx.controls_win && !minirt->options.no_display && !minirt->options.anim.enabled)
		{
			minirt->mlx.controls_win = mlx_new_window(minirt->mlx.mlx,
					CWIN_WIDTH, CWIN_HEIGHT, "Controls");
			events_controls(minirt);
			mlx_loop(minirt->mlx.mlx);
		}
	}
	return (0);
}

int	keyup_render(int key, t_minirt *minirt)
{
	keyup_common(key, minirt);
	return (0);
}
