/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_prop.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 12:45:15 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/10 13:02:26 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "ui.h"

void	draw_sphere_prop(t_img *img, t_minirt *minirt,
	t_ttf *ttf)
{
	draw_string(img, ttf, "Position", (t_point2){320, 490});
	if (!((t_sphere *)minirt->controls.ui_infos.selected_object->object)
		->material)
		draw_string(img, ttf, "Color", (t_point2){320, 690});
	draw_string(img, ttf, "Material", (t_point2){320, 790});
	draw_string(img, ttf, "Size", (t_point2){320, 890});
	minirt->controls.font[0].size = 20;
	draw_string(img, ttf, "X", (t_point2){310, 526});
	draw_string(img, ttf, "Y", (t_point2){405, 526});
	draw_string(img, ttf, "Z", (t_point2){505, 526});
	minirt->controls.font[0].size = 15;
	display_float_input(img, &minirt->controls.float_input[0], ttf);
	display_float_input(img, &minirt->controls.float_input[1], ttf);
	display_float_input(img, &minirt->controls.float_input[2], ttf);
	minirt->controls.font[0].size = 20;
	draw_string(img, ttf, "Diameter", (t_point2){310, 920});
	display_float_input(img, &minirt->controls.float_input[6], ttf);
	display_mat_dropdown(minirt, &minirt->controls.dropdown[0]);
	if (!((t_sphere *)minirt->controls.ui_infos.selected_object->object)
		->material)
		display_color_picker(img, &minirt->controls.color_picker[0], ttf);
}

void	draw_plane_prop(t_img *img, t_minirt *mrt, t_ttf *ttf)
{
	draw_string(img, ttf, "Position", (t_point2){320, 490});
	draw_string(img, ttf, "Orientation", (t_point2){320, 590});
	if (!((t_plane *)mrt->controls.ui_infos.selected_object->object)->material)
		draw_string(img, ttf, "Color", (t_point2){320, 690});
	draw_string(img, ttf, "Material", (t_point2){320, 790});
	mrt->controls.font[0].size = 20;
	draw_string(img, ttf, "X", (t_point2){310, 526});
	draw_string(img, ttf, "Y", (t_point2){405, 526});
	draw_string(img, ttf, "Z", (t_point2){505, 526});
	mrt->controls.font[0].size = 15;
	display_float_input(img, &mrt->controls.float_input[0], ttf);
	display_float_input(img, &mrt->controls.float_input[1], ttf);
	display_float_input(img, &mrt->controls.float_input[2], ttf);
	mrt->controls.font[0].size = 20;
	draw_string(img, ttf, "X", (t_point2){310, 626});
	draw_string(img, ttf, "Y", (t_point2){405, 626});
	draw_string(img, ttf, "Z", (t_point2){505, 626});
	mrt->controls.font[0].size = 15;
	display_float_input(img, &mrt->controls.float_input[3], ttf);
	display_float_input(img, &mrt->controls.float_input[4], ttf);
	display_float_input(img, &mrt->controls.float_input[5], ttf);
	mrt->controls.font[0].size = 20;
	display_mat_dropdown(mrt, &mrt->controls.dropdown[0]);
	if (!((t_plane *)mrt->controls.ui_infos.selected_object->object)->material)
		display_color_picker(img, &mrt->controls.color_picker[0], ttf);
}

static inline void	draw_cyl_prop2(t_img *img, t_minirt *minirt,
	t_ttf *ttf)
{
	minirt->controls.font[0].size = 20;
	draw_string(img, ttf, "Diameter", (t_point2){310, 980});
	display_float_input(img, &minirt->controls.float_input[6], ttf);
	draw_string(img, ttf, "Height", (t_point2){310, 1010});
	display_float_input(img, &minirt->controls.float_input[7], ttf);
	display_mat_dropdown(minirt, &minirt->controls.dropdown[2]);
	display_mat_dropdown(minirt, &minirt->controls.dropdown[1]);
	display_mat_dropdown(minirt, &minirt->controls.dropdown[0]);
	if (!((t_cylinder *)minirt->controls.ui_infos.selected_object->object)
		->material)
		display_color_picker(img, &minirt->controls.color_picker[0], ttf);
}

void	draw_cyl_prop(t_img *img, t_minirt *minirt,
	t_ttf *ttf)
{
	draw_string(img, ttf, "Position", (t_point2){320, 490});
	draw_string(img, ttf, "Orientation", (t_point2){320, 590});
	if (!((t_cylinder *)minirt->controls.ui_infos.selected_object->object)
		->material)
		draw_string(img, ttf, "Color", (t_point2){320, 690});
	draw_string(img, ttf, "Material", (t_point2){320, 790});
	draw_string(img, ttf, "Size", (t_point2){320, 950});
	minirt->controls.font[0].size = 20;
	draw_string(img, ttf, "X", (t_point2){310, 526});
	draw_string(img, ttf, "Y", (t_point2){405, 526});
	draw_string(img, ttf, "Z", (t_point2){505, 526});
	draw_string(img, ttf, "X", (t_point2){310, 626});
	draw_string(img, ttf, "Y", (t_point2){405, 626});
	draw_string(img, ttf, "Z", (t_point2){505, 626});
	minirt->controls.font[0].size = 15;
	display_float_input(img, &minirt->controls.float_input[0], ttf);
	display_float_input(img, &minirt->controls.float_input[1], ttf);
	display_float_input(img, &minirt->controls.float_input[2], ttf);
	display_float_input(img, &minirt->controls.float_input[3], ttf);
	display_float_input(img, &minirt->controls.float_input[4], ttf);
	display_float_input(img, &minirt->controls.float_input[5], ttf);
	draw_cyl_prop2(img, minirt, ttf);
}

void	draw_light_prop(t_img *img, t_minirt *minirt,
	t_ttf *ttf)
{
	draw_string(img, ttf, "Position", (t_point2){320, 490});
	draw_string(img, ttf, "Color", (t_point2){320, 690});
	draw_string(img, ttf, "Power", (t_point2){320, 890});
	minirt->controls.font[0].size = 20;
	draw_string(img, ttf, "X", (t_point2){310, 526});
	draw_string(img, ttf, "Y", (t_point2){405, 526});
	draw_string(img, ttf, "Z", (t_point2){505, 526});
	minirt->controls.font[0].size = 15;
	display_float_input(img, &minirt->controls.float_input[0], ttf);
	display_float_input(img, &minirt->controls.float_input[1], ttf);
	display_float_input(img, &minirt->controls.float_input[2], ttf);
	minirt->controls.font[0].size = 20;
	draw_string(img, ttf, "Power", (t_point2){310, 920});
	display_float_input(img, &minirt->controls.float_input[6], ttf);
	minirt->controls.font[0].size = 20;
	display_color_picker(img, &minirt->controls.color_picker[0], ttf);
}
