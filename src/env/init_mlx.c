/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_mlx.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 12:05:40 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/20 15:19:34 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	init_controls_mlx(t_minirt *minirt)
{
	t_mlx	*mlx;

	mlx = &minirt->mlx;
	if (!minirt->options.no_display && !minirt->options.anim.enabled)
		mlx->controls_win = mlx_new_window(mlx->mlx, CWIN_WIDTH, CWIN_HEIGHT,
				"Controls");
	mlx->img_controls.img = mlx_new_image(mlx->mlx, CWIN_WIDTH, CWIN_HEIGHT);
	if (!mlx->img_controls.img || (!mlx->controls_win
			&& !minirt->options.no_display && !minirt->options.anim.enabled))
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

static int	init_mlx_img_win(t_minirt *minirt, t_mlx *mlx)
{
	mlx->render_win = mlx_new_window(mlx->mlx, minirt->scene.win_width,
			minirt->scene.win_height, "miniRT");
	mlx->img.img = mlx_new_image(mlx->mlx, minirt->scene.win_width,
			minirt->scene.win_height);
	if (!mlx->img.img || !mlx->render_win)
		return (free_mlx_error(minirt));
	mlx->img.img_str = mlx_get_data_addr(mlx->img.img, &mlx->img.bits,
			&mlx->img.size_line, &mlx->img.endian);
	if (!mlx->img.img_str)
		return (free_mlx_error(minirt));
	mlx->img.width = minirt->scene.win_width;
	mlx->img.width4 = minirt->scene.win_width * 4;
	mlx->img.height = minirt->scene.win_height;
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
	if (init_mlx_img_win(minirt, mlx) == 0)
		return (0);
	init_controls(minirt);
	return (init_controls_mlx(minirt));
}

void	init_ui_components(t_minirt *minirt)
{
	init_font(minirt);
	init_buttons(minirt);
	init_sliders(minirt);
	init_cps(minirt);
	init_dropdowns(minirt);
	init_float_inputs(minirt);
	init_text_inputs(minirt);
}

int	init_render(t_minirt *minirt)
{
	if (!minirt->options.no_display)
		minirt->screen.render = malloc(sizeof(int) * minirt->scene.win_width * \
		minirt->scene.win_height);
	minirt->screen.float_render = malloc(sizeof(t_fcolor) * \
	minirt->scene.render_width * minirt->scene.render_height);
	if (!minirt->screen.float_render || (!minirt->screen.render
			&& !minirt->options.no_display))
		return (0);
	if (!minirt->options.no_display)
		ft_bzero(minirt->screen.render, sizeof(int) * minirt->scene.win_width
			* minirt->scene.win_height);
	ft_bzero(minirt->screen.float_render, sizeof(t_fcolor)
		* minirt->scene.render_width * minirt->scene.render_height);
	minirt->stats.frame = 0;
	minirt->screen.start_render = 1;
	minirt->screen.pause_render = 0;
	minirt->screen.spp = minirt->options.max_samples;
	minirt->screen.sample = 0;
	init_ui_components(minirt);
	return (1);
}
