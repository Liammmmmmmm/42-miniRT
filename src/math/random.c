/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   random.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 09:32:27 by lilefebv          #+#    #+#             */
/*   Updated: 2025/08/21 21:09:42 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "maths.h"
#include <math.h>

static inline uint64_t	xorshift_rand(uint64_t *rand)
{
	*rand ^= *rand << 13;
	*rand ^= *rand >> 7;
	*rand ^= *rand << 17;
	return (*rand);
}

double	random_double(void)
{
	static uint64_t	rand = (uint64_t)RANDOM_SEED;

	return ((xorshift_rand(&rand) >> 11) / SCALE_FACTOR_HP);
}

double	random_double_neg(void)
{
	return (random_double() * 2 - 1.0);
}

double	random_double_in_interval(double min, double max)
{
	return (min + random_double() * (max - min));
}

t_vec3	random_in_unit_sphere(void)
{
	t_vec3	p;

	p = (t_vec3){random_double_neg(), random_double_neg(), random_double_neg()};
	if (vec3_length_squared(p) >= 1.0)
		p = vec3_multiply_scalar(vec3_unit(p), pow(random_double(), 1.0 / 3.0));
	return (p);
}
