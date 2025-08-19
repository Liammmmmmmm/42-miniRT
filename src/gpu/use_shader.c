/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   use_shader.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 16:31:23 by lilefebv          #+#    #+#             */
/*   Updated: 2025/08/19 10:05:20 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "gpu.h"
#include "minirt.h"
#include "network.h"

static void	float3_add(float *c1, const float *c2)
{
	c1[0] += c2[0];
	c1[1] += c2[1];
	c1[2] += c2[2];
}

static int	is_client_render(const int tpx, t_minirt *m, const float *ptr)
{
	ssize_t	current_time;
	int		i;

	i = -1;
	while (++i < tpx)
		float3_add(&m->screen.client_accumulation[i * 3], &ptr[i * 4]);
	m->screen.client_samples++;
	current_time = get_cpu_time();
	if (!(current_time
			> m->screen.client_last_sample_send + CLIENT_ACCUMULATION_TIME))
		return (0);
	if (send_frame_to_server(m->screen.client_accumulation, (t_sample_net_data)
			{tpx, m->options.anim.frame_i, m->screen.client_samples},
			m->options.client.sockfd) < 0)
	{
		if (passive_mode(m->options.client.sockfd, m) < 0)
			return (print_errorm1("failed to enable passive mode"));
		else
			return (1);
	}
	m->screen.client_samples = 0;
	m->screen.client_last_sample_send = current_time;
	ft_bzero(m->screen.client_accumulation, sizeof(float) * 3
		* m->scene.render_width * m->scene.render_height);
	return (0);
}

static int	get_result(t_minirt *m)
{
	int			i;
	const int	tpx = m->scene.render_width * m->scene.render_height;
	const float	*ptr = (float *)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0,
			tpx * sizeof(float) * 4, GL_MAP_READ_BIT);

	if (!ptr)
	{
		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
		print_error("Failed to map GPU buffer");
		return (1);
	}
	if (m->options.client.enabled)
		return (is_client_render(tpx, m, ptr));
	i = -1;
	if (m->render_mode == 1)
		while (++i < tpx)
			m->viewport.depth_buffer[i] = (int)(ptr[i * 4]);
	else
	{
		while (++i < tpx)
			m->screen.float_render[i] = add_fcolor_float3(
					m->screen.float_render[i], &ptr[i * 4]);
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
