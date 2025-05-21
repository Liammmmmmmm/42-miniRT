/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_picker.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 16:50:31 by lilefebv          #+#    #+#             */
/*   Updated: 2025/05/21 16:32:34 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "mlx_components.h"
#include "minirt.h"

int	color_picker_type(t_color_picker *cp, int key)
{
	if (cp->text_input.cursor_pos == 1 && key == 65288)
		return (1);
	if (text_input_type(&cp->text_input, key, 1))
	{
		cp->btn.background_color = ft_atoi_base(cp->text_input.text + 1,
				"0123456789ABCDEF");
		cp->btn.background_color_on_click = cp->btn.background_color;
		*cp->color = int_to_color(cp->btn.background_color);
		cp->hsv = rgb_to_hsv(cp->btn.background_color);
		return (1);
	}
	return (0);
}

void	active_color_picker(void *param)
{
	((t_color_picker *)param)->active = 1;
}

static void	init_color_picker2(t_color_picker *cp, t_color *color_var)
{
	cp->text_input.is_focused = 0;
	cp->text_input.max_char = 7;
	cp->text_input.padding_left = 10;
	ft_strlcpy(cp->text_input.placeholder, "#color", 8);
	color_to_hex(cp->text_input.text, *color_var);
	cp->text_input.text_color = 0;
}

void	init_color_picker(t_color_picker *cp, t_color *color_var)
{
	cp->active = 0;
	cp->color = color_var;
	cp->hsv = rgb_to_hsv(color_to_int(*color_var));
	cp->btn.background_color = color_to_int(*color_var);
	cp->btn.border_color = 0xFFFFFF;
	cp->btn.background_color_on_click = color_to_int(*color_var);
	cp->btn.border_color_on_click = 0xB0B0B0;
	cp->btn.width = 40;
	cp->btn.height = 20;
	cp->btn.action = &active_color_picker;
	cp->btn.param = cp;
	cp->btn.disabled = 0;
	cp->btn.is_clicked = 0;
	cp->btn.text = NULL;
	cp->btn.x = 0;
	cp->btn.y = 0;
	cp->mouse_in_hue = 0;
	cp->mouse_in_val_sat = 0;
	cp->text_input.background_color = UI_BUTTON_BACK;
	cp->text_input.border_color = UI_SEP_COLOR;
	cp->text_input.border_color_focus = UI_BUTTON_CLICK;
	cp->text_input.cursor_pos = 7;
	cp->text_input.height = 30;
	cp->text_input.width = 240;
	init_color_picker2(cp, color_var);
}
