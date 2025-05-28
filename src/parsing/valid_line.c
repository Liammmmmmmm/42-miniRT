/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   valid_line.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 11:26:28 by lilefebv          #+#    #+#             */
/*   Updated: 2025/05/28 18:11:20 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	cmp_type(const char *type, const char *line)
{
	int	i;

	i = -1;
	while (type[++i])
		if (type[i] != line[i])
			return (0);
	if (line[i] == ' ' || line[i] == '\t')
		return (1);
	return (0);
}

int	is_valid_element(char *line)
{
	if (cmp_type("A", line) || cmp_type("C", line) || cmp_type("l", line)
		|| cmp_type("L", line) || cmp_type("sp", line) || cmp_type("pl", line)
		|| cmp_type("cy", line) || cmp_type("tex", line) || cmp_type("hb", line)
		|| cmp_type("mat", line) || cmp_type("co", line) || cmp_type("cu", line)
		|| cmp_type("W", line) || cmp_type("dl", line))
		return (1);
	return (0);
}

int	is_empty_line(char *line)
{
	int	i;

	i = -1;
	while (line[++i])
	{
		if (line[i] != ' ' && line[i] != '\t' && line[i] != '\n')
			return (0);
	}
	return (1);
}

int	is_valid_line(char *line)
{
	remove_comments(line);
	if (is_valid_element(line))
		return (1);
	if (is_empty_line(line))
		return (0);
	return (-1);
}

int	count_valid_lines(char *filename)
{
	int		fd;
	char	*ln;
	int		valid;
	int		res;
	int		gl;

	res = 0;
	gl = 0;
	fd = open(filename, O_RDONLY);
	if (fd == -1)
		return (print_error(strerror(errno)));
	ln = get_next_line(fd);
	while (ln)
	{
		gl++;
		valid = is_valid_line(ln);
		if (valid == 1)
			res++;
		else if (valid == -1)
			return (print_line_error(gl, ln));
		free(ln);
		ln = get_next_line(fd);
	}
	close(fd);
	return (res);
}
