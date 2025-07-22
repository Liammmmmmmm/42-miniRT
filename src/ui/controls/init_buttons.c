/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_buttons.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 12:55:59 by lilefebv          #+#    #+#             */
/*   Updated: 2025/07/22 10:41:16 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	base_button(t_button *button, t_minirt *minirt)
{
	button->background_color = UI_BUTTON_BACK;
	button->border_color = UI_SEP_COLOR;
	button->background_color_on_click = UI_BUTTON_CLICK;
	button->border_color_on_click = 0xffffff;
	button->height = 24;
	button->width = 280;
	button->x = 10;
	button->param = malloc(sizeof(t_btn_param));
	if (button->param)
		((t_btn_param *)button->param)->minirt = minirt;
	button->action = &click_start_stop;
}

void	init_start_stop(t_minirt *minirt)
{
	base_button(&minirt->controls.buttons[0], minirt);
	base_button(&minirt->controls.buttons[1], minirt);
	base_button(&minirt->controls.buttons[2], minirt);
	minirt->controls.buttons[0].text = ft_strdup("START");
	minirt->controls.buttons[1].text = ft_strdup("STOP");
	minirt->controls.buttons[2].text = ft_strdup("PAUSE/RESUME");
	minirt->controls.buttons[0].y = 938;
	minirt->controls.buttons[1].y = 938;
	minirt->controls.buttons[2].y = 902;
	minirt->controls.buttons[0].disabled = 1;
	if (minirt->controls.buttons[0].param)
		((t_btn_param *)minirt->controls.buttons[0].param)->action = 0;
	if (minirt->controls.buttons[1].param)
		((t_btn_param *)minirt->controls.buttons[1].param)->action = 1;
	if (minirt->controls.buttons[2].param)
		((t_btn_param *)minirt->controls.buttons[2].param)->action = 2;
}

void	base_tab_btn(t_button *button, t_minirt *minirt, int action)
{
	button->background_color = UI_BUTTON_BACK;
	button->border_color = UI_SEP_COLOR;
	button->background_color_on_click = 0xDDDDDD;
	button->border_color_on_click = 0xffffff;
	button->height = 20;
	button->width = 90;
	button->y = 10;
	button->param = malloc(sizeof(t_btn_param));
	if (button->param)
	{
		((t_btn_param *)button->param)->minirt = minirt;
		((t_btn_param *)button->param)->action = action;
	}
	button->action = &change_tab;
}

void	add_rm_object_btn(t_button *button, t_button *button_rm,
	t_minirt *minirt)
{
	base_button(button, minirt);
	button->width = 250;
	button->x = 310;
	button->y = 420;
	button->text = ft_strdup("Ajouter");
	button->action = &add_object;
	base_button(button_rm, minirt);
	button_rm->width = 24;
	button_rm->x = 566;
	button_rm->y = 420;
	button_rm->background_color = 0xa83a32;
	button_rm->background_color_on_click = 0x7a2721;
	button_rm->text = ft_strdup("X");
	button_rm->action = &rm_object;
}

int	init_buttons(t_minirt *minirt)
{
	minirt->controls.nb_buttons = 7;
	minirt->controls.buttons = ft_calloc(minirt->controls.nb_buttons, \
		sizeof(t_button));
	if (!minirt->controls.buttons)
		return (0);
	init_start_stop(minirt);
	minirt->controls.ui_infos.tab_selected = 0;
	base_tab_btn(&minirt->controls.buttons[3], minirt, 0);
	base_tab_btn(&minirt->controls.buttons[4], minirt, 1);
	minirt->controls.buttons[3].text = ft_strdup("Objects");
	minirt->controls.buttons[4].text = ft_strdup("Materials");
	minirt->controls.buttons[3].x = 310;
	minirt->controls.buttons[4].x = 410;
	minirt->controls.buttons[3].background_color = UI_SEP_COLOR;
	add_rm_object_btn(&minirt->controls.buttons[5],
		&minirt->controls.buttons[6], minirt);
	return (1);
}
