/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   password.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 10:10:28 by lilefebv          #+#    #+#             */
/*   Updated: 2025/08/04 10:24:32 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "network.h"

static int	close_fd_m1(int fd)
{
	close(fd);
	return (-1);
}

/**
 * How do we manage a client connection with a password ?
 * First, we generate a "challenge", a 31 random bytes key.
 * We send this key to the client, and the client will use it to hash it's
 * password. Then it will send it to the server, and the server compare it to
 * it's own password hashed with the key. Both are same, the password is ok,
 * otherwise the password isn't ok.
 * 
 * Why doing this ? This allow the server to compare it's password with the
 * client without ever sending it directly, or sending anything that someone
 * could sniff and send to connect to the server.
 */
int	password_connexion(t_client_data *data, int client_fd)
{
	char		challenge[32];
	uint64_t	hash;
	char		client_hash_char[8];
	int			i;

	if (!data->password)
		return (0);
	i = -1;
	while (++i < 31)
		challenge[i] = random_basic_char();
	challenge[31] = '\0';
	send(client_fd, challenge, 32, 0);
	hash = password_hash(data->password, challenge);
	if (recv(client_fd, client_hash_char, 8, 0) != 8)
		return (close_fd_m1(client_fd));
	if (*((unsigned long *)client_hash_char) == hash)
		send(client_fd, "AUTH_OK", 8, 0);
	else
		return (close_fd_m1(client_fd));
	printf("Client successfully connected\n");
	return (0);
}
