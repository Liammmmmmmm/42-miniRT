/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gpu_scene.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 16:03:21 by lilefebv          #+#    #+#             */
/*   Updated: 2025/07/22 18:28:41 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "gpu.h"
#include "gpu_scene.h"
#include <stdio.h>

void	vec3_to_float3(t_vec3 *vec, float *tab)
{
	tab[0] = vec->x;
	tab[1] = vec->y;
	tab[2] = vec->z;
}

void	color_to_float3(t_color *vec, float *tab)
{
	tab[0] = vec->r / 255.0;
	tab[1] = vec->g / 255.0;
	tab[2] = vec->b / 255.0;
}

void	fcolor_to_float3(t_fcolor *vec, float *tab)
{
	tab[0] = vec->r;
	tab[1] = vec->g;
	tab[2] = vec->b;
}

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
		vec3_to_float3(&((t_sphere *)cpu_scene->elements[e].object)->position, gpu[i].position);
		color_to_float3(&((t_sphere *)cpu_scene->elements[e].object)->color, gpu[i].color);
		gpu[i].radius = (float)((t_sphere *)cpu_scene->elements[e].object)->radius;
		if (((t_sphere *)cpu_scene->elements[e].object)->material == NULL)
			gpu[i].material_id = -1;
		else
			gpu[i].material_id = ((t_sphere *)cpu_scene->elements[e].object)->material - cpu_scene->materials;
		i++;
	}
}

void	convert_camera(t_scene *cpu_scene, t_gpu_camera *cam)
{
	vec3_to_float3(&cpu_scene->camera.position, cam->position);
	vec3_to_float3(&cpu_scene->camera.orientation, cam->orientation);
	cam->fov = (float)cpu_scene->camera.fov;
	cam->defocus_angle = (float)cpu_scene->camera.defocus_angle;
	cam->focus_dist = (float)cpu_scene->camera.focus_dist;
}

void	convert_viewport(t_gpu_viewport *dst, t_viewport *src, double ior)
{
	dst->focal_length = src->focal_length;
	dst->height = src->height;
	dst->width = src->width;
	dst->render_w = src->render_w;
	dst->render_h = src->render_h;
	dst->gamma = (float)src->gamma;
	dst->defocus_radius = (float)src->defocus_radius;
	dst->max_bounces = src->max_bounces;
	dst->ior_global = ior;
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

void	convert_material(t_gpu_material *dst, t_mat *src)
{
	color_to_float3(&src->color_value, dst->color_value);
	dst->albedo_tex_index = -1;
	dst->metallic_value = src->metallic_value;
	dst->metallic_tex_index = -1;
	dst->roughness_value = src->roughness_value;
	dst->roughness_tex_index = -1;
	dst->ior = src->ior;
	dst->transmission_value = src->transmission_value;
	dst->transmission_tex_index = -1;
	dst->ao_value = src->ao_value;
	dst->ao_tex_index = -1;
	dst->emission_strength = src->emission_strength;
	dst->emission_strength_tex_index = -1;
	dst->scale = src->scale;
	fcolor_to_float3(&src->emission_color, dst->emission_color);
	dst->emission_color_tex_index = -1;
	dst->normal_tex_index = -1;
	dst->normal_intensity = src->normal_intensity;
}


void	convert_materials(t_scene *cpu_scene, t_gpu_material *gpu)
{
	int	i;

	i = -1;
	while (++i < cpu_scene->mat_amount)
	{
		convert_material(&gpu[i], &cpu_scene->materials[i]);
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
	clean_scene(gpu_structs);

	convert_viewport(&gpu_structs->viewport, viewport, scene->ior_all);
	create_ssbo(&gpu_structs->viewport_ssbo, sizeof(t_gpu_viewport), &gpu_structs->viewport, SSBO_BIND_VIEWPORT);
	convert_camera(scene, &gpu_structs->camera);
	create_ssbo(&gpu_structs->camera_ssbo, sizeof(t_gpu_camera), &gpu_structs->camera, SSBO_BIND_CAMERA);
	
	gpu_structs->mat_am = scene->mat_amount;
	gpu_structs->mat = ft_calloc(gpu_structs->mat_am, sizeof(t_gpu_material));
	if (!gpu_structs->mat)
		return (-1); // Tout free correctement
	convert_materials(scene, gpu_structs->mat);
	create_ssbo(&gpu_structs->mat_ssbo, sizeof(t_gpu_material) * gpu_structs->mat_am, gpu_structs->mat, SSBO_BIND_MATERIALS);

	gpu_structs->spheres_am = 0;
	count_spheres(scene, gpu_structs);
	gpu_structs->spheres = ft_calloc(gpu_structs->spheres_am, sizeof(t_gpu_sphere));
	if (!gpu_structs->spheres)
		return (-1); // Tout free correctement
	convert_spheres(scene, gpu_structs->spheres);
	create_ssbo(&gpu_structs->spheres_ssbo, sizeof(t_gpu_sphere) * gpu_structs->spheres_am, gpu_structs->spheres, SSBO_BIND_SPHERES);

	return (0);
}
