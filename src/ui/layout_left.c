/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   layout_left.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 11:41:23 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/05 16:10:04 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	draw_left_layout(t_img *img, t_minirt *minirt)
{
	minirt->controls.font[0].size = 25;
	draw_string(img, &minirt->controls.font[0], "Camera position", (t_point2){20, 30});
	draw_string(img, &minirt->controls.font[0], "Camera orientation", (t_point2){20, 100});
	draw_string(img, &minirt->controls.font[0], "Camera options", (t_point2){20, 170});
	draw_string(img, &minirt->controls.font[0], "Ambiant", (t_point2){20, 320});
	draw_string(img, &minirt->controls.font[0], "Ray tracing", (t_point2){20, 440});

	minirt->controls.font[0].size = 20;
	draw_string(img, &minirt->controls.font[0], "X", (t_point2){10, 60});
	draw_string(img, &minirt->controls.font[0], "Z", (t_point2){205, 60});
	draw_string(img, &minirt->controls.font[0], "Y", (t_point2){105, 60});

	draw_string(img, &minirt->controls.font[0], "X", (t_point2){10, 130});
	draw_string(img, &minirt->controls.font[0], "Y", (t_point2){105, 130});
	draw_string(img, &minirt->controls.font[0], "Z", (t_point2){205, 130});

	draw_string(img, &minirt->controls.font[0], "FOV", (t_point2){10, 200});
	draw_string(img, &minirt->controls.font[0], "Defocus Angle", (t_point2){10, 230});
	draw_string(img, &minirt->controls.font[0], "Focus distance", (t_point2){10, 260});
	
	draw_string(img, &minirt->controls.font[0], "Power", (t_point2){10, 390});

	draw_string(img, &minirt->controls.font[0], "Max bounces", (t_point2){10, 470});
	
	minirt->controls.font[0].size = 15;
	display_float_input(img, &minirt->controls.float_input[19], &minirt->controls.font[0]);
	display_float_input(img, &minirt->controls.float_input[20], &minirt->controls.font[0]);
	display_float_input(img, &minirt->controls.float_input[21], &minirt->controls.font[0]);
	
	display_float_input(img, &minirt->controls.float_input[22], &minirt->controls.font[0]);
	display_float_input(img, &minirt->controls.float_input[23], &minirt->controls.font[0]);
	display_float_input(img, &minirt->controls.float_input[24], &minirt->controls.font[0]);

	display_float_input(img, &minirt->controls.float_input[25], &minirt->controls.font[0]);
	display_float_input(img, &minirt->controls.float_input[26], &minirt->controls.font[0]);

	display_float_input(img, &minirt->controls.float_input[27], &minirt->controls.font[0]);

	display_float_input(img, &minirt->controls.float_input[28], &minirt->controls.font[0]);

	display_tex_dropdown(minirt, &minirt->controls.dropdown[12]);
	if (!minirt->scene.amb_light.skybox_t)
		display_color_picker(img, &minirt->controls.color_picker[3], &minirt->controls.font[0]);
}
