/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   use_shader.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 16:31:23 by lilefebv          #+#    #+#             */
/*   Updated: 2025/07/24 15:15:01 by lilefebv         ###   ########lyon.fr   */
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
	GLuint cylinders_am = glGetUniformLocation(minirt->shaders_data.program, "cylinders_am");
	glUniform1ui(cylinders_am, (t_uint)minirt->shaders_data.scene.cylinders_am);
	GLuint cones_am = glGetUniformLocation(minirt->shaders_data.program, "cones_am");
	glUniform1ui(cones_am, (t_uint)minirt->shaders_data.scene.cones_am);
	GLuint hypers_am = glGetUniformLocation(minirt->shaders_data.program, "hypers_am");
	glUniform1ui(hypers_am, (t_uint)minirt->shaders_data.scene.hypers_am);
	GLuint triangles_am = glGetUniformLocation(minirt->shaders_data.program, "triangles_am");
	glUniform1ui(triangles_am, (t_uint)minirt->shaders_data.scene.triangles_am);

	glUniform3fv(glGetUniformLocation(minirt->shaders_data.program, "cam.position"), 1, minirt->shaders_data.scene.camera.position);
	glUniform3fv(glGetUniformLocation(minirt->shaders_data.program, "cam.orientation"), 1, minirt->shaders_data.scene.camera.orientation);
	glUniform1f(glGetUniformLocation(minirt->shaders_data.program, "cam.fov"), minirt->shaders_data.scene.camera.fov);
	glUniform1f(glGetUniformLocation(minirt->shaders_data.program, "cam.defocus_angle"), minirt->shaders_data.scene.camera.defocus_angle);
	glUniform1f(glGetUniformLocation(minirt->shaders_data.program, "cam.focus_dist"), minirt->shaders_data.scene.camera.focus_dist);

	glUniform3fv(glGetUniformLocation(minirt->shaders_data.program, "viewport.u"), 1, minirt->shaders_data.scene.viewport.u);
	glUniform3fv(glGetUniformLocation(minirt->shaders_data.program, "viewport.v"), 1, minirt->shaders_data.scene.viewport.v);
	glUniform3fv(glGetUniformLocation(minirt->shaders_data.program, "viewport.pixel_delta_u"), 1, minirt->shaders_data.scene.viewport.pixel_delta_u);
	glUniform3fv(glGetUniformLocation(minirt->shaders_data.program, "viewport.pixel_delta_v"), 1, minirt->shaders_data.scene.viewport.pixel_delta_v);
	glUniform3fv(glGetUniformLocation(minirt->shaders_data.program, "viewport.defocus_disk_u"), 1, minirt->shaders_data.scene.viewport.defocus_disk_u);
	glUniform3fv(glGetUniformLocation(minirt->shaders_data.program, "viewport.defocus_disk_v"), 1, minirt->shaders_data.scene.viewport.defocus_disk_v);
	glUniform3fv(glGetUniformLocation(minirt->shaders_data.program, "viewport.upper_left"), 1, minirt->shaders_data.scene.viewport.upper_left);
	glUniform3fv(glGetUniformLocation(minirt->shaders_data.program, "viewport.pixel00_loc"), 1, minirt->shaders_data.scene.viewport.pixel00_loc);
	glUniform1f(glGetUniformLocation(minirt->shaders_data.program, "viewport.height"), minirt->shaders_data.scene.viewport.height);
	glUniform1f(glGetUniformLocation(minirt->shaders_data.program, "viewport.width"), minirt->shaders_data.scene.viewport.width);
	glUniform1f(glGetUniformLocation(minirt->shaders_data.program, "viewport.render_w"), minirt->shaders_data.scene.viewport.render_w);
	glUniform1f(glGetUniformLocation(minirt->shaders_data.program, "viewport.render_h"), minirt->shaders_data.scene.viewport.render_h);
	glUniform1f(glGetUniformLocation(minirt->shaders_data.program, "viewport.focal_length"), minirt->shaders_data.scene.viewport.focal_length);
	glUniform1f(glGetUniformLocation(minirt->shaders_data.program, "viewport.gamma"), minirt->shaders_data.scene.viewport.gamma);
	glUniform1f(glGetUniformLocation(minirt->shaders_data.program, "viewport.defocus_radius"), minirt->shaders_data.scene.viewport.defocus_radius);
	glUniform1f(glGetUniformLocation(minirt->shaders_data.program, "viewport.max_bounces"), minirt->shaders_data.scene.viewport.max_bounces);
	glUniform1f(glGetUniformLocation(minirt->shaders_data.program, "viewport.ior_global"), minirt->shaders_data.scene.viewport.ior_global);

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
