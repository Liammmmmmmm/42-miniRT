/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   options.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 09:54:53 by lilefebv          #+#    #+#             */
/*   Updated: 2025/08/18 17:51:45 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "options.h"
#include "utils.h"

int	basic_options(t_minirt *minirt, char **argv, int i)
{
	if (ft_strcmp(argv[i], "--no-display") == 0)
		minirt->options.no_display = 1;
	else if (ft_strcmp(argv[i], "--auto-export") == 0)
		minirt->options.auto_export = 1;
	else if (ft_strcmp(argv[i], "--compute-cpu") == 0)
		minirt->options.cpu = 1;
	else if (ft_strcmp(argv[i], "--standard-galactic-alphabet") == 0)
		minirt->options.sga = 1;
	else
		return (0);
	return (1);
}

int	check_every_option(t_minirt *minirt, char **argv, int i)
{
	if (basic_options(minirt, argv, i))
		;
	else if (is_max_samples_option(minirt, argv[i], &i))
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
	while (++i < argc)
	{
		if (check_every_option(minirt, argv, i) == 0)
			return (0);
	}
	return (1);
}
