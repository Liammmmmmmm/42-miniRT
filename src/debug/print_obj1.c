/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_obj1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 17:14:04 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/10 18:52:39 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	print_light(t_light *light)
{
	printf("  " YELLOW "Light:\n" NC);
	printf("    Position: ");
	print_vec3(light->position);
	printf("\n    Brightness: " BLUE "%.2f" NC "\n", light->brightness);
	printf("    Color: ");
	print_color(light->color);
	printf("\n");
}

void	print_sphere(t_sphere *sphere)
{
	printf("  " YELLOW "Sphere:\n" NC);
	printf("    Position: ");
	print_vec3(sphere->position);
	printf("\n    Diameter: " BLUE "%.2f" NC "\n", sphere->diameter);
	print_mat_color(sphere->material, sphere->color);
	printf("\n");
}

void	print_plane(t_plane *plane)
{
	printf("  " YELLOW "Plane:\n" NC);
	printf("    Position: ");
	print_vec3(plane->position);
	printf("\n    Normal: ");
	print_vec3(plane->normal);
	print_mat_color(plane->material, plane->color);
	printf("\n");
}

void	print_cylinder(t_cylinder *cylinder)
{
	printf("  " YELLOW "Cylinder:\n" NC);
	printf("    Position: ");
	print_vec3(cylinder->position);
	printf("\n    Orientation: ");
	print_vec3(cylinder->orientation);
	printf("\n    Diameter: " BLUE "%.2f" NC "\n", cylinder->diameter);
	printf("    Height: " BLUE "%.2f" NC "\n", cylinder->height);
	print_mat_color(cylinder->material, cylinder->color);
	if (cylinder->material_top)
		printf("    Top cap   : %s", cylinder->material_top->name);
	if (cylinder->material_bot)
		printf("    Bottom cap: %s", cylinder->material_bot->name);
	printf("\n");
}

void	print_cone(t_cone *cone)
{
	printf("  " YELLOW "Cone:\n" NC);
	printf("    Position: ");
	print_vec3(cone->position);
	printf("\n    Orientation: ");
	print_vec3(cone->orientation);
	printf("\n    Diameter: " BLUE "%.2f" NC "\n", cone->diameter);
	printf("    Height: " BLUE "%.2f" NC "\n", cone->height);
	print_mat_color(cone->material, cone->color);
	if (cone->material_top)
		printf("    Top cap   : %s", cone->material_top->name);
	printf("\n");
}
