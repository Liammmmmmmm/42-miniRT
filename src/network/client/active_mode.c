/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   active_mode.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 12:18:40 by lilefebv          #+#    #+#             */
/*   Updated: 2025/08/05 14:22:15 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "network.h"
#include "utils.h"

int	try_connect_password(int *sockfd, char *password)
{
	char		challenge[32];
	uint64_t	hash;

	if (recv(*sockfd, challenge, sizeof(challenge), 0) != 32)
	{
		close(*sockfd);
		return (print_errorm1("the server didn't sent the challenge"));
	}
	hash = password_hash(password, challenge);
	if (send(*sockfd, (char *)&hash, 8, 0) < 0)
	{
		close(*sockfd);
		return (print_errorm1("server disconnected"));
	}
	if (recv(*sockfd, challenge, 8, 0) >= 0
		&& ft_strcmp(challenge, "AUTH_OK") == 0)
		printf("Successfully connected to the server\n");
	else
	{
		close(*sockfd);
		return (print_errorm1("Invalid password"));
	}
	return (0);
}

int	active_mode(int *sockfd, char *address, int port, char *password)
{
	struct sockaddr_in	server_addr;

	printf("IP: %s\n", address);
	*sockfd = socket(AF_INET, SOCK_STREAM, 0);
	server_addr = (struct sockaddr_in){.sin_family = AF_INET,
		.sin_port = htons(port), .sin_addr.s_addr = inet_addr(address)
	};
	if (connect_with_timeout(*sockfd, (struct sockaddr*)&server_addr,
			sizeof(server_addr), 5) < 0)
	{
		close(*sockfd);
		return (print_errorm1("The server was not found"));
	}
	if (try_connect_password(sockfd, password) < 0)
		return (-1);
	return (0);
}
