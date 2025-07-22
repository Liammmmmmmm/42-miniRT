/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_render.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 18:37:12 by lilefebv          #+#    #+#             */
/*   Updated: 2025/07/22 15:24:45 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "camera.h"

void	export_scene(t_minirt *minirt)
{
	char	*filename;

	filename = ft_sprintf("%sminirt_export_%s.SAMPLES.%d.%u.ppm",
			minirt->options.output_dir, minirt->scene.name,
			minirt->screen.sample, (unsigned int)get_cpu_time());
	if (filename)
		export_ppm_p6_minirt(filename, minirt);
}

void	open_controls(int key, t_minirt *minirt)
{
	if (key == minirt->controls.open_controls)
	{
		if (!minirt->mlx.controls_win && !minirt->options.no_display
			&& !minirt->options.anim.enabled)
		{
			minirt->mlx.controls_win = mlx_new_window(minirt->mlx.mlx,
					CWIN_WIDTH, CWIN_HEIGHT, "Controls");
			events_controls(minirt);
			mlx_loop(minirt->mlx.mlx);
		}
	}
}

int	keydown_move(int key, t_minirt *minirt)
{
	if (key == KEY_W)
	{
		minirt->scene.camera.position = vec3_add(minirt->scene.camera.position, vec3_multiply_scalar(minirt->scene.camera.orientation, MOVE_SENSI));
		return (restart_minirt(minirt));
	}
	else if (key == KEY_S)
	{
		minirt->scene.camera.position = vec3_subtract(minirt->scene.camera.position, vec3_multiply_scalar(minirt->scene.camera.orientation, MOVE_SENSI));
		return (restart_minirt(minirt));
	}
	else if (key == KEY_A)
	{
		minirt->scene.camera.position = vec3_subtract(minirt->scene.camera.position, get_right_vector(minirt));
		return (restart_minirt(minirt));
	}
	else if (key == KEY_D)
	{
		minirt->scene.camera.position = vec3_add(minirt->scene.camera.position, get_right_vector(minirt));
		return (restart_minirt(minirt));
	}
	return (0);
}

int	keydown_render(int key, t_minirt *minirt)
{
	if (keydown_move(key, minirt))
		;
	else if (key == KEY_H && minirt->controls.values.debug == 0)
		minirt->controls.values.debug = 1;
	else if (key == KEY_H && minirt->controls.values.debug == 1)
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
		export_scene(minirt);
	else if (key == KEY_SPACE)
		minirt->screen.pause_render = !minirt->screen.pause_render;
	keydown_common(key, minirt);
	open_controls(key, minirt);
	return (0);
}

int	keyup_render(int key, t_minirt *minirt)
{
	keyup_common(key, minirt);
	return (0);
}
