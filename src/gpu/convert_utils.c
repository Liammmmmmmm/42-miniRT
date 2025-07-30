/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 19:17:24 by lilefebv          #+#    #+#             */
/*   Updated: 2025/07/29 19:50:19 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "gpu.h"

void	vec3_to_float3(t_vec3 *vec, float *tab)
{
	tab[0] = vec->x;
	tab[1] = vec->y;
	tab[2] = vec->z;
}

void	vert_to_gpu_vert(t_vertex *ver, t_gpu_vertex *gpu_ver)
{
	vec3_to_float3(&ver->normal, gpu_ver->normal);
	vec3_to_float3(&ver->pos, gpu_ver->pos);
	gpu_ver->u = ver->u;
	gpu_ver->v = ver->v;
}

void	color_to_float3(t_color *vec, float *tab)
{
	tab[0] = vec->r / 255.0;
	tab[1] = vec->g / 255.0;
	tab[2] = vec->b / 255.0;
}

void	fcolor_to_float3(t_fcolor *vec, float *tab)
{
	tab[0] = vec->r;
	tab[1] = vec->g;
	tab[2] = vec->b;
}

void	create_ssbo(GLuint *ssbo, size_t size, void *data, long bind)
{
	glGenBuffers(1, ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, *ssbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, GL_STATIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bind, *ssbo);
}
