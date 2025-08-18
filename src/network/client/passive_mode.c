/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   passive_mode.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 11:48:32 by lilefebv          #+#    #+#             */
/*   Updated: 2025/08/18 10:46:49 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "network.h"
#include "utils.h"
#include "maths.h"
#include <sys/select.h>
#include <string.h>

#define PASSIVE_PORT 14242

static int	init_passive_listening(int *pass_fd)
{
	struct sockaddr_in	pass_addr;
	int					reuse;

	*pass_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (*pass_fd < 0)
		return (print_errorm1("passive socket failed"));
	pass_addr = (struct sockaddr_in){.sin_family = AF_INET,
		.sin_port = htons(PASSIVE_PORT), .sin_addr.s_addr = INADDR_ANY};
	reuse = 1;
	setsockopt(*pass_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
	if (bind(*pass_fd, (struct sockaddr *)&pass_addr, sizeof(pass_addr)) < 0)
	{
		close(*pass_fd);
		return (print_errorm1("bind passive failed"));
	}
	if (listen(*pass_fd, 1) < 0)
	{
		close(*pass_fd);
		return (print_errorm1("listen passive failed"));
	}
	printf("Passively waiting on port %d...\n", PASSIVE_PORT);
	return (0);
}

static int	passive_listening_network_and_cli_loop(int pass_fd, int *sockfd,
	struct sockaddr_in server_addr, socklen_t addr_len)
{
	fd_set	read_fds;

	while (1)
	{
		FD_ZERO(&read_fds);
		FD_SET(pass_fd, &read_fds);
		FD_SET(STDIN_FILENO, &read_fds);
		if (select(pass_fd + 1, &read_fds, NULL, NULL, NULL) < 0)
			break ;
		if (read_stdin(&read_fds) < 0)
			return (0);
		if (FD_ISSET(pass_fd, &read_fds))
		{
			*sockfd = accept(pass_fd, (struct sockaddr *)&server_addr,
					&addr_len);
			break ;
		}
	}
	return (1);
}

int	passive_mode(int *sockfd, t_minirt *minirt)
{
	int					pass_fd;
	struct sockaddr_in	server_addr;
	socklen_t			addr_len;
	char				server_ip[INET_ADDRSTRLEN];

	minirt->screen.start_render = 0;
	if (init_passive_listening(&pass_fd) < 0)
		return (-1);
	addr_len = sizeof(server_addr);
	if (passive_listening_network_and_cli_loop(pass_fd, sockfd, server_addr,
			addr_len) == 0)
		return (0);
	close(pass_fd);
	if (*sockfd < 0)
		return (print_errorm1("accept server request failed"));
	inet_ntop(AF_INET, &server_addr.sin_addr, server_ip, INET_ADDRSTRLEN);
	printf("Connected to the server %s\n", server_ip);
	return (0);
}
