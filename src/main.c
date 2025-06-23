/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 15:31:47 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/23 18:59:59 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "bmp_parsing.h"
#include "options.h"
#include "error_message.h"

int	render_next_frame(t_minirt *minirt)
{
	render_frame(minirt);
	return (1);
}

int	clean(t_minirt *minirt)
{
	free_scene(&minirt->scene, NULL);
	free_mlx(minirt);
	clear_buttons(minirt);
	clear_sliders(minirt);
	clear_dropdown(minirt);
	clear_float_inputs(minirt);
	clear_text_inputs(minirt);
	free(minirt->screen.render);
	free(minirt->screen.float_render);
	free_ttf(&minirt->controls.font[0]);
	free_anim(&minirt->options.anim);
	free(minirt->micrort.render);
	return (1);
}

int	init_all(t_minirt *minirt)
{
	minirt->rand = (uint32_t)get_cpu_time();
	if (minirt->rand == 0)
		minirt->rand = 1;
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
	if (!init_render(minirt))
		return (clean(minirt));
	return (0);
}

int	main(int argc, char **argv)
{
	t_minirt	minirt;

	if (argc < 2)
		return (print_error1(ERR_MAIN));
	ft_bzero(&minirt, sizeof(t_minirt));
	if (!parse_options(&minirt, argc, argv))
		return (1);
	if (parse_scene(&minirt, argv[1]) == 0)
		return (1);
	if (init_all(&minirt))
		return (1);
	print_scene(&minirt.scene);
	if (minirt.options.anim.enabled)
		debug_print_animation(&minirt.options.anim);
	events(&minirt);
	mlx_loop_hook(minirt.mlx.mlx, render_next_frame, &minirt);
	mlx_loop(minirt.mlx.mlx);
	clean(&minirt);
	return (0);
}
