/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 13:10:02 by lilefebv          #+#    #+#             */
/*   Updated: 2025/08/18 16:21:02 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "gpu.h"
#include "network.h"
#include "utils.h"
#include "minirt.h"
#include <signal.h>

static int	verify_server_connection(int *sockfd, t_minirt *minirt)
{
	if (!is_connection_alive(*sockfd))
	{
		printf("Server disconnected\n");
		if (passive_mode(sockfd, minirt) < 0)
			return (print_errorm1("failed to enable passive mode"));
		else
			minirt->screen.start_render = 0;
	}
	return (0);
}

int	read_stdin(fd_set *readfds)
{
	char	*line;

	if (!FD_ISSET(STDIN_FILENO, readfds))
		return (0);
	line = get_next_line(STDIN_FILENO);
	if (!line || ft_strcmp("exit\n", line) == 0)
	{
		free(line);
		return (-1);
	}
	free(line);
	return (0);
}

static int	read_socket(int *sockfd, fd_set *readfds, t_minirt *minirt)
{
	char		header[10];
	ssize_t		received;
	char		*data;
	uint64_t	bytes_to_read;
	uint16_t	type;

	if (!FD_ISSET(*sockfd, readfds))
		return (0);
	received = recv(*sockfd, header, 10, 0);
	if (received <= 0)
		recv_error_client(sockfd, minirt);
	if (received != 10)
		return (0);
	bytes_to_read = *(uint64_t *)header;
	type = *(uint16_t *)(header + 8);
	printf("Received %u (%lu bytes)\n", type, bytes_to_read);
	glUseProgram(minirt->shaders_data.program);
	if (bytes_to_read == 0)
		return (no_data_instruction(minirt, type));
	data = recv_large_data(*sockfd, bytes_to_read);
	if (!data)
		return (0);
	receive_scene_if_forest(minirt, type, data, bytes_to_read);
	free(data);
	return (0);
}

static int	listen_stdin_and_socket(int *sockfd, t_minirt *minirt)
{
	fd_set			readfds;
	struct timeval	select_timeout;
	int				ret;

	FD_ZERO(&readfds);
	FD_SET(STDIN_FILENO, &readfds);
	FD_SET(*sockfd, &readfds);
	select_timeout = (struct timeval){.tv_sec = 1, .tv_usec = 0};
	if (select(*sockfd + 1, &readfds, NULL, NULL, &select_timeout) < 0)
		return (print_errorm1("select error"));
	ret = read_stdin(&readfds);
	if (ret == -1)
		return (-1);
	else if (ret == 1)
		return (1);
	ret = read_socket(sockfd, &readfds, minirt);
	if (ret == -1)
		return (-1);
	else if (ret == 1 && *sockfd == -1)
		return (-1);
	return (0);
}

void	connect_client(char *ip, int port, char *password, t_minirt *minirt)
{
	int	sockfd;
	int	keepalive;

	minirt->screen.start_render = 0;
	minirt->screen.client_samples = 0;
	signal(SIGPIPE, do_nothing);
	if (active_mode(&sockfd, ip, port, password) < 0)
		if (passive_mode(&sockfd, minirt) < 0)
			return ((void)print_error("Failed to enable passive mode"));
	keepalive = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, &keepalive, sizeof(keepalive));
	while (1)
	{
		if (verify_server_connection(&sockfd, minirt))
			break ;
		if (listen_stdin_and_socket(&sockfd, minirt) < 0)
			break ;
		calc_sample_for_server(minirt, &sockfd);
	}
	close(sockfd);
}
