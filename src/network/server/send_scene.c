/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   send_scene.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 11:16:08 by lilefebv          #+#    #+#             */
/*   Updated: 2025/08/06 12:05:22 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "gpu.h"
#include "network.h"

int	convert_textures_server(t_scene *scene, t_gpu_textures *gtx)
{
	count_tex(scene, gtx);
	gtx->checkers = ft_calloc(gtx->checkers_am, sizeof(t_gpu_checker));
	gtx->images = ft_calloc(gtx->images_am, sizeof(t_gpu_tex_data));
	gtx->images_stream = ft_calloc(gtx->total_pixel_images * 4, sizeof(float));
	gtx->textures_types_indices = ft_calloc(scene->tex_amount,
			sizeof(t_type_indice));
	if (!gtx->checkers || !gtx->images || !gtx->images_stream
		|| !gtx->textures_types_indices)
	{
		free(gtx->checkers);
		free(gtx->images);
		free(gtx->images_stream);
		free(gtx->textures_types_indices);
		ft_bzero(gtx, sizeof(t_gpu_textures));
		print_errorm1("Malloc failed, textures not transmitted.");
	}
	convert_textures(scene, gtx);
	return (0);
}

int	convert_scene_server(t_minirt *minirt, t_scene *scene, t_viewport *viewport,
	t_gpu_structs *gpu)
{
	clean_scene(gpu);
	convert_viewport(&gpu->viewport, viewport, scene->ior_all);
	convert_camera(scene, &gpu->camera);
	convert_ambiant(scene, &gpu->ambiant, scene);
	gpu->photon_am = minirt->scene.photon_map.photon_count;
	gpu->cells_am = gpu->photon_am * 2;
	count_obj(scene, gpu);
	if (malloc_every_object_gpu(gpu, scene) == -1)
		return (-1);
	convert_materials(scene, gpu->mat);
	convert_all_obj(scene, gpu);
	convert_lights(scene, gpu);
	convert_plane(scene, gpu->planes);
	convert_bvh_node(scene, gpu->bvh_node);
	build_caustic_grid(scene, gpu, 0.1f, gpu->cells_am);
	return (0);
}


