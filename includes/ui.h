/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 14:22:47 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/10 14:50:35 by lilefebv         ###   ########lyon.fr   */
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

// typedef struct s_ui_param
// {
// 	int	key;
// 	int	mouse_x;
// 	int	mouse_y;
// }	t_ui_param;

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

int		init_dropdowns(t_minirt *minirt);
void	clear_dropdown(t_minirt *minirt);

int		init_float_inputs(t_minirt *minirt);
void	clear_float_inputs(t_minirt *minirt);

int		init_text_inputs(t_minirt *minirt);
void	clear_text_inputs(t_minirt *minirt);

void	draw_main_box(t_img *img, t_dropdown *dropdown);
int		dropdown_common_define(t_dropdown *dropdown, int *i1, int *i2, int i);
void	draw_dropdown_select_box(t_img *img, t_dropdown *dropdown);
int		toggle_dropdown(int key, t_point2 pos, t_dropdown *dropdown);

void	display_mat_dropdown(t_minirt *minirt, t_dropdown *dropdown);
int		mouse_down_dropdown_mat(t_minirt *minirt, int key, t_point2 pos, t_dropdown *dropdown);

void	display_tex_dropdown(t_minirt *minirt, t_dropdown *dropdown);
int		mouse_down_dropdown_tex(t_minirt *minirt, int key, t_point2 pos, t_dropdown *d);

void	display_dropdown(t_minirt *minirt, t_dropdown *dropdown);
int		mouse_down_dropdown(int key, t_point2 pos, t_dropdown *d);

void	change_tab(void *vparam);

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
void	float_input_position(t_minirt *minirt, int x, int y);
void	float_input_orientation(t_minirt *minirt, int x, int y);
int		manage_scroll_selected_object(int key, int x, int y, t_minirt *minirt);
int		material_tab_events(int key, int x, int y, t_minirt *minirt);

double	parse_expression(const char *expr);


#endif