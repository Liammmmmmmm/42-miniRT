/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   options_common.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 22:55:03 by madelvin          #+#    #+#             */
/*   Updated: 2025/08/21 22:55:04 by madelvin         ###   ########.fr       */
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

int	is_caustic_option(t_minirt *minirt, char *argvi, int *y)
{
	int	i;

	if (ft_strcmp(argvi, "--caustic") == 0)
		return (print_error_sy("Caustic need values", y));
	else if (ft_strncmp(argvi, "--caustics=", 11) == 0)
	{
		if (ft_strlen(argvi + 11) > 0 && ft_strlen(argvi + 14) < 10)
		{
			i = -1;
			while ((argvi + 14)[++i])
			{
				if (!ft_isdigit((argvi + 14)[i]))
					return (print_error_sy("Invalid caustic values", y));
			}
			minirt->controls.max_bounces = ft_atoi(argvi + 14);
			return (1);
		}
		return (print_error_sy("Invalid caustic values", y));
	}
	return (0);
}

int	is_max_bounces_option(t_minirt *minirt, char *argvi, int *y)
{
	int	i;

	if (ft_strcmp(argvi, "--max-bounces") == 0)
		return (print_error_sy("Max bounces option need a value", y));
	else if (ft_strncmp(argvi, "--max-bounces=", 14) == 0)
	{
		if (ft_strlen(argvi + 14) > 0 && ft_strlen(argvi + 14) < 10)
		{
			i = -1;
			while ((argvi + 14)[++i])
			{
				if (!ft_isdigit((argvi + 14)[i]))
					return (print_error_sy("Invalid bounces value", y));
			}
			minirt->controls.max_bounces = ft_atoi(argvi + 14);
			return (1);
		}
		return (print_error_sy("Invalid bounces value", y));
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
