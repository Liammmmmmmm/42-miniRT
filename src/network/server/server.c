/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 18:22:53 by lilefebv          #+#    #+#             */
/*   Updated: 2025/08/04 11:43:34 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "network.h"
#include "utils.h"
#include <signal.h>

int	g_server_fd = -1;

static int	init_server(int	port)
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

void	whait_for_client(char *password)
{
	t_client_data	*client_data;
	socklen_t		client_len;
	pthread_t		thread;
	
	client_len = sizeof(client_data->client_addr);
	client_data = malloc(sizeof(t_client_data));
	if (!client_data)
		return ;
	client_data->client_fd = accept(g_server_fd,
		(struct sockaddr*)&client_data->client_addr, &client_len);
	if (client_data->client_fd < 0)
	{
		free(client_data);
		return ((void)print_error("accept failed"));
	}
	client_data->password = password;
	if (pthread_create(&thread, NULL, handle_client, (void*)client_data) != 0)
	{
		close(client_data->client_fd);
		free(client_data);
		return ((void)print_error("pthread_create failed"));
	}
	pthread_detach(thread);
}

void	minirt_server(char *password, int port)
{
	if (init_server(port) < 0)
		return ;
	print_network_info(port);

	pthread_t cli_thread;
	if (pthread_create(&cli_thread, NULL, cli_thread_routine, NULL) != 0)
		print_error("pthread_create failed, no cli");
	while (g_server_fd != -1)
		whait_for_client(password);
}
