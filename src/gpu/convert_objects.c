/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert_objects.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 19:21:20 by lilefebv          #+#    #+#             */
/*   Updated: 2025/07/30 10:14:33 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "gpu.h"

void	convert_camera(t_scene *cpu_scene, t_gpu_camera *cam)
{
	vec3_to_float3(&cpu_scene->camera.position, cam->position);
	vec3_to_float3(&cpu_scene->camera.orientation, cam->orientation);
	cam->fov = (float)cpu_scene->camera.fov;
	cam->defocus_angle = (float)cpu_scene->camera.defocus_angle;
	cam->focus_dist = (float)cpu_scene->camera.focus_dist;
}

void	convert_ambiant(t_scene *cpu_scene, t_gpu_amb_light *amb,
	t_scene *scene)
{
	color_to_float3(&cpu_scene->amb_light.skybox_c, amb->skybox_color);
	amb->ratio = (float)cpu_scene->amb_light.ratio;
	amb->skybox_tex_index = cpu_scene->amb_light.skybox_t - scene->textures;
	if (cpu_scene->amb_light.skybox_t == NULL)
		amb->skybox_tex_index = -1;
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

void	convert_plane(t_scene *cpu_scene, t_gpu_plane *gpu)
{
	int		i;
	t_plane	*p;

	i = 0;
	while (i < cpu_scene->obj_lst.plane_nb)
	{
		p = cpu_scene->obj_lst.plane_lst[i]->object;
		vec3_to_float3(&p->position, gpu[i].position);
		vec3_to_float3(&p->normal, gpu[i].normal);
		color_to_float3(&p->color, gpu[i].color);
		if (p->material == NULL)
			gpu[i].material_id = -1;
		else
			gpu[i].material_id = p->material - cpu_scene->materials;
		gpu[i].d = (p->position.x * p->normal.x + p->position.y * p->normal.y
				+ p->position.z * p->normal.z);
		i++;
	}
}

void	convert_all_obj(t_scene *cpu_scene, t_gpu_structs *gpu_scene)
{
	uint32_t	i;
	uint32_t	primitive;
	uint32_t	hyper;
	uint32_t	triangle;

	i = 0;
	primitive = 0;
	hyper = 0;
	triangle = 0;
	while (i < cpu_scene->bvh.size)
	{
		if (cpu_scene->bvh.obj_list[i]->type == SPHERE)
			convert_sphere(cpu_scene, gpu_scene, &primitive, i);
		else if (cpu_scene->bvh.obj_list[i]->type == CYLINDER)
			convert_cylinder(cpu_scene, gpu_scene, &primitive, i);
		else if (cpu_scene->bvh.obj_list[i]->type == CONE)
			convert_cone(cpu_scene, gpu_scene, &primitive, i);
		else if (cpu_scene->bvh.obj_list[i]->type == HYPERBOLOID)
			convert_hyper(cpu_scene, gpu_scene, &hyper, i);
		else if (cpu_scene->bvh.obj_list[i]->type == TRIANGLE)
			convert_triangle(cpu_scene, gpu_scene, &triangle, i);
		i++;
	}
}
