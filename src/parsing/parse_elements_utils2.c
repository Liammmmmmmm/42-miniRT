/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_elements_utils2.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 14:08:39 by lilefebv          #+#    #+#             */
/*   Updated: 2025/03/24 13:58:13 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static int	is_valid_co_normal(char *str, double *co)
{
	if (is_valid_float(str))
	{
		*co = ft_atod(str);
		if (*co > 1.0 || *co < -1.0)
			return (0);
		return (1);
	}
	return (0);
}

int	parse_vector_normalized(char *str, t_vec3 *vec)
{
	char	**co;

	co = ft_split_in_line(str, ",");
	if (!co)
		return (print_error(strerror(errno)));
	if (char_tab_len(co) != 3)
	{
		free(co);
		return (print_error("Vector must have 3 coordinates : <x,y,z>"));
	}
	if (!is_valid_co_normal(co[0], &vec->x)
		|| !is_valid_co_normal(co[1], &vec->y)
		|| !is_valid_co_normal(co[2], &vec->z))
	{
		free(co);
		return (print_error("Invalid vector format. Expected format: "
			"x,y,z [-1.0;1.0]"));
	}
	free(co);
	return (1);
}

int	is_valid_brightness(char *str, double *co)
{
	if (is_valid_float(str))
	{
		*co = ft_atod(str);
		if (*co > 1.0 || *co < 0)
			return (0);
		return (1);
	}
	return (0);
}

int	is_valid_size(char *str, double *co)
{
	if (is_valid_float(str))
	{
		*co = ft_atod(str);
		if (*co < 0)
			return (0);
		return (1);
	}
	return (0);
}
