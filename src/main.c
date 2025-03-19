/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 15:31:47 by lilefebv          #+#    #+#             */
/*   Updated: 2025/03/19 16:17:05 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	print_error(char *err)
{
	ft_dprintf(2, RED"[Error]"NC" %s\n", err);
	return (0);
}

int	print_error1(char *err)
{
	ft_dprintf(2, RED"[Error]"NC" %s\n", err);
	return (1);
}

int	main(int argc, char **argv)
{
	t_minirt	minirt;

	if (argc < 2)
		return (print_error1("Missing scene file. Usage: ./miniRT <scene.rt>"));
	if (argc > 2)
		return (print_error1("Too many arguments. Usage: ./miniRT <scene.rt>"));
	if (parse_scene(&minirt, argv[1]) == 0)
		return (1);
	print_scene(&minirt.scene);
	free_scene(&minirt.scene, NULL);
}
