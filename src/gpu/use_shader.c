/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   use_shader.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 16:31:23 by lilefebv          #+#    #+#             */
/*   Updated: 2025/07/29 10:32:10 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "gpu.h"
#include "minirt.h"

void	compute_frame_gpu(t_minirt *minirt)
{
	size_t	i;
	size_t	tpx;
	
	tpx = minirt->scene.render_width * minirt->scene.render_height;
	
	glUseProgram(minirt->shaders_data.program);
	GLuint sample_count = glGetUniformLocation(minirt->shaders_data.program, "sample_count");
	glUniform1ui(sample_count, minirt->screen.sample);
 
	glDispatchCompute(
		(minirt->scene.render_width + 7) / 8,
		(minirt->scene.render_height + 7) / 8,
		1
	);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, minirt->shaders_data.ssbo);
	float	*ptr = (float *)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, tpx * sizeof(float) * 4, GL_MAP_READ_BIT);
	if (!ptr)
	{
		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
		print_error("Failed to map GPU buffer");
		return ;
	}
	i = 0;
	// printf("Pixel 1072 600 : %f %f %f\n", minirt->screen.float_render[600 * 1920 + 1072].r, minirt->screen.float_render[600 * 1920 + 1072].g, minirt->screen.float_render[600 * 1920 + 1072].b);
	// printf("Pixel 1072 600 : %f %f %f\n", ptr[(600 * 1920 + 1072) * 4], ptr[(600 * 1920 + 1072) * 4 + 1], ptr[(600 * 1920 + 1072) * 4 + 2]);
	if (minirt->render_mode == 1) // BVH heatmap
	{
		while (i < tpx)
		{
			minirt->viewport.depth_buffer[i] = (int)(ptr[i * 4]);
			i++;
		}
	}
	else
	{
		while (i < tpx)
		{
			minirt->screen.float_render[i].r += ptr[i * 4];
			minirt->screen.float_render[i].g += ptr[i * 4 + 1];
			minirt->screen.float_render[i].b += ptr[i * 4 + 2];
			i++;
		}
	}
	//printf("Pixel 1072 600 : %f %f %f\n", minirt->screen.float_render[600 * 1920 + 1072].r, minirt->screen.float_render[600 * 1920 + 1072].g, minirt->screen.float_render[600 * 1920 + 1072].b);

	minirt->screen.sample++;
	minirt->screen.sample_total_anim++;
	minirt->screen.last_sample_am = minirt->screen.sample;
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);	
}
