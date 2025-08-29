/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 16:19:43 by lilefebv          #+#    #+#             */
/*   Updated: 2025/08/29 10:19:38 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "gpu.h"
#include "network.h"
#include "utils.h"
#include "minirt.h"

void	do_nothing(int sig)
{
	(void)sig;
	print_error("Server disconnected while sending a frame");
}

void	calc_sample_for_server(t_minirt *minirt, int *sockfd)
{
	if (!minirt->screen.start_render)
		return ;
	printf("Called to calculate a frame\n");
	minirt->options.client.sockfd = sockfd;
	minirt->screen.sample = 1;
	render_gpu(minirt);
}

int	no_data_instruction(t_minirt *minirt, uint16_t type)
{
	if (type == SRV_COMPUTE_START)
	{
		delete_ssbo(&minirt->shaders_data.ssbo);
		init_ssbo(&minirt->shaders_data, minirt->scene.render_width,
			minirt->scene.render_height);
		free(minirt->screen.client_accumulation);
		minirt->screen.client_accumulation = ft_calloc(sizeof(float),
				3 * minirt->scene.render_width * minirt->scene.render_height);
		if (minirt->screen.client_accumulation)
			minirt->screen.start_render = 1;
		minirt->screen.client_samples = 0;
		minirt->screen.client_last_sample_send = get_cpu_time();
	}
	else if (type == SRV_COMPUTE_STOP)
		minirt->screen.start_render = 0;
	return (0);
}

int	recv_error_client(int *sockfd, t_minirt *minirt)
{
	*sockfd = -1;
	if (passive_mode(sockfd, minirt) < 0)
		return (print_errorm1("failed to enable passive mode"));
	else
		return (1);
}
