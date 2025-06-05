/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rm_object.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 16:50:19 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/05 10:02:56 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	rm_object(void *vparam)
{
	t_minirt	*minirt;
	int			i;
	int			j;
	int			removed;

	minirt = ((t_btn_param *)vparam)->minirt;
	if (minirt->scene.el_amount <= 0)
		return ;
	i = 0;
	j = 0;
	removed = 0;
	while (i < minirt->scene.el_amount)
	{
		minirt->scene.elements[j] = minirt->scene.elements[i];
		if (&minirt->scene.elements[i] != minirt->controls.ui_infos.selected_object && removed == 0)
			j++;
		else
			removed = 1;
		i++;
	}
	if (removed)
	{
		minirt->controls.ui_infos.selected_object = NULL;
		minirt->scene.el_amount--;
	}
	stop_minirt(minirt);
	minirt->scene.build_bvh = 1;
}
