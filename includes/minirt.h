/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 09:40:06 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/04 16:50:41 by lilefebv         ###   ########lyon.fr   */
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
# include "ppm.h"
# include "parsing.h"

# define PI_D 3.1415926535897

# define WIN_WIDTH 1920
# define WIN_HEIGHT 1080

# define MAX_WIN_SIZE 32767

// Control window
# define CWIN_WIDTH 600
# define CWIN_HEIGHT 1080

# define MIN_SCALE 1e-3

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

void	init_animated_items(t_minirt *minirt);
void	set_dependant_values(t_minirt *minirt);

void	debug_ray(t_minirt *minirt);
void	add_material(t_minirt *minirt);

void	add_object(void *vparam);
void	rm_object(void *vparam);

/*═════════════════════════════════════════════════════════════════════════════╗
║                                   MICRORT                                    ║
╚═════════════════════════════════════════════════════════════════════════════*/

int		init_micrort(t_minirt *minirt);
void	put_micrort_to_image(t_minirt *minirt);
void	render_micrort(t_minirt *minirt);


/*═════════════════════════════════════════════════════════════════════════════╗
║                                    UTILS                                     ║
╚═════════════════════════════════════════════════════════════════════════════*/

void	print_dlight(t_dlight *light);
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

void	export_ppm_p6_minirt(const char *filename, t_minirt *minirt);

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
void	put_render_to_buff(t_minirt *minirt);
void	copy_buff_to_image(t_minirt *minirt);
void	render(t_minirt *minirt);

void	check_sample_amount(t_minirt *minirt);

/*═════════════════════════════════════════════════════════════════════════════╗
║                                  RAY TRACING                                 ║
╚═════════════════════════════════════════════════════════════════════════════*/

t_fcolor	get_background_color(t_minirt *minirt, t_ray ray);
t_viewport	init_viewport(t_minirt *minirt);
t_vec3		defocus_disk_sample(t_minirt *minirt);
t_color		compute_light(t_hit_record *hit_record, t_minirt *minirt);
t_fcolor	get_solid_texture(t_vec3 p, t_color c1, t_color c2, double scale);
t_fcolor	get_solid_texture_default(t_vec3 point, double scale);
int			init_plane_light_lst(t_minirt *minirt);

t_fcolor	get_hdr_pixel(t_hdr *hdr, int x, int y);


// V2 

t_fcolor	compute_light_v2(t_hit_record *hit_record, t_minirt *minirt);
t_fcolor	path_trace(t_minirt *minirt, t_ray ray, int max_depth);
t_fcolor	path_trace_normal(t_minirt *minirt, t_ray ray);
void		debug_path_trace(t_minirt *minirt, t_ray ray, int max_depth);
t_fcolor	add_skybox(t_minirt *minirt, t_ray *ray, t_hit_record *hit_record,
	t_ray_data data);
void		material_manager_v3(t_minirt *minirt, t_ray *ray,
		t_hit_record *hit_record, t_ray_data data);

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