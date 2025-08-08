/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_shader.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 15:31:47 by lilefebv          #+#    #+#             */
/*   Updated: 2025/08/08 10:55:43 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "gpu.h"
#include "minirt.h"
#include "bmp_parsing.h"
#include "options.h"
#include "error_message.h"
#include "network.h"

int	render_next_frame(t_minirt *minirt)
{
	if (minirt->options.server.enabled)
		render_frame_server(minirt);
	else
		render_frame(minirt);
	return (1);
}

int	clean(t_minirt *minirt)
{
	pthread_mutex_destroy(&minirt->screen.sample_mutex);
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
	free(minirt->screen.client_accumulation);
	return (1);
}

int	init_all(t_minirt *minirt)
{
	if (pthread_mutex_init(&minirt->screen.sample_mutex, NULL))
		return (clean(minirt));
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
	if (!minirt->options.server.enabled)
	{
		if (init_shader(&minirt->shaders_data, minirt->scene.render_width,
				minirt->scene.render_height, &minirt->scene) == -1)
			return (clean(minirt));	
	}
	return (0);
}

void	clean_server_gpu_obj(t_shader_data *shader_data)
{
	clean_scene(&shader_data->scene);
	free(shader_data->tex.checkers);
	free(shader_data->tex.images);
	free(shader_data->tex.images_stream);
	free(shader_data->tex.textures_types_indices);
}

int	main(int argc, char **argv)
{
	t_minirt	minirt;
	pthread_t	server;

	if (argc < 2)
		return (print_error1(ERR_MAIN));
	ft_bzero(&minirt, sizeof(t_minirt));
	if (!parse_options(&minirt, argc, argv))
		return (1);
	if (!minirt.options.client.enabled)
	{
		if (parse_scene(&minirt, argv[1]) == 0)
			return (1);
	}
	if (init_all(&minirt))
		return (1);
	// print_scene(&minirt.scene);
	// if (minirt.options.anim.enabled)
	// debug_print_animation(&minirt.options.anim);
	if (minirt.options.server.enabled)
	{
		minirt.options.server.minirt = &minirt;
		if (pthread_create(&server, NULL, server_thread_routine, &minirt.options.server) != 0)
			return (clean(&minirt));
	}
	if (minirt.options.client.enabled)
	{
		connect_client(minirt.options.client.ip, minirt.options.client.port, minirt.options.client.password, &minirt);
	}
	else
	{
		if (minirt.options.server.enabled)
			events_server(&minirt);
		else
		{
			mlx_do_key_autorepeatoff(minirt.mlx.mlx);
			events(&minirt);
		}
		mlx_loop_hook(minirt.mlx.mlx, render_next_frame, &minirt);
		mlx_loop(minirt.mlx.mlx);
		if (!minirt.options.server.enabled)
			mlx_do_key_autorepeaton(minirt.mlx.mlx);
	}
	if (minirt.options.server.enabled)
	{
		shutdown(g_server_fd, SHUT_RDWR);
		close(g_server_fd);
		g_server_fd = -1;
		pthread_join(server, NULL);
	}
	clean(&minirt);
	if (!minirt.options.server.enabled)
		clean_shaders(&minirt.shaders_data);
	else
		clean_server_gpu_obj(&minirt.shaders_data);
	return (0);
}
