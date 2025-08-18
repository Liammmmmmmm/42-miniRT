/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   send_frame.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 16:50:09 by lilefebv          #+#    #+#             */
/*   Updated: 2025/08/18 16:11:08 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "network.h"
#include "utils.h"

int	send_frame_to_server(const float *ptr, t_sample_net_data sett, int *sockfd)
{
	char	header[14];

	*(uint64_t *)header = sett.tpx * 4 * 3;
	*(uint16_t *)(header + 8) = sett.samples;
	*(uint32_t *)(header + 10) = sett.anime_i;
	if (*sockfd == -1)
		return (0);
	if (send(*sockfd, header, 14, 0) < 0)
		return (-1);
	if (send(*sockfd, ptr, sett.tpx * 4 * 3, 0) < 0)
		return (-1);
	printf("Successfully sent %u frames to the server\n", sett.samples);
	return (0);
}
