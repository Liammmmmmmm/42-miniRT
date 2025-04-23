/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destroy.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 18:26:37 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/22 18:27:06 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	destroy_controls(t_minirt *minirt)
{
	if (!minirt)
		return (0);
	if (minirt->mlx.controls_win)
	{
		mlx_destroy_window(minirt->mlx.mlx, minirt->mlx.controls_win);
		minirt->mlx.controls_win = NULL;
		mlx_loop(minirt->mlx.mlx);
	}
	return (0);
}

int	destroy(t_minirt *minirt)
{
	if (minirt != NULL)
		mlx_loop_end(minirt->mlx.mlx);
	return (0);
}
