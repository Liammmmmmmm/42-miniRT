/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gpu_scene.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 16:03:21 by lilefebv          #+#    #+#             */
/*   Updated: 2025/07/02 14:47:56 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "gpu.h"
#include "gpu_scene.h"
#include <stdio.h>

void convert_spheres(t_scene *cpu_scene, t_gpu_sphere *gpu)
{
	int	e;
	int	i;

	e = -1;
	i = 0;
	while (++e < cpu_scene->el_amount)
	{
		if (cpu_scene->elements[e].type != SPHERE)
			continue ;
		gpu[i].position[0] = (float)((t_sphere *)cpu_scene->elements[e].object)->position.x;
		gpu[i].position[1] = (float)((t_sphere *)cpu_scene->elements[e].object)->position.y;
		gpu[i].position[2] = (float)((t_sphere *)cpu_scene->elements[e].object)->position.z;
		gpu[i].color[0] = (float)((t_sphere *)cpu_scene->elements[e].object)->color.r / 255.0;
		gpu[i].color[1] = (float)((t_sphere *)cpu_scene->elements[e].object)->color.g / 255.0;
		gpu[i].color[2] = (float)((t_sphere *)cpu_scene->elements[e].object)->color.b / 255.0;
		gpu[i].radius = (float)((t_sphere *)cpu_scene->elements[e].object)->radius;
		printf("SPHERE %d : %f\n", i, gpu[i].radius);
		gpu[i].material_id = -1;
		i++;
	}
}

void	convert_camera(t_scene *cpu_scene, t_gpu_camera *cam)
{
	cam->position[0] = (float)cpu_scene->camera.position.x;
	cam->position[1] = (float)cpu_scene->camera.position.y;
	cam->position[2] = (float)cpu_scene->camera.position.z;
	cam->orientation[0] = (float)cpu_scene->camera.orientation.x;
	cam->orientation[1] = (float)cpu_scene->camera.orientation.y;
	cam->orientation[2] = (float)cpu_scene->camera.orientation.z;
	cam->fov = (float)cpu_scene->camera.fov;
	cam->defocus_angle = (float)cpu_scene->camera.defocus_angle;
	cam->focus_dist = (float)cpu_scene->camera.focus_dist;
}

void	vec3_to_float3(t_vec3 *vec, float *tab)
{
	tab[0] = vec->x;
	tab[1] = vec->y;
	tab[2] = vec->z;
}

void	convert_viewport(t_gpu_viewport *dst, t_viewport *src)
{
	dst->focal_length = src->focal_length;
	dst->height = src->height;
	dst->width = src->width;
	dst->render_w = src->render_w;
	dst->render_h = src->render_h;
	dst->gamma = (float)src->gamma;
	dst->defocus_radius = (float)src->defocus_radius;
	dst->max_bounces = src->max_bounces;
	vec3_to_float3(&src->u, dst->u);
	vec3_to_float3(&src->v, dst->v);
	vec3_to_float3(&src->pixel_delta_u, dst->pixel_delta_u);
	vec3_to_float3(&src->pixel_delta_v, dst->pixel_delta_v);
	vec3_to_float3(&src->upper_left, dst->upper_left);
	vec3_to_float3(&src->pixel00_loc, dst->pixel00_loc);
	vec3_to_float3(&src->defocus_disk_u, dst->defocus_disk_u);
	vec3_to_float3(&src->defocus_disk_v, dst->defocus_disk_v);
}

void	count_spheres(t_scene *scene, t_gpu_structs *gpu_structs)
{
	int	i;

	i = -1;
	while (++i < scene->el_amount)
	{
		if (scene->elements[i].type == SPHERE)
			gpu_structs->spheres_am++;		
	}
}

void	create_ssbo(GLuint *ssbo, size_t size, void *data, long bind)
{
	glGenBuffers(1, ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, *ssbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, GL_STATIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bind, *ssbo);
}

int	convert_scene(t_scene *scene, t_viewport *viewport, t_gpu_structs *gpu_structs)
{
	convert_viewport(&gpu_structs->viewport, viewport);
	create_ssbo(&gpu_structs->viewport_ssbo, sizeof(t_gpu_viewport), &gpu_structs->viewport, SSBO_BIND_VIEWPORT);
	convert_camera(scene, &gpu_structs->camera);
	create_ssbo(&gpu_structs->camera_ssbo, sizeof(t_gpu_camera), &gpu_structs->camera, SSBO_BIND_CAMERA);
	gpu_structs->spheres_am = 0;
	count_spheres(scene, gpu_structs);
	gpu_structs->spheres = ft_calloc(gpu_structs->spheres_am, sizeof(t_gpu_sphere));
	if (!gpu_structs->spheres)
		return (-1);
	convert_spheres(scene, gpu_structs->spheres);
	create_ssbo(&gpu_structs->spheres_ssbo, sizeof(t_gpu_sphere) * gpu_structs->spheres_am, gpu_structs->spheres, SSBO_BIND_SPHERES);

	printf("SCENE TRANSFORMEE GPU\n");
	printf("sp %f %f %f\n", gpu_structs->spheres[0].position[0], gpu_structs->spheres[0].position[1], gpu_structs->spheres[0].position[2]);

	return (0);
}
