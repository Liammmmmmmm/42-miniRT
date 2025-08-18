/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert_textures2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 19:28:11 by lilefebv          #+#    #+#             */
/*   Updated: 2025/08/18 17:35:15 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "gpu.h"

void	convert_checker(t_scene *scene, t_gpu_textures *gpu, int *check_i,
	int i)
{
	gpu->textures_types_indices[i].indice = *check_i;
	gpu->checkers[*check_i].scale = scene->textures[i].checker.scale;
	color_to_float3(&scene->textures[i].checker.c1, gpu->checkers[*check_i].c1);
	color_to_float3(&scene->textures[i].checker.c2, gpu->checkers[*check_i].c2);
	(*check_i)++;
}
