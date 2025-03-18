/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   valid_line.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 11:26:28 by lilefebv          #+#    #+#             */
/*   Updated: 2025/03/18 12:27:16 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

/**
 * @return 1 if the line start by `type` folowed by a space or tab, 0 otherwise
 */
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

static int	is_valid_element(char *line)
{
	if (is_valid_type("A", line) || is_valid_type("C", line)
		|| is_valid_type("L", line) || is_valid_type("sp", line)
		|| is_valid_type("pl", line) || is_valid_type("cy", line))
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

/**
 * @return -1 for an invalid line, 0 for an empty line, 1 for a valid line
 */
int is_valid_line(char *line)
{
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
