/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tranform_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 12:44:29 by lilefebv          #+#    #+#             */
/*   Updated: 2025/03/18 14:01:14 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static void rm_multiple_spaces(char *line)
{
	int	i;
	int	n;
	int	sp;

	i = 0;
	n = 0;
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
	line[n - 1] = '\0';
}

char	*remove_useless_spaces(char *line)
{
	int	i;

	i = -1;
	while (line[++i])
		if (line[i] == '\t' || line[i] == '\n')
			line[i] = ' ';
	rm_multiple_spaces(line);
	return (line);
}
