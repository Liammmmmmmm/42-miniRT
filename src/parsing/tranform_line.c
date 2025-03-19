/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tranform_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 12:44:29 by lilefebv          #+#    #+#             */
/*   Updated: 2025/03/19 11:05:54 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static void	rm_multiple_spaces(char *line, int n, int i)
{
	int	sp;

	sp = 0;
	while (line[i])
	{
		if (line[i] != ' ')
		{
			sp = 0;
			line[n++] = line[i];
		}
		else if (line[i] == ' ' && sp == 0)
		{
			sp = 1;
			line[n++] = line[i];
		}
		i++;
		if (line[i - 1] == ',')
			while (line[i] == ' ')
				i++;
	}
	if (sp)
		line[n - 1] = '\0';
	else
		line[n] = '\0';
}

char	*remove_useless_spaces(char *line)
{
	int	i;

	i = -1;
	while (line[++i])
		if (line[i] == '\t' || line[i] == '\n')
			line[i] = ' ';
	rm_multiple_spaces(line, 0, 0);
	return (line);
}

char	*remove_comments(char *line)
{
	int	i;

	i = -1;
	while (line[++i])
	{
		if (line[i] == '#')
		{
			line[i] = '\0';
			break ;
		}
	}
	return (line);
}
