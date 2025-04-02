/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 14:59:09 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/02 15:59:43 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "font.h"

t_table_directory	*get_table_directory(t_font_directoy *ft_dir, char c[4])
{
	uint16_t	i;

	i = 0;
	while (i < ft_dir->off_sub.num_tables)
	{
		if (ft_dir->tbl_dir[i].tag_c[0] == c[3]
			&& ft_dir->tbl_dir[i].tag_c[1] == c[2]
			&& ft_dir->tbl_dir[i].tag_c[2] == c[1]
			&& ft_dir->tbl_dir[i].tag_c[3] == c[0])
			return (&ft_dir->tbl_dir[i]);
		i++;
	}
	return (NULL);
}
