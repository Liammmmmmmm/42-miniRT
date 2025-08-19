/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_shader_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 18:09:17 by lilefebv          #+#    #+#             */
/*   Updated: 2025/08/19 10:21:32 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "gpu.h"
#include "minirt.h"
#include "bmp_parsing.h"
#include "options.h"
#include "error_message.h"
#include "network.h"

void	default_startup(t_minirt *minirt)
{
	if (minirt->options.server.enabled)
		events_server(minirt);
	else
	{
		mlx_do_key_autorepeatoff(minirt->mlx.mlx);
		events(minirt);
	}
	mlx_loop_hook(minirt->mlx.mlx, render_next_frame, minirt);
	mlx_loop(minirt->mlx.mlx);
	if (!minirt->options.server.enabled)
		mlx_do_key_autorepeaton(minirt->mlx.mlx);
}

int	all_startup_options(t_minirt *minirt, pthread_t *server)
{
	if (minirt->options.server.enabled)
	{
		minirt->options.server.minirt = minirt;
		if (pthread_create(server, NULL, server_thread_routine,
			&minirt->options.server) != 0)
			return (clean(minirt));
	}
	if (minirt->options.client.enabled)
	{
		connect_client(minirt->options.client.ip, minirt->options.client.port,
			minirt->options.client.password, minirt);
	}
	else
		default_startup(minirt);
	return (0);
}

void	clean_everything(t_minirt *minirt, pthread_t server)
{
	if (minirt->options.server.enabled)
	{
		shutdown(g_server_fd, SHUT_RDWR);
		close(g_server_fd);
		g_server_fd = -1;
		pthread_join(server, NULL);
	}
	clean(minirt);
	if (!minirt->options.server.enabled)
		clean_shaders(&minirt->shaders_data);
	else
		clean_server_gpu_obj(&minirt->shaders_data);
}
