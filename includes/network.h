/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   network.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 18:17:56 by lilefebv          #+#    #+#             */
/*   Updated: 2025/08/06 15:02:33 by lilefebv         ###   ########lyon.fr   */
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

enum e_scene_transmission_index
{
	SRV_NULL,
	SRV_COMPUTE_START,
	SRV_COMPUTE_STOP,
	SRV_CHECKERS,
	SRV_IMAGES,
	SRV_IMAGE_STREAM,
	SRV_TEX_TYPES_INDICES,
	SRV_SKYBOX_WH,
	SRV_CDF_CONDITIONAL_INVERSE,
	SRV_PDF_JOINT,
	SRV_CDF_MARGINAL_INVERSE,
	SRV_MATERIALS,
	SRV_AMBIANT,
	SRV_VIEWPORT,
	SRV_CAMERA,
	SRV_HYPER,
	SRV_TRIANGLE,
	SRV_LIGHTS,
	SRV_PRIM_TYPES_INDICES,
	SRV_PRIMITIVES,
	SRV_BVH_NODE,
	SRV_PLANES,
	SRV_PHOTONS,
	SRV_CELLS,
	SRV_PHOTON_INDICES,
	SRV_WIDTH_RENDER,
	SRV_HEIGHT_RENDER,
	SRV_MAX_BOUNCES,
	SRV_RENDER_MODE,
	SRV_PLANES_AM,
	SRV_LIGHTS_AM,
	SRV_GRID_WORLD_MIN,
	SRV_CELL_SIZE,
	SRV_TABLE_SIZE
};

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
int			convert_scene_server(t_minirt *minirt, t_scene *scene,
	t_viewport *viewport, t_gpu_structs *gpu);
int			convert_textures_server(t_scene *scene, t_gpu_textures *gtx);
void		init_scene_server(t_minirt *minirt);

// ----------- CLIENT ----------- //

int			passive_mode(int *sockfd);
int			active_mode(int *sockfd, char *address, int port, char *password);
void		connect_client(char *ip, int port, char *password, t_minirt *minirt);
int			send_frame_to_server(const float *ptr, uint64_t tpx, uint16_t samples, int *sockfd);
int			read_stdin(fd_set *readfds);

// ----------- UTILS ----------- //

int			connect_with_timeout(int sockfd, const struct sockaddr *addr,
	socklen_t addrlen, int timeout_sec);
uint64_t	password_hash(const char *input, const char *challenge);
char		random_basic_char();
int			is_connection_alive(int sockfd);
char		*recv_large_data(int fd, uint64_t bytes_to_read);
int			send_scene_data(int *sockfd, char *data, uint64_t size, uint16_t type);


#endif
