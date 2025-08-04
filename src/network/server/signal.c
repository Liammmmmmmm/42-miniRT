/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 10:07:56 by lilefebv          #+#    #+#             */
/*   Updated: 2025/08/04 15:37:18 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "network.h"

void	handle_sigint(int sig)
{
	(void)sig;
	printf("\nArrêt du serveur...\n");
	shutdown(g_server_fd, SHUT_RDWR);
	close(g_server_fd);
	g_server_fd = -1;
	exit(130);
}
