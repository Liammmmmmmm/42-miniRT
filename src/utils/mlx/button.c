/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   button.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 14:47:09 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/20 15:52:41 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mlx_components.h"
#include <stdio.h>

void	display_button(t_img *img, const t_button button, t_ttf *ttf)
{
	int			i;
	int			j;
	t_point2	start;
	int			bg_color;
	int			br_color;

	if (button.disabled)
		return ;
	if (button.is_clicked)
	{
		bg_color = button.background_color_on_click;
		br_color = button.border_color_on_click;
	}
	else
	{
		bg_color = button.background_color;
		br_color = button.border_color;
	}
	i = button.x;
	while (i <= button.x + button.width)
	{
		put_pixel_image(img, i, button.y, br_color);
		put_pixel_image(img, i, button.y + button.height, br_color);
		i++;
	}
	i = button.y + 1;
	while (i < button.y + button.height)
	{
		put_pixel_image(img, button.x, i, br_color);
		put_pixel_image(img, button.x + button.width, i, br_color);
		j = button.x + 1;
		while (j < button.x + button.width)
		{
			put_pixel_image(img, j, i, bg_color);
			j++;
		}
		i++;
	}
	if (button.text)
	{
		start.x = button.x + button.width * 0.5 - (get_string_width(ttf, button.text) * 0.5);
		start.y = button.y + button.height * 0.5 + ((get_height(ttf)) * 0.5);
		draw_string(img, ttf, button.text, start);
		put_pixel_image(img, 20, start.y, 0xFF0000);
		put_pixel_image(img, 20, start.y - get_height(ttf), 0xFF0000);
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
