/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gpu_struct.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 15:19:55 by lilefebv          #+#    #+#             */
/*   Updated: 2025/08/21 21:00:57 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GPU_STRUCT_H
# define GPU_STRUCT_H

# include <GLFW/glfw3.h>
# include "gpu_scene.h"

typedef struct s_shader_data
{
	GLFWwindow		*window;
	GLuint			program;
	GLuint			ssbo;
	t_gpu_structs	scene;
	t_gpu_textures	tex;
}	t_shader_data;

#endif
