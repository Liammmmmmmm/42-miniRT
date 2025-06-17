/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_material.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 12:48:08 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/11 17:40:31 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_mat	create_default_mat(void)
{
	t_mat	mat;

	ft_bzero(&mat, sizeof(t_mat));
	mat.ior = 1.45;
	ft_strlcpy(mat.name, "Default mat", 21);
	mat.ao_value = 1;
	mat.color_value = (t_color){255, 255, 255};
	mat.normal_intensity = 1;
	mat.scale = 1;
	return (mat);
}

void	change_custom_obj_mat(t_minirt *minirt, t_mat *new, int i)
{
	size_t			j;
	t_custom_object	*obj;

	obj = (t_custom_object *)minirt->scene.elements[i].object;
	j = 0;
	while (j < obj->triangle_count)
	{
		obj->triangles->material
			= (obj->material - &minirt->scene.materials[0]) + &new[1];
		j++;
	}
}

void	change_primitive_mat(t_minirt *minirt, t_mat *new, int i)
{
	if (minirt->scene.elements[i].type == SPHERE)
		((t_sphere *)minirt->scene.elements->object)->material
			= (((t_sphere *)minirt->scene.elements[i].object)->material
				- &minirt->scene.materials[0]) + &new[1];
	if (minirt->scene.elements->type == CYLINDER)
		((t_cylinder *)minirt->scene.elements->object)->material
			= (((t_cylinder *)minirt->scene.elements[i].object)->material
				- &minirt->scene.materials[0]) + &new[1];
	if (minirt->scene.elements->type == PLANE)
		((t_plane *)minirt->scene.elements->object)->material
			= (((t_plane *)minirt->scene.elements[i].object)->material
				- &minirt->scene.materials[0]) + &new[1];
	if (minirt->scene.elements->type == CONE)
		((t_cone *)minirt->scene.elements->object)->material
			= (((t_cone *)minirt->scene.elements[i].object)->material
				- &minirt->scene.materials[0]) + &new[1];
	if (minirt->scene.elements->type == HYPERBOLOID)
		((t_hyperboloid *)minirt->scene.elements->object)->material
			= (((t_hyperboloid *)minirt->scene.elements[i].object)->material
				- &minirt->scene.materials[0]) + &new[1];
	if (minirt->scene.elements->type == TRIANGLE)
		((t_triangle *)minirt->scene.elements->object)->material
			= (((t_triangle *)minirt->scene.elements[i].object)->material
				- &minirt->scene.materials[0]) + &new[1];
}

void	change_all_material(t_minirt *minirt, t_mat *new)
{
	int	i;

	printf("entry\n");
	(void)new;
	i = 0;
	while (i < minirt->scene.el_amount)
	{
		change_primitive_mat(minirt, new, i);
		if (minirt->scene.elements->type == CUSTOM)
			change_custom_obj_mat(minirt, new, i);
		i++;
	}
}

void	add_material(t_minirt *minirt)
{
	t_mat	*new;
	int		i;

	new = malloc((minirt->scene.mat_amount + 1) * sizeof(t_mat));
	if (!new)
		return ((void)print_error("Can allocate more space for a material"));
	i = 0;
	while (i < minirt->scene.mat_amount)
	{
		new[i + 1] = minirt->scene.materials[i];
		i++;
	}
	minirt->scene.mat_amount++;
	new[0] = create_default_mat();
	change_all_material(minirt, new);
	free(minirt->scene.materials);
	minirt->scene.materials = new;
	minirt->controls.ui_infos.selected_material = &new[0];
	set_selected_mat(minirt, &new[0]);
}
