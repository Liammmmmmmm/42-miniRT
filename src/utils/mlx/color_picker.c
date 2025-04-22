/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_picker.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 16:50:31 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/22 15:15:02 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "mlx_components.h"
#include "minirt.h"

int imax(int i1, int i2)
{
	if (i1 > i2)
		return (i1);
	return (i2);
}

int	iclamp(int min, int val, int max)
{
	if (min > val)
		return (min);
	else if (max < val)
		return (max);
	return (val);
}

void	color_to_hex(char *dest, t_color color)
{
	dest[0] = '#';
	dest[1] = "0123456789ABCDEF"[color.r / 16];
	dest[2] = "0123456789ABCDEF"[color.r % 16];
	dest[3] = "0123456789ABCDEF"[color.g / 16];
	dest[4] = "0123456789ABCDEF"[color.g % 16];
	dest[5] = "0123456789ABCDEF"[color.b / 16];
	dest[6] = "0123456789ABCDEF"[color.b % 16];
	dest[7] = '\0';
}

/**
 * Why * 1.290322 ?
 * x * 1.290322 == x / 279 * 360, juste optimization
 */
void	draw_hue_colors(t_img *img, t_color_picker *cp)
{
	int x;
	int	y;
	int color;

	x = cp->btn.x + 1;
	while (x < cp->btn.x + 280)
	{
		y = cp->btn.y + 151;
		color = hsv_to_rgb((x - cp->btn.x - 1) * 1.290322, 1.0, 1.0);
		while (y < cp->btn.y + 190)
		{
			put_pixel_image(img, x, y, color);
			y++;
		}
		x++;
	}
	draw_vertical_line(img, cp->btn.x + 279 * cp->hsv.hue / 360 + 1, cp->btn.y + 150, cp->btn.y + 190);
	draw_horizontal_line(img, cp->btn.y + 151, imax(cp->btn.x + 279 * cp->hsv.hue / 360 - 1, cp->btn.x), cp->btn.x + 279 * cp->hsv.hue / 360 + 3);
	draw_horizontal_line(img, cp->btn.y + 152, cp->btn.x + 279 * cp->hsv.hue / 360, cp->btn.x + 279 * cp->hsv.hue / 360 + 2);
	draw_horizontal_line(img, cp->btn.y + 188, cp->btn.x + 279 * cp->hsv.hue / 360, cp->btn.x + 279 * cp->hsv.hue / 360 + 2);
	draw_horizontal_line(img, cp->btn.y + 189, imax(cp->btn.x + 279 * cp->hsv.hue / 360 - 1, cp->btn.x), cp->btn.x + 279 * cp->hsv.hue / 360 + 3);
}

/**
 * x * 0.003584 = x / 279
 * x * 0.006711 = x / 149
 */
void	draw_sat_val_colors(t_img *img, t_color_picker *cp)
{
	int x;
	int	y;

	x = cp->btn.x + 1;
	while (x < cp->btn.x + 280)
	{
		y = cp->btn.y + 1;
		while (y < cp->btn.y + 150)
		{
			put_pixel_image(img, x, y, hsv_to_rgb(cp->hsv.hue, (x - cp->btn.x - 1) * 0.003584, 1 - ((y - cp->btn.y - 1) * 0.006711)));
			y++;
		}
		x++;
	}
	circle_bres(cp->btn.x + 1 + 279 * cp->hsv.sat, cp->btn.y + 150 - 149 * cp->hsv.val, 3, img, 0xFFFFFF);
}

void	display_color_picker(t_img *img, t_color_picker *cp, uint8_t font8b[96][5])
{
	int	x;
	int	y;

	display_button(img, cp->btn, NULL);
	if (!cp->active)
		return ;
	draw_horizontal_line(img, cp->btn.y, cp->btn.x, cp->btn.x + 280);
	draw_horizontal_line(img, cp->btn.y + 150, cp->btn.x, cp->btn.x + 280);
	// draw_horizontal_line(img, cp->btn.y + 190, cp->btn.x, cp->btn.x + 280);
	// draw_horizontal_line(img, cp->btn.y + 220, cp->btn.x, cp->btn.x + 280);
	draw_vertical_line(img, cp->btn.x, cp->btn.y, cp->btn.y + 220);
	draw_vertical_line(img, cp->btn.x + 280, cp->btn.y, cp->btn.y + 221);
	draw_hue_colors(img, cp);
	draw_sat_val_colors(img, cp);
	display_text_input(img, &cp->text_input, font8b);
	x = cp->btn.x + 240;
	while (++x < 280 + cp->btn.x)
	{
		y = cp->btn.y + 190;
		while (++y < 220 + cp->btn.y)
			put_pixel_image(img, x, y, cp->btn.background_color);
	}
	draw_vertical_line(img, cp->btn.x + 240, cp->btn.y + 190, cp->btn.y + 221);
	draw_horizontal_line(img, cp->btn.y + 190, cp->btn.x + 240, cp->btn.x + 280);
	draw_horizontal_line(img, cp->btn.y + 220, cp->btn.x + 240, cp->btn.x + 280);
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
	if (mouse_x > cp->btn.x && mouse_x < cp->btn.x + 280 && mouse_y > cp->btn.y && mouse_y < cp->btn.y + 220)
	{
		if (mouse_y < cp->btn.y + 150)
		{
			cp->hsv.sat = (mouse_x - cp->btn.x - 1) * 0.003584;
			cp->hsv.val = 1 - ((mouse_y - cp->btn.y - 1) * 0.006711);
			cp->mouse_in_hue = 1;
		}
		else if  (mouse_y < cp->btn.y + 190)
		{
			cp->hsv.hue = (mouse_x - cp->btn.x - 1) * 1.290322;
			cp->mouse_in_val_sat = 1;
		}
		else
		{
			text_input_focus(&cp->text_input, mouse_x, mouse_y);
		}
		cp->btn.background_color = hsv_to_rgb(cp->hsv.hue, cp->hsv.sat, cp->hsv.val);
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
	cp->btn.background_color = hsv_to_rgb(cp->hsv.hue, cp->hsv.sat, cp->hsv.val);
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

void	active_color_picker(void *param)
{
	((t_color_picker *)param)->active = 1;
}

int	color_picker_type(t_color_picker *cp, int key)
{
	if (text_input_type(&cp->text_input, key, 1))
	{
		cp->btn.background_color = ft_atoi_base(cp->text_input.text + 1, "0123456789ABCDEF");
		cp->btn.background_color_on_click = cp->btn.background_color;
		*cp->color = int_to_color(cp->btn.background_color);
		cp->hsv = rgb_to_hsv(cp->btn.background_color);
		return (1);
	}
	return (0);
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
	cp->text_input.is_focused = 0;
	cp->text_input.max_char = 7;
	cp->text_input.padding_left = 10;
	ft_strlcpy(cp->text_input.placeholder, "#color", 8);
	color_to_hex(cp->text_input.text, *color_var);
	printf("%s\n", cp->text_input.text);
	cp->text_input.text_color = 0;
}
