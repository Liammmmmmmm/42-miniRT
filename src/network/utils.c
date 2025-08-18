/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 17:03:37 by lilefebv          #+#    #+#             */
/*   Updated: 2025/08/18 16:23:30 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "network.h"

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

char	random_basic_char(void)
{
	const int	randnb = rand() % 82;

	if (randnb < 26)
		return ('a' + randnb);
	else if (randnb < 72)
		return ('A' + randnb - 26);
	else
		return ('0' + randnb - 72);
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

int	is_connection_alive(int sockfd)
{
	char	buf[1];
	ssize_t	ret;

	ret = recv(sockfd, buf, 1, MSG_PEEK | MSG_DONTWAIT);
	if (ret == 0)
		return (0);
	else if (ret == -1)
	{
		if (errno == EAGAIN || errno == EWOULDBLOCK)
			return (1);
		return (0);
	}
	return (1);
}
