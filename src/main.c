/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 15:31:47 by lilefebv          #+#    #+#             */
/*   Updated: 2025/03/17 16:04:06 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	print_error(char *err)
{
	ft_dprintf(2, RED"[Error]"NC" %s\n", err);
	return (0);
}

int main(int argc, char **argv)
{
	t_minirt	minirt;

	if (argc != 2)
		return (print_error("bad arg amoumt"));
	parse_scene(&minirt, argv[1]);
	
}