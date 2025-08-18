/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 18:22:53 by lilefebv          #+#    #+#             */
/*   Updated: 2025/08/18 10:32:04 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "network.h"
#include "utils.h"
#include <signal.h>

int	g_server_fd = -1;

static int	init_server(int port)
{
	int					reuse;
	struct sockaddr_in	addr;

	signal(SIGINT, handle_sigint);
	g_server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (g_server_fd < 0)
	{
		print_error("server socket failed");
		return (-1);
	}
	reuse = 1;
	setsockopt(g_server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
	addr = (struct sockaddr_in){.sin_family = AF_INET, .sin_port = htons(port),
		.sin_addr.s_addr = INADDR_ANY};
	if (bind(g_server_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
	{
		print_error("server bind failed");
		return (-1);
	}
	if (listen(g_server_fd, MAX_CLIENTS) < 0)
	{
		print_error("server listen failed");
		return (-1);
	}
	return (0);
}

void	whait_for_client(char *password, t_minirt *minirt)
{
	t_client_data	*client_data;
	socklen_t		client_len;
	pthread_t		thread;

	client_len = sizeof(client_data->client_addr);
	client_len = sizeof(struct sockaddr_in);
	client_data = malloc(sizeof(t_client_data));
	if (!client_data)
		return ;
	client_data->client_fd = accept(g_server_fd,
			(struct sockaddr*)&client_data->client_addr, &client_len);
	if (client_data->client_fd < 0)
	{
		free(client_data);
		return ;
	}
	client_data->password = password;
	client_data->minirt = minirt;
	if (pthread_create(&thread, NULL, handle_client, (void *)client_data) != 0)
	{
		close(client_data->client_fd);
		free(client_data);
		return ((void)print_error("pthread_create failed"));
	}
	pthread_detach(thread);
}

void	minirt_server(char *password, int port, t_minirt *minirt)
{
	pthread_t	cli_thread;

	if (init_server(port) < 0)
		return ;
	print_network_info(port);
	if (pthread_create(&cli_thread, NULL, cli_thread_routine, (void *)minirt)
		!= 0)
		print_error("pthread_create failed, no cli");
	convert_textures_server(&minirt->scene, &minirt->shaders_data.tex);
	init_scene_server(minirt);
	ft_bzero(minirt->screen.float_render, sizeof(t_fcolor)
		* minirt->viewport.render_w * minirt->viewport.render_h);
	convert_scene_server(minirt, &minirt->scene, &minirt->viewport,
		&minirt->shaders_data.scene);
	while (g_server_fd != -1)
		whait_for_client(password, minirt);
	pthread_cancel(cli_thread);
	pthread_join(cli_thread, NULL);
}

void	*server_thread_routine(void *arg)
{
	t_server	*server;

	server = arg;
	minirt_server(server->password, server->port, server->minirt);
	return (NULL);
}
