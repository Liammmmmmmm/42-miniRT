#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "libft.h"

#ifndef BUFFER_SIZE
	#define BUFFER_SIZE 1024
#endif
#define MAX_CLIENTS 1000

int connect_with_timeout(int sockfd, const struct sockaddr *addr, socklen_t addrlen, int timeout_sec);
unsigned long	password_hash(const char *input, const char *challenge);

int g_server_fd = -1;

typedef struct {
	int client_fd;
	struct sockaddr_in client_addr;
	char	*password;
} t_client_data;

void handle_sigint(int sig) {
	printf("\nArrêt du serveur...\n");
	shutdown(g_server_fd, SHUT_RDWR);
	close(g_server_fd);
	g_server_fd = -1;
	exit(130);
}

char	random_basic_char()
{
	const int	randnb = rand() % 82;
	if (randnb < 26)
		return ('a' + randnb);
	else if (randnb < 72)
		return ('A' + randnb - 26);
	else
		return ('0' + randnb - 72);
}

int	password_connexion(t_client_data *data, int client_fd)
{
	if (!data->password)
		return (0);

	char challenge[32];
	for (int i = 0; i < 31; i++) {
        challenge[i] = random_basic_char();
    }
    challenge[31] = '\0';
	send(client_fd, challenge, 32, 0);

	unsigned long hash = password_hash(data->password, challenge);

	char client_hash_char[8];
	ssize_t bytes = recv(client_fd, client_hash_char, 8, 0);
	if (bytes != 8)
	{
		close(client_fd);
		return (-1);
	}

	unsigned long	client_hash = *((unsigned long *)client_hash_char);

	if (client_hash == hash)
	{
		printf("Client connecté avec succes\n");
		send(client_fd, "AUTH_OK", 8, 0);
	}
	else
	{
		printf("Echec de la connexion du client\n");
		close(client_fd);
		return (-1);
	}
	return (0);
}

void *handle_client(void *arg)
{
	t_client_data *data = (t_client_data*)arg;
	int client_fd = data->client_fd;
	char client_ip[INET_ADDRSTRLEN];
	
	inet_ntop(AF_INET, &(data->client_addr.sin_addr), client_ip, INET_ADDRSTRLEN);
	printf("Thread: Nouveau client %s\n", client_ip);

	char buffer[BUFFER_SIZE];
	ssize_t bytes_received;

	if (password_connexion(data, client_fd) < 0)
		return (NULL);

	while ((bytes_received = recv(client_fd, buffer, BUFFER_SIZE, 0)) > 0
		&& g_server_fd != -1)
	{
		buffer[bytes_received] = '\0';
		printf("Reçu de %s: %s\n", client_ip, buffer);
		send(client_fd, "Ok bro j'ai recu\0", 18, 0);
	}

	if (bytes_received == 0) {
		printf("Client %s déconnecté\n", client_ip);
	} else {
		perror("recv error");
	}

	close(client_fd);
	free(data);
	return NULL;
}

int connect_to_passive_client(const char *client_ip)
{
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in client_addr = {
		.sin_family = AF_INET,
		.sin_port = htons(14242),
		.sin_addr.s_addr = inet_addr(client_ip)
	};
	
	if (connect_with_timeout(sockfd, (struct sockaddr*)&client_addr, sizeof(client_addr), 3) < 0) {
		close(sockfd);
		return -1;
	}
	return sockfd;
}

void	replace_breakline_zero(char *line)
{
	while (*line)
	{
		if (*line == '\n')
			*line = '\0';
		line++;
	}
}

void *cli_thread_routine(void *arg)
{
	(void)arg;
	while (g_server_fd != -1)
	{
		char *line = get_next_line(0);
		if (ft_strcmp(line, "exit\n") == 0)
		{
			printf("\nArrêt du serveur...\n");
			shutdown(g_server_fd, SHUT_RDWR);
			close(g_server_fd);
			g_server_fd = -1;
			exit(130);
			break ;
		}
		else if (ft_strncmp(line, "link ", 5) == 0)
		{
			if (ft_strlen(line) < 8)
			{
				printf("Invalid client address\n");
				free(line);
				continue ;
			}
			replace_breakline_zero(line);
			printf("Try connect to address : %s\n", line + 5);
			int client_id = connect_to_passive_client(line + 5);
			if (client_id == -1)
			{
				printf("Invalid client address\n");
				free(line);
				continue ;
			}

			t_client_data *client_data = malloc(sizeof(t_client_data));
			if (!client_data)
			{
				printf("Malloc error\n");
				free(line);
				continue ;
			}
		
			client_data->client_fd = client_id;
			client_data->client_addr = (struct sockaddr_in){
				.sin_family = AF_INET,
				.sin_port = htons(14242),
				.sin_addr.s_addr = inet_addr(line + 5)
			};

			pthread_t thread_id;
			if (pthread_create(&thread_id, NULL, handle_client, (void*)client_data) != 0) {
				perror("pthread_create failed");
				close(client_data->client_fd);
				free(client_data);
				continue;
			}
			pthread_detach(thread_id);

		}
		free(line);
	}
	return NULL;
}

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		printf("You need to launch the server with a password. ./server <password>\n");
		return (1);
	}

	signal(SIGINT, handle_sigint);

	g_server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (g_server_fd < 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	int reuse = 1;
	setsockopt(g_server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

	struct sockaddr_in addr = {
		.sin_family = AF_INET,
		.sin_port = htons(4242),
		.sin_addr.s_addr = INADDR_ANY
	};

	if (bind(g_server_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	if (listen(g_server_fd, MAX_CLIENTS) < 0) {
		perror("listen failed");
		exit(EXIT_FAILURE);
	}

	printf("Serveur en écoute sur le port 4242...\n");

	pthread_t cli_thread;
	if (pthread_create(&cli_thread, NULL, cli_thread_routine, NULL) != 0) {
		perror("pthread_create failed, no cli");
	}

	while (g_server_fd != -1)
	{
		t_client_data* client_data = malloc(sizeof(t_client_data));
		socklen_t client_len = sizeof(client_data->client_addr);
		
		client_data->client_fd = accept(g_server_fd, 
			(struct sockaddr*)&client_data->client_addr, 
			&client_len);
		
		if (client_data->client_fd < 0) {
			perror("accept failed");
			free(client_data);
			continue;
		}

		client_data->password = argv[1];

		pthread_t thread_id;
		if (pthread_create(&thread_id, NULL, handle_client, (void*)client_data) != 0) {
			perror("pthread_create failed");
			close(client_data->client_fd);
			free(client_data);
			continue;
		}
		pthread_detach(thread_id);
	}

	return 0;
}