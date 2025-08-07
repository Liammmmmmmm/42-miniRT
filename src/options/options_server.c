/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   options_server.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 09:54:53 by lilefebv          #+#    #+#             */
/*   Updated: 2025/08/07 11:54:29 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "options.h"
#include "utils.h"

int	is_max_samples_option(t_minirt *minirt, char *argvi, int *y)
{
	int	i;

	if (ft_strcmp(argvi, "--max-samples") == 0)
		return (print_error_sy("Max sample option need a value", y));
	else if (ft_strncmp(argvi, "--max-samples=", 14) == 0)
	{
		if (ft_strlen(argvi + 14) > 0 && ft_strlen(argvi + 14) < 10)
		{
			i = -1;
			while ((argvi + 14)[++i])
			{
				if (!ft_isdigit((argvi + 14)[i]))
					return (print_error_sy("Invalid sample value", y));
			}
			minirt->options.max_samples = ft_atoi(argvi + 14);
			return (1);
		}
		return (print_error_sy("Invalid sample value", y));
	}
	return (0);
}

int	is_out_dir_option(t_minirt *minirt, char *argvi, int *y)
{
	if (ft_strcmp(argvi, "--output-dir") == 0)
	{
		*y = 0;
		return (print_error1("Output dir option need a value"));
	}
	else if (ft_strncmp(argvi, "--output-dir=", 13) == 0)
	{
		if (argvi[ft_strlen(argvi) - 1] != '/')
		{
			*y = 0;
			return (print_error1("Output directory must end with a `/'"));
		}
		minirt->options.output_dir = argvi + 13;
		return (1);
	}
	return (0);
}

int	basic_options(t_minirt *minirt, char **argv, int i)
{
	if (ft_strcmp(argv[i], "--no-display") == 0)
		minirt->options.no_display = 1;
	else if (ft_strcmp(argv[i], "--auto-export") == 0)
		minirt->options.auto_export = 1;
	else if (ft_strcmp(argv[i], "--compute-cpu") == 0)
	{
		if (minirt->options.client.enabled)
			return (print_error1("Cannot use cpu for a client"));
		minirt->options.cpu = 1;
	}
	else if (ft_strcmp(argv[i], "--standard-galactic-alphabet") == 0)
		minirt->options.sga = 1;
	else
		return (0);
	return (1);
}

int	check_every_option2(t_minirt *minirt, char **argv, int *i)
{
	if (is_max_samples_option(minirt, argv[*i], i))
	{
		if (*i == 0)
			return (0);
	}
	else if (is_server_option(minirt, argv[*i], i))
	{
		if (*i == 0)
			return (0);
	}
	else if (is_client_option(minirt, argv[*i], i))
	{
		if (*i == 0)
			return (0);
	}
	else
		return (-1);
	return (1);
}

int	check_every_option(t_minirt *minirt, char **argv, int i)
{
	if (basic_options(minirt, argv, i))
		;
	else if (check_every_option2(minirt, argv, &i) >= 0)
	{
		if (i == 0)
			return (0);
	}
	else if (is_out_dir_option(minirt, argv[i], &i))
	{
		if (i == 0)
			return (0);
	}
	else if (is_animate_option(minirt, argv[i], &i))
	{
		if (i == 0)
			return (0);
	}
	else
	{
		ft_dprintf(2, RED"[Error]"NC" Unknown option `%s'\n", argv[i]);
		return (0);
	}
	return (1);
}

int	parse_options(t_minirt *minirt, int argc, char **argv)
{
	int	i;

	minirt->options.no_display = 0;
	minirt->options.auto_export = 0;
	minirt->options.max_samples = 100000;
	minirt->options.output_dir = empty_static_string();
	i = 1;
	if (argc >= 2)
		is_client_option(minirt, argv[1], &i);
	if (i == 0)
		return (0);
	while (++i < argc)
	{
		if (check_every_option(minirt, argv, i) == 0)
			return (0);
	}
	return (1);
}
