/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   use_shader.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 16:31:23 by lilefebv          #+#    #+#             */
/*   Updated: 2025/07/22 21:16:23 by madelvin         ###   ########.fr       */
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
	GLuint width_loc = glGetUniformLocation(minirt->shaders_data.program, "width_render");
	glUniform1ui(width_loc, minirt->scene.render_width);
	GLuint height_loc = glGetUniformLocation(minirt->shaders_data.program, "height_render");
	glUniform1ui(height_loc, minirt->scene.render_height);
	GLuint sample_count = glGetUniformLocation(minirt->shaders_data.program, "sample_count");
	glUniform1ui(sample_count, minirt->screen.sample);
	GLuint max_bounces = glGetUniformLocation(minirt->shaders_data.program, "max_bounces");
	glUniform1ui(max_bounces, (t_uint)minirt->controls.max_bounces);

	GLuint spheres_am = glGetUniformLocation(minirt->shaders_data.program, "spheres_am");
	glUniform1ui(spheres_am, (t_uint)minirt->shaders_data.scene.spheres_am);

	GLuint planes_am = glGetUniformLocation(minirt->shaders_data.program, "planes_am");
	glUniform1ui(planes_am, (t_uint)minirt->shaders_data.scene.planes_am);

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
	while (i < tpx)
	{
		minirt->screen.float_render[i].r += ptr[i * 4];
		minirt->screen.float_render[i].g += ptr[i * 4 + 1];
		minirt->screen.float_render[i].b += ptr[i * 4 + 2];
		i++;
	}
	//printf("Pixel 1072 600 : %f %f %f\n", minirt->screen.float_render[600 * 1920 + 1072].r, minirt->screen.float_render[600 * 1920 + 1072].g, minirt->screen.float_render[600 * 1920 + 1072].b);

	minirt->screen.sample++;
	minirt->screen.sample_total_anim++;
	minirt->screen.last_sample_am = minirt->screen.sample;
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);	
}
