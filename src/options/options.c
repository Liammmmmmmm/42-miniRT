/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   options.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 09:54:53 by lilefebv          #+#    #+#             */
/*   Updated: 2025/05/16 12:37:43 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	init_default_options(t_minirt *minirt)
{
	// Probleme avec no display, comment on quitte le programme sans attendre la fin des samples ?
	minirt->options.no_display = 0;
	minirt->options.auto_export = 0;
	minirt->options.max_samples = 100000;
	minirt->options.output_dir = empty_static_string();
}

int	is_max_samples_option(t_minirt *minirt, char *argvi, int *y)
{
	int	i;

	if (ft_strcmp(argvi, "--max-samples") == 0)
	{
		*y = 0;		
		return (print_error1("Max sample option need a value"));
	}
	else if (ft_strncmp(argvi, "--max-samples=", 14) == 0)
	{
		if (ft_strlen(argvi + 14) > 0 && ft_strlen(argvi + 14) < 10)
		{
			i = -1;
			while ((argvi + 14)[++i])
			{
				if (!ft_isdigit((argvi + 14)[i]))
				{
					*y = 0;
					return (print_error1("Invalid sample value"));
				}
			}
			minirt->options.max_samples = ft_atoi(argvi + 14);
			return (1);
		}
		*y = 0;
		return (print_error1("Invalid sample value"));
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

int	parse_options(t_minirt *minirt, int argc, char **argv)
{
	int	i;

	init_default_options(minirt);
	i = 1;
	while (++i < argc)
	{
		if (ft_strcmp(argv[i], "--no-display") == 0)
			minirt->options.no_display = 1;
		else if (ft_strcmp(argv[i], "--auto-export") == 0)
			minirt->options.auto_export = 1;
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
		else
		{
			ft_dprintf(2, RED"[Error]"NC" Unknown option `%s'\n", argv[i]);
			return (0);
		}		
	}
	return (1);
}
