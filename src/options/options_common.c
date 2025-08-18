/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   options_common.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 17:51:35 by lilefebv          #+#    #+#             */
/*   Updated: 2025/08/18 18:08:33 by lilefebv         ###   ########lyon.fr   */
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
