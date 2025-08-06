/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   use_shader.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 16:31:23 by lilefebv          #+#    #+#             */
/*   Updated: 2025/08/06 15:49:37 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "gpu.h"
#include "minirt.h"
#include "network.h"

static int	get_result(t_minirt *m)
{
	int			i;
	const int	tpx = m->scene.render_width * m->scene.render_height;
	printf("TPX : %d\n", tpx);
	const float	*ptr = (float *)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0,
			tpx * sizeof(float) * 4, GL_MAP_READ_BIT);

	if (!ptr)
	{
		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
		print_error("Failed to map GPU buffer");
		return (1);
	}

	if (m->options.client.enabled)
	{
		send_frame_to_server(ptr, tpx, 1, m->options.client.sockfd);
		return (0);
	}

	i = -1;
	if (m->render_mode == 1)
		while (++i < tpx)
			m->viewport.depth_buffer[i] = (int)(ptr[i * 4]);
	else
	{
		while (++i < tpx)
		{
			m->screen.float_render[i].r += ptr[i * 4];
			m->screen.float_render[i].g += ptr[i * 4 + 1];
			m->screen.float_render[i].b += ptr[i * 4 + 2];
		}
	}
	return (0);
}

void	compute_frame_gpu(t_minirt *m)
{
	glUseProgram(m->shaders_data.program);
	if (m->options.client.enabled)
		glUniform1ui(glGetUniformLocation(m->shaders_data.program,
				"sample_count"), rand());
	else
		glUniform1ui(glGetUniformLocation(m->shaders_data.program,
				"sample_count"), m->screen.sample);
	glDispatchCompute(
		(m->scene.render_width + 7) / 8, (m->scene.render_height + 7) / 8, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m->shaders_data.ssbo);
	if (get_result(m))
		return ;
	m->screen.sample++;
	m->screen.sample_total_anim++;
	m->screen.last_sample_am = m->screen.sample;
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
}
