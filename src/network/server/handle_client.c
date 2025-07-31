/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_client.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 18:28:59 by lilefebv          #+#    #+#             */
/*   Updated: 2025/07/31 18:29:12 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "network.h"

void	*handle_client(void *arg)
{
	t_client_data *data = (t_client_data*)arg;
	int client_fd = data->client_fd;
	char client_ip[INET_ADDRSTRLEN];
	
	inet_ntop(AF_INET, &(data->client_addr.sin_addr), client_ip, INET_ADDRSTRLEN);
	printf("Thread: Nouveau client %s\n", client_ip);

	char buffer[BUFFER_SIZE];
	ssize_t bytes_received;

	if (password_connexion(data, client_fd) < 0)
		return (NULL);

	while ((bytes_received = recv(client_fd, buffer, BUFFER_SIZE, 0)) > 0
		&& g_server_fd != -1)
	{
		buffer[bytes_received] = '\0';
		printf("Reçu de %s: %s\n", client_ip, buffer);
		send(client_fd, "Ok bro j'ai recu\0", 18, 0);
	}

	if (bytes_received == 0) {
		printf("Client %s déconnecté\n", client_ip);
	} else {
		perror("recv error");
	}

	close(client_fd);
	free(data);
	return NULL;
}