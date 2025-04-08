/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_scene.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:57:12 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/08 21:06:51 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	print_color(t_color color)
{
    printf("(" YELLOW "%d, %d, %d" NC ")", color.r, color.g, color.b);
}

void	print_vec3(t_vec3 vec)
{
    printf("(" CYAN "%.2f, %.2f, %.2f" NC ")", vec.x, vec.y, vec.z);
}

void	print_scene(t_scene *scene)
{
	int i;
	printf(GREEN "\n===== Scene Debug Information =====\n" NC);
	
	printf("\n" RED "Ambient Light:\n" NC);
	printf("  Ratio: " BLUE "%.2f" NC "\n", scene->amb_light.ratio);
	printf("  Color: ");
	print_color(scene->amb_light.color);
	printf("\n");
	
	printf("\n" RED "Camera:\n" NC);
	printf("  Position: ");
	print_vec3(scene->camera.position);
	printf("\n");
	printf("  Orientation: ");
	print_vec3(scene->camera.orientation);
	printf("\n");
	printf("  FOV: " BLUE "%d" NC "\n", scene->camera.fov);
	
	printf("\n" RED "Scene Objects (%d):\n" NC, scene->el_amount);
	i = -1;
	while (++i < scene->el_amount && scene->elements)
	{
		t_object *obj = &scene->elements[i];
		switch (obj->type) {
			case LIGHT: {
				t_light *light = (t_light *)obj->object;
				printf("  " YELLOW "Light:\n" NC);
				printf("    Position: ");
				print_vec3(light->position);
				printf("\n    Brightness: " BLUE "%.2f" NC "\n", light->brightness);
				printf("    Color: ");
				print_color(light->color);
				printf("\n");
				break;
			}
			case SPHERE: {
				t_sphere *sphere = (t_sphere *)obj->object;
				printf("  " YELLOW "Sphere:\n" NC);
				printf("    Position: ");
				print_vec3(sphere->position);
				printf("\n    Diameter: " BLUE "%.2f" NC "\n", sphere->diameter);
				if (sphere->material)
					printf("    Material: %s", sphere->material->name);
				else
				{
					printf("    Color: ");
					print_color(sphere->color);
				}
				printf("\n");
				break;
			}
			case PLANE: {
				t_plane *plane = (t_plane *)obj->object;
				printf("  " YELLOW "Plane:\n" NC);
				printf("    Position: ");
				print_vec3(plane->position);
				printf("\n    Normal: ");
				print_vec3(plane->normal);
				if (plane->material)
					printf("    Material: %s", plane->material->name);
				else
				{
					printf("\n    Color: ");
					print_color(plane->color);
				}
				printf("\n");
				break;
			}
			case CYLINDER: {
				t_cylinder *cylinder = (t_cylinder *)obj->object;
				printf("  " YELLOW "Cylinder:\n" NC);
				printf("    Position: ");
				print_vec3(cylinder->position);
				printf("\n    Orientation: ");
				print_vec3(cylinder->orientation);
				printf("\n    Diameter: " BLUE "%.2f" NC "\n", cylinder->diameter);
				printf("    Height: " BLUE "%.2f" NC "\n", cylinder->height);
				if (cylinder->material)
					printf("    Material: %s", cylinder->material->name);
				else
				{
					printf("    Color: ");
					print_color(cylinder->color);
				}
				printf("\n");
				break;
			}
			default:
				printf("  " RED "Unknown object type!\n" NC);
				break;
		}
	}

	printf("\n" RED "Scene Textures (%d):\n" NC, scene->tex_amount);
	i = -1;
	while (++i < scene->tex_amount && scene->textures)
	{
		t_tex *texture = &scene->textures[i];

		printf("  " YELLOW "Tex: `%s'\n" NC, texture->name);		
		printf("    Associated fd: " BLUE "%d" NC "\n", texture->fd);
	}

	printf("\n" RED "Scene Materials (%d):\n" NC, scene->mat_amount);
	i = -1;
	while (++i < scene->mat_amount && scene->materials)
	{
		t_mat *material = &scene->materials[i];

		printf("  " YELLOW "Mat: `%s'\n" NC, material->name);
		
		if (material->color_tex)
			printf("    Color Texture: %s\n", material->color_tex->name);
		else
		{
			printf("    Color Value: ");
			print_color(material->color_value);
			printf("\n");
		}

		if (material->metallic_tex)
			printf("    Metallic Texture: %s\n", material->metallic_tex->name);
		else
			printf("    Metallic Value: %.2f\n", material->metallic_value);

		if (material->roughness_tex)
			printf("    Roughness Texture: %s\n", material->roughness_tex->name);
		else
			printf("    Roughness Value: %.2f\n", material->roughness_value);

		printf("    IOR: %.2f\n", material->ior);
		printf("    Transmission: %.2f\n", material->transmission);
		printf("    Emission Strength: %.2f\n", material->emission_strength);

		printf("    Emission Color: ");
		print_color(material->emission_color);
		printf("\n");

		if (material->normal)
			printf("    Normal Texture: %s\n", material->normal->name);
	}
	printf(GREEN "====================================\n" NC);
}
