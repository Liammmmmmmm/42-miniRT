/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   send_uniforms.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 17:43:45 by lilefebv          #+#    #+#             */
/*   Updated: 2025/07/29 18:47:46 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "gpu.h"

#define VP "viewport."

void	viewport_uniforms(GLuint p, t_gpu_viewport *vp)
{
	glUniform3fv(glGetUniformLocation(p, VP"u"), 1, vp->u);
	glUniform3fv(glGetUniformLocation(p, VP"v"), 1, vp->v);
	glUniform3fv(glGetUniformLocation(p, VP"pixel_delta_u"), 1,
		vp->pixel_delta_u);
	glUniform3fv(glGetUniformLocation(p, VP"pixel_delta_v"), 1,
		vp->pixel_delta_v);
	glUniform3fv(glGetUniformLocation(p, VP"defocus_disk_u"), 1,
		vp->defocus_disk_u);
	glUniform3fv(glGetUniformLocation(p, VP"defocus_disk_v"), 1,
		vp->defocus_disk_v);
	glUniform3fv(glGetUniformLocation(p, VP"upper_left"), 1, vp->upper_left);
	glUniform3fv(glGetUniformLocation(p, VP"pixel00_loc"), 1, vp->pixel00_loc);
	glUniform1f(glGetUniformLocation(p, VP"height"), vp->height);
	glUniform1f(glGetUniformLocation(p, VP"width"), vp->width);
	glUniform1i(glGetUniformLocation(p, VP"render_w"), vp->render_w);
	glUniform1i(glGetUniformLocation(p, VP"render_h"), vp->render_h);
	glUniform1f(glGetUniformLocation(p, VP"focal_length"), vp->focal_length);
	glUniform1f(glGetUniformLocation(p, VP"gamma"), vp->gamma);
	glUniform1f(glGetUniformLocation(p, VP"defocus_radius"), vp->defocus_radius)
	;
	glUniform1i(glGetUniformLocation(p, VP"max_bounces"), vp->max_bounces);
	glUniform1f(glGetUniformLocation(p, VP"ior_global"), vp->ior_global);
}

void	camera_uniforms(GLuint p, t_gpu_camera *c)
{
	glUniform3fv(glGetUniformLocation(p, "cam.position"), 1, c->position);
	glUniform3fv(glGetUniformLocation(p, "cam.orientation"), 1, c->orientation);
	glUniform1f(glGetUniformLocation(p, "cam.fov"), c->fov);
	glUniform1f(glGetUniformLocation(p, "cam.defocus_angle"), c->defocus_angle);
	glUniform1f(glGetUniformLocation(p, "cam.focus_dist"), c->focus_dist);
}

void	ambiant_uniforms(GLuint p, t_gpu_amb_light *a)
{
	glUniform3fv(glGetUniformLocation(p, "ambiant.skybox_color"), 1,
		a->skybox_color);
	glUniform1f(glGetUniformLocation(p, "ambiant.ratio"), a->ratio);
	glUniform1i(glGetUniformLocation(p, "ambiant.skybox_tex_index"),
		a->skybox_tex_index);
}

void	send_uniforms(t_minirt *minirt)
{
	const GLuint	prog = minirt->shaders_data.program;

	glUseProgram(prog);
	glUniform1ui(glGetUniformLocation(prog, "width_render"),
		minirt->scene.render_width);
	glUniform1ui(glGetUniformLocation(prog, "height_render"),
		minirt->scene.render_height);
	glUniform1ui(glGetUniformLocation(prog, "max_bounces"),
		(t_uint)minirt->controls.max_bounces);
	glUniform1ui(glGetUniformLocation(prog, "render_mode"),
		(t_uint)minirt->render_mode);
	glUniform1ui(glGetUniformLocation(prog, "planes_am"),
		(t_uint)minirt->shaders_data.scene.planes_am);
	glUniform1ui(glGetUniformLocation(prog, "lights_am"),
		(t_uint)minirt->shaders_data.scene.lights_am);
	camera_uniforms(prog, &minirt->shaders_data.scene.camera);
	ambiant_uniforms(prog, &minirt->shaders_data.scene.ambiant);
	viewport_uniforms(prog, &minirt->shaders_data.scene.viewport);
}
