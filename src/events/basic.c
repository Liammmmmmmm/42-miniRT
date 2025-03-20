/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   basic.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 15:23:57 by lilefebv          #+#    #+#             */
/*   Updated: 2025/03/20 15:30:05 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	destroy(t_minirt *minirt)
{
	if (minirt != NULL)
		mlx_loop_end(minirt->mlx.mlx);
	return (0);
}

int	keydown_render(int key, t_minirt *minirt)
{
	if (key == KEY_ESC)
		mlx_loop_end(minirt->mlx.mlx);
	return (0);
}

void	events(t_minirt *minirt)
{
	// Quand on voudra faire les controles faudra faire avec autre chose que render win
	// mlx_hook(minirt->mlx.render_win, ON_MOUSEDOWN, 1L << 2, mouse_down, minirt);
	// mlx_hook(minirt->mlx.render_win, ON_MOUSEUP, 1L << 3, mouse_up, minirt);
	// mlx_hook(minirt->mlx.render_win, ON_MOUSEMOVE, 1L << 6, mouse_move, minirt);
	mlx_hook(minirt->mlx.render_win, ON_DESTROY, 0, destroy, minirt);
	mlx_hook(minirt->mlx.render_win, ON_KEYDOWN, 1L << 0, keydown_render, minirt);
	// mlx_hook(minirt->mlx.render_win, ON_KEYUP, 1L << 1, keyup, minirt);
}
