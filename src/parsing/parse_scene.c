/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_scene.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 15:20:25 by lilefebv          #+#    #+#             */
/*   Updated: 2025/03/19 15:24:04 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	is_valid_extension(char *filename)
{
	int	i;

	i = ft_strlen(filename);
	if (filename[i - 3] == '.' && filename[i - 2] == 'r'
		&& filename[i - 1] == 't')
		return (1);
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

int	parse_scene(t_minirt *minirt, char *filename)
{
	int		ln_amount;
	char	**lines;

	if (!is_valid_extension(filename))
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
	if (parse_elements(&minirt->scene, lines, ln_amount) == 0)
		return (0);
	ft_free_tab_null_term(lines);
	return (1);
}
