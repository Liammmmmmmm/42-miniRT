/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_parser_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 14:42:04 by madelvin          #+#    #+#             */
/*   Updated: 2025/05/07 17:07:54 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static int	count_while(char *line, t_obj_temp *tmp, int fd)
{
	size_t	i;

	i = 0;
	while (line)
	{
		if (ft_strncmp(line, "v ", 2) == 0)
			tmp->v_count++;
		else if (ft_strncmp(line, "vn ", 3) == 0)
			tmp->vn_count++;
		else if (ft_strncmp(line, "vt ", 3) == 0)
			tmp->vt_count++;
		else if (ft_strncmp(line, "f ", 2) == 0)
			tmp->face_count++;
		free(line);
		line = get_next_line(fd);
		i++;
	}
	return (i);
}

char	count_obj_data(const char *filename, t_obj_temp *tmp, size_t *size)
{
	char	*line;
	int		fd;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		print_error("Could not open file");
		return (1);
	}
	line = get_next_line(fd);
	if (!line)
	{
		print_error("Empty or unreadable .obj file");
		return (1);
	}
	*size = count_while(line, tmp, fd);
	close(fd);
	return (0);
}

void	ft_free_tab_face(t_face *tab, size_t i)
{
	size_t	c;

	c = 0;
	while (c < i)
	{
		free(tab[c].v_idx);
		free(tab[c].vn_idx);
		free(tab[c].vt_idx);
		c++;
	}
	free(tab);
}

void	ft_free_tab_triangle(t_triangle **tab, size_t i)
{
	size_t	c;

	c = 0;
	while (c < i)
	{
		free(tab[c]);
		c++;
	}
	free(tab);
}
