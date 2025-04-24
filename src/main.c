/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 15:31:47 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/22 17:51:40 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "bmp_parsing.h"

int	render_next_frame(t_minirt *minirt)
{
	render_frame(minirt);
	return (1);
}

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

int	clean(t_minirt *minirt)
{
	free_scene(&minirt->scene, NULL);
	free_mlx(minirt);
	clear_buttons(minirt);
	clear_sliders(minirt);
	if (minirt->screen.render)
		free(minirt->screen.render);
	free_ttf(&minirt->controls.font[0]);
	return (1);
}

int	main(int argc, char **argv)
{
	t_minirt	minirt;

	if (argc < 2)
		return (print_error1("Missing scene file. Usage: ./miniRT <scene.rt>"));
	if (argc > 2)
		return (print_error1("Too many arguments. Usage: ./miniRT <scene.rt>"));
	ft_bzero(&minirt, sizeof(t_minirt));
	if (parse_scene(&minirt, argv[1]) == 0)
		return (1);
	if (!init_ui(&minirt))
		return (clean(&minirt));
	if (!init_mlx(&minirt))
		return (clean(&minirt));
	if (!init_render(&minirt))
		return (clean(&minirt));
	print_scene(&minirt.scene);
	events(&minirt);
	mlx_loop_hook(minirt.mlx.mlx, render_next_frame, &minirt);
	mlx_loop(minirt.mlx.mlx);
	clean(&minirt);
}
