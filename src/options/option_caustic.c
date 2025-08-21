/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   option_caustic.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 21:44:57 by madelvin          #+#    #+#             */
/*   Updated: 2025/08/21 22:38:35 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "options.h"

static char	is_str_digit(char *str)
{
	int	i;

	if (!str || str[0] == '\0')
		return (0);
	i = 0;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

static char	is_str_float(char *str)
{
	int	i;
	int	dot_count;

	if (!str || str[0] == '\0')
		return (0);
	i = 0;
	dot_count = 0;
	while (str[i])
	{
		if (str[i] == '.')
			dot_count++;
		else if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (dot_count <= 1);
}

int	extract_sep(char *values, char **sep1, char **sep2)
{
	*sep1 = ft_strchr(values, ';');
	if (!*sep1)
		return (1);
	*sep2 = ft_strchr(*sep1 + 1, ';');
	if (!*sep2)
		return (1);
	return (0);
}

int	parse_caustic_option(t_minirt *minirt, char *arg, int *y)
{
	char	*values;
	char	*sep1;
	char	*sep2;

	if (ft_strncmp(arg, "--caustic=", 10) != 0)
		return (0);
	values = arg + 10;
	if (extract_sep(values, &sep1, &sep2))
		return (print_error_sy("Invalid caustic format.", y));
	*sep1 = '\0';
	*sep2 = '\0';
	if (!is_str_digit(values) || !is_str_float(sep1 + 1)
		|| !is_str_float(sep2 + 1))
		return (print_error_sy("Invalid caustic values: type mismatch", y));
	minirt->options.caustic.nb_photon = ft_atoi(values);
	minirt->options.caustic.caustic_radius = ft_atof(sep1 + 1);
	minirt->options.caustic.norm_factor = ft_atof(sep2 + 1);
	if (minirt->options.caustic.nb_photon <= 0
		|| minirt->options.caustic.caustic_radius <= 0.0
		|| minirt->options.caustic.norm_factor <= 0.0)
		return (print_error_sy("Caustic values must be positive.", y));
	minirt->options.caustic.caustic_enable = 1;
	return (1);
}
