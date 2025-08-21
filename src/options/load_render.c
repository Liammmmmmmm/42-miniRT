/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_render.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 12:22:07 by lilefebv          #+#    #+#             */
/*   Updated: 2025/08/21 12:34:34 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "options.h"

int	is_load_render_option(t_minirt *minirt, char *argvi, int *y)
{
	if (ft_strcmp(argvi, "--load-render") == 0)
		return (print_error_sy("Load render option need a file", y));
	else if (ft_strncmp(argvi, "--load-render=", 14) == 0)
	{
		if (!read_bin_file(&minirt->options.load_render, argvi + 14))
			return (print_error_sy("Can't open the saved render file", y));
		return (1);
	}
	return (0);
}
