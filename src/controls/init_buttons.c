/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_buttons.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 12:55:59 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/11 15:30:39 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	base_button(t_button *button, t_minirt *minirt)
{
	button->background_color = 0xB4B4B4;
	button->border_color = 0xffffff;
	button->background_color_on_click = 0x919191;
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
	minirt->controls.buttons[0].y = 1038;
	minirt->controls.buttons[1].y = 1038;
	minirt->controls.buttons[2].y = 1002;
	minirt->controls.buttons[0].disabled = 1;
	if (minirt->controls.buttons[0].param)
		((t_btn_param *)minirt->controls.buttons[0].param)->action = 0;
	if (minirt->controls.buttons[1].param)
		((t_btn_param *)minirt->controls.buttons[1].param)->action = 1;
	if (minirt->controls.buttons[2].param)
		((t_btn_param *)minirt->controls.buttons[2].param)->action = 2;
}

int	init_buttons(t_minirt *minirt)
{
	minirt->controls.nb_buttons = 3;
	minirt->controls.buttons = ft_calloc(minirt->controls.nb_buttons, sizeof(t_button));
	if (!minirt->controls.buttons)
		return (0);
	init_start_stop(minirt);
	return (1);
}

void	clear_buttons(t_minirt *minirt)
{
	int	i;

	if (!minirt->controls.buttons)
		return ;
	i = -1;
	while (++i < minirt->controls.nb_buttons)
	{
		free(minirt->controls.buttons[i].text);
		free(minirt->controls.buttons[i].param);
	}
	free(minirt->controls.buttons);
}
