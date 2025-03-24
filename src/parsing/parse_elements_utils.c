/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_elements_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 11:06:52 by lilefebv          #+#    #+#             */
/*   Updated: 2025/03/24 12:55:44 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	is_valid_fov(char *str, unsigned char *fov)
{
	int	i;
	int	ffov;
	
	i = -1;
	if (str[0] == '+')
		i++;
	while (str[++i])
		if (!ft_isdigit(str[i]) || i > 4)
			return (0);
	ffov = ft_atoi(str);
	if (ffov >= 0 && ffov <= 180)
	{
		*fov = (unsigned char)ffov;
		return (1);
	}
	return (0);
}

static int	is_valid_color_nb(char *str, unsigned char *ccolor)
{
	int	i;
	int	fcolor;
	
	i = -1;
	if (str[0] == '+')
		i++;
	while (str[++i])
		if (!ft_isdigit(str[i]) || i > 4)
			return (0);
	fcolor = ft_atoi(str);
	if (fcolor >= 0 && fcolor <= 255)
	{
		*ccolor = (unsigned char)fcolor;
		return (1);
	}
	return (0);
}

int	parse_color(char *color, t_color *store)
{
	char	**colors;

	colors = ft_split_in_line(color, ",");
	if (!colors)
		return (print_error(strerror(errno)));
	if (char_tab_len(colors) != 3)
	{
		free(colors);
		return (print_error("Color must have 3 components : <r,g,b>"));
	}
	if (!is_valid_color_nb(colors[0], &store->r)
		|| !is_valid_color_nb(colors[1], &store->g)
		|| !is_valid_color_nb(colors[2], &store->b))
	{
		free(colors);
		return (print_error("Invalid color format. Expected format: "
			"r,g,b [0;255]"));
	}
	free(colors);
	return (1);
}

static int	is_valid_co(char *str, double *co)
{
	if (is_valid_float(str))
	{
		*co = ft_atof(str);
		return (1);
	}
	return (0);
}

int	parse_vector(char *str, t_vec3 *vec)
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
	if (!is_valid_co(co[0], &vec->x)
		|| !is_valid_co(co[1], &vec->y)
		|| !is_valid_co(co[2], &vec->z))
	{
		free(co);
		return (print_error("Invalid vector format. Expected format: x,y,z"));
	}
	free(co);
	return (1);
}
