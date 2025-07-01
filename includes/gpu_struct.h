/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gpu_struct.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 15:19:55 by lilefebv          #+#    #+#             */
/*   Updated: 2025/07/01 17:00:55 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef GPU_STRUCT_H
# define GPU_STRUCT_H

#include <GLFW/glfw3.h>
#include "gpu_scene.h"

typedef struct s_shader_data
{
	GLFWwindow		*window;
	GLuint			program;
	GLuint			ssbo;
	t_gpu_structs	scene;
}	t_shader_data;

#endif
