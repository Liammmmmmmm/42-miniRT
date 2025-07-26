/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_shaders.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 16:28:59 by lilefebv          #+#    #+#             */
/*   Updated: 2025/07/26 17:24:45 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "gpu.h"
#include "minirt.h"

void	delete_ssbo(GLuint *ssbo)
{
    if (*ssbo && *ssbo != 0)
    {
        glDeleteBuffers(1, ssbo);
        *ssbo = 0;
    }
}

void	clean_importance_sampling(t_gpu_textures *gpu_tex)
{
	glDeleteTextures(1, &gpu_tex->cdf_ci_texture2d);
	glDeleteTextures(1, &gpu_tex->pdf_joint_texture2d);
	glDeleteTextures(1, &gpu_tex->cdf_mi_texture2d);
}

void	clean_scene(t_gpu_structs *gpu_structs)
{
	free(gpu_structs->mat);
	delete_ssbo(&gpu_structs->mat_ssbo);

	free(gpu_structs->spheres);
	delete_ssbo(&gpu_structs->spheres_ssbo);

	free(gpu_structs->planes);
	delete_ssbo(&gpu_structs->planes_ssbo);
	
	free(gpu_structs->hypers);
	delete_ssbo(&gpu_structs->hypers_ssbo);

	free(gpu_structs->cylinders);
	delete_ssbo(&gpu_structs->cylinders_ssbo);

	free(gpu_structs->cones);
	delete_ssbo(&gpu_structs->cones_ssbo);

	free(gpu_structs->triangles);
	delete_ssbo(&gpu_structs->triangles_ssbo);

	free(gpu_structs->lights);
	delete_ssbo(&gpu_structs->lights_ssbo);

	free(gpu_structs->prim_types_indices);
	delete_ssbo(&gpu_structs->prim_types_indices_ssbo);

	free(gpu_structs->bvh_node);
	delete_ssbo(&gpu_structs->bvh_node_ssbo);

	ft_bzero(gpu_structs, sizeof(t_gpu_structs));
}

void	clean_tex(t_gpu_textures *gpu_tex)
{
	free(gpu_tex->images);
	delete_ssbo(&gpu_tex->images_ssbo);
	free(gpu_tex->checkers);
	delete_ssbo(&gpu_tex->checkers_ssbo);
	free(gpu_tex->textures_types_indices);
	delete_ssbo(&gpu_tex->textures_types_indices_ssbo);
}

void	clean_shaders(t_shader_data *shader_data)
{
	delete_ssbo(&shader_data->ssbo);
	clean_scene(&shader_data->scene);
	
	delete_ssbo(&shader_data->tex.checkers_ssbo);
	delete_ssbo(&shader_data->tex.images_ssbo);
	delete_ssbo(&shader_data->tex.images_stream_ssbo);
	delete_ssbo(&shader_data->tex.textures_types_indices_ssbo);

	glDeleteProgram(shader_data->program);
	glfwDestroyWindow(shader_data->window);
	glfwTerminate();
}
