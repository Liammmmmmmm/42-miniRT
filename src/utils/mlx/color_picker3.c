/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_picker3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 16:29:59 by lilefebv          #+#    #+#             */
/*   Updated: 2025/05/21 16:35:45 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "mlx_components.h"
#include "maths.h"

static void	color_picker_handle_click(t_color_picker *cp, int mouse_x,
	int mouse_y)
{
	if (mouse_y < cp->btn.y + 150)
	{
		cp->hsv.sat = (mouse_x - cp->btn.x - 1) * 0.003584;
		cp->hsv.val = 1 - ((mouse_y - cp->btn.y - 1) * 0.006711);
		cp->mouse_in_hue = 1;
	}
	else if (mouse_y < cp->btn.y + 190)
	{
		cp->hsv.hue = (mouse_x - cp->btn.x - 1) * 1.290322;
		cp->mouse_in_val_sat = 1;
	}
	else
		text_input_focus(&cp->text_input, mouse_x, mouse_y);
}

int	color_picker_action(t_color_picker *cp, int mouse_x, int mouse_y)
{
	int	is_active;

	is_active = cp->active;
	cp->text_input.x = cp->btn.x;
	cp->text_input.y = cp->btn.y + 190;
	cp->text_input.is_focused = 0;
	if (!is_active)
		return (button_action(&cp->btn, mouse_x, mouse_y, &is_active));
	if (mouse_x > cp->btn.x && mouse_x < cp->btn.x + 280
		&& mouse_y > cp->btn.y && mouse_y < cp->btn.y + 220)
	{
		color_picker_handle_click(cp, mouse_x, mouse_y);
		cp->btn.background_color = hsv_to_rgb(cp->hsv.hue, cp->hsv.sat,
				cp->hsv.val);
		cp->btn.background_color_on_click = cp->btn.background_color;
		*cp->color = int_to_color(cp->btn.background_color);
		color_to_hex(cp->text_input.text, *cp->color);
	}
	else
		cp->active = 0;
	return (1);
}

int	color_picker_mouse_move(t_color_picker *cp, int mouse_x, int mouse_y)
{
	mouse_x = iclamp(cp->btn.x + 1, mouse_x, cp->btn.x + 279);
	if (!cp->active)
		return (0);
	if (cp->mouse_in_hue)
	{
		mouse_y = iclamp(cp->btn.y + 1, mouse_y, cp->btn.y + 150);
		cp->hsv.sat = (mouse_x - cp->btn.x - 1) * 0.003584;
		cp->hsv.val = 1 - ((mouse_y - cp->btn.y - 1) * 0.006711);
	}
	else if (cp->mouse_in_val_sat)
		cp->hsv.hue = (mouse_x - cp->btn.x - 1) * 1.290322;
	else
		return (0);
	cp->btn.background_color = hsv_to_rgb(cp->hsv.hue, cp->hsv.sat, cp->hsv.val)
		;
	cp->btn.background_color_on_click = cp->btn.background_color;
	*cp->color = int_to_color(cp->btn.background_color);
	color_to_hex(cp->text_input.text, *cp->color);
	return (1);
}

int	color_picker_release(t_color_picker *cp)
{
	button_release(&cp->btn);
	cp->mouse_in_val_sat = 0;
	cp->mouse_in_hue = 0;
	return (1);
}
