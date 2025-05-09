/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   verify_elements.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 09:50:04 by lilefebv          #+#    #+#             */
/*   Updated: 2025/05/09 17:12:59 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static int	count_elements(char **lines, char *type)
{
	int	res;
	int	i;

	i = 0;
	res = 0;
	while (lines[i])
	{
		if (cmp_type(type, lines[i]))
			res++;
		i++;
	}
	return (res);
}

int	valid_elements_amount(char **lines)
{
	int	count;

	count = count_elements(lines, "A");
	if (count < 1)
		return (not_enough_el_error("A"));
	if (count > 1)
		return (to_many_el_error("A"));
	count = count_elements(lines, "C");
	if (count < 1)
		return (not_enough_el_error("C"));
	if (count > 1)
		return (to_many_el_error("C"));
	if (count_elements(lines, "L") > 1)
		return (to_many_el_error("L"));
	if (count_elements(lines, "W") > 1)
		return (to_many_el_error("W"));
	return (1);
}
