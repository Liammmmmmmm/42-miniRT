/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 13:10:02 by lilefebv          #+#    #+#             */
/*   Updated: 2025/08/06 10:38:56 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "network.h"
#include "utils.h"
#include "minirt.h"
#include <signal.h>

static int	verify_server_connection(int *sockfd)
{
	if (!is_connection_alive(*sockfd))
	{
		printf("Server disconnected\n");
		if (passive_mode(sockfd) < 0)
			return (print_errorm1("failed to enable passive mode"));
	}
	return (0);
}

int	read_stdin(int *sockfd, fd_set *readfds)
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
	// char header[10];
	// *(uint64_t *)header = 1234567890123456;
	// *(uint16_t *)(header + 8) = 12345;
	// if (send(*sockfd, header, 10, 0) < 0)
	// {
	// 	free(line);
	// 	if (passive_mode(sockfd) < 0)
	// 		return (print_errorm1("failed to enable passive mode"));
	// 	else
	// 		return (1);
	// }
	free(line);
	return (0);
}

static int	read_socket(int *sockfd, fd_set *readfds)
{
	char	response[1024];
	ssize_t	bytes;

	if (!FD_ISSET(*sockfd, readfds))
		return (0);
	bytes = recv(*sockfd, response, sizeof(response), 0);
	if (bytes <= 0)
	{
		*sockfd = -1;
		if (passive_mode(sockfd) < 0)
			return (print_errorm1("failed to enable passive mode"));
		else
			return (1);
	}
	response[bytes] = '\0';
	printf("%s", response);
	return (0);
}

static int	listen_stdin_and_socket(int *sockfd)
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
	ret = read_stdin(sockfd, &readfds);
	if (ret == -1)
		return (-1);
	else if (ret == 1)
		return (1);
	ret = read_socket(sockfd, &readfds);
	if (ret == -1)
		return (-1);
	else if (ret == 1 && *sockfd == -1)
		return (-1);
	return (0);
}

void	calc_sample_for_server(t_minirt *minirt, int *sockfd)
{
	minirt->options.client.sockfd = sockfd;
	render(minirt);
}

void	do_nothing(int sig)
{
	(void)sig;
	print_error("Server disconnected while sending a frame");
}

void	connect_client(char *ip, int port, char *password, t_minirt *minirt)
{
	int	sockfd;
	int	keepalive;

	signal(SIGPIPE, do_nothing);
	if (active_mode(&sockfd, ip, port, password) < 0)
		if (passive_mode(&sockfd) < 0)
			return ((void)print_error("Failed to enable passive mode"));
	keepalive = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, &keepalive, sizeof(keepalive));
	while (1)
	{
		if (verify_server_connection(&sockfd))
			break ;
		if (listen_stdin_and_socket(&sockfd) < 0)
			break ;
		calc_sample_for_server(minirt, &sockfd);
	}
	close(sockfd);
}
