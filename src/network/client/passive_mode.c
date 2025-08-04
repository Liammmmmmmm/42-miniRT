/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   passive_mode.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 11:48:32 by lilefebv          #+#    #+#             */
/*   Updated: 2025/08/04 15:36:21 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "network.h"
#include "utils.h"

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
	if (bind(*pass_fd, (struct sockaddr*)&pass_addr, sizeof(pass_addr)) < 0)
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

int	passive_mode(int *sockfd)
{
	int					pass_fd;
	struct sockaddr_in	server_addr;
	socklen_t			addr_len;
	char				server_ip[INET_ADDRSTRLEN];

	if (init_passive_listening(&pass_fd) < 0)
		return (-1);
	addr_len = sizeof(server_addr);
	*sockfd = accept(pass_fd, (struct sockaddr*)&server_addr, &addr_len);
	close(pass_fd);
	if (*sockfd < 0)
		return (print_errorm1("accept server request failed"));
	inet_ntop(AF_INET, &server_addr.sin_addr, server_ip, INET_ADDRSTRLEN);
	printf("Connected to the server %s\n", server_ip);
	return (0);
}