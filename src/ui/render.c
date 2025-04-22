/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 15:31:03 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/22 15:40:42 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	render_ui(t_minirt *minirt)
{
	int	i;

	if (!minirt->mlx.controls_win)
		return ;
	init_layout(&minirt->mlx.img_controls, minirt);
	minirt->controls.font[0].size = 20;
	minirt->controls.font[0].color = 0;
	i = -1;
	while (++i < minirt->controls.nb_buttons)
		display_button(&minirt->mlx.img_controls, minirt->controls.buttons[i], &minirt->controls.font[0]);
	i = -1;
	while (++i < minirt->controls.nb_sliders)
		display_slider_int(&minirt->mlx.img_controls, minirt->controls.sliders[i]);
	i = -1;
	while (++i < minirt->controls.nb_color_picker)
		display_color_picker(&minirt->mlx.img_controls, &minirt->controls.color_picker[i], &minirt->controls.font[0]);
		
	mlx_put_image_to_window(minirt->mlx.mlx, minirt->mlx.controls_win, minirt->mlx.img_controls.img, 0, 0);
}
