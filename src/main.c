/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 15:31:47 by lilefebv          #+#    #+#             */
/*   Updated: 2025/08/29 10:12:33 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "error_message.h"
#include "options.h"

int	main(int argc, char **argv)
{
	t_minirt	minirt;

	if (argc < 2)
		return (print_error1(ERR_MAIN));
	ft_bzero(&minirt, sizeof(t_minirt));
	if (!parse_options(&minirt, argc, argv))
		return (1);
	if (minirt.options.cpu)
		return (main_cpu(&minirt, argv));
	else
		return (main_gpu(&minirt, argv));
}
