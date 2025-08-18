/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   receive_scene.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 15:58:35 by lilefebv          #+#    #+#             */
/*   Updated: 2025/08/18 16:08:46 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "gpu.h"
#include "network.h"
#include "utils.h"
#include "minirt.h"

void	re_create_ssbo(GLuint *ssbo, size_t size, void *data, long bind);
void	receive_scene_if_forest6(t_minirt *minirt, uint16_t type, char *data,
			uint64_t size);

static void	receive_scene_if_forest5(t_minirt *minirt, uint16_t type,
	char *data, uint64_t size)
{
	if (type == SRV_PLANES_AM && size == sizeof(int))
	{
		minirt->shaders_data.scene.planes_am = *(int *)data;
		glUniform1ui(glGetUniformLocation(minirt->shaders_data.program,
				"planes_am"), minirt->shaders_data.scene.planes_am);
	}
	else if (type == SRV_LIGHTS_AM && size == sizeof(int))
	{
		minirt->shaders_data.scene.lights_am = *(int *)data;
		glUniform1ui(glGetUniformLocation(minirt->shaders_data.program,
				"lights_am"), minirt->shaders_data.scene.lights_am);
	}
	else if (type == SRV_RENDER_MODE && size == sizeof(char))
	{
		minirt->render_mode = *(char *)data;
		glUniform1ui(glGetUniformLocation(minirt->shaders_data.program,
				"render_mode"), minirt->render_mode);
	}
	else
		receive_scene_if_forest6(minirt, type, data, size);
}

static void	receive_scene_if_forest4(t_minirt *minirt, uint16_t type,
	char *data, uint64_t size)
{
	if (type == SRV_GRID_WORLD_MIN && size == sizeof(float) * 3)
	{
		minirt->shaders_data.scene.grid_world_min[0] = *(float *)data;
		minirt->shaders_data.scene.grid_world_min[1] = *((float *)data + 1);
		minirt->shaders_data.scene.grid_world_min[2] = *((float *)data + 2);
		glUniform3fv(glGetUniformLocation(minirt->shaders_data.program,
				"grid_world_min"), 1, minirt->shaders_data.scene.grid_world_min)
		;
	}
	else if (type == SRV_CELL_SIZE && size == sizeof(float))
	{
		minirt->shaders_data.scene.cell_size = *(float *)data;
		glUniform1ui(glGetUniformLocation(minirt->shaders_data.program,
				"cell_size"), minirt->shaders_data.scene.cell_size);
	}
	else if (type == SRV_RENDER_MODE && size == sizeof(char))
	{
		minirt->render_mode = *(char *)data;
		glUniform1ui(glGetUniformLocation(minirt->shaders_data.program,
				"render_mode"), minirt->render_mode);
	}
	else
		receive_scene_if_forest5(minirt, type, data, size);
}

static void	receive_scene_if_forest3(t_minirt *minirt, uint16_t type,
	char *data, uint64_t size)
{
	if (type == SRV_WIDTH_RENDER && size == sizeof(int))
	{
		minirt->scene.render_width = *(int *)data;
		glUniform1ui(glGetUniformLocation(minirt->shaders_data.program,
				"width_render"), minirt->scene.render_width);
	}
	else if (type == SRV_HEIGHT_RENDER && size == sizeof(int))
	{
		minirt->scene.render_height = *(int *)data;
		glUniform1ui(glGetUniformLocation(minirt->shaders_data.program,
				"height_render"), minirt->scene.render_height);
	}
	else if (type == SRV_MAX_BOUNCES && size == sizeof(double))
	{
		minirt->controls.max_bounces = *(double *)data;
		glUniform1ui(glGetUniformLocation(minirt->shaders_data.program,
				"max_bounces"), minirt->controls.max_bounces);
	}
	else
		receive_scene_if_forest4(minirt, type, data, size);
}

static void	receive_scene_if_forest2(t_minirt *minirt, uint16_t type,
	char *data, uint64_t size)
{
	t_gpu_structs	*gps;

	gps = &minirt->shaders_data.scene;
	if (type == SRV_LIGHTS)
		re_create_ssbo(&gps->lights_ssbo, size, data, SSBO_BIND_LIGHTS);
	else if (type == SRV_PRIM_TYPES_INDICES)
		re_create_ssbo(&gps->prim_types_indices_ssbo, size, data,
			SSBO_BIND_PRIM_TYPE_INDICE);
	else if (type == SRV_PRIMITIVES)
		re_create_ssbo(&gps->primitive_ssbo, size, data, SSBO_BIND_PRIMITIVE);
	else if (type == SRV_BVH_NODE)
		re_create_ssbo(&gps->bvh_node_ssbo, size, data, SSBO_BIND_BVH);
	else if (type == SRV_PLANES)
		re_create_ssbo(&gps->planes_ssbo, size, data, SSBO_BIND_PLANE);
	else if (type == SRV_PHOTONS)
		re_create_ssbo(&gps->photons_ssbo, size, data, SSBO_BIND_PHOTON);
	else if (type == SRV_CELLS)
		re_create_ssbo(&gps->cells_ssbo, size, data, SSBO_BIND_CELL);
	else if (type == SRV_PHOTON_INDICES)
		re_create_ssbo(&gps->photon_indices_ssbo, size, data,
			SSBO_BIND_PHOTON_INDICE);
	else if (type == SRV_ANIM_FRAME && size == sizeof(t_uint))
		minirt->options.anim.frame_i = *(t_uint *)data;
	else
		receive_scene_if_forest3(minirt, type, data, size);
}

void	receive_scene_if_forest(t_minirt *minirt, uint16_t type, char *data,
	uint64_t size)
{
	if (type == SRV_CHECKERS)
		re_create_ssbo(&minirt->shaders_data.tex.checkers_ssbo, size, data,
			SSBO_BIND_CHECKERS);
	else if (type == SRV_IMAGES)
		re_create_ssbo(&minirt->shaders_data.tex.images_ssbo, size, data,
			SSBO_BIND_IMAGES);
	else if (type == SRV_IMAGE_STREAM)
		re_create_ssbo(&minirt->shaders_data.tex.images_stream_ssbo, size, data,
			SSBO_BIND_IMAGES_STREAM);
	else if (type == SRV_TEX_TYPES_INDICES)
		re_create_ssbo(&minirt->shaders_data.tex.textures_types_indices_ssbo,
			size, data, SSBO_BIND_TEX_TYPE_INDICE);
	else if (type == SRV_SKYBOX_WH && size == sizeof(t_point2))
		minirt->scene.amb_light.srv_skybox_wh = *(t_point2 *)data;
	else if (type == SRV_MATERIALS)
		re_create_ssbo(&minirt->shaders_data.scene.mat_ssbo, size, data,
			SSBO_BIND_MATERIALS);
	else if (type == SRV_HYPER)
		re_create_ssbo(&minirt->shaders_data.scene.hypers_ssbo, size, data,
			SSBO_BIND_HYPERS);
	else if (type == SRV_TRIANGLE)
		re_create_ssbo(&minirt->shaders_data.scene.triangles_ssbo, size, data,
			SSBO_BIND_TRIANGLES);
	else
		receive_scene_if_forest2(minirt, type, data, size);
}
