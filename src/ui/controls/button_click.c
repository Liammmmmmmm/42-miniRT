/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   button_click.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 14:07:20 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/21 15:32:50 by lilefebv         ###   ########lyon.fr   */
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

void	change_tab(void *vparam)
{
	t_btn_param	*param;

	if (!vparam)
		return ;
	param = (t_btn_param *)vparam;
	param->minirt->controls.ui_infos.tab_selected = param->action;
	if (param->action == 0)
	{
		param->minirt->controls.buttons[3].background_color = UI_SEP_COLOR;
		param->minirt->controls.buttons[4].background_color = UI_BUTTON_BACK;
	}
	else if (param->action == 1)
	{
		param->minirt->controls.buttons[4].background_color = UI_SEP_COLOR;
		param->minirt->controls.buttons[3].background_color = UI_BUTTON_BACK;
	}
}
