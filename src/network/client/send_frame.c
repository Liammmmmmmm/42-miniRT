/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   send_frame.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 16:50:09 by lilefebv          #+#    #+#             */
/*   Updated: 2025/08/05 17:24:27 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "network.h"
#include "utils.h"

int	send_frame_to_server(const float *ptr, uint64_t tpx, uint16_t samples, int *sockfd)
{
	char	header[10];

	*(uint64_t *)header = tpx * 4 * 4;
	*(uint16_t *)(header + 8) = samples;
	if (send(*sockfd, header, 10, 0) < 0)
	{
		if (passive_mode(sockfd) < 0)
			return (print_errorm1("failed to enable passive mode"));
		else
			return (1);
	}
	printf("%f %f %f\n", ptr[0], ptr[1], ptr[2]);
	if (send(*sockfd, ptr, tpx * 4 * 4, 0) < 0)
	{
		if (passive_mode(sockfd) < 0)
			return (print_errorm1("failed to enable passive mode"));
		else
			return (1);
	}
	printf("Successfully sent %u frames to the server\n", samples);
	return (0);
}