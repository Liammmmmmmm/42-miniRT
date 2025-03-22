/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   button.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 14:47:09 by lilefebv          #+#    #+#             */
/*   Updated: 2025/03/22 18:38:10 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mlx_components.h"

void	display_button(t_img *img, const t_button button, unsigned char font[96][5])
{
	int		i;
	int		j;
	t_point start;
	int		bg_color;
	int		br_color;

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
		put_pixel_image(img->img_str, i, button.y, br_color);
		put_pixel_image(img->img_str, i, button.y + button.height, br_color);
		i++;
	}
	i = button.y + 1;
	while (i < button.y + button.height)
	{
		put_pixel_image(img->img_str, button.x, i, br_color);
		put_pixel_image(img->img_str, button.x + button.width, i, br_color);
		j = button.x + 1;
		while (j < button.x + button.width)
		{
			put_pixel_image(img->img_str, j, i, bg_color);
			j++;
		}
		i++;
	}
	if (button.text)
	{
		start.x = button.x + button.width / 2 - (string_size(button.text) / 2);
		start.y = button.y + button.height / 2 - 3;
		start.color = 0;
		string_to_img(img, font, start, button.text);
	}
}

int	button_action(t_button *button, int mouse_x, int mouse_y)
{
	if (button->x <= mouse_x && button->x + button->width >= mouse_x
		&& button->y <= mouse_y && button->y + button->height >= mouse_y)
	{
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
