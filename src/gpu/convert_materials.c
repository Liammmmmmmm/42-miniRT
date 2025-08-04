/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert_materials.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 19:25:29 by lilefebv          #+#    #+#             */
/*   Updated: 2025/07/29 19:27:19 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "gpu.h"

static void	convert_material2(t_gpu_material *dst, t_mat *src, t_scene *scene)
{
	dst->ao_value = src->ao_value;
	dst->ao_tex_index = src->ao_tex - scene->textures;
	if (src->ao_tex == NULL)
		dst->ao_tex_index = -1;
	dst->emission_strength = src->emission_strength;
	dst->emission_strength_tex_index = src->emission_strength_tex
		- scene->textures;
	if (src->emission_strength_tex == NULL)
		dst->emission_strength_tex_index = -1;
	dst->scale = src->scale;
	fcolor_to_float3(&src->emission_color, dst->emission_color);
	dst->emission_color_tex_index = src->emission_color_tex - scene->textures;
	if (src->emission_color_tex == NULL)
		dst->emission_color_tex_index = -1;
	dst->normal_tex_index = src->normal - scene->textures;
	if (src->normal == NULL)
		dst->normal_tex_index = -1;
	dst->normal_intensity = src->normal_intensity;
}

void	convert_material(t_gpu_material *dst, t_mat *src, t_scene *scene)
{
	color_to_float3(&src->color_value, dst->color_value);
	dst->color_tex_index = src->color_tex - scene->textures;
	if (src->color_tex == NULL)
		dst->color_tex_index = -1;
	dst->metallic_value = src->metallic_value;
	dst->metallic_tex_index = src->metallic_tex - scene->textures;
	if (src->metallic_tex == NULL)
		dst->metallic_tex_index = -1;
	dst->roughness_value = src->roughness_value;
	dst->roughness_tex_index = src->roughness_tex - scene->textures;
	if (src->roughness_tex == NULL)
		dst->roughness_tex_index = -1;
	dst->ior = src->ior;
	dst->transmission_value = src->transmission_value;
	dst->transmission_tex_index = src->transmission_tex - scene->textures;
	if (src->transmission_tex == NULL)
		dst->transmission_tex_index = -1;
	convert_material2(dst, src, scene);
}

void	convert_materials(t_scene *cpu_scene, t_gpu_material *gpu)
{
	int	i;

	i = -1;
	while (++i < cpu_scene->mat_amount)
		convert_material(&gpu[i], &cpu_scene->materials[i], cpu_scene);
}
