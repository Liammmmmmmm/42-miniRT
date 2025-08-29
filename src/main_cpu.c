/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_cpu.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 15:31:47 by lilefebv          #+#    #+#             */
/*   Updated: 2025/08/29 10:14:03 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "bmp_parsing.h"
#include "options.h"
#include "error_message.h"

static int	render_next_frame(t_minirt *minirt)
{
	render_frame(minirt);
	return (1);
}

static int	init_all(t_minirt *minirt)
{
	if (minirt->scene.win_height == -1 || minirt->scene.win_width == -1)
	{
		minirt->scene.win_width = WIN_WIDTH;
		minirt->scene.win_height = WIN_HEIGHT;
		minirt->scene.render_width = WIN_WIDTH;
		minirt->scene.render_height = WIN_HEIGHT;
	}
	if (!init_ui(minirt))
		return (clean(minirt));
	if (!init_micrort(minirt))
		return (clean(minirt));
	if (!init_mlx(minirt))
		return (clean(minirt));
	init_controls(minirt);
	if (!init_render(minirt))
		return (clean(minirt));
	return (0);
}

int	main_cpu(t_minirt *minirt, char **argv)
{
	if (parse_scene(minirt, argv[1]) == 0)
		return (1);
	if (init_all(minirt))
		return (1);
	print_scene(&minirt->scene);
	if (minirt->options.anim.enabled)
		debug_print_animation(&minirt->options.anim);
	mlx_do_key_autorepeatoff(minirt->mlx.mlx);
	events(minirt);
	mlx_loop_hook(minirt->mlx.mlx, render_next_frame, minirt);
	mlx_loop(minirt->mlx.mlx);
	mlx_do_key_autorepeaton(minirt->mlx.mlx);
	clean(minirt);
	return (0);
}
