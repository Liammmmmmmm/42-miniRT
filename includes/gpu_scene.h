/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gpu_scene.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 15:53:33 by lilefebv          #+#    #+#             */
/*   Updated: 2025/08/21 22:39:11 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GPU_SCENE_H
# define GPU_SCENE_H

# include <GLFW/glfw3.h>
# include "scene_structs.h"

# define SSBO_BIND_LIGHTS 2
# define SSBO_BIND_MATERIALS 3
# define SSBO_BIND_BVH 4
# define SSBO_BIND_PRIM_TYPE_INDICE 5
# define SSBO_BIND_PRIMITIVE 6
# define SSBO_BIND_PLANE 7
# define SSBO_BIND_HYPERS 8
# define SSBO_BIND_TRIANGLES 9
# define SSBO_BIND_CHECKERS 10
# define SSBO_BIND_IMAGES 11
# define SSBO_BIND_IMAGES_STREAM 12
# define SSBO_BIND_TEX_TYPE_INDICE 13
# define SSBO_BIND_PHOTON_INDICE 14
# define SSBO_BIND_PHOTON 15
# define SSBO_BIND_CELL 16

typedef struct s_gpu_viewport
{
	float	focal_length;
	float	height;
	float	width;
	int		render_w;
	int		render_h;
	float	gamma;
	float	defocus_radius;
	int		max_bounces;
	float	u[3];
	float	ior_global;
	float	v[3];
	float	pixel_delta_u[3];
	float	pixel_delta_v[3];
	float	upper_left[3];
	float	pixel00_loc[3];
	float	defocus_disk_u[3];
	float	defocus_disk_v[3];
} __attribute__((aligned(16)))	t_gpu_viewport;

typedef struct s_gpu_amb_light
{
	float	ratio;
	float	skybox_color[3];
	int		skybox_tex_index;
} __attribute__((aligned(16)))	t_gpu_amb_light;

typedef struct s_gpu_camera
{
	float	position[3];
	float	orientation[3];
	float	fov;
	float	focus_dist;
	float	defocus_angle;
} __attribute__((aligned(16)))	t_gpu_camera;

typedef struct s_gpu_cell
{
	uint32_t	start_index;
	uint32_t	count;
}	t_gpu_cell;

typedef struct s_gpu_photon
{
	float	color[4];
	float	position[4];
} __attribute__((aligned(16)))	t_gpu_photon;

typedef struct s_gpu_plane
{
	float	position[3];
	int		material_id;
	float	normal[3];
	float	d;
	float	color[3];
	float	_padding;
} __attribute__((aligned(16)))	t_gpu_plane;

typedef struct s_gpu_primitive
{
	float	position[3];
	int		material_id;
	float	orientation[3];
	int		material_id_top;
	float	color[3];
	int		material_id_bot;
	float	radius;
	float	height;
} __attribute__((aligned(16)))	t_gpu_primitive;

typedef struct s_gpu_hyper
{
	float	position[3];
	int		material_id;
	float	orientation[3];
	float	height;
	float	color[3];
	float	a;
	float	b;
	float	c;
	float	shape;
} __attribute__((aligned(16)))	t_gpu_hyper;

typedef struct s_gpu_light
{
	float	pos[3];
	int		type;
	float	color[3];
	float	brightness;
	float	radius;
	t_uint	shadow_sample;
} __attribute__((aligned(16)))	t_gpu_light;

typedef struct s_gpu_vertex
{
	float	pos[3];
	float	u;
	float	normal[3];
	float	v;
} __attribute__((aligned(16)))	t_gpu_vertex;

typedef struct s_gpu_triangle
{
	t_gpu_vertex	v0;
	t_gpu_vertex	v1;
	t_gpu_vertex	v2;
	float			color[3];
	int				material_id;
	float			center[3];
	int				obj_id;
} __attribute__((aligned(16)))	t_gpu_triangle;

typedef struct s_gpu_material
{
	float	color_value[3];
	int32_t	color_tex_index;
	float	metallic_value;
	int32_t	metallic_tex_index;
	float	roughness_value;
	int32_t	roughness_tex_index;
	float	ior;
	float	transmission_value;
	int32_t	transmission_tex_index;
	float	ao_value;
	int32_t	ao_tex_index;
	float	emission_strength;
	int32_t	emission_strength_tex_index;
	float	scale;
	float	emission_color[3];
	int32_t	emission_color_tex_index;
	int32_t	normal_tex_index;
	float	normal_intensity;
} __attribute__((aligned(16)))	t_gpu_material;

typedef struct s_gpu_aabb
{
	float	min[3];
	float	_padding;
	float	max[3];
} __attribute__((aligned(16)))	t_gpu_aabb;

typedef struct s_gpu_bvh_node
{
	t_gpu_aabb	node_bounds;
	uint32_t	left_child;
	uint32_t	right_child;
	uint32_t	first_prim;
	uint32_t	prim_count;
	uint32_t	is_leaf;
} __attribute__((aligned(16)))	t_gpu_bvh_node;

typedef struct s_gpu_checker
{
	float	c1[3];
	float	scale;
	float	c2[3];
} __attribute__((aligned(16)))	t_gpu_checker;

typedef struct s_gpu_tex_data
{
	uint32_t	width;
	uint32_t	height;
	uint32_t	offset;
	float		exposure;
} __attribute__((aligned(16)))	t_gpu_tex_data;

typedef struct s_type_indice
{
	uint32_t	type;
	uint32_t	indice;
} __attribute__((aligned(8)))	t_type_indice;

typedef struct s_gpu_structs
{
	t_gpu_viewport	viewport;
	t_gpu_camera	camera;
	t_gpu_amb_light	ambiant;
	GLuint			primitive_ssbo;
	int				primitive_am;
	t_gpu_primitive	*primitives;
	int				spheres_am;
	t_gpu_material	*mat;
	int				mat_am;
	GLuint			mat_ssbo;
	t_gpu_bvh_node	*bvh_node;
	int				bvh_node_am;
	GLuint			bvh_node_ssbo;
	int				prim_indice_am;
	GLuint			planes_ssbo;
	int				planes_am;
	t_gpu_plane		*planes;
	t_type_indice	*prim_types_indices;
	GLuint			prim_types_indices_ssbo;
	int				cylinders_am;
	int				cones_am;
	int				hypers_am;
	t_gpu_hyper		*hypers;
	GLuint			hypers_ssbo;
	int				triangles_am;
	t_gpu_triangle	*triangles;
	GLuint			triangles_ssbo;
	int				lights_am;
	t_gpu_light		*lights;
	GLuint			lights_ssbo;
	t_gpu_cell		*cells;
	GLuint			cells_ssbo;
	uint32_t		cells_am;
	t_gpu_photon	*photons;
	GLuint			photons_ssbo;
	uint32_t		photon_am;
	float			cell_size;
	int				table_size;
	float			grid_world_min[3];
	float			grid_world_max[3];
	uint32_t		*photon_indices;
	GLuint			photon_indices_ssbo;
}	t_gpu_structs;

typedef struct s_gpu_textures
{
	int				checkers_am;
	t_gpu_checker	*checkers;
	GLuint			checkers_ssbo;
	int				images_am;
	t_gpu_tex_data	*images;
	GLuint			images_ssbo;
	uint32_t		total_pixel_images;
	float			*images_stream;
	GLuint			images_stream_ssbo;
	t_type_indice	*textures_types_indices;
	GLuint			textures_types_indices_ssbo;
	GLuint			cdf_ci_texture2d;
	GLuint			pdf_joint_texture2d;
	GLuint			cdf_mi_texture2d;
}	t_gpu_textures;

int	convert_textures_init(t_scene *scene, t_gpu_textures *gpu_tex);

#endif
