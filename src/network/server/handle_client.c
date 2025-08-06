/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_client.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 18:28:59 by lilefebv          #+#    #+#             */
/*   Updated: 2025/08/06 10:00:36 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "network.h"
#include "utils.h"

static void	*close_connection(t_client_data data)
{
	close(data.client_fd);
	return (NULL);
}

int	receive_client_info(t_minirt *minirt, t_client_data *data, ssize_t *bytes_received, char *c_ip)
{
	char		header_buff[10];
	uint64_t	bytes_to_read;
	uint16_t	samples;
	char		*data_buff;
	float		*data_float;

	int	tpx = minirt->scene.render_height * minirt->scene.render_width;
	
	*bytes_received = recv(data->client_fd, header_buff, 10, 0);
	if (*bytes_received <= 0)
		return (-1);
	if (*bytes_received != 10)
		return (0);
	bytes_to_read = *(uint64_t *)header_buff;
	samples = *(uint16_t *)(header_buff + 8);

	printf("Client qui tente d'envoyer %u frames (%lu bytes)\n", samples, bytes_to_read);

	if (bytes_to_read > (uint64_t)(tpx * 4 * 4))
		return (0); // Secure on empeche un boug de faire malloc 17go au serveur
	
		
	data_buff = malloc(bytes_to_read);
	if (!data_buff)
		return (0);
	
	uint64_t total_received = 0;
	while (total_received < bytes_to_read)
	{
		ssize_t chunk_received = recv(data->client_fd, data_buff + total_received, 
									bytes_to_read - total_received, 0);
		
		if (chunk_received <= 0)
		{
			free(data_buff);
			return (-1);
		}
		total_received += chunk_received;
	}
	
	*bytes_received = total_received;
		
	if (*bytes_received != (ssize_t)bytes_to_read || *bytes_received != tpx * 4 * 4)
	{
		free(data_buff);
		return (0);
	}

	data_float = (float *)data_buff;

	pthread_mutex_lock(&minirt->screen.sample_mutex);

	int	i = -1;
	while (++i < tpx)
	{
		// printf("Pixel : %f %f %f\n", data_float[i * 4], data_float[i * 4 + 1], data_float[i * 4 + 2]);
		minirt->screen.float_render[i].r += data_float[i * 4];
		minirt->screen.float_render[i].g += data_float[i * 4 + 1];
		minirt->screen.float_render[i].b += data_float[i * 4 + 2];
	}

	minirt->screen.sample += samples;
	minirt->screen.sample_total_anim++;
	minirt->screen.last_sample_am = minirt->screen.sample;

	pthread_mutex_unlock(&minirt->screen.sample_mutex);

	free(data_buff);
	
	printf("Successfully recieved %u samples from %s (%lu bytes)\n", samples, c_ip, bytes_to_read);
	
	// send(data->client_fd, "Ok bro j'ai recu\0", 18, 0);
	return (0);
}

void	*handle_client(void *arg)
{
	t_client_data	data;
	char			c_ip[INET_ADDRSTRLEN];
	// char			buffer[SERV_BUFF_SIZE + 1];
	ssize_t			bytes_received;

	data = *(t_client_data *)arg;
	free(arg);
	inet_ntop(AF_INET, &(data.client_addr.sin_addr), c_ip, INET_ADDRSTRLEN);
	printf("Thread: New client %s\n", c_ip);
	if (password_connexion(&data, data.client_fd) < 0)
	{
		print_error("Client password connection failed");
		return (close_connection(data));
	}
	while (g_server_fd != -1)
	{
		if (receive_client_info(data.minirt, &data, &bytes_received, c_ip) < 0)
			break ;
	}
	if (bytes_received == 0)
		printf("Client %s disconnected\n", c_ip);
	else
		print_error("recv error");
	return (close_connection(data));
}
