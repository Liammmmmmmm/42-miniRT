/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_dropdown.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 17:13:44 by lilefebv          #+#    #+#             */
/*   Updated: 2025/05/23 14:28:20 by lilefebv         ###   ########lyon.fr   */
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

void	base_dropdown_tex(t_dropdown *dropdown)
{
	dropdown->x = 400;
	dropdown->height = 20;
	dropdown->width = 180;
	dropdown->deployed_height = 180;
	dropdown->active = 0;
	dropdown->selected = NULL;
	dropdown->scroll_offset = 0;
}

void	init_tex_dropdown(t_minirt *minirt)
{
	base_dropdown_tex(&minirt->controls.dropdown[3]);
	base_dropdown_tex(&minirt->controls.dropdown[4]);
	base_dropdown_tex(&minirt->controls.dropdown[5]);
	base_dropdown_tex(&minirt->controls.dropdown[6]);
	base_dropdown_tex(&minirt->controls.dropdown[7]);
	base_dropdown_tex(&minirt->controls.dropdown[8]);
	base_dropdown_tex(&minirt->controls.dropdown[9]);
	base_dropdown_tex(&minirt->controls.dropdown[10]);
	minirt->controls.dropdown[3].y = 800;
	minirt->controls.dropdown[4].y = 830;
	minirt->controls.dropdown[5].y = 860;
	minirt->controls.dropdown[6].y = 860;
	minirt->controls.dropdown[7].y = 860;
	minirt->controls.dropdown[8].y = 860;
	minirt->controls.dropdown[9].y = 860;
	minirt->controls.dropdown[10].y = 860;
}

int	init_dropdowns(t_minirt *minirt)
{
	minirt->controls.nb_dropdown = 11;
	minirt->controls.dropdown = ft_calloc(minirt->controls.nb_dropdown, \
		sizeof(t_dropdown));
	if (!minirt->controls.dropdown)
		return (0);
	base_dropdown(&minirt->controls.dropdown[0]);
	minirt->controls.dropdown[0].y = 800;
	base_dropdown(&minirt->controls.dropdown[1]);
	minirt->controls.dropdown[1].y = 830;
	base_dropdown(&minirt->controls.dropdown[2]);
	minirt->controls.dropdown[2].y = 860;
	init_tex_dropdown(minirt);
	return (1);
}

void	clear_dropdown(t_minirt *minirt)
{
	free(minirt->controls.dropdown);
}
