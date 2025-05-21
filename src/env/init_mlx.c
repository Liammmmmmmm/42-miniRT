/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_mlx.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 12:05:40 by lilefebv          #+#    #+#             */
/*   Updated: 2025/05/16 14:16:11 by lilefebv         ###   ########lyon.fr   */
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

int	init_controls_mlx(t_minirt *minirt)
{
	t_mlx	*mlx;

	mlx = &minirt->mlx;
	if (!minirt->options.no_display)
		mlx->controls_win = mlx_new_window(mlx->mlx, CWIN_WIDTH, CWIN_HEIGHT,
				"Controls");
	mlx->img_controls.img = mlx_new_image(mlx->mlx, CWIN_WIDTH, CWIN_HEIGHT);
	if (!mlx->img_controls.img || (!mlx->controls_win && !minirt->options.no_display))
		return (free_mlx_error(minirt));
	mlx->img_controls.img_str = mlx_get_data_addr(mlx->img_controls.img,
			&mlx->img_controls.bits,
			&mlx->img_controls.size_line, &mlx->img_controls.endian);
	if (!mlx->img_controls.img_str)
		return (free_mlx_error(minirt));
	mlx->img_controls.width = CWIN_WIDTH;
	mlx->img_controls.width4 = CWIN_WIDTH * 4;
	mlx->img_controls.height = CWIN_HEIGHT;
	return (1);
}

int	init_mlx(t_minirt *minirt)
{
	t_mlx	*mlx;

	if (minirt->scene.win_height > MAX_WIN_SIZE)
		minirt->scene.win_height = MAX_WIN_SIZE;
	if (minirt->scene.win_width > MAX_WIN_SIZE)
		minirt->scene.win_width = MAX_WIN_SIZE;
	mlx = &minirt->mlx;
	mlx->mlx = mlx_init();
	if (!mlx->mlx)
		return (free_mlx_error(minirt));
	if (minirt->options.no_display)
	{
		minirt->scene.win_width = 600;
		minirt->scene.win_height = 190;
	}
	mlx->render_win = mlx_new_window(mlx->mlx, minirt->scene.win_width, minirt->scene.win_height, "miniRT");
	mlx->img.img = mlx_new_image(mlx->mlx, minirt->scene.win_width, minirt->scene.win_height);
	if (!mlx->img.img || !mlx->render_win)
		return (free_mlx_error(minirt));
	mlx->img.img_str = mlx_get_data_addr(mlx->img.img, &mlx->img.bits,
			&mlx->img.size_line, &mlx->img.endian);
	if (!mlx->img.img_str)
		return (free_mlx_error(minirt));
	mlx->img.width = minirt->scene.win_width;
	mlx->img.width4 = minirt->scene.win_width * 4;
	mlx->img.height = minirt->scene.win_height;
	init_controls(minirt);
	return (init_controls_mlx(minirt));
}

int	init_render(t_minirt *minirt)
{
	if (!minirt->options.no_display)
		minirt->screen.render = malloc(sizeof(int) * minirt->scene.win_width * minirt->scene.win_height);
	minirt->screen.float_render = malloc(sizeof(t_fcolor) * minirt->scene.render_width * minirt->scene.render_height);
	if (!minirt->screen.float_render || (!minirt->screen.render && !minirt->options.no_display))
		return (0);
	if (!minirt->options.no_display)
		ft_bzero(minirt->screen.render, sizeof(int) * minirt->scene.win_width * minirt->scene.win_height);
	ft_bzero(minirt->screen.float_render, sizeof(t_fcolor) * minirt->scene.render_width * minirt->scene.render_height);
	minirt->stats.frame = 0;
	minirt->screen.start_render = 1;
	minirt->screen.pause_render = 0;
	minirt->screen.spp = minirt->options.max_samples;
	minirt->screen.sample = 0;
	init_font(minirt);
	init_buttons(minirt);
	init_sliders(minirt);
	init_cps(minirt);
	init_dropdowns(minirt);
	init_float_inputs(minirt);
	return (1);
}
