/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   layout_left.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 11:41:23 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/10 14:59:51 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	draw_texts(t_img *img, t_minirt *minirt, t_ttf *ttf)
{
	minirt->controls.font[0].size = 25;
	draw_string(img, ttf, "Camera position", (t_point2){20, 30});
	draw_string(img, ttf, "Camera orientation", (t_point2){20, 100});
	draw_string(img, ttf, "Camera options", (t_point2){20, 170});
	draw_string(img, ttf, "Ambiant", (t_point2){20, 320});
	draw_string(img, ttf, "Ray tracing", (t_point2){20, 440});
	draw_string(img, ttf, "Global", (t_point2){20, 560});
	minirt->controls.font[0].size = 20;
	draw_string(img, ttf, "X", (t_point2){10, 60});
	draw_string(img, ttf, "Z", (t_point2){205, 60});
	draw_string(img, ttf, "Y", (t_point2){105, 60});
	draw_string(img, ttf, "X", (t_point2){10, 130});
	draw_string(img, ttf, "Y", (t_point2){105, 130});
	draw_string(img, ttf, "Z", (t_point2){205, 130});
	draw_string(img, ttf, "FOV", (t_point2){10, 200});
	draw_string(img, ttf, "Defocus Angle", (t_point2){10, 230});
	draw_string(img, ttf, "Focus distance", (t_point2){10, 260});
	draw_string(img, ttf, "Power", (t_point2){10, 390});
	draw_string(img, ttf, "Max bounces", (t_point2){10, 470});
	draw_string(img, ttf, "Resolution", (t_point2){10, 500});
	draw_string(img, ttf, "Gamma", (t_point2){10, 590});
	if (minirt->scene.render_height < minirt->scene.win_height
		&& minirt->scene.render_width < minirt->scene.win_width)
		draw_string(img, ttf, "Upscaling", (t_point2){10, 620});
}

void	draw_left_layout(t_img *img, t_minirt *minirt)
{
	t_ttf	*ttf;

	ttf = &minirt->controls.font[0];
	draw_texts(img, minirt, ttf);
	minirt->controls.font[0].size = 15;
	display_float_input(img, &minirt->controls.float_input[19], ttf);
	display_float_input(img, &minirt->controls.float_input[20], ttf);
	display_float_input(img, &minirt->controls.float_input[21], ttf);
	display_float_input(img, &minirt->controls.float_input[22], ttf);
	display_float_input(img, &minirt->controls.float_input[23], ttf);
	display_float_input(img, &minirt->controls.float_input[24], ttf);
	display_float_input(img, &minirt->controls.float_input[25], ttf);
	display_float_input(img, &minirt->controls.float_input[26], ttf);
	display_float_input(img, &minirt->controls.float_input[27], ttf);
	display_float_input(img, &minirt->controls.float_input[28], ttf);
	display_float_input(img, &minirt->controls.float_input[29], ttf);
	display_float_input(img, &minirt->controls.float_input[30], ttf);
	display_float_input(img, &minirt->controls.float_input[31], ttf);
	if (minirt->scene.render_height < minirt->scene.win_height
		&& minirt->scene.render_width < minirt->scene.win_width)
		display_dropdown(minirt, &minirt->controls.dropdown[13]);
	display_tex_dropdown(minirt, &minirt->controls.dropdown[12]);
	if (!minirt->scene.amb_light.skybox_t)
		display_color_picker(img, &minirt->controls.color_picker[3], ttf);
}
