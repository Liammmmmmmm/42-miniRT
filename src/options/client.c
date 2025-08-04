/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 17:13:09 by lilefebv          #+#    #+#             */
/*   Updated: 2025/08/04 17:28:02 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "options.h"
#include "utils.h"

#define CLIENT_OPT_ERR "invalid client value. Usage: --client=IP:PORT@PASSWORD"

int	check_part(t_minirt *minirt, char *argvi, int *y, char *port)
{
	int		i;

	i = -1;
	while (*(argvi + 9 + (++i)))
		if (!ft_isdigit(*(argvi + 9 + i)) && *(argvi + 9 + i) != '.')
			return (print_error_sy(CLIENT_OPT_ERR, y));
	if (ft_strlen(port) > 5 || ft_strlen(port) < 1
		|| ft_strlen(minirt->options.client.password) < 1)
		return (print_error_sy(CLIENT_OPT_ERR, y));
	i = -1;
	while (port[++i])
		if (!ft_isdigit(port[i]))
			return (print_error_sy(CLIENT_OPT_ERR, y));
	minirt->options.client.port = ft_atoi(port);
	if (minirt->options.client.port > 65535)
		return (print_error_sy(CLIENT_OPT_ERR, y));
	if (minirt->options.server.enabled)
		return (print_error_sy("Cannot be both server and client", y));
	if (minirt->options.cpu)
		return (print_error_sy("Cannot be a client on the cpu", y));
	minirt->options.client.enabled = 1;
	return (0);
}

int	is_client_option(t_minirt *minirt, char *argvi, int *y)
{
	char	*port;

	if (ft_strcmp(argvi, "--client") == 0)
		return (print_error_sy(CLIENT_OPT_ERR, y));
	else if (ft_strncmp(argvi, "--client=", 9) != 0)
		return (0);
	if (ft_strlen(argvi + 9) < 5)
		return (print_error_sy(CLIENT_OPT_ERR, y));
	if (ft_strchr(argvi + 9, ':') == NULL)
		return (print_error_sy(CLIENT_OPT_ERR, y));
	port = cut_string(argvi + 9, ':');
	if (ft_strchr(port, '@') == NULL)
		return (print_error_sy(CLIENT_OPT_ERR, y));
	minirt->options.client.password = cut_string(port, '@');
	check_part(minirt, argvi, y, port);
	return (1);
}
