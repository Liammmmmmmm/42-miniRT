/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   random.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 09:32:27 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/23 19:29:46 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "maths.h"
#include <math.h>

uint64_t	xorshift_rand(uint64_t *rand)
{
	*rand ^= *rand << 13;
	*rand ^= *rand >> 7;
	*rand ^= *rand << 17;
	return (*rand);
}

double	random_double(uint64_t *rand)
{
	return ((xorshift_rand(rand) >> 11) * SCALE_FACTOR_HP);
}

double	random_double_in_interval(double min, double max, uint64_t *rand)
{
	return (min + random_double(rand) * (max - min));
}
