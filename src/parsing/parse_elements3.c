/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_elements3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:00:55 by lilefebv          #+#    #+#             */
/*   Updated: 2025/05/13 12:05:28 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	parse_cylinder(t_scene *scene, char *line)
{
	char		**parts;
	t_cylinder	*cyl;
	int			nb_parts;

	cyl = ft_calloc(sizeof(t_cylinder), 1);
	if (!cyl)
		return (print_error(strerror(errno)));
	scene->elements[scene->el_amount].type = CYLINDER;
	scene->elements[scene->el_amount++].object = cyl;
	parts = ft_split_in_line(line, " ");
	if (!parts)
		return (print_error(strerror(errno)));
	nb_parts = char_tab_len(parts);
	if (nb_parts < 6 || nb_parts > 8)
		return (invalid_struct_error(CYLINDER, parts));
	if (!parse_vector(parts[1], &cyl->position))
		return (invalid_struct_error(CYLINDER, parts));
	if (!parse_vector_normalized(parts[2], &cyl->orientation))
		return (invalid_struct_error(CYLINDER, parts));
	if (!is_valid_size(parts[3], &cyl->diameter))
		return (invalid_size_error(parts));
	if (!is_valid_size(parts[4], &cyl->height))
		return (invalid_size_error(parts));
	if (!parse_color_or_mat(parts[5], &cyl->color, &cyl->material, scene))
		return (invalid_struct_error(CYLINDER, parts));
	cyl->material_top = NULL;
	cyl->material_bot = NULL;
	if (nb_parts > 6 && !parse_only_mat(parts[6], &cyl->material_top, scene))
		return (invalid_struct_error(CYLINDER, parts));
	if (nb_parts > 7 && !parse_only_mat(parts[7], &cyl->material_bot, scene))
		return (invalid_struct_error(CYLINDER, parts));
	cyl->orientation = vec3_unit(cyl->orientation);
	free(parts);
	return (1);
}

int	parse_texture(t_scene *scene, char *line)
{
	static int	i = 0;
	char		**parts;
	int			valid_name;

	parts = ft_split_in_line(line, " ");
	if (!parts)
		return (print_error(strerror(errno)));
	if (char_tab_len(parts) != 3)
		return (invalid_struct_error(TEXTURE, parts));
	valid_name = is_valid_variable_name_tex(parts[1], scene);
	if (valid_name == -1)
		return (texture_error(0, parts));
	else if (valid_name == -2)
		return (texture_error(1, parts));
	else if (valid_name == 0)
		return (texture_error(2, parts));
	ft_strlcpy(scene->textures[i].name, parts[1], 21);
	scene->textures[i].type = IMAGE;
	get_texture_image(&scene->textures[i], parts[2]);
	if (ft_strnstr(parts[1], "bump", 20) && scene->textures[i].type == IMAGE)
		bump_to_normal(&scene->textures[i].img);
	free(parts);
	i++;
	return (1);
}

static int	parse_mat_prop(char **parts, t_mat *mat, t_scene *scene, int nb_parts)
{
	if (parse_color_or_tex(parts[2], &mat->color_value,
			&mat->color_tex, scene) == 0)
		return (0);
	if (parse_double_b_or_tex(parts[3], &mat->metallic_value,
			&mat->metallic_tex, scene) == 0)
		return (material_item_error(2, mat->name));
	if (parse_double_b_or_tex(parts[4], &mat->roughness_value,
			&mat->roughness_tex, scene) == 0)
		return (material_item_error(3, mat->name));
	if (is_valid_double_el_no_bordered(parts[5], &mat->ior) == 0)
		return (material_item_error(4, mat->name));
	if (is_valid_double_el(parts[6], &mat->transmission) == 0)
		return (material_item_error(5, mat->name));
	mat->ao_value = 1.0;
	if (parse_double_b_or_tex(parts[7], &mat->ao_value, &mat->ao_tex,
			scene) == 0)
		return (material_item_error(8, mat->name));
	if (is_valid_size(parts[8], &mat->emission_strength) == 0)
		return (material_item_error(6, mat->name));
	if (parse_color(parts[9], &mat->emission_color) == 0)
		return (0);
	mat->scale = 1;
	if (nb_parts >= 11 && is_valid_double_el_no_bordered(parts[10], &mat->scale) == 0)
		return (material_item_error(10, mat->name));
	if (nb_parts >= 12)
		mat->normal = get_texture(parts[11], scene);
	if (nb_parts >= 12 && mat->normal == NULL)
		return (material_item_error(7, mat->name));
	mat->normal_intensity = 1;
	if (nb_parts >= 13 && is_valid_double_el_no_bordered(parts[12], &mat->normal_intensity) == 0)
		return (material_item_error(9, parts[12]));
	return (1);
}

int	parse_material(t_scene *scene, char *line)
{
	static int	y = 0;
	char		**parts;
	int			valid_name;
	int			nb_parts;

	parts = ft_split_in_line(line, " ");
	if (!parts)
		return (print_error(strerror(errno)));
	nb_parts = char_tab_len(parts);
	if (nb_parts < 10 || nb_parts > 13)
		return (invalid_struct_error(MATERIAL, parts));
	valid_name = is_valid_variable_name_mat(parts[1], scene);
	if (valid_name == -1)
		return (material_error(0, parts));
	else if (valid_name == -2)
		return (material_error(1, parts));
	else if (valid_name == 0)
		return (material_error(2, parts));
	ft_strlcpy(scene->materials[y].name, parts[1], 21);
	if (parse_mat_prop(parts, &scene->materials[y], scene, nb_parts) == 0)
		return (invalid_struct_error(MATERIAL, parts));
	free(parts);
	y++;
	return (1);
}
