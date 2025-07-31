/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gpu_scene.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 16:03:21 by lilefebv          #+#    #+#             */
/*   Updated: 2025/07/31 17:33:59 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gpu.h"
#include "gpu_scene.h"
#include <stdio.h>
#include "maths.h"

static void	free_objects(t_gpu_structs *gpu)
{
	free(gpu->mat);
	free(gpu->hypers);
	free(gpu->triangles);
	free(gpu->lights);
	free(gpu->prim_types_indices);
	free(gpu->primitives);
	free(gpu->bvh_node);
	free(gpu->planes);
}

static int	malloc_every_object_gpu(t_gpu_structs *gpu, t_scene *scene)
{
	gpu->mat_am = scene->mat_amount;
	gpu->prim_indice_am = scene->bvh.size;
	count_obj(scene, gpu);
	gpu->primitive_am = gpu->spheres_am + gpu->cylinders_am + gpu->cones_am;
	gpu->lights_am = scene->obj_lst.light_nb;
	gpu->bvh_node_am = scene->bvh.bvh_nodes_used;
	gpu->planes_am = scene->obj_lst.plane_nb;
	gpu->mat = ft_calloc(gpu->mat_am, sizeof(t_gpu_material));
	gpu->hypers = ft_calloc(gpu->hypers_am, sizeof(t_gpu_hyper));
	gpu->triangles = ft_calloc(gpu->triangles_am, sizeof(t_gpu_triangle));
	gpu->lights = ft_calloc(gpu->lights_am, sizeof(t_gpu_light));
	gpu->prim_types_indices = ft_calloc(scene->bvh.size, sizeof(t_type_indice));
	gpu->primitives = ft_calloc(gpu->primitive_am, sizeof(t_gpu_primitive));
	gpu->bvh_node = ft_calloc(gpu->bvh_node_am, sizeof(t_gpu_bvh_node));
	gpu->planes = ft_calloc(gpu->planes_am, sizeof(t_gpu_plane));
	gpu->photons = ft_calloc(gpu->photon_am, sizeof(t_gpu_photon));
	if (!gpu->mat || !gpu->hypers || !gpu->triangles || !gpu->lights
		|| !gpu->planes || !gpu->prim_types_indices || !gpu->primitives
		|| !gpu->bvh_node || !gpu->photons)
	{
		free_objects(gpu);
		return (-1);
	}
	return (0);
}

void	convert_scene(t_minirt *minirt, t_scene *scene, t_viewport *viewport,
	t_gpu_structs *gpu)
{
	convert_viewport(&gpu->viewport, viewport, scene->ior_all);
	convert_camera(scene, &gpu->camera);
	convert_ambiant(scene, &gpu->ambiant, scene);
	camera_uniforms(minirt->shaders_data.program,
		&minirt->shaders_data.scene.camera);
	ambiant_uniforms(minirt->shaders_data.program,
		&minirt->shaders_data.scene.ambiant);
	viewport_uniforms(minirt->shaders_data.program,
		&minirt->shaders_data.scene.viewport);
	send_uniforms(minirt);
}

static void	all_ssbo(t_gpu_structs *gpu)
{
	create_ssbo(&gpu->mat_ssbo, sizeof(t_gpu_material) * gpu->mat_am,
		gpu->mat, SSBO_BIND_MATERIALS);
	create_ssbo(&gpu->hypers_ssbo, sizeof(t_gpu_hyper) * gpu->hypers_am,
		gpu->hypers, SSBO_BIND_HYPERS);
	create_ssbo(&gpu->triangles_ssbo, sizeof(t_gpu_triangle)
		* gpu->triangles_am, gpu->triangles, SSBO_BIND_TRIANGLES);
	create_ssbo(&gpu->lights_ssbo, sizeof(t_gpu_light) * gpu->lights_am,
		gpu->lights, SSBO_BIND_LIGHTS);
	create_ssbo(&gpu->primitive_ssbo, sizeof(t_gpu_primitive)
		* gpu->primitive_am, gpu->primitives, SSBO_BIND_PRIMITIVE);
	create_ssbo(&gpu->bvh_node_ssbo, sizeof(t_gpu_bvh_node) * gpu->bvh_node_am,
		gpu->bvh_node, SSBO_BIND_BVH);
	create_ssbo(&gpu->prim_types_indices_ssbo,
		sizeof(t_type_indice) * gpu->prim_indice_am,
		gpu->prim_types_indices, SSBO_BIND_PRIM_TYPE_INDICE);
	create_ssbo(&gpu->planes_ssbo, sizeof(t_gpu_plane) * gpu->planes_am,
		gpu->planes, SSBO_BIND_PLANE);
	create_ssbo(&gpu->photon_indices_ssbo, sizeof(uint32_t) * gpu->photon_am,
		gpu->photon_indices, SSBO_BIND_PHOTON_INDICE);
	create_ssbo(&gpu->photons_ssbo, sizeof(t_gpu_photon) * gpu->photon_am,
		gpu->photons, SSBO_BIND_PHOTON);
	create_ssbo(&gpu->photons_ssbo, sizeof(t_gpu_photon) * gpu->photon_am,
		gpu->photons, SSBO_BIND_CELL);
}

int	convert_scene_build(t_minirt *minirt, t_scene *scene, t_viewport *viewport,
	t_gpu_structs *gpu)
{
	clean_scene(gpu);
	convert_scene(minirt, scene, viewport, gpu);
	gpu->photon_am = minirt->scene.photon_map.photon_count;
	if (malloc_every_object_gpu(gpu, scene) == -1)
		return (-1);
	convert_materials(scene, gpu->mat);
	convert_all_obj(scene, gpu);
	convert_lights(scene, gpu);
	convert_plane(scene, gpu->planes);
	convert_bvh_node(scene, gpu->bvh_node);
	build_caustic_grid(scene, gpu, 0.1f, gpu->photon_am * 2);
	all_ssbo(gpu);
	send_uniforms(minirt);
	send_importance_sampling(minirt, &minirt->scene);
	return (0);
}
