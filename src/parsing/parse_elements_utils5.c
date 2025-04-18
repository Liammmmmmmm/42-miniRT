/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_elements_utils5.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 17:41:31 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/11 17:53:16 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	is_extension(char *filename, char *extension)
{
	int	fi;
	int	ei;

	fi = ft_sstrlen(filename) - 1;
	ei = ft_sstrlen(extension) - 1;
	while (ei >= -1 && fi >= 0)
	{
		if (!((ei == -1 && filename[fi] == '.')
				|| (filename[fi] == extension[ei])))
			break ;
		ei--;
		fi--;
	}
	if (ei == -2)
		return (1);
	return (0);
}
