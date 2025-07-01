/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   random.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 09:32:27 by lilefebv          #+#    #+#             */
/*   Updated: 2025/07/01 16:01:10 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include <math.h>

double	random_double(void)
{
	return (rand() / (RAND_MAX + 1.0));
}

double random_double_neg(void)
{
    return (rand() / (RAND_MAX / 2.0)) - 1.0;
}

double	random_double_in_interval(double min, double max)
{
	return (min + (rand() / ((double)RAND_MAX / (max - min))));
}

t_vec3	random_in_unit_sphere(void)
{
	t_vec3	p;

	while (1)
	{
		p \
	= (t_vec3){random_double_neg(), random_double_neg(), random_double_neg()};
		if (vec3_length_squared(p) < 1)
			return (p);
	}
}