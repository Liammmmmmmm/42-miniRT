/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 09:39:37 by lilefebv          #+#    #+#             */
/*   Updated: 2025/07/31 16:20:33 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

# include "libft.h"
# include "mlx_base.h"
# include "mlx_components.h"
# include "font.h"
# include "hdr_parsing.h"
# include "basic_structs.h"
# include "gpu_struct.h"
# include "scene_structs.h"

typedef unsigned char t_bool;
typedef unsigned char t_uchar;

typedef	enum	e_mat_type
{
	DEFFAULT,
	EMISSIVE
}	t_mat_type;

typedef struct s_lcolor
{
	__uint64_t	r;
	__uint64_t	g;
	__uint64_t	b;
}	t_lcolor;

typedef struct s_lsc_point
{
	t_uint		x;
	t_uint		y;
	t_lcolor	color;
}	t_lsc_point;

typedef struct s_key_map
{
	int from;
	int to;
} t_key_map;

/**
 * @struct s_point
 * @brief Structure representing a point in 2D space with a color.
 * 
 * @param x X-coordinate of the point on the screen.
 * @param y Y-coordinate of the point on the screen.
 * @param z Depth of the point.
 * @param color Color of the point.
 */
typedef struct s_sc_point
{
	t_uint	x;		/* X-coordinate of the point on the screen. */
	t_uint	y;		/* Y-coordinate of the point on the screen. */
	float	z;		/* Depth of the point. */
	t_color	color;	/* Color of the point. */
}	t_sc_point;

typedef struct s_calc_trigo
{
	double	sin_yaw;
	double	cos_yaw;
	double	sin_pitch;
	double	cos_pitch;
	double	sin_roll;
	double	cos_roll;
}	t_calc_trigo;

typedef struct s_ray
{
	t_vec3	orig;	/* The origin of the ray. */
	t_vec3	dir;	/* The direction of the ray. */
}	t_ray;	/*	Add more section for future (length_squared for optimisation)	*/

typedef struct s_interpolate_pixel_bicubic
{
	float	v[4];
	float	wx[4];
	float	wy[4];
	int		x[4];
	int		y[4];
}	t_interpolate_pixel_bicubic;

typedef struct s_range
{
	int	start;
	int	end;
	int	median;
}	t_range;

typedef struct s_hash_entry
{
    uint32_t hash;
    uint32_t photon_index;
}   t_hash_entry;

typedef struct s_quadratic
{
	double	a;
	double	b;
	double	c;
	double	disc;
	double	t0;
	double	t1;
	double	t_hit;
	double	t_hit2;
	double  dd;
	double  oo;
}	t_quadratic;

typedef struct s_moller
{
	t_vec3	e1;
	t_vec3	e2;
	t_vec3	h;
	t_vec3	s;
	t_vec3	q;
	float	f;
	float	u;
	float	v;
	float	t;
}	t_moller;

typedef struct s_ray_data
{
	t_fcolor	*power;
	t_fcolor	*accumulation;
}	t_ray_data;

typedef struct s_face
{
	size_t	*v_idx;
	size_t	*vt_idx;
	size_t	*vn_idx;
	size_t	vertex_count;
}	t_face;

typedef struct s_obj_temp
{
	t_vec3	*v;
	t_vec3	*vt;
	t_vec3	*vn;
	t_face	*face;
	size_t	v_count;
	size_t	vn_count;
	size_t	vt_count;
	size_t	face_count;
	char	*name;
}	t_obj_temp;

typedef enum e_upscalings
{
	NO_UPSCALING,
	BILINEAR,
	BICUBIC
}	t_upscalings;

typedef struct s_face_idx_triplet
{
	int	v_idx;
	int	vt_idx;
	int	vn_idx;
}	t_face_idx_triplet;

typedef struct s_parser_temp_data
{
	t_vector	temp_positions;
	t_vector	temp_normals;
	t_vector	temp_uvs;
	t_vector	temp_faces;
	char		*object_name;
}	t_parser_temp_data;

typedef struct s_cache_entry
{
	t_face_idx_triplet		key;
	uint32_t				value;
	struct s_cache_entry	*next;
}	t_cache_entry;

typedef struct s_mesh_build_data
{
	t_vector			*vertices;
	t_vector			*indices;
	t_cache_entry		**cache;
	size_t				map_size;
}	t_mesh_build_data;

typedef struct s_vec2
{
	double x;
	double y;
}	t_vec2;

typedef struct s_vertex_indexed
{
	t_vec3	pos;
	t_vec3	normal;
	t_vec2	uv;
}	t_vertex_indexed;

typedef struct s_mesh
{
	t_vertex_indexed	*vertices;
	uint32_t			*indices;
	size_t				vertex_count;
	size_t				index_count;
	char				*name;
}	t_mesh;

typedef	struct s_hit_record
{
	t_object	*obj;
	t_vec3		point;
	t_vec3		normal;
	double		t;
	t_mat		*mat;
	char		front_face;
	t_fcolor	color;
	double		u;
	double		v;
	t_obj_part	part;
}	t_hit_record;

typedef struct s_hit_register_data
{
	t_hit_record	hit_record;
	char			is_light;
	t_ray			*ray;
	int				depth;			
}	t_hit_register_data;

typedef struct s_axis
{
	double	orig;
	double	dir;
	double	min;
	double	max;
}	t_axis;

typedef struct s_mlx
{
	void	*mlx;
	void	*render_win;
	void	*controls_win;
	t_img	img;
	t_img	img_controls;
}	t_mlx;

typedef struct s_screen
{
	int			*render;
	t_fcolor	*float_render;
	int			sample;
	int			sample_total_anim;
	int			last_sample_am;
	ssize_t		last_sample_time;
	ssize_t		first_sample_time;
	int			spp; // sample per pixel
	t_bool		start_render;
	t_bool		pause_render;
}	t_screen;

typedef struct s_stats
{
	int	frame;
}	t_stats;

typedef struct s_keydown
{
	t_bool	lmb;
	t_bool	mmb;
	t_bool	rmb;
	t_bool	lshift;
	t_bool	rshift;
	t_bool	lctrl;
	t_bool	rctrl;
}	t_keydown;

typedef struct s_edited_val
{
	int		focus_dist;
	int		defocus_angle;
	int		fov;
	int		gamma;
	char	debug;
	int		upscaling_ratio;
	int		upscaling_selected;
}	t_edited_val;

typedef struct s_ui_infos
{
	uint8_t		tab_selected;
	t_color		picker_no_color;
	t_object	*selected_object;
	int			objects_scroll_offset;
	t_mat		*selected_material;
	int			materials_scroll_offset;
	void		*add_object;
}	t_ui_infos;

typedef struct s_movements
{
	t_bool	forward;
	t_bool	back;
	t_bool	right;
	t_bool	left;
	t_bool	up;
	t_bool	down;
	t_bool	last_frame_is_moving;
	t_bool	mouse;
	int		speed_modifier;
}	t_movements;

typedef struct s_controls
{
	t_keydown		keydown;
	t_movements		movements;
	t_uint			mlxc;
	t_uint			mlyc;
	t_uint			mlxr;
	t_uint			mlyr;
	int				mlxc_s;
	int				mlyc_s;
	t_uchar			font8b[96][5];
	int				open_controls;
	int				nb_buttons;
	int				btn_clicked;
	t_button		*buttons;
	t_edited_val	values;
	int				nb_sliders;
	t_int_slider	*sliders;
	int				nb_color_picker;
	t_color_picker	color_picker[4];
	int				nb_dropdown;
	t_dropdown		*dropdown;
	int				nb_float_input;
	t_float_input	*float_input;
	int				nb_text_input;
	t_text_input	*text_input;
	t_ttf			font[1];
	t_ui_infos		ui_infos;
	int				selected_x;
	int				selected_y;
	t_vec3			traced_ray[11];
	double			max_bounces;
	double			res_render_x;
	double			res_render_y;
	void			*selected_upscaling;
}	t_controls;

typedef struct s_obj_anim
{
	t_objects	obj;
	t_uint		obj_num;
	t_uint		frames;
	t_vec3		*points;
	t_vec3		*orientations;
}	t_obj_anim;

typedef struct s_animation
{
	t_bool		enabled;
	t_uint		nb_objects;
	t_obj_anim	*objects;
	t_uint		frame_i;
	t_uint		frames;
}	t_animation;

typedef struct s_options
{
	t_bool		no_display;
	t_bool		auto_export;
	int			max_samples;
	char		*output_dir;
	t_animation	anim;
	t_bool		sga;
	t_bool		cpu;
}	t_options;

typedef struct s_micrort
{
	t_fcolor	*render;
	t_camera	camera;
	t_viewport	viewport;
	t_sphere	sphere;
	t_uint		max_sample;
	t_uint		sample;
}	t_micrort;

typedef struct s_minirt
{
	t_scene			scene;
	t_mlx			mlx;
	t_screen		screen;
	t_stats			stats;
	t_viewport		viewport;
	t_controls		controls;
	t_options		options;
	t_micrort		micrort;
	t_shader_data	shaders_data;
	char			render_mode;
}	t_minirt;

typedef struct s_upscale_data
{
	t_minirt	*minirt;
	int			x;
	int			y;
	int			sw;
	int			sh;
	int			dw;
	int			dh;
	int			divide;
	float		scaleX;
	float		scaleY;
}	t_upscale_data;

typedef struct s_btn_param
{
	t_minirt	*minirt;
	int			action;
}	t_btn_param;

#endif