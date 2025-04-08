/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   button_click.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 14:07:20 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/08 21:03:53 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	click_start_stop(void *vparam)
{
	t_btn_param	*param;

	if (!vparam)
		return ;
	param = (t_btn_param *)vparam;
	if (param->action == 0)
	{
		param->minirt->screen.pause_render = 0;
		param->minirt->screen.start_render = 1;
		param->minirt->controls.buttons[0].disabled = 1;
		param->minirt->controls.buttons[1].disabled = 0;
		param->minirt->controls.buttons[2].disabled = 0;
	}
	else if (param->action == 1)
	{
		param->minirt->screen.pause_render = 0;
		param->minirt->screen.start_render = 0;
		param->minirt->screen.sample = 0;
		param->minirt->controls.buttons[0].disabled = 0;
		param->minirt->controls.buttons[1].disabled = 1;
		param->minirt->controls.buttons[2].disabled = 1;
	}
	else if (param->action == 2)
		param->minirt->screen.pause_render \
			= !param->minirt->screen.pause_render;
}
