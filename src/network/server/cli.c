/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cli.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 18:22:00 by lilefebv          #+#    #+#             */
/*   Updated: 2025/08/04 10:11:23 by lilefebv         ###   ########lyon.fr   */
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

	// Faudra aussi faire remonter tout ca d'une maniere ou d'une autre pour vraiment tout arreter
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

void	link_client(char *line)
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
	client_data = malloc(sizeof(t_client_data));
	if (!client_data)
		return ((void)print_error("malloc failed"));
	client_data->client_fd = client_fd;
	client_data->client_addr = (struct sockaddr_in){.sin_family = AF_INET,
		.sin_port = htons(14242), .sin_addr.s_addr = inet_addr(line + 5)};
	if (pthread_create(&thread, NULL, handle_client, (void *)client_data) != 0)
	{
		close(client_data->client_fd);
		free(client_data);
		return ((void)print_error("pthread_create failed"));
	}
	pthread_detach(thread);
}

void	*cli_thread_routine(void *arg)
{
	char	*line;

	(void)arg;
	while (g_server_fd != -1)
	{
		line = get_next_line(0);
		if (!line)
			continue ;
		if (ft_strcmp(line, "exit\n") == 0)
		{
			stop_server();
			free(line);
			break ;
		}
		else if (ft_strncmp(line, "link ", 5) == 0)
			link_client(line);
		free(line);
	}
	return (NULL);
}
