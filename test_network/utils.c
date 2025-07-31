#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <time.h>
#include <sys/select.h>
#include "libft.h"

int connect_with_timeout(int sockfd, const struct sockaddr *addr, socklen_t addrlen, int timeout_sec) {
	// 1. Passe le socket en non-bloquant
	int flags = fcntl(sockfd, F_GETFL, 0);
	fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);

	// 2. Lance la connexion (retourne immédiatement)
	int connect_result = connect(sockfd, addr, addrlen);
	if (connect_result == 0) {
		// Connexion immédiate (rare)
		fcntl(sockfd, F_SETFL, flags);  // Rétablit le mode bloquant
		return 0;
	}

	if (errno != EINPROGRESS) {
		// Erreur autre que "en cours"
		return -1;
	}

	// 3. Configure le timeout avec select()
	fd_set writefds;
	FD_ZERO(&writefds);
	FD_SET(sockfd, &writefds);

	struct timeval timeout = {
		.tv_sec = timeout_sec,
		.tv_usec = 0
	};

	int select_result = select(sockfd + 1, NULL, &writefds, NULL, &timeout);
	if (select_result <= 0) {
		// Timeout ou erreur
		close(sockfd);
		return -1;
	}

	// 4. Vérifie si la connexion a réussi
	int socket_error;
	socklen_t len = sizeof(socket_error);
	getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &socket_error, &len);

	if (socket_error != 0) {
		// Erreur de connexion
		close(sockfd);
		errno = socket_error;
		return -1;
	}

	// 5. Rétablit le mode bloquant
	fcntl(sockfd, F_SETFL, flags);
	return 0;
}

/**
 * djb2 variant
 */
unsigned long	password_hash(const char *input, const char *challenge)
{
	unsigned long	hash = 5381;
	int				c;

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
