/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_client.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 18:28:59 by lilefebv          #+#    #+#             */
/*   Updated: 2025/08/04 10:38:15 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "network.h"
#include "utils.h"

static void	*close_connection(t_client_data *data)
{
	close(data->client_fd);
	free(data);
	return (NULL);
}

void	*handle_client(void *arg)
{
	t_client_data	*data;
	char			c_ip[INET_ADDRSTRLEN];
	char			buffer[SERV_BUFF_SIZE + 1];
	ssize_t			bytes_received;

	data = arg;
	inet_ntop(AF_INET, &(data->client_addr.sin_addr), c_ip, INET_ADDRSTRLEN);
	printf("Thread: New client %s\n", c_ip);
	if (password_connexion(data, data->client_fd) < 0)
	{
		print_error("Client password connection failed");
		return (close_connection(data));
	}
	bytes_received = recv(data->client_fd, buffer, SERV_BUFF_SIZE, 0);
	while (bytes_received > 0 && g_server_fd != -1)
	{
		buffer[bytes_received] = '\0';
		printf("Reçu de %s: %s\n", c_ip, buffer);
		send(data->client_fd, "Ok bro j'ai recu\0", 18, 0);
	}
	if (bytes_received == 0)
		printf("Client %s disconnected\n", c_ip);
	else
		print_error("recv error");
	return (close_connection(data));
}