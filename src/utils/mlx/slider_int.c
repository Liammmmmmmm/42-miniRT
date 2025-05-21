/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   slider_int.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 14:47:09 by lilefebv          #+#    #+#             */
/*   Updated: 2025/05/21 16:06:05 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "mlx_components.h"

void	display_slider_int(t_img *img, const t_int_slider slider)
{
	int	i;
	int	j;

	i = slider.y;
	while (i < slider.y + slider.height)
	{
		j = slider.x;
		while (j < slider.x + slider.width)
		{
			put_pixel_image(img, j, i, slider.bar_color);
			j++;
		}
		i++;
	}
	draw_filled_circle((t_circle){slider.x + ((*slider.value - slider.min) * \
		slider.width / (slider.max - slider.min)), slider.y + slider.height / \
		2, 7}, img, slider.point_color);
}

int	slider_mouse_down(t_int_slider *slider, int mouse_x, int mouse_y)
{
	int	circle_center_x;
	int	circle_center_y;

	circle_center_x = slider->x + ((*slider->value - slider->min) * \
		slider->width / (slider->max - slider->min));
	circle_center_y = slider->y + slider->height * 0.5;
	if (circle_center_x - 7 <= mouse_x && circle_center_x + 7 >= mouse_x
		&& circle_center_y - 7 <= mouse_y && circle_center_y + 7 >= mouse_y)
	{
		slider->is_clicked = 1;
		return (1);
	}
	if (slider->x <= mouse_x && slider->x + slider->width >= mouse_x
		&& slider->y <= mouse_y && slider->y + slider->height >= mouse_y)
	{
		*slider->value = (mouse_x - slider->x) * (slider->max - slider->min) / \
			slider->width;
		if (*slider->value < slider->min)
			*slider->value = slider->min;
		else if (*slider->value > slider->max)
			*slider->value = slider->max;
		slider->is_clicked = 1;
		return (1);
	}
	return (0);
}

int	slider_mouse_up(t_int_slider *slider)
{
	if (slider->is_clicked == 1)
	{
		slider->is_clicked = 0;
		return (1);
	}
	return (0);
}

int	slider_mouse_move(t_int_slider *slider, int mouse_x)
{
	int	dx;

	if (slider->is_clicked == 1)
	{
		dx = mouse_x - slider->x;
		*slider->value = dx * (slider->max - slider->min) / slider->width;
		if (*slider->value < slider->min)
			*slider->value = slider->min;
		else if (*slider->value > slider->max)
			*slider->value = slider->max;
		return (1);
	}
	return (0);
}
