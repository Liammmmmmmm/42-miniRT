/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   float_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 13:51:54 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/27 14:07:57 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "mlx_components.h"

void	display_float_input(t_img *img, t_float_input *input, t_ttf *ttf)
{
	int			i;
	int			j;
	t_point2	start;
	int			br_color;

	if (input->is_focused)
		br_color = input->border_color_focus;
	else
		br_color = input->border_color;
	i = input->x;
	while (i <= input->x + input->width)
	{
		put_pixel_image(img, i, input->y, br_color);
		put_pixel_image(img, i, input->y + input->height, br_color);
		i++;
	}
	i = input->y + 1;
	while (i < input->y + input->height)
	{
		put_pixel_image(img, input->x, i, br_color);
		put_pixel_image(img, input->x + input->width, i, br_color);
		j = input->x + 1;
		while (j < input->x + input->width)
		{
			put_pixel_image(img, j, i, input->background_color);
			j++;
		}
		i++;
	}
	if (input->text[0])
	{
		start.x = input->x + input->padding_left;
		start.y = input->y + input->height * 0.5 + ((get_height(ttf)) * 0.5);
		ttf->color = input->text_color;
		draw_string(img, ttf, input->text, start);
	}
}

int	float_input_focus(t_float_input *input, int mouse_x, int mouse_y)
{
	if (input->x <= mouse_x && input->x + input->width >= mouse_x
		&& input->y <= mouse_y && input->y + input->height >= mouse_y)
	{
		input->is_focused = 1;
		return (1);
	}
	input->is_focused = 0;
	return (0);
}

int	float_input_type(t_float_input *input, int key)
{
	if (input->is_focused)
	{
		if (key == ',')
			key = '.';
		if (ft_isdigit(key) || key == '-' || key == '.')
		{
			if (input->cursor_pos >= 16)
				return (1);
			input->text[input->cursor_pos] = key;
			input->cursor_pos++;
			input->text[input->cursor_pos] = 0;
		}
		else if (key == 65288 && input->cursor_pos > 0)
		{
			input->cursor_pos--;
			input->text[input->cursor_pos] = 0;
		}
		return (1);
	}
	return (0);
}

