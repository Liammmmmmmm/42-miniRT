/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   send_scene.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 11:16:08 by lilefebv          #+#    #+#             */
/*   Updated: 2025/08/26 16:59:03 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gpu.h"
#include "network.h"

static void	send_changing_map_part2(t_minirt *minirt, int *fd,
	t_gpu_structs *gsc)
{
	send_scene_data(fd, (char *)gsc->bvh_node,
		sizeof(t_gpu_bvh_node) * gsc->bvh_node_am, SRV_BVH_NODE);
	send_scene_data(fd, (char *)gsc->planes,
		sizeof(t_gpu_plane) * gsc->planes_am, SRV_PLANES);
	send_scene_data(fd, (char *)gsc->photons,
		sizeof(t_gpu_photon) * gsc->photon_am, SRV_PHOTONS);
	send_scene_data(fd, (char *)gsc->cells,
		sizeof(t_gpu_cell) * gsc->cells_am, SRV_CELLS);
	send_scene_data(fd, (char *)gsc->photon_indices,
		sizeof(uint32_t) * gsc->photon_am, SRV_PHOTON_INDICES);
	send_scene_data(fd, (char *)&minirt->controls.max_bounces, sizeof(double),
		SRV_MAX_BOUNCES);
	send_scene_data(fd, (char *)&minirt->render_mode, sizeof(char),
		SRV_RENDER_MODE);
	send_scene_data(fd, (char *)&minirt->shaders_data.scene.planes_am,
		sizeof(int), SRV_PLANES_AM);
	send_scene_data(fd, (char *)&minirt->shaders_data.scene.lights_am,
		sizeof(int), SRV_LIGHTS_AM);
	send_scene_data(fd, (char *)minirt->shaders_data.scene.grid_world_min,
		sizeof(float) * 3, SRV_GRID_WORLD_MIN);
	send_scene_data(fd, (char *)&minirt->shaders_data.scene.cell_size,
		sizeof(float), SRV_CELL_SIZE);
	send_scene_data(fd, (char *)&minirt->shaders_data.scene.table_size,
		sizeof(int), SRV_TABLE_SIZE);
}

void	send_changing_map_part(t_minirt *minirt, int *fd)
{
	t_gpu_structs	*gsc;

	gsc = &minirt->shaders_data.scene;
	send_scene_data(fd, NULL, 0, SRV_COMPUTE_STOP);
	send_scene_data(fd, (char *)&gsc->camera, sizeof(t_gpu_camera), SRV_CAMERA);
	send_scene_data(fd, (char *)&gsc->ambiant, sizeof(t_gpu_amb_light),
		SRV_AMBIANT);
	send_scene_data(fd, (char *)&gsc->viewport, sizeof(t_gpu_viewport),
		SRV_VIEWPORT);
	send_scene_data(fd, (char *)&minirt->options.anim.frame_i, sizeof(t_uint),
		SRV_ANIM_FRAME);
	send_scene_data(fd, (char *)gsc->hypers,
		sizeof(t_gpu_hyper) * gsc->hypers_am, SRV_HYPER);
	send_scene_data(fd, (char *)gsc->triangles,
		sizeof(t_gpu_triangle) * gsc->triangles_am, SRV_TRIANGLE);
	send_scene_data(fd, (char *)gsc->lights,
		sizeof(t_gpu_light) * gsc->lights_am, SRV_LIGHTS);
	send_scene_data(fd, (char *)gsc->prim_types_indices,
		sizeof(t_type_indice) * gsc->prim_indice_am, SRV_PRIM_TYPES_INDICES);
	send_scene_data(fd, (char *)gsc->primitives,
		sizeof(t_gpu_primitive) * gsc->primitive_am, SRV_PRIMITIVES);
	send_changing_map_part2(minirt, fd, gsc);
	send_scene_data(fd, NULL, 0, SRV_COMPUTE_START);
}

static void	send_importance_sampling_network(t_minirt *minirt, int *fd)
{
	t_point2	p;

	if (minirt->scene.amb_light.skybox_t
		&& minirt->scene.amb_light.skybox_t->hdr.pixels
		&& minirt->scene.amb_light.cdf_conditional_inverse
		&& minirt->scene.amb_light.cdf_marginal_inverse
		&& minirt->scene.amb_light.pdf_joint)
	{
		p.x = minirt->scene.amb_light.skybox_t->hdr.width;
		p.y = minirt->scene.amb_light.skybox_t->hdr.height;
		send_scene_data(fd, (char *)&p, sizeof(t_point2), SRV_SKYBOX_WH);
		send_scene_data(fd,
			(char *)minirt->scene.amb_light.cdf_conditional_inverse,
			sizeof(float) * p.x * p.y, SRV_CDF_CONDITIONAL_INVERSE);
		send_scene_data(fd,
			(char *)minirt->scene.amb_light.cdf_marginal_inverse,
			sizeof(float) * p.y, SRV_CDF_MARGINAL_INVERSE);
		send_scene_data(fd, (char *)minirt->scene.amb_light.pdf_joint,
			sizeof(float) * p.x * p.y, SRV_PDF_JOINT);
	}
}

void	send_map_first_time(t_minirt *minirt, int *fd)
{
	t_gpu_textures	*gtx;
	t_gpu_structs	*gsc;

	gtx = &minirt->shaders_data.tex;
	gsc = &minirt->shaders_data.scene;
	send_scene_data(fd, (char *)gtx->checkers,
		sizeof(t_gpu_checker) * gtx->checkers_am, SRV_CHECKERS);
	send_scene_data(fd, (char *)gtx->images,
		sizeof(t_gpu_tex_data) * gtx->images_am, SRV_IMAGES);
	send_scene_data(fd, (char *)gtx->images_stream,
		sizeof(float) * gtx->total_pixel_images * 4, SRV_IMAGE_STREAM);
	send_scene_data(fd, (char *)gtx->textures_types_indices,
		sizeof(t_type_indice) * minirt->scene.tex_amount, SRV_TEX_TYPES_INDICES)
	;
	send_importance_sampling_network(minirt, fd);
	send_scene_data(fd, (char *)&minirt->scene.render_width, sizeof(int),
		SRV_WIDTH_RENDER);
	send_scene_data(fd, (char *)&minirt->scene.render_height, sizeof(int),
		SRV_HEIGHT_RENDER);
	send_scene_data(fd, (char *)gsc->mat, sizeof(t_gpu_material) * gsc->mat_am,
		SRV_MATERIALS);
	send_changing_map_part(minirt, fd);
}
