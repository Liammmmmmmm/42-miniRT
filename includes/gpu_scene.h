/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gpu_scene.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 15:53:33 by lilefebv          #+#    #+#             */
/*   Updated: 2025/07/23 13:47:09 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GPU_SCENE_H
# define GPU_SCENE_H

#include <GLFW/glfw3.h>
#include "scene_structs.h"

# define SSBO_BIND_VIEWPORT 1
# define SSBO_BIND_CAMERA 2
# define SSBO_BIND_MATERIALS 3
# define SSBO_BIND_BVH 4
# define SSBO_BIND_PRIM_INDICE 5
# define SSBO_BIND_PRIM_TYPE 6
# define SSBO_BIND_SPHERES 7
# define SSBO_BIND_PLANE 8
# define SSBO_BIND_HYPERS 9
# define SSBO_BIND_CYLINDERS 10
# define SSBO_BIND_CONES 11
# define SSBO_BIND_TRIANGLES 12

/**
 * Tous les _pad sont pour l'alignement std430 et les vec3 qui prennent 16 octets en glsl
 */

typedef struct s_gpu_viewport {
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
	float	_pad2;
	float	pixel_delta_u[3];
	float	_pad3;
	float	pixel_delta_v[3];
	float	_pad4;
	float	upper_left[3];
	float	_pad5;
	float	pixel00_loc[3];
	float	_pad6;
	float	defocus_disk_u[3];
	float	_pad7;
	float	defocus_disk_v[3];
}	__attribute__((aligned(16))) t_gpu_viewport;

typedef struct s_gpu_camera
{
	float	position[3];
	float	_pad1;
	float	orientation[3];
	float	_pad2;
	float	fov;
	float	focus_dist;  
	float	defocus_angle;
}	__attribute__((aligned(16))) t_gpu_camera;


/**
 * /!\ ATTENTION A L'ORDRE DES VARIABLES (a cause de l'alignement des vec3 par le gpu)
 */

typedef struct s_gpu_sphere
{
	float	position[3];
	float	radius;
	float	color[3];
	int		material_id;
}	__attribute__((aligned(16))) t_gpu_sphere;

typedef struct s_gpu_plane
{
	float	position[3];
	int		material_id;
	float	normal[3];
	float	d;
	float	color[3];
	float   _padding;
}	__attribute__((aligned(16))) t_gpu_plane;

typedef struct s_gpu_cylinder
{
	float	position[3];
	int		material_id;
	float	orientation[3];
	float	diameter;
	float	color[3];
	float	height;
	int		material_id_top;
	int		material_id_bot;
}	__attribute__((aligned(16))) t_gpu_cylinder;

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
}	__attribute__((aligned(16))) t_gpu_hyper;

typedef struct s_gpu_cone
{
	float	position[3];
	int		material_id;
	float	orientation[3];
	float	diameter;
	float	color[3];
	float	height;
	int		material_id_top;
}	__attribute__((aligned(16))) t_gpu_cone;

typedef	struct s_gpu_vertex
{
	float	pos[3];
	float	u;
	float	normal[3];
	float	v;
}	__attribute__((aligned(16))) t_gpu_vertex;

typedef struct s_gpu_triangle
{
	t_gpu_vertex	v0;
	t_gpu_vertex	v1;
	t_gpu_vertex	v2;
	float	color[3];
	int		material_id;
	float	center[3];
	int		obj_id;
}	__attribute__((aligned(16))) t_gpu_triangle;


typedef struct s_gpu_material
{
	float	color_value[3];
	int32_t	albedo_tex_index;
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
}	__attribute__((aligned(16))) t_gpu_material;

typedef struct s_gpu_aabb
{
	float       min[3];
	float       _padding;
	float       max[3];
} __attribute__((aligned(16))) t_gpu_aabb;

typedef struct s_gpu_bvh_node
{
	t_gpu_aabb	node_bounds;
	uint32_t	left_child;
	uint32_t	right_child;
	uint32_t	first_prim;
	uint32_t	prim_count;
	uint32_t	is_leaf;
} __attribute__((aligned(16))) t_gpu_bvh_node;

typedef struct s_gpu_structs
{
	t_gpu_viewport	viewport;
	GLuint			viewport_ssbo;
	t_gpu_camera	camera;
	GLuint			camera_ssbo;
	int				spheres_am;
	t_gpu_sphere	*spheres;
	GLuint			spheres_ssbo;
	t_gpu_material	*mat;
	int				mat_am;
	GLuint			mat_ssbo;
	t_gpu_bvh_node	*bvh_node;
	int				bvh_node_am;
	GLuint			bvh_node_ssbo;
	int				prim_indice_am;
	GLuint			prim_indice_ssbo;
	GLuint			planes_ssbo;
	int				planes_am;
	t_gpu_plane		*planes;
	uint32_t		*prim_type;
	uint32_t		*prim_indice;
	int				cylinders_am;
	t_gpu_cylinder	*cylinders;
	GLuint			cylinders_ssbo;
	int				cones_am;
	t_gpu_cone		*cones;
	GLuint			cones_ssbo;
	int				hypers_am;
	t_gpu_hyper		*hypers;
	GLuint			hypers_ssbo;
	int				triangles_am;
	t_gpu_triangle	*triangles;
	GLuint			triangles_ssbo;
}	t_gpu_structs;

int	convert_scene(t_scene *scene, t_viewport *viewport, t_gpu_structs *gpu_structs);

#endif
