/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 15:23:57 by lilefebv          #+#    #+#             */
/*   Updated: 2025/05/16 12:46:48 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	events_controls(t_minirt *minirt)
{
	if (!minirt->mlx.controls_win)
		return ;
	mlx_hook(minirt->mlx.controls_win, ON_DESTROY, 0, destroy_controls, minirt);
	mlx_hook(minirt->mlx.controls_win, ON_KEYDOWN, 1L << 0,
		keydown_controls, minirt);
	mlx_hook(minirt->mlx.controls_win, ON_KEYUP, 1L << 0,
		keyup_controls, minirt);
	mlx_hook(minirt->mlx.controls_win, ON_MOUSEDOWN, 1L << 2,
		mousedown_controls, minirt);
	mlx_hook(minirt->mlx.controls_win, ON_MOUSEUP, 1L << 3,
		mouseup_controls, minirt);
	mlx_hook(minirt->mlx.controls_win, ON_MOUSEMOVE, 1L << 6,
		mouse_move_controls, minirt);
}

void	events(t_minirt *minirt)
{
	mlx_hook(minirt->mlx.render_win, ON_DESTROY, 0, destroy, minirt);
	mlx_hook(minirt->mlx.render_win, ON_KEYDOWN, 1L << 0, keydown_render,
		minirt);
	mlx_hook(minirt->mlx.render_win, ON_KEYUP, 1L << 0, keyup_render, minirt);
	mlx_hook(minirt->mlx.render_win, ON_MOUSEDOWN, 1L << 2, mousedown_render,
		minirt);
	mlx_hook(minirt->mlx.render_win, ON_MOUSEUP, 1L << 3, mouseup_render,
		minirt);
	events_controls(minirt);
}
