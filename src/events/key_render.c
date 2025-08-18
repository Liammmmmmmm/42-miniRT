/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_render.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 18:37:12 by lilefebv          #+#    #+#             */
/*   Updated: 2025/08/18 17:19:56 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "camera.h"

void	keydown_move(int key, t_minirt *minirt)
{
	if (key == KEY_W)
		minirt->controls.movements.forward = 1;
	else if (key == KEY_S)
		minirt->controls.movements.back = 1;
	else if (key == KEY_A)
		minirt->controls.movements.left = 1;
	else if (key == KEY_D)
		minirt->controls.movements.right = 1;
	else if (key == KEY_SPACE)
		minirt->controls.movements.up = 1;
	else if (key == KEY_LSHIFT)
		minirt->controls.movements.down = 1;
}

int	key_render_mode(int key, t_minirt *minirt)
{
	if (key == KEY_H && minirt->controls.values.debug == 1)
	{
		minirt->controls.values.debug = 0;
		copy_buff_to_image(minirt);
		mlx_put_image_to_window(minirt->mlx.mlx, minirt->mlx.render_win,
			minirt->mlx.img.img, 0, 0);
		return (1);
	}
	else if (key == KEY_N)
	{
		toggle_vals(&minirt->render_mode, 2, 0);
		minirt->screen.sample = 0;
		return (1);
	}
	else if (key == KEY_B)
	{
		if (minirt->viewport.depth_buffer != NULL)
		{
			toggle_vals(&minirt->render_mode, 1, 0);
			minirt->screen.sample = 0;
		}
	}
	return (0);
}

int	keydown_render(int key, t_minirt *minirt)
{
	keydown_move(key, minirt);
	if (key == KEY_H && minirt->controls.values.debug == 0)
		minirt->controls.values.debug = 1;
	else if (key == KEY_H && minirt->controls.values.debug == 1)
	{
		minirt->controls.values.debug = 0;
		copy_buff_to_image(minirt);
		mlx_put_image_to_window(minirt->mlx.mlx, minirt->mlx.render_win,
			minirt->mlx.img.img, 0, 0);
	}
	else if (key_render_mode(key, minirt))
		;
	else if (key == KEY_P)
		export_scene(minirt);
	else if (key == KEY_SPACE)
		minirt->screen.pause_render = !minirt->screen.pause_render;
	keydown_common(key, minirt);
	open_controls(key, minirt);
	return (0);
}

void	keyup_move(int key, t_minirt *minirt)
{
	if (key == KEY_W)
		minirt->controls.movements.forward = 0;
	else if (key == KEY_S)
		minirt->controls.movements.back = 0;
	else if (key == KEY_A)
		minirt->controls.movements.left = 0;
	else if (key == KEY_D)
		minirt->controls.movements.right = 0;
	else if (key == KEY_SPACE)
		minirt->controls.movements.up = 0;
	else if (key == KEY_LSHIFT)
		minirt->controls.movements.down = 0;
}

int	keyup_render(int key, t_minirt *minirt)
{
	keyup_move(key, minirt);
	keyup_common(key, minirt);
	return (0);
}
