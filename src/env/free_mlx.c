/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_mlx.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 15:16:38 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/20 15:16:49 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	free_mlx(t_minirt *minirt)
{
	if (minirt->mlx.img.img)
		mlx_destroy_image(minirt->mlx.mlx, minirt->mlx.img.img);
	if (minirt->mlx.img_controls.img)
		mlx_destroy_image(minirt->mlx.mlx, minirt->mlx.img_controls.img);
	if (minirt->mlx.render_win)
		mlx_destroy_window(minirt->mlx.mlx, minirt->mlx.render_win);
	if (minirt->mlx.controls_win)
		mlx_destroy_window(minirt->mlx.mlx, minirt->mlx.controls_win);
	if (minirt->mlx.mlx)
		mlx_destroy_display(minirt->mlx.mlx);
	free(minirt->mlx.mlx);
}

int	free_mlx_error(t_minirt *minirt)
{
	print_error(strerror(errno));
	free_mlx(minirt);
	return (0);
}
