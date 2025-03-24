/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_elements3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:00:55 by lilefebv          #+#    #+#             */
/*   Updated: 2025/03/24 17:08:57 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	parse_cylinder(t_scene *scene, char *line)
{
	char		**parts;
	t_cylinder	*cylinder;

	cylinder = malloc(sizeof(t_cylinder));
	if (!cylinder)
		return (print_error(strerror(errno)));
	scene->elements[scene->el_amount].type = CYLINDER;
	scene->elements[scene->el_amount++].object = cylinder;
	parts = ft_split_in_line(line, " ");
	if (!parts)
		return (print_error(strerror(errno)));
	if (char_tab_len(parts) != 6)
		return (invalid_struct_error(CYLINDER, parts));
	if (!parse_vector(parts[1], &cylinder->position))
		return (invalid_struct_error(CYLINDER, parts));
	if (!parse_vector_normalized(parts[2], &cylinder->orientation))
		return (invalid_struct_error(CYLINDER, parts));
	if (!is_valid_size(parts[3], &cylinder->diameter))
		return (invalid_size_error(parts));
	if (!is_valid_size(parts[4], &cylinder->height))
		return (invalid_size_error(parts));
	if (!parse_color(parts[5], &cylinder->color))
		return (invalid_struct_error(CYLINDER, parts));
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
	scene->textures[i].fd = open(parts[2], O_RDONLY);
	if (scene->textures[i].fd == -1)
		return (texture_error(3, parts));
	free(parts);
	i++;
	return (1);
}

int	parse_material(t_scene *scene, char *line)
{
	static int	y = 0;
	char		**parts;
	int			valid_name;

	parts = ft_split_in_line(line, " ");
	if (!parts)
		return (print_error(strerror(errno)));
	if (char_tab_len(parts) != 9 && char_tab_len(parts) != 10)
		return (invalid_struct_error(MATERIAL, parts));
	valid_name = is_valid_variable_name_mat(parts[1], scene);
	// if (valid_name == -1)
	// 	return (texture_error(0, parts));
	// else if (valid_name == -2)
	// 	return (texture_error(1, parts));
	// ft_strlcpy(scene->textures[y].name, parts[1], 21);
	// scene->textures[y].type = IMAGE;
	// scene->textures[y].fd = open(parts[2], O_RDONLY);
	// if (scene->textures[y].fd == -1)
	// 	return (texture_error(2, parts));
	free(parts);
	y++;
	return (1);
}
