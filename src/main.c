/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 15:31:47 by lilefebv          #+#    #+#             */
/*   Updated: 2025/03/18 12:41:23 by lilefebv         ###   ########lyon.fr   */
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

	if (argc < 2)
		return (print_error("Missing scene file. Usage: ./miniRT <scene.rt>"));
	if (argc > 2)
		return (print_error("Too many arguments. Usage: ./miniRT <scene.rt>"));
	parse_scene(&minirt, argv[1]);
	
}