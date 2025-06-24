/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   use_shader.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 16:31:23 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/24 17:35:22 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "gpu.h"
#include "minirt.h"

void	compute_frame_gpu(t_minirt *minirt)
{
	size_t			i;
	const size_t	tpx = minirt->scene.render_width * minirt->scene.render_height;

	glUseProgram(minirt->shaders_data.program);
	GLuint width_loc = glGetUniformLocation(minirt->shaders_data.program, "width_render");
	glUniform1ui(width_loc, minirt->scene.render_width);
	glDispatchCompute(minirt->scene.render_width, minirt->scene.render_height, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, minirt->shaders_data.ssbo);
	float	*ptr = (float *)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, tpx * sizeof(float) * 3, GL_MAP_READ_BIT);
	if (!ptr)
	{
		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
		print_error("Failed to map GPU buffer");
		return ;
	}
	i = 0;
	while (i < tpx)
	{
		printf("%f %f %f\n", ptr[i * 3], ptr[i * 3], ptr[i * 3]);
		minirt->screen.float_render[i].r += ptr[i * 3];
		minirt->screen.float_render[i].g += ptr[i * 3 + 1];
		minirt->screen.float_render[i].b += ptr[i * 3 + 2];
		i++;
	}
	minirt->screen.sample++;
	minirt->screen.sample_total_anim++;
	minirt->screen.last_sample_am = minirt->screen.sample;
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);	
}
