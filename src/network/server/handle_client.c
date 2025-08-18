/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_client.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 18:28:59 by lilefebv          #+#    #+#             */
/*   Updated: 2025/08/18 10:28:05 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "network.h"
#include "utils.h"

static void	*close_connection(t_client_data data)
{
	close(data.client_fd);
	return (NULL);
}

static float	*get_data(ssize_t *bytes_received, int client_fd,
	t_sample_utils_data sd)
{
	char		header_buff[14];
	uint64_t	bytes_to_read;
	char		*data_buff;

	*bytes_received = recv(client_fd, header_buff, 14, 0);
	if (*bytes_received <= 0)
		return (NULL);
	if (*bytes_received != 14)
		return (NULL);
	bytes_to_read = *(uint64_t *)header_buff;
	*sd.samples = *(uint16_t *)(header_buff + 8);
	*sd.anime_i = *(uint32_t *)(header_buff + 10);
	if (bytes_to_read > (uint64_t)(sd.tpx * 4 * 3))
		return (NULL);
	data_buff = recv_large_data(client_fd, bytes_to_read);
	if (!data_buff)
		return (NULL);
	*bytes_received = bytes_to_read;
	if ((uint64_t)(*bytes_received) != sd.tpx * 4 * 3
		|| *sd.anime_i != sd.frame_i)
	{
		free(data_buff);
		return (NULL);
	}
	return ((float *)data_buff);
}

int	receive_client_info(t_minirt *minirt, t_client_data *data,
	ssize_t *bytes_received, char *c_ip)
{
	uint16_t	samples;
	uint32_t	anime_i;
	float		*data_float;
	int			tpx;
	int			i;

	tpx = minirt->scene.render_height * minirt->scene.render_width;
	data_float = get_data(bytes_received, data->client_fd, (t_sample_utils_data)
		{&anime_i, &samples, (uint32_t)tpx, minirt->options.anim.frame_i});
	if (!data_float)
		return (0);
	pthread_mutex_lock(&minirt->screen.sample_mutex);
	i = -1;
	while (++i < tpx)
		minirt->screen.float_render[i] = (t_fcolor){data_float[i * 3],
			data_float[i * 3 + 1], data_float[i * 3 + 2]};
	minirt->screen.sample += samples;
	minirt->screen.sample_total_anim++;
	minirt->screen.last_sample_am = minirt->screen.sample;
	pthread_mutex_unlock(&minirt->screen.sample_mutex);
	free(data_float);
	printf("Successfully recieved %u samples from %s (%lu bytes)\n", samples,
		c_ip, *bytes_received);
	return (0);
}

void	client_life(t_client_data *data, ssize_t *bytes_received,
	char c_ip[INET_ADDRSTRLEN])
{
	pthread_t	monitoring;

	if (password_connexion(data, data->client_fd) < 0)
	{
		print_error("Client password connection failed");
		close(data->client_fd);
		return ;
	}
	send_map_first_time(data->minirt, &data->client_fd);
	if (pthread_create(&monitoring, NULL, client_monitoring, (void *)data)
		!= 0)
	{
		close(data->client_fd);
		print_error("pthread_create failed");
		return ;
	}
	while (g_server_fd != -1)
	{
		if (receive_client_info(data->minirt, data, bytes_received, c_ip) < 0)
			break ;
	}
	data->monitor = 0;
	pthread_join(monitoring, NULL);
}

void	*handle_client(void *arg)
{
	t_client_data	data;
	char			c_ip[INET_ADDRSTRLEN];
	ssize_t			bytes_received;

	data = *(t_client_data *)arg;
	data.monitor = 1;
	free(arg);
	inet_ntop(AF_INET, &(data.client_addr.sin_addr), c_ip, INET_ADDRSTRLEN);
	printf("Thread: New client %s\n", c_ip);
	bytes_received = 0;
	client_life(&data, &bytes_received, c_ip);
	if (bytes_received == 0)
		printf("Client %s disconnected\n", c_ip);
	else
		print_error("recv error");
	return (close_connection(data));
}
