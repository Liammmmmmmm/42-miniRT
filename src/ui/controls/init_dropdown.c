/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_dropdown.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 17:13:44 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/10 15:41:00 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	init_choose_upscaling(t_minirt *minirt)
{
	base_dropdown(&minirt->controls.dropdown[13]);
	minirt->controls.dropdown[13].x = 10;
	minirt->controls.dropdown[13].y = 630;
	minirt->controls.selected_upscaling = (void *)NO_UPSCALING;
	minirt->controls.dropdown[13].selected
		= &minirt->controls.selected_upscaling;
	minirt->controls.dropdown[13].val_amount = 3;
	minirt->controls.dropdown[13].values = ft_calloc(
			minirt->controls.dropdown[13].val_amount, sizeof(t_dropdown_node));
	if (!minirt->controls.dropdown[13].values)
		return ;
	minirt->controls.dropdown[13].values[0].ref = (void *)NO_UPSCALING;
	minirt->controls.dropdown[13].values[0].text
		= get_upscaling_name(NO_UPSCALING);
	minirt->controls.dropdown[13].values[1].ref = (void *)BILINEAR;
	minirt->controls.dropdown[13].values[1].text = get_upscaling_name(BILINEAR);
	minirt->controls.dropdown[13].values[2].ref = (void *)BICUBIC;
	minirt->controls.dropdown[13].values[2].text = get_upscaling_name(BICUBIC);
}

int	init_dropdowns(t_minirt *minirt)
{
	minirt->controls.nb_dropdown = 14;
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
	init_choose_object(minirt);
	init_choose_upscaling(minirt);
	return (1);
}

void	clear_dropdown(t_minirt *minirt)
{
	int	i;

	i = 0;
	while (i < minirt->controls.nb_dropdown)
	{
		free(minirt->controls.dropdown[i].values);
		i++;
	}
	free(minirt->controls.dropdown);
}
