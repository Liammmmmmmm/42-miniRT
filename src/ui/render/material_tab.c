/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   material_tab.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 12:05:00 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/10 13:02:26 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "ui.h"

static inline void	draw_selected_mat_tab(t_img *img, t_minirt *minirt,
	t_ttf *ttf)
{
	display_text_input(img, &minirt->controls.text_input[0], ttf);
	minirt->controls.font[0].size = 15;
	draw_string(img, ttf, "Albedo", (t_point2){310, 360});
	draw_string(img, ttf, "Metallic", (t_point2){310, 410});
	if (!minirt->controls.ui_infos.selected_material->metallic_tex)
		display_float_input(img, &minirt->controls.float_input[11], ttf);
	draw_string(img, ttf, "Roughness", (t_point2){310, 460});
	if (!minirt->controls.ui_infos.selected_material->roughness_tex)
		display_float_input(img, &minirt->controls.float_input[12], ttf);
	draw_string(img, ttf, "IOR", (t_point2){310, 510});
	display_float_input(img, &minirt->controls.float_input[13], ttf);
	draw_string(img, ttf, "Transmission", (t_point2){310, 560});
	if (!minirt->controls.ui_infos.selected_material->transmission_tex)
		display_float_input(img, &minirt->controls.float_input[14], ttf);
	draw_string(img, ttf, "Ambiant occlusion", (t_point2){310, 610});
	if (!minirt->controls.ui_infos.selected_material->ao_tex)
		display_float_input(img, &minirt->controls.float_input[15], ttf);
	draw_string(img, ttf, "Emission", (t_point2){310, 660});
	if (!minirt->controls.ui_infos.selected_material->emission_strength_tex)
		display_float_input(img, &minirt->controls.float_input[16], ttf);
	draw_string(img, ttf, "Scale", (t_point2){310, 740});
	display_float_input(img, &minirt->controls.float_input[17], ttf);
}

static inline void	draw_selected_mat_tab2(t_img *img, t_minirt *minirt,
	t_ttf *ttf)
{
	draw_string(img, ttf, "Normal", (t_point2){310, 790});
	draw_string(img, ttf, "Map", (t_point2){360, 815});
	draw_string(img, ttf, "Intensity", (t_point2){330, 845});
	display_float_input(img, &minirt->controls.float_input[18], ttf);
	minirt->controls.font[0].size = 20;
	display_tex_dropdown(minirt, &minirt->controls.dropdown[10]);
	display_tex_dropdown(minirt, &minirt->controls.dropdown[9]);
	display_tex_dropdown(minirt, &minirt->controls.dropdown[8]);
	display_tex_dropdown(minirt, &minirt->controls.dropdown[7]);
	display_tex_dropdown(minirt, &minirt->controls.dropdown[6]);
	display_tex_dropdown(minirt, &minirt->controls.dropdown[5]);
	display_tex_dropdown(minirt, &minirt->controls.dropdown[4]);
	display_tex_dropdown(minirt, &minirt->controls.dropdown[3]);
	if (!minirt->controls.ui_infos.selected_material->color_tex)
		display_color_picker(img, &minirt->controls.color_picker[1], ttf);
	if (!minirt->controls.ui_infos.selected_material->emission_color_tex)
		display_color_picker(img, &minirt->controls.color_picker[2], ttf);
}

void	draw_material_tab(t_img *img, t_minirt *minirt, t_ttf *ttf)
{
	draw_horizontal_line(img, 260, 300, img->width);
	minirt->controls.font[0].size = 30;
	draw_box_2d(img, (t_point2){301, 261}, (t_point2){600, 300}, UI_BUTTON_BACK)
	;
	minirt->controls.font[0].color = 0;
	draw_string(img, ttf, "Add a material", (t_point2){340, 290});
	minirt->controls.font[0].color = 0xffffff;
	draw_horizontal_line(img, 300, 300, img->width);
	draw_horizontal_line(img, 860, 300, img->width);
	draw_list_materials(minirt);
	put_micrort_to_image(minirt);
	if (minirt->controls.ui_infos.selected_material)
	{
		draw_selected_mat_tab(img, minirt, ttf);
		draw_selected_mat_tab2(img, minirt, ttf);
	}
}
