/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 13:10:02 by lilefebv          #+#    #+#             */
/*   Updated: 2025/08/04 15:32:07 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "network.h"
#include "utils.h"

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

static int	read_stdin(int *sockfd, fd_set *readfds)
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
	if (send(*sockfd, line, ft_strlen(line), 0) < 0)
	{
		free(line);
		if (passive_mode(sockfd) < 0)
			return (print_errorm1("failed to enable passive mode"));
		else
			return (1);
	}
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
		if (passive_mode(sockfd) < 0)
			return (print_errorm1("failed to enable passive mode"));
		else
			return (1);
	}
	response[bytes] = '\0';
	printf("%s", response);
	return (0);
}

static int	listen_stdin_and_socker(int *sockfd)
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
	else if (ret == 1)
		return (1);
	return (0);
}

void	connect_client(char *ip, int port, char *password)
{
	int	sockfd;
	int	keepalive;

	if (active_mode(&sockfd, ip, port, password) < 0)
		if (passive_mode(&sockfd) < 0)
			return ((void)print_error("Failed to enable passive mode"));
	keepalive = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, &keepalive, sizeof(keepalive));
	while (1)
	{
		if (verify_server_connection(&sockfd))
			break ;
		if (listen_stdin_and_socker(&sockfd) < 0)
			break ;
	}
	close(sockfd);
}
