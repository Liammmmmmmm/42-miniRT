/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 09:39:37 by lilefebv          #+#    #+#             */
/*   Updated: 2025/05/12 09:19:03 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

# include "libft.h"
# include "mlx_base.h"
# include "mlx_components.h"
# include "font.h"

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

typedef struct s_vec3
{
	double	x;
	double	y;
	double	z;
}	t_vec3;

typedef	struct s_interval
{
	double	min;
	double	max;
}	t_interval;

typedef struct s_ray
{
	t_vec3	orig;	/* The origin of the ray. */
	t_vec3	dir;	/* The direction of the ray. */
}	t_ray;	/*	Add more section for future (length_squared for optimisation)	*/

typedef enum e_tex_type
{
	IMAGE,
	COLOR,
}	t_tex_type;

typedef struct s_tex_img
{
	uint32_t	width;
	uint32_t	height;
	t_color		*pixel_data;
}	t_tex_img;

typedef struct s_tex
{
	char		name[21];
	t_tex_type	type;
	t_tex_img	img;
}	t_tex;

typedef struct s_mat
{
	char	name[21];
	t_tex	*color_tex;
	t_color	color_value;
	t_tex	*metallic_tex;
	double	metallic_value;
	t_tex	*roughness_tex;
	double	roughness_value;
	double	ior;
	double	transmission;
	t_tex	*ao_tex;
	double	ao_value;
	double	emission_strength;
	t_color	emission_color;
	double	scale;
	t_tex	*normal;
	double	normal_intensity;
}	t_mat;

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

typedef struct s_amb_light
{
	double	ratio;
	t_color	color;
	t_bool	have_skybox;
	t_color	skybox_c;
	t_tex	*skybox_t;
}	t_amb_light;

typedef struct s_ray_data
{
	t_fcolor	*power;
	t_fcolor	*accumulation;
}	t_ray_data;

typedef struct s_camera
{
	t_vec3			position;
	t_vec3			orientation;
	unsigned char	fov;
    double			focus_dist;  
	double			defocus_angle;
}	t_camera;

typedef struct s_light
{
	t_vec3	position;
	double	brightness;
	t_color	color;
}	t_light;

typedef struct s_hyperboloid
{
	t_vec3	position;
	t_vec3	orientation;
	double	a;
	double	b;
	double	c;
	double	shape;
	t_mat	*material;
	t_color	color;
	double	height;
}	t_hyperboloid;

typedef struct s_sphere
{
	t_vec3	position;
	double	diameter;
	t_mat	*material;
	t_color	color;
	double	sqrt_radius;
	double	radius;
}	t_sphere;

typedef struct s_plane
{
	t_vec3	position;
	t_vec3	normal;
	t_color	color;
	t_mat	*material;
}	t_plane;

typedef struct s_cone
{
	t_vec3	position;
	t_vec3	orientation;
	double	diameter;
	double	height;
	t_mat	*material;
	t_mat	*material_top;
	t_color	color;
}	t_cone;

typedef struct s_cylinder
{
	t_vec3	position;
	t_vec3	orientation;
	double	diameter;
	double	height;
	t_mat	*material;
	t_mat	*material_top;
	t_mat	*material_bot;
	t_color	color;
}	t_cylinder;

typedef struct s_vertex
{
	t_vec3	pos;
	t_vec3	normal;
	double	u;
	double	v;
	double	angle;
}	t_vertex;

typedef struct s_triangle
{
	t_vertex	v0;
	t_vertex	v1;
	t_vertex	v2;
	t_vec3		center;
	t_mat		*material;
	t_color		color;
}	t_triangle;

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

typedef enum e_objects
{
	NULL_OBJ,
	TEXTURE,
	MATERIAL,
	AMBIANT_LIGHT,
	CAMERA,
	LIGHT,
	SPHERE,
	PLANE,
	CYLINDER,
	CONE,
	HYPERBOLOID,
	TRIANGLE,
	CUSTOM,
	WINDOW
}	t_objects;

typedef struct s_object
{
	void		*object;
	t_objects	type;
}	t_object;

typedef struct s_custom_object
{
	char		*name;
	t_triangle	*triangles;
	t_object	*obj_list;
	size_t		triangle_count;
	t_vec3		position;
	t_vec3		orientation;
	t_vec3		scale;
	t_mat		*material;
	t_color		color;
	int			index;
}	t_custom_object;

typedef enum e_obj_part
{
	DEFAULT,
	TOP_CAP,
	BOTTOM_CAP
}	t_obj_part;

typedef	struct s_hit_record
{
	t_object	*obj;
	t_vec3		point;
	t_vec3		normal;
	double		t;
	t_mat		*mat;
	char		front_face;
	t_color		color;
	double		u;
	double		v;
	t_obj_part	part;
}	t_hit_record;

typedef struct s_obj_lst
{
	t_object	**light_lst;
	int			light_nb;
	t_object	**plane_lst;
	int			plane_nb;
}	t_obj_lst;

typedef struct s_aabb
{
	t_vec3	min;
	t_vec3	max;
}	t_aabb;

typedef struct s_bvh_node
{
	t_aabb		node_bounds;
	uint32_t	left_child;
	uint32_t	right_child;
	uint32_t	first_prim;
	uint32_t	prim_count;
	char		is_leaf;
}	t_bvh_node;

typedef struct s_bvh
{
	uint32_t	*prim_indices;
	t_bvh_node	*bvh_nodes;
	uint16_t	bvh_nodes_used;	
	t_object	**obj_list;
	uint32_t	*closest_t;
	uint32_t	node_index;
	char		valid; 
	uint32_t	size;
	uint32_t	actual;
	int			normal_mode;
}	t_bvh;

typedef struct s_axis
{
	double	orig;
	double	dir;
	double	min;
	double	max;
}	t_axis;

typedef struct s_scene
{
	t_tex		*textures;
	int			tex_amount;
	t_mat		*materials;
	int			mat_amount;
	t_object	*elements;	// Liste des tous les objets de la scene
	t_obj_lst	obj_lst;
	int			el_amount;	// Nombre d'elements dans la liste
	t_amb_light	amb_light;
	t_camera	camera;
	double		ior_all;
	t_bvh		bvh;
	int			win_width;
	int			win_height;
}	t_scene;

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
	t_lsc_point	*render;
	t_fcolor	*float_render;
	int			sample;
	int			last_sample_am;
	ssize_t		last_sample_time;
	int			spp; // sample per pixel
	t_bool		start_render;
	t_bool		pause_render;
}	t_screen;

typedef struct s_stats
{
	int	frame;
}	t_stats;

typedef struct s_viewport
{
	float	focal_length;
	float	height;
	float	width;
	int		render_w;
	int		render_h;
	float	gamma;
	t_vec3	u;
	t_vec3	v;
	t_vec3	pixel_delta_u;
	t_vec3	pixel_delta_v;
	t_vec3	upper_left;
	t_vec3	pixel00_loc;
	double	defocus_radius;
	t_vec3	defocus_disk_u;
    t_vec3	defocus_disk_v;
}	t_viewport;

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
}	t_ui_infos;

typedef struct s_controls
{
	t_keydown		keydown;
	t_uint			mlxc;
	t_uint			mlyc;
	t_uint			mlxr;
	t_uint			mlyr;
	t_uchar			font8b[96][5];
	int				open_controls;
	int				nb_buttons;
	int				btn_clicked;
	t_button		*buttons;
	t_edited_val	values;
	int				nb_sliders;
	t_int_slider	*sliders;
	int				nb_color_picker;
	t_color_picker	color_picker[1];
	int				nb_dropdown;
	t_dropdown		*dropdown;
	int				nb_float_input;
	t_float_input	*float_input;
	t_ttf			font[1];
	t_ui_infos		ui_infos;
	int				selected_x;
	int				selected_y;
	t_vec3			traced_ray[11];
}	t_controls;

typedef struct s_minirt
{
	t_scene		scene;
	t_mlx		mlx;
	t_screen	screen;
	t_stats		stats;
	t_viewport	viewport;
	t_controls	controls;
	t_custom_object	obj;
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