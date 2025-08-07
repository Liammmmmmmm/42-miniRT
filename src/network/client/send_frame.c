/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   send_frame.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 16:50:09 by lilefebv          #+#    #+#             */
/*   Updated: 2025/08/07 17:42:29 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "network.h"
#include "utils.h"

int	send_frame_to_server(const float *ptr, uint64_t tpx, uint16_t samples, uint32_t anime_i, int *sockfd)
{
	char	header[14];

	*(uint64_t *)header = tpx * 4 * 3;
	*(uint16_t *)(header + 8) = samples;
	*(uint32_t *)(header + 10) = anime_i;
	if (*sockfd == -1)
		return (0);
	if (send(*sockfd, header, 14, 0) < 0)
		return (-1);
	if (send(*sockfd, ptr, tpx * 4 * 3, 0) < 0)
		return (-1);
	printf("Successfully sent %u frames to the server\n", samples);
	return (0);
}
