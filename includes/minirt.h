/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 09:40:06 by lilefebv          #+#    #+#             */
/*   Updated: 2025/05/08 12:57:12 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# include "font.h"
# include "structs.h"
# include "libft.h"
# include "maths.h"
# include <mlx.h>
# include <fcntl.h>
# include <stdio.h>
# include <string.h>
# include <errno.h>
# include <math.h>
# include "bmp_parsing.h"
# include "obj_parsing.h"
# include "material.h"
# include "ui.h"

# define PI_D 3.1415926535897

# define WIN_WIDTH 1920
# define WIN_HEIGHT 1080

// Control window
# define CWIN_WIDTH 600
# define CWIN_HEIGHT 1080

typedef enum e_mouse_buttons
{
	LEFT_CLICK = 1,
	MIDDLE_CLICK,
	RIGHT_CLICK,
	SCROLL_UP,
	SCROLL_DOWN
}	t_mouse_buttons;

typedef enum e_event
{
	ON_KEYDOWN = 2,
	ON_KEYUP = 3,
	ON_MOUSEDOWN = 4,
	ON_MOUSEUP = 5,
	ON_MOUSEMOVE = 6,
	ON_EXPOSE = 12,
	ON_DESTROY = 17
}	t_event;

typedef enum e_keys
{
	KEY_SPACE = 32,
	KEY_INF = 44,
	KEY_MINUS = 45,
	KEY_SUP = 46,
	KEY_PLUS = 61,
	KEY_A = 97,
	KEY_B = 98,
	KEY_C = 99,
	KEY_D = 100,
	KEY_E = 101,
	KEY_F = 102,
	KEY_G = 103,
	KEY_H = 104,
	KEY_I = 105,
	KEY_J = 106,
	KEY_K = 107,
	KEY_L = 108,
	KEY_N = 110,
	KEY_O = 111,
	KEY_P = 112,
	KEY_Q = 113,
	KEY_R = 114,
	KEY_S = 115,
	KEY_T = 116,
	KEY_V = 118,
	KEY_W = 119,
	KEY_X = 120,
	KEY_Z = 122,
	NUM_0 = 65438,
	NUM_1 = 65436,
	NUM_2 = 65433,
	NUM_3 = 65435,
	NUM_4 = 65430,
	NUM_5 = 65437,
	NUM_6 = 65432,
	NUM_7 = 65429,
	NUM_8 = 65431,
	NUM_9 = 65434,
	KEY_ESC = 65307,
	KEY_LSHIFT = 65505,
	KEY_RSHIFT = 65506,
	KEY_LCTRL = 65507,
	KEY_RCTRL = 65508,
}	t_keys;

# define ERR_F "Invalid format for"

int		print_error(char *err);

/*═════════════════════════════════════════════════════════════════════════════╗
║                                    UTILS                                     ║
╚═════════════════════════════════════════════════════════════════════════════*/

void	print_light(t_light *light);
void	print_cone(t_cone *cone);
void	print_cylinder(t_cylinder *cylinder);
void	print_plane(t_plane *plane);
void	print_sphere(t_sphere *sphere);
void	print_hyperboloid(t_hyperboloid *hyp);
void	print_objects(t_scene *scene);

void	print_color(t_color color);
void	print_vec3(t_vec3 vec);
void	print_mat_color(t_mat *mat, t_color color);
void	print_tex_color_text(t_tex *tex, t_color color, char *txt);
void	print_tex_double_text(t_tex *tex, double value, char *txt);

void	print_scene(t_scene *scene);
void	print_progress_bar(size_t actual, size_t max);
int		char_tab_len(char **tab);
char	**free_ret_null(char **fre);

/*═════════════════════════════════════════════════════════════════════════════╗
║                                   PARSING                                    ║
╚═════════════════════════════════════════════════════════════════════════════*/

int		not_enough_el_error(char *el);
int		to_many_el_error(char *el);
int		print_line_error(int nb, char *line);
int		invalid_struct_error(t_objects type, char **splited);
int		invalid_float_error(char **splited, int i);
int		invalid_size_error(char **splited);
int		texture_error(int error, char **parts);
int		material_error(int error, char **parts);
int		material_item_error(int error, char *part);
int		texture_item_error(int error, char *part);

/**
 * @return 1 if the line start by `type` folowed by a space or tab, 0 otherwise
 */
int		cmp_type(const char *type, const char *line);

int		is_empty_line(char *line);

/**
 * @return -1 for an invalid line, 0 for an empty line, 1 for a valid line
 */
int		is_valid_line(char *line);

/**
 * @return 1 valid, -1 incorrect characters, -2 to long
 */
int		is_valid_variable_name(char *str);

/**
 * @return 1 valid, 0 already used, -1 incorrect characters, -2 to long
 */
int		is_valid_variable_name_tex(char *str, t_scene *scene);

/**
 * @return 1 valid, 0 already used, -1 incorrect characters, -2 to long
 */
int		is_valid_variable_name_mat(char *str, t_scene *scene);

int		is_valid_element(char *line);
int		contains_var_chars(char *str);

int		count_valid_lines(char *filename);
char	*remove_useless_spaces(char *line);
char	*remove_comments(char *line);
int		valid_elements_amount(char **lines);

int		free_scene(t_scene *scene, char **lines);
int		parse_elements(t_scene *scene, char **lines, int ln_amount);

t_mat	*get_material(char *str, t_scene *scene);
int		is_valid_double_el(char *str, double *co);
int		is_valid_double_el_no_bordered(char *str, double *co);
int		is_valid_size(char *str, double *co);
int		is_valid_fov(char *str, unsigned char *fov);
int		parse_color(char *color, t_color *store);
int		parse_vector(char *str, t_vec3 *vec);
int		parse_vector_normalized(char *str, t_vec3 *vec);
int		parse_color_or_mat(char *str, t_color *store, t_mat **mat, t_scene *scene);
int		parse_only_mat(char *str, t_mat **mat, t_scene *scene);
int		parse_color_or_tex(char *str, t_color *store, t_tex **tex, t_scene *scene);
int		parse_double_b_or_tex(char *str, double *d, t_tex **tex, t_scene *scene);
int		parse_double_or_tex(char *str, double *d, t_tex **tex, t_scene *scene);
t_tex	*get_texture(char *str, t_scene *scene);
void	get_texture_image(t_tex *tex, char *filename);
int		is_extension(char *filename, char *extension);

int		parse_ambiant_light(t_scene *scene, char *line);
int		parse_camera(t_scene *scene, char *line);
int		parse_light(t_scene *scene, char *line);
int		parse_sphere(t_scene *scene, char *line);
int		parse_plane(t_scene *scene, char *line);
int		parse_cylinder(t_scene *scene, char *line);
int		parse_texture(t_scene *scene, char *line);
int		parse_material(t_scene *scene, char *line);
int		parse_cone(t_scene *scene, char *line);
int		parse_hyperboloid(t_scene *scene, char *line);
int		parse_obj_custom(t_scene *scene, char *line);

void	bump_to_normal(t_tex_img *bump);

int		parse_scene(t_minirt *minirt, char *filename);
int		parse_ambient_light_ratio_and_color(t_scene *scene, char **parts);
int		parse_ambient_light_skybox(t_scene *scene, char **parts);

/*═════════════════════════════════════════════════════════════════════════════╗
║                                     MLX                                      ║
╚═════════════════════════════════════════════════════════════════════════════*/

int		init_mlx(t_minirt *minirt);
void	free_mlx(t_minirt *minirt);

void	put_sp_image(t_img *img, t_sc_point *sp);
void	put_pixel_image(t_img *img, int x, int y, int color);

/*═════════════════════════════════════════════════════════════════════════════╗
║                                    EVENTS                                    ║
╚═════════════════════════════════════════════════════════════════════════════*/

int		destroy_controls(t_minirt *minirt);
int		destroy(t_minirt *minirt);

void	keydown_common(int key, t_minirt *minirt);
void	keyup_common(int key, t_minirt *minirt);

int		keydown_render(int key, t_minirt *minirt);
int		keyup_render(int key, t_minirt *minirt);

int		keydown_controls(int key, t_minirt *minirt);
int		keyup_controls(int key, t_minirt *minirt);

void	mouseup_common(int key, t_minirt *minirt);
void	mousedown_common(int key, t_minirt *minirt);

int		mouse_move_controls(int x, int y, t_minirt *minirt);
int		mouseup_controls(int key, int x, int y, t_minirt *minirt);
int		mousedown_controls(int key, int x, int y, t_minirt *minirt);

int		mousedown_render(int key, int x, int y, t_minirt *minirt);
int		mouseup_render(int key, int x, int y, t_minirt *minirt);

void	events(t_minirt *minirt);
void	events_controls(t_minirt *minirt);

/*═════════════════════════════════════════════════════════════════════════════╗
║                                    RENDER                                    ║
╚═════════════════════════════════════════════════════════════════════════════*/

void	stop_minirt(t_minirt *minirt);

void	render_frame(t_minirt *minirt);
int		init_render(t_minirt *minirt);

int		calc_gradiant_color(int color_a, int color_b, float ratio);
void	put_render_to_frame(t_minirt *minirt);
void	render(t_minirt *minirt);

/*═════════════════════════════════════════════════════════════════════════════╗
║                                  RAY TRACING                                 ║
╚═════════════════════════════════════════════════════════════════════════════*/

t_ray_data	ray_color(t_minirt *minirt, t_ray ray, int depth, char	*hit);
t_color		get_background_color(t_minirt *minirt, t_ray ray);
t_viewport	init_viewport(t_minirt *minirt);
t_vec3		defocus_disk_sample(t_minirt *minirt);
t_color		compute_light(t_hit_record *hit_record, t_minirt *minirt);
t_color		get_solid_texture(t_vec3 point, double scale);
int			init_plane_light_lst(t_minirt *minirt);



// V2 

t_fcolor	compute_light_v2(t_hit_record *hit_record, t_minirt *minirt);
t_fcolor	path_trace(t_minirt *minirt, t_ray ray, int max_depth);


/*═════════════════════════════════════════════════════════════════════════════╗
║                                    EDIT MOD                                  ║
╚═════════════════════════════════════════════════════════════════════════════*/

t_object	*select_object(t_minirt *minirt, int x, int y);
void		draw_selected_object(t_minirt *minirt);

void		bilinear_upscale(t_minirt *minirt);
void		neighbor_upscale(t_minirt *minirt);
void		lanczos3_upscale(t_minirt *minirt);
void		bicubic_upscale(t_minirt *minirt);
void		no_upscaling(t_minirt *minirt);
void		no_upscaling_float(t_minirt *minirt);

int		parse_obj(char *file, t_custom_object *obj);

#endif