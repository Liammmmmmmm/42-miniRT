/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 12:05:40 by lilefebv          #+#    #+#             */
/*   Updated: 2025/03/23 17:43:49 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	free_mlx(t_minirt *minirt)
{
	if (minirt->mlx.img.img)
		mlx_destroy_image(minirt->mlx.mlx, minirt->mlx.img.img);
	if (minirt->mlx.render_win)
		mlx_destroy_window(minirt->mlx.mlx, minirt->mlx.render_win);
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

int	init_mlx(t_minirt *minirt)
{
	t_mlx	*mlx;

	mlx = &minirt->mlx;
	mlx->mlx = mlx_init();
	if (!mlx->mlx)
		return (free_mlx_error(minirt));
	mlx->render_win = mlx_new_window(mlx->mlx, WIN_WIDTH, WIN_HEIGHT, "miniRT");
	mlx->img.img = mlx_new_image(mlx->mlx, WIN_WIDTH, WIN_HEIGHT);
	if (!mlx->img.img || !mlx->render_win)
		return (free_mlx_error(minirt));
	mlx->img.img_str = mlx_get_data_addr(mlx->img.img, &mlx->img.bits,
			&mlx->img.size_line, &mlx->img.endian);
	if (!mlx->img.img_str)
		return (free_mlx_error(minirt));
	mlx->img.width = WIN_WIDTH;
	mlx->img.height = WIN_HEIGHT;
	
	return (1);
}

int	init_render(t_minirt *minirt)
{
	int	i;

	minirt->screen.render = malloc(sizeof(t_sc_point) * WIN_WIDTH * WIN_HEIGHT);
	if (!minirt->screen.render)
		return (0);
	i = 0;
	while (i < WIN_WIDTH * WIN_HEIGHT)
	{
		minirt->screen.render[i].x = i % WIN_WIDTH;
		minirt->screen.render[i].y = i / WIN_WIDTH;
		minirt->screen.render[i].z = 0;
		ft_bzero(&minirt->screen.render[i].color, sizeof(t_color));
		i++;
	}
	minirt->stats.frame = 0;
	minirt->screen.start_render = 1;
	return (1);
}
