/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   network.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 18:17:56 by lilefebv          #+#    #+#             */
/*   Updated: 2025/08/06 10:19:45 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef NETWORK_H
# define NETWORK_H

# include <errno.h>
# include <fcntl.h>
# include <sys/socket.h>
# include <sys/select.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <stdlib.h>
# include <pthread.h>
#include <stdio.h>
# include "libft.h"
# include "structs.h"

// ----------- SERVER ----------- //

# define MAX_CLIENTS 100
# define SERV_BUFF_SIZE 16000

extern int	g_server_fd;

typedef struct s_client_data
{
	int 				client_fd;
	struct sockaddr_in	client_addr;
	char				*password;
	t_minirt			*minirt;
}	t_client_data;

void		handle_sigint(int sig);
void		print_network_info(int port);
int			connect_to_passive_client(const char *client_ip);
int			password_connexion(t_client_data *data, int client_fd);
void		*cli_thread_routine(void *arg);
void		*handle_client(void *arg);
void		*server_thread_routine(void *arg);

// ----------- CLIENT ----------- //

int			passive_mode(int *sockfd);
int			active_mode(int *sockfd, char *address, int port, char *password);
void		connect_client(char *ip, int port, char *password, t_minirt *minirt);
int			send_frame_to_server(const float *ptr, uint64_t tpx, uint16_t samples, int *sockfd);
int			read_stdin(int *sockfd, fd_set *readfds);

// ----------- UTILS ----------- //

int			connect_with_timeout(int sockfd, const struct sockaddr *addr,
	socklen_t addrlen, int timeout_sec);
uint64_t	password_hash(const char *input, const char *challenge);
char		random_basic_char();
int			is_connection_alive(int sockfd);

#endif
