/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_elements5.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 13:31:25 by madelvin          #+#    #+#             */
/*   Updated: 2025/08/21 20:37:26 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "error_message.h"

int	parse_dlight(t_scene *scene, char *line)
{
	char		**parts;
	t_dlight	*light;

	light = ft_calloc(sizeof(t_dlight), 1);
	if (!light)
		return (print_error(strerror(errno)));
	scene->elements[scene->el_amount].type = DIRECTIONAL_LIGHT;
	scene->elements[scene->el_amount++].object = light;
	parts = ft_split_in_line(line, " ");
	if (!parts)
		return (print_error(strerror(errno)));
	if (char_tab_len(parts) != 4)
		return (invalid_struct_error(DIRECTIONAL_LIGHT, parts));
	if (!parse_vector_normalized(parts[1], &light->orientation))
		return (invalid_struct_error(DIRECTIONAL_LIGHT, parts));
	if (!is_valid_double_el(parts[2], &light->brightness))
	{
		free(parts);
		return (print_error(ERR_ILR));
	}
	if (!parse_color(parts[3], &light->color))
		return (invalid_struct_error(DIRECTIONAL_LIGHT, parts));
	free(parts);
	return (1);
}

static int	parse_win_size_parts(t_scene *scene, char **parts, int nb_parts)
{
	if (nb_parts == 5)
	{
		if (!is_valid_positive_int(parts[3], &scene->render_width))
			return (invalid_struct_error(WINDOW, parts));
		if (!is_valid_positive_int(parts[4], &scene->render_height))
			return (invalid_struct_error(WINDOW, parts));
	}
	else
	{
		scene->render_width = scene->win_width;
		scene->render_height = scene->win_height;
	}
	return (1);
}

int	parse_win_size(t_scene *scene, char *line)
{
	char	**parts;
	int		nb_parts;

	parts = ft_split_in_line(line, " ");
	if (!parts)
		return (print_error(strerror(errno)));
	nb_parts = char_tab_len(parts);
	if (nb_parts != 3 && nb_parts != 5)
		return (invalid_struct_error(WINDOW, parts));
	if (!is_valid_positive_int(parts[1], &scene->win_width))
		return (invalid_struct_error(WINDOW, parts));
	if (!is_valid_positive_int(parts[2], &scene->win_height))
		return (invalid_struct_error(WINDOW, parts));
	if (parse_win_size_parts(scene, parts, nb_parts) == 0)
		return (0);
	if (scene->render_width < 2 || scene->render_height < 2
		|| scene->win_width < 10 || scene->win_height < 10)
		return (invalid_struct_error(WINDOW, parts));
	free(parts);
	return (1);
}

static int	parse_texture_type(t_scene *sc, char **parts, int parts_nb,
	int i)
{
	if (ft_strcmp(parts[2], "image") == 0)
	{
		if (parts_nb != 4)
			return (invalid_struct_error(TEXTURE, parts));
		sc->textures[i].type = IMAGE;
		get_texture_image(&sc->textures[i], parts[3]);
		if (ft_strnstr(parts[1], "bump", 20) && sc->textures[i].type == IMAGE)
			bump_to_normal(&sc->textures[i].img);
	}
	else if (ft_strcmp(parts[2], "checker_local") == 0
		|| ft_strcmp(parts[2], "checker_global") == 0)
	{
		if (parts_nb != 6)
			return (invalid_struct_error(TEXTURE, parts));
		sc->textures[i].type = CHECKER_LOCAL;
		if (is_valid_size(parts[3], &sc->textures[i].checker.scale) == 0)
			return (texture_item_error(3, parts[3]));
		if (parse_color(parts[4], &sc->textures[i].checker.c1) == 0)
			return (texture_item_error(4, parts[4]));
		if (parse_color(parts[5], &sc->textures[i].checker.c2) == 0)
			return (texture_item_error(4, parts[5]));
	}
	else
		return (invalid_struct_error(TEXTURE, parts));
	return (1);
}

int	parse_texture(t_scene *scene, char *line)
{
	static int	i = 0;
	char		**parts;
	int			valid_name;
	int			parts_nb;

	parts = ft_split_in_line(line, " ");
	if (!parts)
		return (print_error(strerror(errno)));
	parts_nb = char_tab_len(parts);
	if (parts_nb < 3)
		return (invalid_struct_error(TEXTURE, parts));
	valid_name = is_valid_variable_name_tex(parts[1], scene);
	if (valid_name == -1)
		return (texture_error(0, parts));
	else if (valid_name == -2)
		return (texture_error(1, parts));
	else if (valid_name == 0)
		return (texture_error(2, parts));
	ft_strlcpy(scene->textures[i].name, parts[1], 21);
	if (parse_texture_type(scene, parts, parts_nb, i++) == 0)
		return (0);
	if (ft_strcmp(parts[2], "checker_global") == 0)
		scene->textures[i - 1].type = CHECKER_GLOBAL;
	free(parts);
	return (1);
}
