/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert_others.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 19:55:44 by lilefebv          #+#    #+#             */
/*   Updated: 2025/07/30 09:27:18 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "gpu.h"

static void	convert_point_light(t_scene *cpu_scene, t_gpu_structs *gpu_scene,
	int *lights)
{
	t_light	*lobj;

	lobj = cpu_scene->obj_lst.light_lst[*lights]->object;
	vec3_to_float3(&lobj->position, gpu_scene->lights[*lights].pos);
	color_to_float3(&lobj->color, gpu_scene->lights[*lights].color);
	gpu_scene->lights[*lights].type = 0;
	gpu_scene->lights[*lights].brightness = lobj->brightness;
	gpu_scene->lights[*lights].radius = lobj->radius;
	gpu_scene->lights[*lights].shadow_sample = (t_uint)lobj->shadow_sample;
	(*lights)++;
}

static void	convert_dir_light(t_scene *cpu_scene, t_gpu_structs *gpu_scene,
	int *lights)
{
	t_dlight	*lobj;

	lobj = cpu_scene->obj_lst.light_lst[*lights]->object;
	vec3_to_float3(&lobj->orientation, gpu_scene->lights[*lights].pos);
	color_to_float3(&lobj->color, gpu_scene->lights[*lights].color);
	gpu_scene->lights[*lights].type = 1;
	gpu_scene->lights[*lights].brightness = lobj->brightness;
	gpu_scene->lights[*lights].radius = 0;
	gpu_scene->lights[*lights].shadow_sample = 0;
	(*lights)++;
}

void	convert_lights(t_scene *cpu_scene, t_gpu_structs *gpu_scene)
{
	int	lights;

	lights = 0;
	while (lights < cpu_scene->obj_lst.light_nb)
	{
		if (cpu_scene->obj_lst.light_lst[lights]->type == LIGHT)
			convert_point_light(cpu_scene, gpu_scene, &lights);
		else if (cpu_scene->obj_lst.light_lst[lights]->type
			== DIRECTIONAL_LIGHT)
			convert_dir_light(cpu_scene, gpu_scene, &lights);
	}
}

void	convert_bvh_node(t_scene *cpu_scene, t_gpu_bvh_node *bvh_node)
{
	uint32_t	i;

	i = 0;
	while (i < (cpu_scene->bvh.bvh_nodes_used))
	{
		bvh_node[i].first_prim = cpu_scene->bvh.prim_indices[
			cpu_scene->bvh.bvh_nodes[i].first_prim];
		bvh_node[i].is_leaf = cpu_scene->bvh.bvh_nodes[i].is_leaf;
		bvh_node[i].right_child = cpu_scene->bvh.bvh_nodes[i].right_child;
		bvh_node[i].left_child = cpu_scene->bvh.bvh_nodes[i].left_child;
		vec3_to_float3(&cpu_scene->bvh.bvh_nodes[i].node_bounds.max,
			bvh_node[i].node_bounds.max);
		vec3_to_float3(&cpu_scene->bvh.bvh_nodes[i].node_bounds.min,
			bvh_node[i].node_bounds.min);
		bvh_node[i].prim_count = cpu_scene->bvh.bvh_nodes[i].prim_count;
		i++;
	}
}
