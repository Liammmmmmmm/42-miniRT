/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_common.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 18:29:12 by lilefebv          #+#    #+#             */
/*   Updated: 2025/05/23 15:20:11 by lilefebv         ###   ########lyon.fr   */
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
