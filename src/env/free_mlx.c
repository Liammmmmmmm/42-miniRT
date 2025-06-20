/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_mlx.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 15:16:38 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/20 16:39:26 by madelvin         ###   ########.fr       */
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
