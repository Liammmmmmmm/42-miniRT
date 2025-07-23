/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_shaders.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 16:28:59 by lilefebv          #+#    #+#             */
/*   Updated: 2025/07/23 09:24:20 by lilefebv         ###   ########lyon.fr   */
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
	gpu_structs->mat = NULL;
	delete_ssbo(&gpu_structs->mat_ssbo);

	free(gpu_structs->spheres);
	gpu_structs->spheres = NULL;
	delete_ssbo(&gpu_structs->spheres_ssbo);
}

void	clean_shaders(t_shader_data *shader_data)
{
	delete_ssbo(&shader_data->ssbo);
	clean_scene(&shader_data->scene);
	glDeleteProgram(shader_data->program);
	glfwDestroyWindow(shader_data->window);
	glfwTerminate();
}
