/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   animation.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 15:00:59 by lilefebv          #+#    #+#             */
/*   Updated: 2025/05/16 15:48:10 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	animate_option_parsing(t_minirt *minirt, char *str)
{
	char	**parts;

	
}

int	is_animate_option(t_minirt *minirt, char *argvi, int *y)
{
	int	i;

	if (ft_strcmp(argvi, "--animate") == 0)
	{
		*y = 0;		
		return (print_error1("Max sample option need a value"));
	}
	else if (ft_strncmp(argvi, "--animate=", 10) == 0)
	{
		if (ft_strlen(argvi + 14) > 30)
		{
			
			return (1);
		}
		*y = 0;
		return (print_error1("Invalid sample value"));
	}
	return (0);
}
