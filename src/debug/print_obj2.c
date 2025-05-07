/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_obj2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 17:16:48 by lilefebv          #+#    #+#             */
/*   Updated: 2025/05/07 17:23:32 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	print_hyperboloid(t_hyperboloid *hyp)
{
	printf("  " YELLOW "Hyperboloid:\n" NC);
	printf("    Position: ");
	print_vec3(hyp->position);
	printf("\n    Orientation: ");
	print_vec3(hyp->orientation);
	printf("\n    Height: " BLUE "%.2f" NC "\n", hyp->height);
	printf("    a: " BLUE "%.2f" NC "\n", hyp->a);
	printf("    b: " BLUE "%.2f" NC "\n", hyp->b);
	printf("    c: " BLUE "%.2f" NC "\n", hyp->c);
	printf("    shape: " BLUE "%.2f" NC "\n", hyp->shape);
	print_mat_color(hyp->material, hyp->color);
	printf("\n");
}

void	print_objects(t_scene *scene)
{
	int			i;
	t_object	*obj;

	printf("\n" RED "Scene Objects (%d):\n" NC, scene->el_amount);
	i = -1;
	while (++i < scene->el_amount && scene->elements)
	{
		obj = &scene->elements[i];
		if (obj->type == LIGHT)
			print_light((t_light *)obj->object);
		else if (obj->type == SPHERE)
			print_sphere((t_sphere *)obj->object);
		else if (obj->type == PLANE)
			print_plane((t_plane *)obj->object);
		else if (obj->type == CYLINDER)
			print_cylinder((t_cylinder *)obj->object);
		else if (obj->type == CONE)
			print_cone((t_cone *)obj->object);
		else if (obj->type == HYPERBOLOID)
			print_hyperboloid((t_hyperboloid *)obj->object);
		else
			printf("  " RED "Unknown object type!\n" NC);
	}
}
