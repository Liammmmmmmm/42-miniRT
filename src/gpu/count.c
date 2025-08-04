/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 19:50:45 by lilefebv          #+#    #+#             */
/*   Updated: 2025/07/31 17:12:28 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gpu.h"

void	count_obj(t_scene *scene, t_gpu_structs *gpu_structs)
{
	uint32_t	i;

	i = 0;
	while (i < scene->bvh.size)
	{
		if (scene->bvh.obj_list[i]->type == SPHERE)
			gpu_structs->spheres_am++;
		else if (scene->bvh.obj_list[i]->type == CYLINDER)
			gpu_structs->cylinders_am++;
		else if (scene->bvh.obj_list[i]->type == CONE)
			gpu_structs->cones_am++;
		else if (scene->bvh.obj_list[i]->type == HYPERBOLOID)
			gpu_structs->hypers_am++;
		else if (scene->bvh.obj_list[i]->type == TRIANGLE)
			gpu_structs->triangles_am++;
		i++;
	}
}

void	add_image_size(t_scene *scene, t_gpu_textures *gpu_textures, int i)
{
	if (scene->textures[i].type == IMAGE)
	{
		if (scene->textures[i].img.rgba == NULL)
			return ;
		gpu_textures->total_pixel_images += scene->textures[i].img.height
			* scene->textures[i].img.width;
	}
	else
	{
		if (scene->textures[i].hdr.pixels == NULL)
			return ;
		gpu_textures->total_pixel_images += scene->textures[i].hdr.height
			* scene->textures[i].hdr.width;
	}
}

void	count_tex(t_scene *scene, t_gpu_textures *gpu_textures)
{
	int	i;

	i = -1;
	while (++i < scene->tex_amount)
	{
		if (scene->textures[i].type == CHECKER_GLOBAL
			|| scene->textures[i].type == CHECKER_LOCAL)
			gpu_textures->checkers_am++;
		if (scene->textures[i].type == HDR || scene->textures[i].type == IMAGE)
		{
			gpu_textures->images_am++;
			add_image_size(scene, gpu_textures, i);
		}
	}
}
