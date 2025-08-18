/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 16:39:25 by lilefebv          #+#    #+#             */
/*   Updated: 2025/08/04 17:20:55 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "options.h"
#include "utils.h"

#define SRV_OPT_ERR "invalid server value. Usage: --server=PORT@PASSWORD"

char	*cut_string(char *str, char split)
{
	while (*str)
	{
		if (*str == split)
		{
			*str = 0;
			return (str + 1);
		}
		str++;
	}
	return (NULL);
}

int	is_server_option(t_minirt *minirt, char *argvi, int *y)
{
	int		i;

	if (ft_strcmp(argvi, "--server") == 0)
		return (print_error_sy("server option need a port and a password", y));
	else if (ft_strncmp(argvi, "--server=", 9) != 0)
		return (0);
	if (ft_strlen(argvi + 9) < 3)
		return (print_error_sy(SRV_OPT_ERR, y));
	if (ft_strchr(argvi + 9, '@') == NULL)
		return (print_error_sy(SRV_OPT_ERR, y));
	minirt->options.server.password = cut_string(argvi + 9, '@');
	if (ft_strlen(argvi + 9) > 5 || ft_strlen(argvi + 9) < 1
		|| ft_strlen(minirt->options.server.password) < 1)
		return (print_error_sy(SRV_OPT_ERR, y));
	i = -1;
	while (*(argvi + 9 + (++i)))
		if (!ft_isdigit(*(argvi + 9 + i)))
			return (print_error_sy(SRV_OPT_ERR, y));
	minirt->options.server.port = ft_atoi(argvi + 9);
	if (minirt->options.server.port > 65535)
		return (print_error_sy(SRV_OPT_ERR, y));
	if (minirt->options.client.enabled)
		return (print_error_sy("Cannot be both server and client", y));
	minirt->options.server.enabled = 1;
	return (1);
}
