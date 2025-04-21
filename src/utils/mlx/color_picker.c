/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_picker.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 16:50:31 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/21 17:07:58 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "mlx_components.h"

void	display_color_picker(t_img *img, t_color_picker *button)
{
	
}

int	color_picker_action(t_color_picker *button, int mouse_x, int mouse_y)
{
	// if (button->disabled || *is_clicked)
	// 	return (0);
	// if (button->x <= mouse_x && button->x + button->width >= mouse_x
	// 	&& button->y <= mouse_y && button->y + button->height >= mouse_y)
	// {
	// 	*is_clicked = 1;
	// 	button->is_clicked = 1;
	// 	button->action(button->param);
	// 	return (1);
	// }
	return (0);
}

int	color_picker_release(t_color_picker *button)
{
	// button->is_clicked = 0;
	return (1);
}

