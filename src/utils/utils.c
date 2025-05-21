/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:25:59 by lilefebv          #+#    #+#             */
/*   Updated: 2025/05/16 11:18:52 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	char_tab_len(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
		i++;
	return (i);
}

char	**free_ret_null(char **fre)
{
	free(fre);
	return (NULL);
}

void	ft_izero(int *pointer, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		*((int *)pointer + i) = 0;
		i++;
	}
}

char	*empty_static_string(void)
{
	static char	*tab = "";

	return (tab);
}
