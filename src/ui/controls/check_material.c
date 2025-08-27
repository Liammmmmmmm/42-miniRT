/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_material.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 13:25:08 by madelvin          #+#    #+#             */
/*   Updated: 2025/08/27 13:32:44 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

char	check_mat(t_minirt *minirt, int i)
{
	if (minirt->scene.elements[i].type == SPHERE)
		return (!((t_sphere *)minirt->scene.elements[i].object)->material);
	if (minirt->scene.elements->type == CYLINDER)
		return (!((t_cylinder *)minirt->scene.elements[i].object)->material);
	if (minirt->scene.elements->type == PLANE)
		return (!((t_plane *)minirt->scene.elements[i].object)->material);
	if (minirt->scene.elements->type == CONE)
		return (!((t_cone *)minirt->scene.elements[i].object)->material);
	if (minirt->scene.elements->type == HYPERBOLOID)
		return (!((t_hyperboloid *)minirt->scene.elements[i].object)->material);
	if (minirt->scene.elements->type == TRIANGLE)
		return (!((t_triangle *)minirt->scene.elements[i].object)->material);
	return (1);
}
