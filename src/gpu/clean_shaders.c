/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_shaders.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 16:28:59 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/24 17:01:35 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "gpu.h"
#include "minirt.h"

void	clean_shaders(t_shader_data *shader_data)
{
	glDeleteBuffers(1, &shader_data->ssbo);
	glDeleteProgram(shader_data->program);
	glfwDestroyWindow(shader_data->window);
	glfwTerminate();
}
