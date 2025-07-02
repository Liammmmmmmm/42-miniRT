/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gpu_scene.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 15:53:33 by lilefebv          #+#    #+#             */
/*   Updated: 2025/07/02 15:03:43 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef GPU_SCENE_H
# define GPU_SCENE_H

#include <GLFW/glfw3.h>
#include "scene_structs.h"

# define SSBO_BIND_VIEWPORT 1
# define SSBO_BIND_CAMERA 2
# define SSBO_BIND_SPHERES 4

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
	float	_pad1;
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


typedef struct s_gpu_structs
{
	t_gpu_viewport	viewport;
	GLuint			viewport_ssbo;
	t_gpu_camera	camera;
	GLuint			camera_ssbo;
	int				spheres_am;
	t_gpu_sphere	*spheres;
	GLuint			spheres_ssbo;
}	t_gpu_structs;

int	convert_scene(t_scene *scene, t_viewport *viewport, t_gpu_structs *gpu_structs);

#endif
