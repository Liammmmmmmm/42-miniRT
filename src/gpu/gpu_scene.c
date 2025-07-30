/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gpu_scene.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 16:03:21 by lilefebv          #+#    #+#             */
/*   Updated: 2025/07/29 19:55:41 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "gpu.h"
#include "gpu_scene.h"
#include <stdio.h>
#include "maths.h"

void convert_all_obj(t_scene *cpu_scene, t_gpu_structs *gpu_scene)
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
		{
			gpu_scene->prim_types_indices[i].indice = primitive;
			gpu_scene->prim_types_indices[i].type = 0;
			gpu_scene->primitives[primitive].radius = ((t_sphere *)cpu_scene->bvh.obj_list[i]->object)->radius;
			vec3_to_float3(&((t_sphere *)cpu_scene->bvh.obj_list[i]->object)->position, gpu_scene->primitives[primitive].position);
			color_to_float3(&((t_sphere *)cpu_scene->bvh.obj_list[i]->object)->color, gpu_scene->primitives[primitive].color);
			if (((t_sphere *)cpu_scene->bvh.obj_list[i]->object)->material == NULL)
				gpu_scene->primitives[primitive].material_id = -1;
			else
				gpu_scene->primitives[primitive].material_id = ((t_sphere *)cpu_scene->bvh.obj_list[i]->object)->material - cpu_scene->materials;
			primitive++;
		}
		else if (cpu_scene->bvh.obj_list[i]->type == CYLINDER)
		{
			gpu_scene->prim_types_indices[i].indice = primitive;
			gpu_scene->prim_types_indices[i].type = 1;
			gpu_scene->primitives[primitive].height = ((t_cylinder *)cpu_scene->bvh.obj_list[i]->object)->height;
			gpu_scene->primitives[primitive].radius = ((t_cylinder *)cpu_scene->bvh.obj_list[i]->object)->diameter / 2;
			vec3_to_float3(&((t_cylinder *)cpu_scene->bvh.obj_list[i]->object)->position, gpu_scene->primitives[primitive].position);
			vec3_to_float3(&((t_cylinder *)cpu_scene->bvh.obj_list[i]->object)->orientation, gpu_scene->primitives[primitive].orientation);
			color_to_float3(&((t_cylinder *)cpu_scene->bvh.obj_list[i]->object)->color, gpu_scene->primitives[primitive].color);
			if (((t_cylinder *)cpu_scene->bvh.obj_list[i]->object)->material == NULL)
				gpu_scene->primitives[primitive].material_id = -1;
			else
				gpu_scene->primitives[primitive].material_id = ((t_cylinder *)cpu_scene->bvh.obj_list[i]->object)->material - cpu_scene->materials;
			if (((t_cylinder *)cpu_scene->bvh.obj_list[i]->object)->material_bot == NULL)
				gpu_scene->primitives[primitive].material_id_top = -1;
			else
				gpu_scene->primitives[primitive].material_id_top = ((t_cylinder *)cpu_scene->bvh.obj_list[i]->object)->material_top - cpu_scene->materials;
			if (((t_cylinder *)cpu_scene->bvh.obj_list[i]->object)->material_bot == NULL)
				gpu_scene->primitives[primitive].material_id_bot = -1;
			else
				gpu_scene->primitives[primitive].material_id_bot = ((t_cylinder *)cpu_scene->bvh.obj_list[i]->object)->material_bot - cpu_scene->materials;
			primitive++;
		}
		else if (cpu_scene->bvh.obj_list[i]->type == CONE)
		{
			gpu_scene->prim_types_indices[i].indice = primitive;
			gpu_scene->prim_types_indices[i].type = 2;
			gpu_scene->primitives[primitive].height = ((t_cone *)cpu_scene->bvh.obj_list[i]->object)->height;
			gpu_scene->primitives[primitive].radius = ((t_cone *)cpu_scene->bvh.obj_list[i]->object)->diameter / 2;
			vec3_to_float3(&((t_cone *)cpu_scene->bvh.obj_list[i]->object)->orientation, gpu_scene->primitives[primitive].orientation);
			vec3_to_float3(&((t_cone *)cpu_scene->bvh.obj_list[i]->object)->position, gpu_scene->primitives[primitive].position);
			color_to_float3(&((t_cone *)cpu_scene->bvh.obj_list[i]->object)->color, gpu_scene->primitives[primitive].color);
			if (((t_cone *)cpu_scene->bvh.obj_list[i]->object)->material == NULL)
				gpu_scene->primitives[primitive].material_id = -1;
			else
				gpu_scene->primitives[primitive].material_id = ((t_cone *)cpu_scene->bvh.obj_list[i]->object)->material - cpu_scene->materials;
			if (((t_cone *)cpu_scene->bvh.obj_list[i]->object)->material_top == NULL)
				gpu_scene->primitives[primitive].material_id_top = -1;
			else
				gpu_scene->primitives[primitive].material_id_top = ((t_cone *)cpu_scene->bvh.obj_list[i]->object)->material_top - cpu_scene->materials;
			primitive++;
		}
		else if (cpu_scene->bvh.obj_list[i]->type == HYPERBOLOID)
		{
			gpu_scene->prim_types_indices[i].indice = hyper;
			gpu_scene->prim_types_indices[i].type = 3;
			gpu_scene->hypers[hyper].a = ((t_hyperboloid *)cpu_scene->bvh.obj_list[i]->object)->a;
			gpu_scene->hypers[hyper].b = ((t_hyperboloid *)cpu_scene->bvh.obj_list[i]->object)->b;
			gpu_scene->hypers[hyper].c = ((t_hyperboloid *)cpu_scene->bvh.obj_list[i]->object)->c;
			gpu_scene->hypers[hyper].shape = ((t_hyperboloid *)cpu_scene->bvh.obj_list[i]->object)->shape;
			gpu_scene->hypers[hyper].height = ((t_hyperboloid *)cpu_scene->bvh.obj_list[i]->object)->height;
			vec3_to_float3(&((t_hyperboloid *)cpu_scene->bvh.obj_list[i]->object)->orientation, gpu_scene->hypers[hyper].orientation);
			vec3_to_float3(&((t_hyperboloid *)cpu_scene->bvh.obj_list[i]->object)->position, gpu_scene->hypers[hyper].position);
			color_to_float3(&((t_hyperboloid *)cpu_scene->bvh.obj_list[i]->object)->color, gpu_scene->hypers[hyper].color);
			if (((t_hyperboloid *)cpu_scene->bvh.obj_list[i]->object)->material == NULL)
				gpu_scene->hypers[hyper].material_id = -1;
			else
				gpu_scene->hypers[hyper].material_id = ((t_hyperboloid *)cpu_scene->bvh.obj_list[i]->object)->material - cpu_scene->materials;
			hyper++;
		}
		else if (cpu_scene->bvh.obj_list[i]->type == TRIANGLE)
		{
			gpu_scene->prim_types_indices[i].indice = triangle;
			gpu_scene->prim_types_indices[i].type = 4;
			vert_to_gpu_vert(&((t_triangle *)cpu_scene->bvh.obj_list[i]->object)->v0, &gpu_scene->triangles[triangle].v0);
			vert_to_gpu_vert(&((t_triangle *)cpu_scene->bvh.obj_list[i]->object)->v1, &gpu_scene->triangles[triangle].v1);
			vert_to_gpu_vert(&((t_triangle *)cpu_scene->bvh.obj_list[i]->object)->v2, &gpu_scene->triangles[triangle].v2);
			vec3_to_float3(&((t_triangle *)cpu_scene->bvh.obj_list[i]->object)->center, gpu_scene->triangles[triangle].center);
			gpu_scene->triangles[triangle].obj_id = -1;
			color_to_float3(&((t_triangle *)cpu_scene->bvh.obj_list[i]->object)->color,	gpu_scene->triangles[triangle].color);
			if (((t_triangle *)cpu_scene->bvh.obj_list[i]->object)->material == NULL)
				gpu_scene->triangles[triangle].material_id = -1;
			else
				gpu_scene->triangles[triangle].material_id = ((t_triangle *)cpu_scene->bvh.obj_list[i]->object)->material - cpu_scene->materials;
			triangle++;
		}
		i++;
	}
}



int	convert_scene(t_minirt *minirt, t_scene *scene, t_viewport *viewport, t_gpu_structs *gpu_structs)
{
	clean_scene(gpu_structs);

	convert_viewport(&gpu_structs->viewport, viewport, scene->ior_all);
	convert_camera(scene, &gpu_structs->camera);
	convert_ambiant(scene, &gpu_structs->ambiant, scene);

	
	// ---------------- MATERIALS ---------------- //
	gpu_structs->mat_am = scene->mat_amount;
	gpu_structs->mat = ft_calloc(gpu_structs->mat_am, sizeof(t_gpu_material));
	if (!gpu_structs->mat)
		return (-1); // Tout free correctement
	convert_materials(scene, gpu_structs->mat);
	create_ssbo(&gpu_structs->mat_ssbo, sizeof(t_gpu_material) * gpu_structs->mat_am, gpu_structs->mat, SSBO_BIND_MATERIALS);

	// ---------------- OBJECTS ---------------- //
	count_obj(scene, gpu_structs);
	gpu_structs->primitive_am = gpu_structs->spheres_am + gpu_structs->cylinders_am + gpu_structs->cones_am;
	gpu_structs->lights_am = scene->obj_lst.light_nb;
	gpu_structs->hypers = ft_calloc(gpu_structs->hypers_am, sizeof(t_gpu_hyper));
	gpu_structs->triangles = ft_calloc(gpu_structs->triangles_am, sizeof(t_gpu_triangle));
	gpu_structs->lights = ft_calloc(gpu_structs->lights_am, sizeof(t_gpu_light));
	gpu_structs->prim_types_indices = ft_calloc(scene->bvh.size, sizeof(t_type_indice));
	gpu_structs->primitives	= ft_calloc(gpu_structs->primitive_am, sizeof(t_gpu_primitive));
	// if (!gpu_structs->spheres)
	// 	return (-1); // Tout free correctement
	convert_all_obj(scene, gpu_structs);
	convert_lights(scene, gpu_structs);
	create_ssbo(&gpu_structs->hypers_ssbo, sizeof(t_gpu_hyper) * gpu_structs->hypers_am, gpu_structs->hypers, SSBO_BIND_HYPERS);
	create_ssbo(&gpu_structs->triangles_ssbo, sizeof(t_gpu_triangle) * gpu_structs->triangles_am, gpu_structs->triangles, SSBO_BIND_TRIANGLES);
	create_ssbo(&gpu_structs->lights_ssbo, sizeof(t_gpu_light) * gpu_structs->lights_am, gpu_structs->lights, SSBO_BIND_LIGHTS);
	create_ssbo(&gpu_structs->primitive_ssbo, sizeof(t_gpu_primitive) * gpu_structs->primitive_am, gpu_structs->primitives, SSBO_BIND_PRIMITIVE);


	gpu_structs->bvh_node_am = scene->bvh.bvh_nodes_used;
	gpu_structs->bvh_node = ft_calloc(gpu_structs->bvh_node_am, sizeof(t_gpu_bvh_node));
	if (!gpu_structs->bvh_node)
		return (-1); // Tout free correctement
	convert_bvh_node(scene, gpu_structs->bvh_node);
	create_ssbo(&gpu_structs->bvh_node_ssbo, sizeof(t_gpu_bvh_node) * gpu_structs->bvh_node_am, gpu_structs->bvh_node, SSBO_BIND_BVH);
	
	gpu_structs->prim_indice_am = scene->bvh.size;
	create_ssbo(&gpu_structs->prim_types_indices_ssbo, sizeof(t_type_indice) * gpu_structs->prim_indice_am, gpu_structs->prim_types_indices, SSBO_BIND_PRIM_TYPE_INDICE);

	gpu_structs->planes_am = scene->obj_lst.plane_nb;
	gpu_structs->planes = ft_calloc(gpu_structs->planes_am, sizeof(t_gpu_plane));
	if (!gpu_structs->planes)
		return (-1); // Tout free correctement
	convert_plane(scene, gpu_structs->planes);
	create_ssbo(&gpu_structs->planes_ssbo, sizeof(t_gpu_plane) * gpu_structs->planes_am, gpu_structs->planes, SSBO_BIND_PLANE);
	
	send_uniforms(minirt);
	
	// -------- IMPORTANCE SAMPLING --------- //

	send_importance_sampling(minirt, &minirt->scene);
	

	return (0);
}
