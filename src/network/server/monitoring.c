/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 14:50:09 by lilefebv          #+#    #+#             */
/*   Updated: 2025/08/07 18:01:55 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "network.h"

void	check_sample_amount_server(t_minirt *minirt, int fd, t_uint *last_frame_map, t_bool *asked_to_stop)
{
	pthread_mutex_lock(&minirt->screen.sample_mutex);
	if (minirt->screen.sample >= minirt->screen.spp)
	{
		if (minirt->screen.client_sample_exceed == 0)
		{
			minirt->screen.client_sample_exceed = get_cpu_time();
			if (minirt->options.anim.enabled && minirt->options.anim.frame_i
				< minirt->options.anim.frames)
			{
				minirt->options.anim.frame_i++;
				init_scene_server(minirt);
				convert_scene_server(minirt, &minirt->scene, &minirt->viewport, &minirt->shaders_data.scene);
				minirt->options.anim.frame_i--;
			}
		}
		if (*asked_to_stop == 0)
		{
			send_scene_data(&fd, NULL, 0, SRV_COMPUTE_STOP);
			*asked_to_stop = 1;
		}
		if (minirt->options.anim.enabled && minirt->options.anim.frame_i
			< minirt->options.anim.frames)
		{
			minirt->options.anim.frame_i++;
			if (*last_frame_map != minirt->options.anim.frame_i)
			{
				send_changing_map_part(minirt, &fd);
				*last_frame_map = minirt->options.anim.frame_i;
			}
			minirt->options.anim.frame_i--;
		}
	}
	pthread_mutex_unlock(&minirt->screen.sample_mutex);
}

void	*client_monitoring(void *arg)
{
	t_client_data	*data;
	t_minirt		*minirt;
	t_uint			last_frame_map;
	t_bool			asked_to_stop;

	last_frame_map = 0;
	asked_to_stop = 0;
	data = arg;
	minirt = data->minirt;
	while (g_server_fd != -1 && data->monitor)
	{
		check_sample_amount_server(minirt, data->client_fd, &last_frame_map, &asked_to_stop);
		if (last_frame_map == minirt->options.anim.frame_i)
			asked_to_stop = 0;
		usleep(1000000);
	}
	return (NULL);
}
