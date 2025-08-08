/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cli.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 18:22:00 by lilefebv          #+#    #+#             */
/*   Updated: 2025/08/08 11:01:03 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "network.h"
#include "utils.h"

void	stop_server(void)
{
	printf("\nArrêt du serveur...\n");
	shutdown(g_server_fd, SHUT_RDWR);
	close(g_server_fd);
	g_server_fd = -1;
}

static void	replace_breakline_zero(char *line)
{
	while (*line)
	{
		if (*line == '\n')
			*line = '\0';
		line++;
	}
}

void	link_client(char *line, t_minirt *minirt)
{
	int				client_fd;
	t_client_data	*client_data;
	pthread_t		thread;

	if (ft_strlen(line) < 8)
		return ((void)print_error("Invalid client address"));
	replace_breakline_zero(line);
	printf("Trying to connect to: %s\n", line + 5);
	client_fd = connect_to_passive_client(line + 5);
	if (client_fd == -1)
		return ((void)print_error("Invalid client address"));
	client_data = ft_calloc(1, sizeof(t_client_data));
	if (!client_data)
		return ((void)print_error("malloc failed"));
	client_data->client_fd = client_fd;
	client_data->client_addr = (struct sockaddr_in){.sin_family = AF_INET,
		.sin_port = htons(14242), .sin_addr.s_addr = inet_addr(line + 5)};
	client_data->minirt = minirt;
	if (pthread_create(&thread, NULL, handle_client, (void *)client_data) != 0)
	{
		close(client_data->client_fd);
		free(client_data);
		return ((void)print_error("pthread_create failed"));
	}
	pthread_detach(thread);
}

static void	cleanup_line(void *line_ptr)
{
	char	*line;

	line = *(char **)line_ptr;
	if (line)
	{
		free(line);
		*(char **)line_ptr = NULL;
	}
}

void	*cli_thread_routine(void *arg)
{
	char	*line;

	line = NULL;
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
	pthread_cleanup_push(cleanup_line, &line);
	while (g_server_fd != -1)
	{
		line = get_next_line(STDIN_FILENO);
		if (!line)
			break ;
		if (ft_strcmp(line, "exit\n") == 0)
		{
			stop_server();
			free(line);
			line = NULL;
			break ;
		}
		else if (ft_strncmp(line, "link ", 5) == 0)
			link_client(line, arg);
		free(line);
		line = NULL;
	}
	pthread_cleanup_pop(1);
	return (NULL);
}
