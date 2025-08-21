/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   importance_sampling.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 18:49:12 by lilefebv          #+#    #+#             */
/*   Updated: 2025/08/21 22:02:26 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gpu.h"

/*
Good to know:
glTexImage2D(
	GL_TEXTURE_2D,	// Target
	0,				// Mipmap level
	GL_R32F,		// Format interne (1 canal float)
	width, height,	// Dimensions
	0,				// Border (doit être 0)
	GL_RED,			// Format des données (1 canal)
	GL_FLOAT,		// Type des données
	data			// Pointeur vers les données
);
*/
GLuint	create_texture2d_from_data(const float *data, int width, int height)
{
	GLuint	texture_id;

	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, width, height, 0, GL_RED, GL_FLOAT,
		data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	return (texture_id);
}

/*
Good to know:
glTexImage1D(
	GL_TEXTURE_1D,	// Target
	0,				// Mipmap level
	GL_R32F,		// Format interne (1 canal float)
	width,			// Taille de la texture (1 dimension)
	0,				// Border (doit être 0)
	GL_RED,			// Format des données (1 canal)
	GL_FLOAT,		// Type des données
	data			// Pointeur vers les données
);
*/
GLuint	create_texture1d_from_data(const float *data, int width)
{
	GLuint	texture_id;

	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_1D, texture_id);
	glTexImage1D(GL_TEXTURE_1D, 0, GL_R32F, width, 0, GL_RED, GL_FLOAT, data);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	return (texture_id);
}

void	send_importance_sampling(t_minirt *minirt, t_scene *scene)
{
	t_point2	p;

	clean_importance_sampling(&minirt->shaders_data.tex);
	if (!(scene->amb_light.skybox_t && scene->amb_light.skybox_t->hdr.pixels))
		return ;
	p.x = scene->amb_light.skybox_t->hdr.width;
	p.y = scene->amb_light.skybox_t->hdr.height;
	minirt->shaders_data.tex.cdf_ci_texture2d = create_texture2d_from_data(
			scene->amb_light.cdf_conditional_inverse, p.x, p.y);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, minirt->shaders_data.tex.cdf_ci_texture2d);
	glUniform1i(glGetUniformLocation(minirt->shaders_data.program,
			"cdf_conditional_inverse"), 0);
	minirt->shaders_data.tex.cdf_mi_texture2d = create_texture1d_from_data(
			scene->amb_light.cdf_marginal_inverse, p.y);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_1D, minirt->shaders_data.tex.cdf_mi_texture2d);
	glUniform1i(glGetUniformLocation(minirt->shaders_data.program,
			"cdf_marginal_inverse"), 2);
	minirt->shaders_data.tex.pdf_joint_texture2d = create_texture2d_from_data(
			scene->amb_light.pdf_joint, p.x, p.y);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, minirt->shaders_data.tex.pdf_joint_texture2d);
	glUniform1i(glGetUniformLocation(minirt->shaders_data.program, "pdf_joint"),
		1);
}
