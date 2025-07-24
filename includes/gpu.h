/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gpu.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 15:19:55 by lilefebv          #+#    #+#             */
/*   Updated: 2025/07/24 10:37:29 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef GPU_H
# define GPU_H

#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include "libft.h"
#include "utils.h"
#include "gpu_struct.h"

void	init_ssbo(t_shader_data *shader_data, size_t render_width,
	size_t render_height);
int		init_shader(t_shader_data *shader_data, size_t render_width,
	size_t render_height, t_scene *scene);
void	compute_frame_gpu(t_minirt *minirt);

void	delete_ssbo(GLuint *ssbo);
void	clean_scene(t_gpu_structs *gpu_structs);
void	clean_shaders(t_shader_data *shader_data);

#endif
