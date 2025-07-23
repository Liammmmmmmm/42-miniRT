/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_shaders.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 16:28:59 by lilefebv          #+#    #+#             */
/*   Updated: 2025/07/23 14:13:50 by madelvin         ###   ########.fr       */
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

void	clean_scene(t_gpu_structs *gpu_structs)
{
	delete_ssbo(&gpu_structs->viewport_ssbo);
	delete_ssbo(&gpu_structs->camera_ssbo);
	free(gpu_structs->mat);
	delete_ssbo(&gpu_structs->mat_ssbo);

	free(gpu_structs->spheres);
	delete_ssbo(&gpu_structs->spheres_ssbo);
	
	free(gpu_structs->hypers);
	delete_ssbo(&gpu_structs->hypers_ssbo);

	free(gpu_structs->cylinders);
	delete_ssbo(&gpu_structs->cylinders_ssbo);

	free(gpu_structs->cones);
	delete_ssbo(&gpu_structs->cones_ssbo);

	free(gpu_structs->triangles);
	delete_ssbo(&gpu_structs->triangles_ssbo);

	free(gpu_structs->prim_indice);
	delete_ssbo(&gpu_structs->prim_indice_ssbo);

	free(gpu_structs->prim_type);
	delete_ssbo(&gpu_structs->prim_type_ssbo);

	ft_bzero(gpu_structs, sizeof(t_gpu_structs));
}

void	clean_shaders(t_shader_data *shader_data)
{
	delete_ssbo(&shader_data->ssbo);
	clean_scene(&shader_data->scene);
	glDeleteProgram(shader_data->program);
	glfwDestroyWindow(shader_data->window);
	glfwTerminate();
}
