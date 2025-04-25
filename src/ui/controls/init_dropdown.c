/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_dropdown.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 17:13:44 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/24 16:11:03 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	base_dropdown(t_dropdown *dropdown)
{
	dropdown->x = 310;
	dropdown->height = 20;
	dropdown->width = 280;
	dropdown->deployed_height = 180;
	dropdown->active = 0;
	dropdown->selected = NULL;
	dropdown->scroll_offset = 0;
}

int	init_dropdowns(t_minirt *minirt)
{
	minirt->controls.nb_dropdown = 1;
	minirt->controls.dropdown = ft_calloc(minirt->controls.nb_dropdown, \
		sizeof(t_dropdown));
	if (!minirt->controls.dropdown)
		return (0);
	base_dropdown(&minirt->controls.dropdown[0]);
	minirt->controls.dropdown[0].y = 800;
	return (1);
}

void	clear_dropdown(t_minirt *minirt)
{
	free(minirt->controls.dropdown);
}
