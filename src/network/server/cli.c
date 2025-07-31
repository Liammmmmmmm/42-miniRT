/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cli.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 18:22:00 by lilefebv          #+#    #+#             */
/*   Updated: 2025/07/31 18:25:00 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "network.h"

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
