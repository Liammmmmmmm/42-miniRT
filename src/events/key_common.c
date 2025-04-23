/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_common.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 18:29:12 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/22 18:30:27 by lilefebv         ###   ########lyon.fr   */
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
		put_render_to_frame(minirt);
		mlx_put_image_to_window(minirt->mlx.mlx, minirt->mlx.render_win,
			minirt->mlx.img.img, 0, 0);
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
