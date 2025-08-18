/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 17:03:37 by lilefebv          #+#    #+#             */
/*   Updated: 2025/08/18 16:23:37 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "network.h"

char	*recv_large_data(int fd, uint64_t bytes_to_read)
{
	char		*data_buff;
	uint64_t	total_received;
	ssize_t		chunk_received;

	data_buff = malloc(bytes_to_read);
	if (!data_buff)
		return (NULL);
	total_received = 0;
	while (total_received < bytes_to_read)
	{
		chunk_received = recv(fd, data_buff + total_received,
				bytes_to_read - total_received, 0);
		if (chunk_received <= 0)
		{
			free(data_buff);
			return (NULL);
		}
		total_received += chunk_received;
	}
	return (data_buff);
}

int	send_scene_data(int *sockfd, char *data, uint64_t size, uint16_t type)
{
	char	header[10];

	if (!data)
		size = 0;
	*(uint64_t *)header = size;
	*(uint16_t *)(header + 8) = type;
	if (*sockfd == -1)
		return (0);
	if (send(*sockfd, header, 10, 0) < 0)
		return (-1);
	if (*sockfd == -1)
		return (0);
	if (size > 0)
	{
		if (send(*sockfd, data, size, 0) < 0)
			return (-1);
	}
	return (0);
}
