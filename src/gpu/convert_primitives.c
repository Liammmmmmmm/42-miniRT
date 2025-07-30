/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert_primitives.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 10:15:18 by lilefebv          #+#    #+#             */
/*   Updated: 2025/07/30 10:16:35 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "gpu.h"

void	convert_sphere(t_scene *cpu, t_gpu_structs *gpu, uint32_t *primitive,
	uint32_t i)
{
	t_sphere	*sp;

	sp = cpu->bvh.obj_list[i]->object;
	gpu->prim_types_indices[i].indice = *primitive;
	gpu->prim_types_indices[i].type = 0;
	gpu->primitives[*primitive].radius = sp->radius;
	vec3_to_float3(&sp->position, gpu->primitives[*primitive].position);
	color_to_float3(&sp->color, gpu->primitives[*primitive].color);
	if (sp->material == NULL)
		gpu->primitives[*primitive].material_id = -1;
	else
		gpu->primitives[*primitive].material_id = sp->material - cpu->materials;
	(*primitive)++;
}

void	convert_cylinder(t_scene *cpu, t_gpu_structs *gpu, uint32_t *primitive,
	uint32_t i)
{
	t_cylinder	*cy;

	cy = cpu->bvh.obj_list[i]->object;
	gpu->prim_types_indices[i].indice = *primitive;
	gpu->prim_types_indices[i].type = 1;
	gpu->primitives[*primitive].height = cy->height;
	gpu->primitives[*primitive].radius = cy->diameter / 2;
	vec3_to_float3(&cy->position, gpu->primitives[*primitive].position);
	vec3_to_float3(&cy->orientation, gpu->primitives[*primitive].orientation);
	color_to_float3(&cy->color, gpu->primitives[*primitive].color);
	if (cy->material == NULL)
		gpu->primitives[*primitive].material_id = -1;
	else
		gpu->primitives[*primitive].material_id = cy->material - cpu->materials;
	if (cy->material_bot == NULL)
		gpu->primitives[*primitive].material_id_top = -1;
	else
		gpu->primitives[*primitive].material_id_top
			= cy->material_top - cpu->materials;
	if (cy->material_bot == NULL)
		gpu->primitives[*primitive].material_id_bot = -1;
	else
		gpu->primitives[*primitive].material_id_bot
			= cy->material_bot - cpu->materials;
	(*primitive)++;
}

void	convert_cone(t_scene *cpu, t_gpu_structs *gpu, uint32_t *primitive,
	uint32_t i)
{
	t_cone	*co;

	co = cpu->bvh.obj_list[i]->object;
	gpu->prim_types_indices[i].indice = *primitive;
	gpu->prim_types_indices[i].type = 2;
	gpu->primitives[*primitive].height = co->height;
	gpu->primitives[*primitive].radius = co->diameter / 2;
	vec3_to_float3(&co->orientation, gpu->primitives[*primitive].orientation);
	vec3_to_float3(&co->position, gpu->primitives[*primitive].position);
	color_to_float3(&co->color, gpu->primitives[*primitive].color);
	if (co->material == NULL)
		gpu->primitives[*primitive].material_id = -1;
	else
		gpu->primitives[*primitive].material_id = co->material - cpu->materials;
	if (co->material_top == NULL)
		gpu->primitives[*primitive].material_id_top = -1;
	else
		gpu->primitives[*primitive].material_id_top
			= co->material_top - cpu->materials;
	(*primitive)++;
}

void	convert_hyper(t_scene *cpu, t_gpu_structs *gpu, uint32_t *hyper,
	uint32_t i)
{
	t_hyperboloid	*hy;

	hy = cpu->bvh.obj_list[i]->object;
	gpu->prim_types_indices[i].indice = *hyper;
	gpu->prim_types_indices[i].type = 3;
	gpu->hypers[*hyper].a = hy->a;
	gpu->hypers[*hyper].b = hy->b;
	gpu->hypers[*hyper].c = hy->c;
	gpu->hypers[*hyper].shape = hy->shape;
	gpu->hypers[*hyper].height = hy->height;
	vec3_to_float3(&hy->orientation, gpu->hypers[*hyper].orientation);
	vec3_to_float3(&hy->position, gpu->hypers[*hyper].position);
	color_to_float3(&hy->color, gpu->hypers[*hyper].color);
	if (hy->material == NULL)
		gpu->hypers[*hyper].material_id = -1;
	else
		gpu->hypers[*hyper].material_id = hy->material - cpu->materials;
	(*hyper)++;
}

void	convert_triangle(t_scene *cpu, t_gpu_structs *gpu, uint32_t *triangle,
	uint32_t i)
{
	t_triangle	*tr;

	tr = cpu->bvh.obj_list[i]->object;
	gpu->prim_types_indices[i].indice = *triangle;
	gpu->prim_types_indices[i].type = 4;
	vert_to_gpu_vert(&tr->v0, &gpu->triangles[*triangle].v0);
	vert_to_gpu_vert(&tr->v1, &gpu->triangles[*triangle].v1);
	vert_to_gpu_vert(&tr->v2, &gpu->triangles[*triangle].v2);
	vec3_to_float3(&tr->center, gpu->triangles[*triangle].center);
	gpu->triangles[*triangle].obj_id = -1;
	color_to_float3(&tr->color, gpu->triangles[*triangle].color);
	if (tr->material == NULL)
		gpu->triangles[*triangle].material_id = -1;
	else
		gpu->triangles[*triangle].material_id = tr->material - cpu->materials;
	(*triangle)++;
}
