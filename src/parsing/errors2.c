/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:05:12 by lilefebv          #+#    #+#             */
/*   Updated: 2025/03/24 17:01:41 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	invalid_size_error(char **splited)
{
	print_error("Invalid size, must be a positive value.");
	free(splited);
	return (0);
}

int	texture_error(int error, char **parts)
{
	if (error == 0)
		ft_dprintf(2, RED"[Error]"NC" Invalid texture name: `%s'\n", parts[1]);
	else if (error == 1)
		ft_dprintf(2, RED"[Error]"NC" texture name too long: `%s'\n", parts[1]);
	else if (error == 2)
		ft_dprintf(2, RED"[Error]"NC" Name already used: `%s'\n", parts[1]);
	else if (error == 3)
		ft_dprintf(2, RED"[Error]"NC" %s: %s\n", parts[2], strerror(errno));
	free(parts);
	return (0);
}
