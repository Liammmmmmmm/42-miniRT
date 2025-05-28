/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   button_click.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 14:07:20 by lilefebv          #+#    #+#             */
/*   Updated: 2025/05/23 14:14:46 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	stop_minirt(t_minirt *minirt)
{
	minirt->screen.pause_render = 0;
	minirt->screen.start_render = 0;
	minirt->screen.sample = 0;
	minirt->controls.buttons[0].disabled = 0;
	minirt->controls.buttons[1].disabled = 1;
	minirt->controls.buttons[2].disabled = 1;
}

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
		set_dependant_values(param->minirt);
	}
	else if (param->action == 1)
		stop_minirt(param->minirt);
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
