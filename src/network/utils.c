/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 17:03:37 by lilefebv          #+#    #+#             */
/*   Updated: 2025/07/31 17:58:46 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <stdlib.h>

/**
 * djb2 hash variant
 */
unsigned long	password_hash(const char *input, const char *challenge)
{
	unsigned long	hash;
	int				c;

	hash = 5381;
	c = *input++;
	while (c)
	{
		hash = ((hash << 5) + hash) + c;
		c = *input++;
	}
	c = *challenge++;
	while (c)
	{
		hash = ((hash << 5) + hash) + c;
		c = *challenge++;
	}
	return (hash);
}

static int	set_flag_ret(int fd, int flags, int ret)
{
	fcntl(fd, F_SETFL, flags);
	return (ret);
}

/**
 * Function explaination:
 * 1. We get the flags for the current state of the fd.
 * 2. We add to this list of flags the flag NONBLOCK in order to avoid
 * staying blocked in the function connect.
 * 3. We try to connect, if we instantly connect we reset the default flags and
 * return 0.
 * 4. We set the differentes values for select.
 * 5. We use select to connect with a timeout.
 * 6. We verify the connexion.
 * 7. We reset the differents flags.
 */
int	connect_with_timeout(int sockfd, const struct sockaddr *addr,
	socklen_t addrlen, int timeout_sec)
{
	fd_set			writefds;
	int				flags;
	int				socket_error;
	socklen_t		len;
	struct timeval	interval;

	if (sockfd < 0 || !addr || timeout_sec < 0)
		return (-1);
	flags = fcntl(sockfd, F_GETFL, 0);
	if (flags == -1 || fcntl(sockfd, F_SETFL, flags | O_NONBLOCK) == -1)
		return (-1);
	if (connect(sockfd, addr, addrlen) == 0)
		return (set_flag_ret(sockfd, flags, 0));
	if (errno != EINPROGRESS)
		return (set_flag_ret(sockfd, flags, -1));
	FD_ZERO(&writefds);
	FD_SET(sockfd, &writefds);
	interval = (struct timeval){.tv_sec = timeout_sec, .tv_usec = 0};
	if (select(sockfd + 1, NULL, &writefds, NULL, &interval) <= 0)
		return (set_flag_ret(sockfd, flags, -1));
	len = sizeof(socket_error);
	getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &socket_error, &len);
	if (socket_error != 0)
		return (set_flag_ret(sockfd, flags, -1));
	return (set_flag_ret(sockfd, flags, 0));
}
