/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 12:21:06 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/20 14:11:03 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	invalid_float_error(char **splited, int i)
{
	ft_dprintf(2, RED"[Error]"NC" Invalid float: `%s'\n", splited[i]);
	free(splited);
	return (0);
}

int	print_line_error(int nb, char *line)
{
	remove_useless_spaces(line);
	ft_dprintf(2, RED"[Error]"NC" Syntax error on line %d: `%s'\n", nb, line);
	free(line);
	return (0);
}

int	to_many_el_error(char *el)
{
	ft_dprintf(2, RED"[Error]"NC" To many elements '%s'\n", el);
	return (0);
}

int	not_enough_el_error(char *el)
{
	ft_dprintf(2, RED"[Error]"NC" No element '%s'. Need at least one.\n", el);
	return (0);
}

int	invalid_size_error(char **splited)
{
	print_error("Invalid size, must be a positive value.");
	free(splited);
	return (0);
}
