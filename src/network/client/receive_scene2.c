/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   receive_scene2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 15:58:35 by lilefebv          #+#    #+#             */
/*   Updated: 2025/08/18 16:07:26 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "gpu.h"
#include "network.h"
#include "utils.h"
#include "minirt.h"

void	re_create_ssbo(GLuint *ssbo, size_t size, void *data, long bind)
{
	delete_ssbo(ssbo);
	create_ssbo(ssbo, size, data, bind);
}

static void	receive_scene_if_forest9(t_minirt *minirt, uint16_t type,
	char *data, uint64_t size)
{
	if (type == SRV_CDF_MARGINAL_INVERSE)
	{
		if (minirt->scene.amb_light.srv_skybox_wh.x
			&& minirt->scene.amb_light.srv_skybox_wh.y)
		{
			glDeleteTextures(1, &minirt->shaders_data.tex.cdf_mi_texture2d);
			minirt->shaders_data.tex.cdf_mi_texture2d
				= create_texture1d_from_data((float *)data, minirt->\
				scene.amb_light.srv_skybox_wh.y);
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_1D,
				minirt->shaders_data.tex.cdf_mi_texture2d);
			glUniform1i(glGetUniformLocation(minirt->shaders_data.program,
					"cdf_marginal_inverse"), 2);
		}
	}
	else if (type == SRV_TABLE_SIZE && size == sizeof(int))
	{
		minirt->shaders_data.scene.table_size = *(int *)data;
		glUniform1ui(glGetUniformLocation(minirt->shaders_data.program,
				"table_size"), minirt->shaders_data.scene.table_size);
	}
}

static void	receive_scene_if_forest8(t_minirt *minirt, uint16_t type,
	char *data, uint64_t size)
{
	if (type == SRV_CDF_CONDITIONAL_INVERSE)
	{
		if (minirt->scene.amb_light.srv_skybox_wh.x
			&& minirt->scene.amb_light.srv_skybox_wh.y)
		{
			glDeleteTextures(1, &minirt->shaders_data.tex.cdf_ci_texture2d);
			minirt->shaders_data.tex.cdf_ci_texture2d
				= create_texture2d_from_data((float *)data, minirt->\
	scene.amb_light.srv_skybox_wh.x, minirt->scene.amb_light.srv_skybox_wh.y);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D,
				minirt->shaders_data.tex.cdf_ci_texture2d);
			glUniform1i(glGetUniformLocation(minirt->shaders_data.program,
					"cdf_conditional_inverse"), 0);
		}
	}
	else
		receive_scene_if_forest9(minirt, type, data, size);
}

static void	receive_scene_if_forest7(t_minirt *minirt, uint16_t type,
	char *data, uint64_t size)
{
	if (type == SRV_PDF_JOINT)
	{
		if (minirt->scene.amb_light.srv_skybox_wh.x
			&& minirt->scene.amb_light.srv_skybox_wh.y)
		{
			glDeleteTextures(1, &minirt->shaders_data.tex.pdf_joint_texture2d);
			minirt->shaders_data.tex.pdf_joint_texture2d
				= create_texture2d_from_data((float *)data, minirt->\
	scene.amb_light.srv_skybox_wh.x, minirt->scene.amb_light.srv_skybox_wh.y);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D,
				minirt->shaders_data.tex.pdf_joint_texture2d);
			glUniform1i(glGetUniformLocation(minirt->shaders_data.program,
					"pdf_joint"), 1);
		}
	}
	else if (type == SRV_CAMERA && size == sizeof(t_gpu_camera))
	{
		minirt->shaders_data.scene.camera = *(t_gpu_camera *)data;
		camera_uniforms(minirt->shaders_data.program,
			&minirt->shaders_data.scene.camera);
	}
	else
		receive_scene_if_forest8(minirt, type, data, size);
}

void	receive_scene_if_forest6(t_minirt *minirt, uint16_t type, char *data,
	uint64_t size)
{
	if (type == SRV_AMBIANT && size == sizeof(t_gpu_amb_light))
	{
		minirt->shaders_data.scene.ambiant = *(t_gpu_amb_light *)data;
		ambiant_uniforms(minirt->shaders_data.program,
			&minirt->shaders_data.scene.ambiant);
	}
	else if (type == SRV_VIEWPORT && size == sizeof(t_gpu_viewport))
	{
		minirt->shaders_data.scene.viewport = *(t_gpu_viewport *)data;
		viewport_uniforms(minirt->shaders_data.program,
			&minirt->shaders_data.scene.viewport);
	}
	else if (type == SRV_MAX_BOUNCES && size == sizeof(double))
	{
		minirt->controls.max_bounces = *(double *)data;
		glUniform1ui(glGetUniformLocation(minirt->shaders_data.program,
				"max_bounces"), minirt->controls.max_bounces);
	}
	else
		receive_scene_if_forest7(minirt, type, data, size);
}
