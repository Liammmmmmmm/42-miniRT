/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   button.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 14:47:09 by lilefebv          #+#    #+#             */
/*   Updated: 2025/05/21 16:19:21 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "mlx_components.h"
#include <stdio.h>

static void	init_colors(const t_button *button, int *bg_color, int *br_color)
{
	if (button->is_clicked)
	{
		*bg_color = button->background_color_on_click;
		*br_color = button->border_color_on_click;
	}
	else
	{
		*bg_color = button->background_color;
		*br_color = button->border_color;
	}
}

static void	draw_background_border(t_img *img, const t_button *button,
	int bg_color, int br_color)
{
	int	i;
	int	j;

	i = button->x;
	while (i <= button->x + button->width)
	{
		put_pixel_image(img, i, button->y, br_color);
		put_pixel_image(img, i, button->y + button->height, br_color);
		i++;
	}
	i = button->y + 1;
	while (i < button->y + button->height)
	{
		put_pixel_image(img, button->x, i, br_color);
		put_pixel_image(img, button->x + button->width, i, br_color);
		j = button->x + 1;
		while (j < button->x + button->width)
		{
			put_pixel_image(img, j, i, bg_color);
			j++;
		}
		i++;
	}
}

void	display_button(t_img *img, const t_button button, t_ttf *ttf)
{
	t_point2	start;
	int			bg_color;
	int			br_color;

	if (button.disabled)
		return ;
	init_colors(&button, &bg_color, &br_color);
	draw_background_border(img, &button, bg_color, br_color);
	if (button.text && ttf)
	{
		start.x = button.x + button.width * 0.5
			- (get_string_width(ttf, button.text) * 0.5);
		start.y = button.y + button.height * 0.5 + ((get_height(ttf)) * 0.5);
		draw_string(img, ttf, button.text, start);
	}
}

int	button_action(t_button *button, int mouse_x, int mouse_y, int *is_clicked)
{
	if (button->disabled || *is_clicked)
		return (0);
	if (button->x <= mouse_x && button->x + button->width >= mouse_x
		&& button->y <= mouse_y && button->y + button->height >= mouse_y)
	{
		*is_clicked = 1;
		button->is_clicked = 1;
		button->action(button->param);
		return (1);
	}
	return (0);
}

int	button_release(t_button *button)
{
	button->is_clicked = 0;
	return (1);
}
