/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   float_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 13:51:54 by lilefebv          #+#    #+#             */
/*   Updated: 2025/05/22 07:47:43 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "mlx_components.h"

static void	draw_float_input_box(t_img *img, t_float_input *input)
{
	int	i;
	int	j;
	int	br_color;

	if (input->is_focused)
		br_color = input->border_color_focus;
	else
		br_color = input->border_color;
	i = input->x - 1;
	while (++i <= input->x + input->width)
	{
		put_pixel_image(img, i, input->y, br_color);
		put_pixel_image(img, i, input->y + input->height, br_color);
	}
	i = input->y;
	while (++i < input->y + input->height)
	{
		put_pixel_image(img, input->x, i, br_color);
		put_pixel_image(img, input->x + input->width, i, br_color);
		j = input->x;
		while (++j < input->x + input->width)
			put_pixel_image(img, j, i, input->background_color);
	}
}

void	display_float_input(t_img *img, t_float_input *input, t_ttf *ttf)
{
	t_point2	start;

	draw_float_input_box(img, input);
	if (input->number && !input->is_focused
		&& *input->number != input->last_displayed)
	{
		input->last_displayed = *input->number;
		input->cursor_pos
			= set_float_to_buff(input->last_displayed, input->text);
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
	if (input->number)
	{
		input->last_displayed = *input->number;
		input->cursor_pos
			= set_float_to_buff(input->last_displayed, input->text);
	}
	input->is_focused = 0;
	return (0);
}

void	link_float_input(t_float_input *input, double *val)
{
	input->number = val;
	if (!val)
	{
		input->last_displayed = 0.0;
		return ;
	}
	input->cursor_pos = set_float_to_buff(*val, input->text);
	input->last_displayed = *val;
}
