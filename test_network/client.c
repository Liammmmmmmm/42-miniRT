#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include "libft.h"
#include <errno.h>
#include <sys/select.h>

#define PASSIVE_PORT 14242

int connect_with_timeout(int sockfd, const struct sockaddr *addr, socklen_t addrlen, int timeout_sec);
unsigned long	password_hash(const char *input, const char *challenge);

int passive_mode(int *sockfd) {
	// Crée un socket en écoute
	int passive_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (passive_fd < 0) {
		perror("passive socket failed");
		return -1;
	}

	struct sockaddr_in passive_addr = {
		.sin_family = AF_INET,
		.sin_port = htons(PASSIVE_PORT),
		.sin_addr.s_addr = INADDR_ANY
	};

	int reuse = 1;
	setsockopt(passive_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

	if (bind(passive_fd, (struct sockaddr*)&passive_addr, sizeof(passive_addr)) < 0)
	{
		perror("bind passive failed");
		close(passive_fd);
		return (-1);
	}

	if (listen(passive_fd, 1) < 0)
	{
		perror("listen passive failed");
		close(passive_fd);
		return (-1);
	}

	printf("En attente passive sur le port %d...\n", PASSIVE_PORT);

	struct sockaddr_in server_addr;
	socklen_t addr_len = sizeof(server_addr);
	*sockfd = accept(passive_fd, (struct sockaddr*)&server_addr, &addr_len);
	close(passive_fd);

	if (*sockfd < 0)
	{
		perror("accept passive failed");
		return (-1);
	}

	char server_ip[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &server_addr.sin_addr, server_ip, INET_ADDRSTRLEN);
	printf("Connexion reçue du serveur %s\n", server_ip);

	return 0;
}

int active_mode(int *sockfd, char *address, char *port, char *password)
{
	*sockfd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in server_addr = {
		.sin_family = AF_INET,
		.sin_port = htons(ft_atoi(port)),
		.sin_addr.s_addr = inet_addr(address)
	};

	if (connect_with_timeout(*sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr), 5) < 0) {
		close(*sockfd);
		return -1;
	}

	char challenge[32];
	ssize_t bytes = recv(*sockfd, challenge, sizeof(challenge), 0);
	if (bytes != 32)
	{
		close(*sockfd);
		return (-1);
	}

	unsigned long hash = password_hash(password, challenge);

	bytes = send(*sockfd, (char *)&hash, 8, 0);
	if (bytes < 0)
	{
		close(*sockfd);
		return (-1);
	}

	bytes = recv(*sockfd, challenge, 8, 0);
	if (bytes >= 0 && ft_strcmp(challenge, "AUTH_OK") == 0)
		printf("Connexion ok\n");
	else
	{
		printf("Mot de passe invalide\n");
		close(*sockfd);
		return (-1);
	}

	return 0;
}

int is_connection_alive(int sockfd) {
	char buf[1];
	ssize_t ret = recv(sockfd, buf, 1, MSG_PEEK | MSG_DONTWAIT);
	
	if (ret == 0) {
		return 0;  // Connexion fermée proprement
	} else if (ret == -1) {
		if (errno == EAGAIN || errno == EWOULDBLOCK) {
			return 1;  // Connexion toujours active (pas de données à lire)
		}
		return 0;  // Erreur = connexion morte
	}
	return 1;  // Données disponibles = connexion active
}

int main(int argc, char **argv) {
	int sockfd;

	if (argc != 4) {
		printf("Usage: %s <ip> <port> <password>\n", argv[0]);
		printf("Aucun serveur fournis, basculement en mode passif...\n");
		if (passive_mode(&sockfd) < 0) {
			printf("Échec du mode passif\n");
			return 1;
		}
	}
	else
	{
		if (active_mode(&sockfd, argv[1], argv[2], argv[3]) < 0)
		{
			printf("Erreur de connexion au serveur...\n");
			if (passive_mode(&sockfd) < 0) {
				printf("Échec du mode passif\n");
				return 1;
			}
		}
	}

	// Configurer le socket pour détecter les déconnexions
	int keepalive = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, &keepalive, sizeof(keepalive));

	while (1) {
		// Vérifier régulièrement la connexion
		if (!is_connection_alive(sockfd)) {
			printf("Serveur déconnecté\n");
			if (passive_mode(&sockfd) < 0) {
				printf("Échec du mode passif\n");
				break ;
			}
			else
				continue ;
		}

		// Gestion de l'entrée utilisateur avec timeout
		fd_set readfds;
		FD_ZERO(&readfds);
		FD_SET(STDIN_FILENO, &readfds);
		FD_SET(sockfd, &readfds);

		struct timeval select_timeout = {.tv_sec = 1, .tv_usec = 0};
		
		int activity = select(sockfd + 1, &readfds, NULL, NULL, &select_timeout);
		
		if (activity < 0) {
			perror("select error");
			break;
		}

		// Vérifier l'entrée utilisateur
		if (FD_ISSET(STDIN_FILENO, &readfds)) {
			char *line = get_next_line(STDIN_FILENO);
			if (!line || ft_strcmp("exit\n", line) == 0) {
				free(line);
				break;
			}
			
			if (send(sockfd, line, ft_strlen(line), 0) < 0) {
				free(line);
				if (passive_mode(&sockfd) < 0) {
					printf("Échec du mode passif\n");
					break ;
				}
				else
					continue ;
			}
			free(line);
		}

		// Vérifier les données du serveur
		if (FD_ISSET(sockfd, &readfds)) {
			char response[1024];
			ssize_t bytes = recv(sockfd, response, sizeof(response), 0);
			
			if (bytes <= 0) {
				if (passive_mode(&sockfd) < 0) {
					printf("Échec du mode passif\n");
					break ;
				}
				else
					continue ;
			}
			
			response[bytes] = '\0';
			printf("%s", response);
		}
	}

	close(sockfd);
	return 0;
}
