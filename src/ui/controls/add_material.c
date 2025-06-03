/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_material.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 12:48:08 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/03 13:20:37 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_mat	create_default_mat()
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
	free(minirt->scene.materials);
	minirt->scene.materials = new;
	minirt->controls.ui_infos.selected_material = &new[0];
	set_selected_mat(minirt, &new[0]);
}
