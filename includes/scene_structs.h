/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_structs.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 17:03:30 by lilefebv          #+#    #+#             */
/*   Updated: 2025/07/31 14:37:05 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCENE_STRUCTS_H
# define SCENE_STRUCTS_H

# include "basic_structs.h"
# include "mlx_base.h"
# include "hdr_parsing.h"

typedef enum e_tex_type
{
	IMAGE,
	HDR,
	COLOR,
	CHECKER_LOCAL,
	CHECKER_GLOBAL,
}	t_tex_type;

typedef enum e_obj_part
{
	DEFAULT,
	TOP_CAP,
	BOTTOM_CAP
}	t_obj_part;

typedef enum e_objects
{
	NULL_OBJ,
	TEXTURE,
	MATERIAL,
	AMBIANT_LIGHT,
	CAMERA,
	LIGHT,
	DIRECTIONAL_LIGHT,
	SPHERE,
	PLANE,
	CYLINDER,
	CONE,
	HYPERBOLOID,
	TRIANGLE,
	CUSTOM,
	WINDOW
}	t_objects;

typedef struct s_aabb
{
	t_vec3	min;
	t_vec3	max;
}	t_aabb;

typedef struct s_tex_img
{
	uint32_t	width;
	uint32_t	height;
	t_rgba		*rgba;
}	t_tex_img;

typedef struct s_checker
{
	double	scale;
	t_color	c1;
	t_color	c2;
}	t_checker;

typedef struct s_tex
{
	char		name[21];
	t_tex_type	type;
	t_tex_img	img;
	t_hdr		hdr;
	t_checker	checker;
}	t_tex;

typedef struct s_mat
{
	char		name[21];
	t_tex		*color_tex;
	t_color		color_value;
	t_tex		*metallic_tex;
	double		metallic_value;
	t_tex		*roughness_tex;
	double		roughness_value;
	double		ior;
	double		transmission_value;
	t_tex		*ao_tex;
	t_tex		*transmission_tex;
	double		ao_value;
	double		emission_strength;
	t_tex		*emission_strength_tex;
	t_fcolor	emission_color;
	t_color		emission_color_tmp;
	t_tex		*emission_color_tex;
	double		scale;
	t_tex		*normal;
	double		normal_intensity;
}	t_mat;

typedef struct s_amb_light
{
	double	ratio;
	t_color	skybox_c;
	t_tex	*skybox_t;
	void	*last_calc_importance;
	double	*gray_scale;
	double	*raw_average;
	float	*pdf_joint;
	double	*importance_map;
	double	*pdf_marginal;
	float	*cdf_marginal_inverse;
	double	*pdf_conditional;
	float	*cdf_conditional_inverse;
	double	*DEBUG_INVERSE_SAMPLING;
	double	*cdf_w;
	double	*cdf_h;
	double	column_average;
}	t_amb_light;

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
	int		shadow_sample;
	float	radius;
	double	shadow_factor;
}	t_light;

typedef struct s_dlight
{
	t_vec3	orientation;
	double	brightness;
	t_color	color;
}	t_dlight;

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
}	t_vertex;

typedef struct s_triangle
{
	t_vertex				v0;
	t_vertex				v1;
	t_vertex				v2;
	t_vec3					center;
	t_mat					*material;
	t_color					color;
	struct s_custom_object	*obj;
}	t_triangle;

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
	t_vec3		prev_position;
	t_vec3		orientation;
	t_vec3		prev_orientation;
	t_vec3		scale;
	t_vec3		prev_scale;
	t_mat		*material;
	t_color		color;
	int			index;
	t_aabb		aabb;
}	t_custom_object;

typedef struct s_obj_lst
{
	t_object	**light_lst;
	int			light_nb;
	t_object	**plane_lst;
	int			plane_nb;
}	t_obj_lst;

typedef struct s_bvh_task
{
	uint32_t	start;
	uint32_t	count;
	uint32_t	parent;
	char		is_left;
}	t_bvh_task;

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
	uint32_t	bvh_nodes_used;
	t_object	**obj_list;
	uint32_t	*closest_t;
	uint32_t	node_index;
	char		valid; 
	uint32_t	size;
	uint32_t	actual;
	int			normal_mode;
	t_vector	task_stack;
	char		*render_mode;
}	t_bvh;

typedef struct s_photon
{
    t_vec3		position;
    t_fcolor	power;
} t_photon;

typedef struct s_kd_node
{
	t_vec3	aabb_min;
	t_vec3	aabb_max;
	int		axis;
	union
	{
		struct
		{
			struct s_kd_node	*left;
			struct s_kd_node	*right;
			float				split_position;
		} internal;
		struct
		{
			int	photon_start_idx;
			int	photon_count;
		} leaf;
	} data;
}	t_kd_node;


typedef struct s_kd_tree
{
	t_kd_node	*root;
	t_photon	*photons;
	t_kd_node   *nodes;
	size_t		photon_count;
	size_t		node_count;
} t_kd_tree;

typedef struct s_knn_result
{
    t_photon    *photon;
    double      dist_sq; // Distance au carré
	double      dist; 
}   t_knn_result;

typedef struct s_knn_search
{
    t_knn_result    *results;     // Tableau des k meilleurs résultats
    size_t          k;            // Le nombre de voisins à chercher
    size_t          count;        // Le nombre de voisins trouvés pour l'instant
    size_t          farthest_idx; // L'index du voisin le plus éloigné dans la liste
}   t_knn_search;

typedef struct s_kd_build_task
{
	int				start;
	int				end;
	int				depth;
	t_kd_node		**parent_link;
	t_vec3			aabb_min;
	t_vec3			aabb_max;
}	t_kd_build_task;

typedef struct s_kd_task_data
{
	t_kd_build_task	task;
	t_kd_build_task	*stack;
	int				*stack_top;
}	t_kd_task_data;

typedef struct s_scene
{
	char		name[20];
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
	t_bool		build_bvh;
	int			win_width;
	int			win_height;
	int			render_width;
	int			render_height;
	t_bool		have_win_el;
	t_kd_tree	photon_map;
}	t_scene;

typedef struct s_viewport
{
	float	focal_length;
	float	height;
	float	width;
	int		render_w;
	int		render_h;
	double	gamma;
	t_vec3	u;
	t_vec3	v;
	t_vec3	pixel_delta_u;
	t_vec3	pixel_delta_v;
	t_vec3	upper_left;
	t_vec3	pixel00_loc;
	double	defocus_radius;
	t_vec3	defocus_disk_u;
	t_vec3	defocus_disk_v;
	int		max_bounces;
	int		*depth_buffer;
}	t_viewport;

#endif