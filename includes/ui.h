/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 14:22:47 by lilefebv          #+#    #+#             */
/*   Updated: 2025/08/21 21:08:42 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UI_H
# define UI_H

# include "structs.h"
# include "utils.h"

# define UI_BACK_COLOR 0x0E0E0E
# define UI_SEP_COLOR 0xFFFFFF
# define UI_BUTTON_BACK 0xB4B4B4
# define UI_BUTTON_CLICK 0x919191

/*═════════════════════════════════════════════════════════════════════════════╗
║                                 CONTROLS / UI                                ║
╚═════════════════════════════════════════════════════════════════════════════*/

void	put_micrort_to_image(t_minirt *minirt);
void	draw_box_2d(t_img *img, t_point2 p1, t_point2 p2, int color);
void	setcolor(t_img *img, uint32_t color);

int		init_ui(t_minirt *minirt);
void	init_font(t_minirt *minirt);
void	init_controls(t_minirt *minirt);
int		init_buttons(t_minirt *minirt);
int		init_sliders(t_minirt *minirt);
int		init_cps(t_minirt *minirt);

void	clear_sliders(t_minirt *minirt);
void	click_start_stop(void *vparam);
void	clear_buttons(t_minirt *minirt);

t_cchar	get_upscaling_name(t_upscalings up);
void	init_choose_object(t_minirt *minirt);
void	init_tex_dropdown(t_minirt *minirt);
void	base_dropdown_tex(t_dropdown *dropdown);
void	base_dropdown(t_dropdown *dropdown);
int		init_dropdowns(t_minirt *minirt);
void	clear_dropdown(t_minirt *minirt);

void	ambiant_and_more(t_minirt *minirt);
void	init_mat_float_input(t_minirt *minirt);
void	mat_float_input(t_float_input *input);
void	base_tiers_float_input(t_float_input *input);
void	base_two_tiers_float_input(t_float_input *input);
int		init_float_inputs(t_minirt *minirt);
void	clear_float_inputs(t_minirt *minirt);

int		init_text_inputs(t_minirt *minirt);
void	clear_text_inputs(t_minirt *minirt);

void	draw_main_box(t_img *img, t_dropdown *dropdown);
int		dropdown_common_define(t_dropdown *dropdown, int *i1, int *i2, int i);
void	draw_dropdown_select_box(t_img *img, t_dropdown *dropdown);
int		toggle_dropdown(int key, t_point2 pos, t_dropdown *dropdown);

void	display_mat_dropdown(t_minirt *minirt, t_dropdown *dropdown);
int		mouse_down_dropdown_mat(t_minirt *minirt, int key, t_point2 pos,
			t_dropdown *dropdown);

void	display_tex_dropdown(t_minirt *minirt, t_dropdown *dropdown);
int		mouse_down_dropdown_tex(t_minirt *minirt, int key, t_point2 pos,
			t_dropdown *d);

void	display_dropdown(t_minirt *minirt, t_dropdown *dropdown);
int		mouse_down_dropdown(int key, t_point2 pos, t_dropdown *d);

void	change_tab(void *vparam);

void	set_selected_hyperboloid(t_minirt *minirt, t_hyperboloid *obj);
void	set_selected_cone(t_minirt *minirt, t_cone *obj);
void	set_selected_cylinder(t_minirt *minirt, t_cylinder *obj);
void	set_selected_plane(t_minirt *minirt, t_plane *obj);
void	set_selected_sphere(t_minirt *minirt, t_sphere *obj);
void	set_selected_object(t_minirt *minirt, t_object *obj);
int		mouse_down_obj(t_minirt *minirt, int key, int mouse_x, int mouse_y);
void	draw_list_objects(t_minirt *minirt);

void	set_selected_mat(t_minirt *minirt, t_mat *mat);
int		mouse_down_mat(t_minirt *minirt, int key, int mouse_x, int mouse_y);
void	draw_list_materials(t_minirt *minirt);

void	init_layout(t_img *img, t_minirt *minirt);
void	draw_material_tab(t_img *img, t_minirt *minirt, t_ttf *ttf);
void	draw_sphere_prop(t_img *img, t_minirt *minirt, t_ttf *ttf);
void	draw_plane_prop(t_img *img, t_minirt *mrt, t_ttf *ttf);
void	draw_cyl_prop(t_img *img, t_minirt *minirt, t_ttf *ttf);
void	draw_light_prop(t_img *img, t_minirt *minirt, t_ttf *ttf);
void	draw_directional_light_prop(t_img *img, t_minirt *minirt,
			t_ttf *ttf);
void	draw_hyperboloid_prop(t_img *img, t_minirt *minirt, t_ttf *ttf);
void	draw_cone_prop(t_img *img, t_minirt *minirt, t_ttf *ttf);
void	draw_custom_prop(t_img *img, t_minirt *minirt, t_ttf *ttf);
void	draw_objects_tab(t_img *img, t_minirt *minirt, t_ttf *ttf);
void	draw_left_layout(t_img *img, t_minirt *minirt);
void	render_ui(t_minirt *minirt);

void	layout_mouse_down(int key, int x, int y, t_minirt *minirt);
int		mouse_down_light(int x, int y, t_minirt *minirt);
int		mouse_down_cone(int key, int x, int y, t_minirt *minirt);
int		mouse_down_hyperboloid(int key, int x, int y, t_minirt *minirt);
int		mouse_down_custom(int key, int x, int y, t_minirt *minirt);
int		mouse_down_directional_light(int x, int y, t_minirt *minirt);
int		mouse_down_selected_object(int key, int x, int y, t_minirt *minirt);
int		color_picker_stop_minirt(t_minirt *minirt, int x, int y,
			t_color_picker *cp);
int		color_picker_stop_minirt_mat(t_minirt *minirt, int x, int y,
			t_color_picker *cp);
void	float_input_position(t_minirt *minirt, int x, int y);
void	float_input_orientation(t_minirt *minirt, int x, int y);
int		manage_scroll_selected_object(int key, int x, int y, t_minirt *minirt);
int		material_tab_events(int key, int x, int y, t_minirt *minirt);

void	add_material(t_minirt *minirt);
void	create_default_sphere(t_object *obj);
void	create_default_cyl(t_object *obj);
void	create_default_cone(t_object *obj);
void	create_default_hyperboloid(t_object *obj);
void	create_default_plane(t_object *obj);
void	add_object(void *vparam);
void	rm_object(void *vparam);

double	parse_expression(const char *expr);

#endif