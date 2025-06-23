/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_scene.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 15:20:25 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/23 14:25:01 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static void	save_scene_name(char *filename, t_minirt *minirt)
{
	int		i;
	char	*last;

	last = ft_strrchr(filename, '/') + 1;
	if (last)
		filename = last;
	last = ft_strrchr(filename, '.');
	*last = '\0';
	ft_strlcpy(minirt->scene.name, filename, 20);
	*last = '.';
}

static int	is_valid_extension(char *filename, t_minirt *minirt)
{
	int	i;

	i = ft_strlen(filename);
	if (filename[i - 3] == '.' && filename[i - 2] == 'r'
		&& filename[i - 1] == 't')
	{
		save_scene_name(filename, minirt);
		return (1);
	}
	return (0);
}

char	**save_valid_lines(char *filename, int ln_amount)
{
	int		fd;
	char	*ln;
	int		c;
	char	**res;

	c = 0;
	res = malloc(sizeof(char *) * (ln_amount + 1));
	if (!res)
		return (NULL);
	fd = open(filename, O_RDONLY);
	if (fd == -1)
		return (free_ret_null(res));
	ln = get_next_line(fd);
	while (ln && c < ln_amount)
	{
		if (is_valid_line(ln))
			res[c++] = remove_useless_spaces(ln);
		else
			free(ln);
		if (c < ln_amount)
			ln = get_next_line(fd);
	}
	close(fd);
	res[c] = NULL;
	return (res);
}

int	count_tex_mat(t_scene *scene, char **lines)
{
	int	i;

	i = -1;
	scene->tex_amount = 0;
	scene->mat_amount = 0;
	while (lines[++i])
	{
		if (cmp_type("tex", lines[i]))
			scene->tex_amount++;
		else if (cmp_type("mat", lines[i]))
			scene->mat_amount++;
	}
	i = -1;
	while (lines[++i])
		if (cmp_type("W", lines[i]))
			scene->have_win_el = 1;
	if (scene->tex_amount)
		scene->textures = ft_calloc(scene->tex_amount, sizeof(t_tex));
	if (scene->mat_amount)
		scene->materials = ft_calloc(scene->mat_amount, sizeof(t_mat));
	if ((scene->tex_amount && !scene->textures)
		|| (scene->mat_amount && !scene->materials))
		return (print_error(strerror(errno)));
	return (1);
}

int	parse_scene(t_minirt *minirt, char *filename)
{
	int		ln_amount;
	char	**lines;

	minirt->scene.win_width = -1;
	minirt->scene.win_height = -1;
	if (!is_valid_extension(filename, minirt))
		return (print_error("Invalid file extension. Expected a '.rt' file."));
	ln_amount = count_valid_lines(filename);
	if (ln_amount == 0)
		return (print_error("Empty scene."));
	lines = save_valid_lines(filename, ln_amount);
	if (!lines)
		return (print_error(strerror(errno)));
	if (valid_elements_amount(lines) == 0)
	{
		ft_free_tab_null_term(lines);
		return (0);
	}
	if (count_tex_mat(&minirt->scene, lines) == 0)
		return (free_scene(&minirt->scene, lines));
	if (parse_elements(&minirt->scene, lines, ln_amount) == 0)
		return (0);
	minirt->scene.ior_all = 1.0;
	ft_free_tab_null_term(lines);
	return (1);
}
