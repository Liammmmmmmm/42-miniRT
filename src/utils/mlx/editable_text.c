/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editable_text.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 15:19:59 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/22 15:46:21 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "mlx_components.h"

void	display_text_input(t_img *img, t_text_input *text_input, t_ttf *ttf)
{
	int			i;
	int			j;
	t_point2	start;
	int			br_color;

	if (text_input->is_focused)
		br_color = text_input->border_color_focus;
	else
		br_color = text_input->border_color;
	i = text_input->x;
	while (i <= text_input->x + text_input->width)
	{
		put_pixel_image(img, i, text_input->y, br_color);
		put_pixel_image(img, i, text_input->y + text_input->height, br_color);
		i++;
	}
	i = text_input->y + 1;
	while (i < text_input->y + text_input->height)
	{
		put_pixel_image(img, text_input->x, i, br_color);
		put_pixel_image(img, text_input->x + text_input->width, i, br_color);
		j = text_input->x + 1;
		while (j < text_input->x + text_input->width)
		{
			put_pixel_image(img, j, i, text_input->background_color);
			j++;
		}
		i++;
	}
	if (text_input->text[0])
	{
		start.x = text_input->x + text_input->padding_left;
		start.y = text_input->y + text_input->height * 0.5 + ((get_height(ttf)) * 0.5);
		ttf->color = text_input->text_color;
		draw_string(img, ttf, text_input->text, start);
	}
	else if (text_input->placeholder[0])
	{
		start.x = text_input->x + text_input->padding_left;
		start.y = text_input->y + text_input->height * 0.5 + ((get_height(ttf)) * 0.5);
		ttf->color = text_input->text_color;
		draw_string(img, ttf, text_input->text, start);
	}
}

int	text_input_focus(t_text_input *text_input, int mouse_x, int mouse_y)
{
	if (text_input->x <= mouse_x && text_input->x + text_input->width >= mouse_x
		&& text_input->y <= mouse_y && \
		text_input->y + text_input->height >= mouse_y)
	{
		text_input->is_focused = 1;
		return (1);
	}
	text_input->is_focused = 0;
	return (0);
}

int	text_input_type(t_text_input *text_input, int key, int is_shift_down)
{
	if (text_input->is_focused)
	{
		if (key >= 32 && key < 127)
		{
			if (text_input->cursor_pos >= 255 || \
					text_input->cursor_pos >= text_input->max_char)
				return (1);
			if (is_shift_down)
				text_input->text[text_input->cursor_pos] = ft_toupper(key);
			else
				text_input->text[text_input->cursor_pos] = key;
			text_input->cursor_pos++;
			if (text_input->cursor_pos >= 256)
				text_input->text[text_input->cursor_pos] = 0;
		}
		else if (key == 65288 && text_input->cursor_pos > 0)
		{
			text_input->cursor_pos--;
			text_input->text[text_input->cursor_pos] = 0;
		}
		return (1);
	}
	return (0);
}
