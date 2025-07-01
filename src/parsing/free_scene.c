/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_scene.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 13:42:28 by madelvin          #+#    #+#             */
/*   Updated: 2025/07/01 16:26:46 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "importance_sampling.h"
#include "caustic.h"

void	free_tex_mat(t_scene *scene)
{
	int	i;

	if (scene->textures)
	{
		i = -1;
		while (++i < scene->tex_amount)
		{
			free(scene->textures[i].img.rgba);
			free(scene->textures[i].hdr.pixels);
		}
		free(scene->textures);
		scene->textures = NULL;
	}
	free(scene->materials);
	scene->materials = NULL;
}

void	free_bvh_obj_lst(t_scene *scene)
{
	if (scene->obj_lst.light_nb != 0 && scene->obj_lst.light_lst)
		free(scene->obj_lst.light_lst);
	if (scene->obj_lst.plane_nb != 0 && scene->obj_lst.plane_lst)
		free(scene->obj_lst.plane_lst);
	if (scene->bvh.valid == 1)
	{
		free(scene->bvh.bvh_nodes);
		free(scene->bvh.obj_list);
		free(scene->bvh.prim_indices);
	}
	scene->obj_lst.light_lst = NULL;
	scene->obj_lst.plane_lst = NULL;
	scene->bvh.bvh_nodes = NULL;
	scene->bvh.obj_list = NULL;
	scene->bvh.prim_indices = NULL;
}

static void	free_scene_element(t_scene *scene)
{
	int	i;

	i = -1;
	while (++i < scene->el_amount)
	{
		if (scene->elements[i].type == CUSTOM)
		{
			free(((t_custom_object *)scene->elements[i].object)->name);
			free(((t_custom_object *)scene->elements[i].object)->triangles);
			free(((t_custom_object *)scene->elements[i].object)->obj_list);
			((t_custom_object *)scene->elements[i].object)->name = NULL;
			((t_custom_object *)scene->elements[i].object)->triangles = NULL;
			((t_custom_object *)scene->elements[i].object)->obj_list = NULL;
		}
		free(scene->elements[i].object);
		scene->elements[i].object = NULL;
		scene->elements[i].type = NULL_OBJ;
	}
	free(scene->elements);
	scene->elements = NULL;
}

int	free_scene(t_scene *scene, char **lines)
{
	if (lines)
		ft_free_tab_null_term(lines);
	if (scene->elements)
		free_scene_element(scene);
	free_tex_mat(scene);
	free_bvh_obj_lst(scene);
	free_importance_sampling_malloc(scene);
	kd_tree_destroy(&scene->photon_map);
	return (0);
}
