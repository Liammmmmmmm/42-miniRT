/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 12:21:06 by lilefebv          #+#    #+#             */
/*   Updated: 2025/03/18 13:01:37 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	print_line_error(int nb, char *line)
{
	remove_useless_spaces(line);
	ft_dprintf(2, RED"[Error]"NC" Syntax error on line %d: `%s'\n", nb, line);
	free(line);
	return (0);
}

