/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 12:25:59 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/11 22:05:22 by madelvin         ###   ########.fr       */
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

int	ft_strlen_to(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return (i);
		i++;
	}
	return (0);
}
