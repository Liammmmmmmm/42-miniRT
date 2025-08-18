/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connect_passive_client.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 09:50:49 by lilefebv          #+#    #+#             */
/*   Updated: 2025/08/18 09:07:24 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "network.h"

int	connect_to_passive_client(const char *client_ip)
{
	int					sockfd;
	struct sockaddr_in	client_addr;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	client_addr = (struct sockaddr_in){
		.sin_family = AF_INET,
		.sin_port = htons(14242),
		.sin_addr.s_addr = inet_addr(client_ip)
	};
	if (connect_with_timeout(sockfd, (struct sockaddr *)&client_addr,
			sizeof(client_addr), 3) < 0)
	{
		close(sockfd);
		return (-1);
	}
	return (sockfd);
}
