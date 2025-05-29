/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_scene.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:57:12 by lilefebv          #+#    #+#             */
/*   Updated: 2025/05/29 12:31:30 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	print_global_info(t_scene *scene)
{
	printf("\n" RED "Ambient Light:\n" NC);
	printf("  Ratio: " BLUE "%.2f" NC "\n", scene->amb_light.ratio);
	printf("\n" RED "Camera:\n" NC);
	printf("  Position: ");
	print_vec3(scene->camera.position);
	printf("\n");
	printf("  Orientation: ");
	print_vec3(scene->camera.orientation);
	printf("\n");
	printf("  FOV: " BLUE "%d" NC "\n", scene->camera.fov);
}

void	print_textures(t_scene *scene)
{
	int		i;
	t_tex	*texture;

	printf("\n" RED "Scene Textures (%d):\n" NC, scene->tex_amount);
	i = -1;
	while (++i < scene->tex_amount && scene->textures)
	{
		texture = &scene->textures[i];
		printf("  " YELLOW "Tex: `%s'\n" NC, texture->name);
		printf("    Texture width : " BLUE "%d" NC "\n", texture->img.width);
		printf("    Texture height: " BLUE "%d" NC "\n", texture->img.height);
	}
}

void	print_mat(t_mat *material)
{
	printf("  " YELLOW "Mat: `%s'\n" NC, material->name);
	print_tex_color_text(material->color_tex, material->color_value, "Color");
	print_tex_double_text(material->metallic_tex, material->metallic_value,
		"Metallic");
	print_tex_double_text(material->roughness_tex, material->roughness_value,
		"Roughness");
	print_tex_double_text(material->ao_tex, material->ao_value,
		"Ambient occlusion");
	printf("    IOR: %.2f\n", material->ior);
	print_tex_double_text(material->transmission_tex, \
		material->transmission_value, "Transmission");
	printf("    Emission Strength: %.2f\n", material->emission_strength);
	printf("    Emission Color: ");
	printf("("YELLOW"%d, %d, %d"NC")\n", \
			(int)(material->emission_color.r * 255), \
			(int)(material->emission_color.g * 255), \
			(int)(material->emission_color.b * 255));
	printf("\n");
	printf("    Texture scale: %.2f\n", material->scale);
	if (material->normal)
	{
		printf("    Normal Texture: %s\n", material->normal->name);
		printf("    Normal intensity: %.2f\n", material->normal_intensity);
	}
}

void	print_materials(t_scene *scene)
{
	int	i;

	printf("\n" RED "Scene Materials (%d):\n" NC, scene->mat_amount);
	i = -1;
	while (++i < scene->mat_amount && scene->materials)
		print_mat(&scene->materials[i]);
	printf(GREEN "====================================\n" NC);
}

void	print_scene(t_scene *scene)
{
	printf(GREEN "\n===== Scene Debug Information =====\n" NC);
	print_global_info(scene);
	print_objects(scene);
	print_textures(scene);
	print_materials(scene);
}
