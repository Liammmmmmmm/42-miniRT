/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kd_tree_build_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 15:31:17 by madelvin          #+#    #+#             */
/*   Updated: 2025/06/24 15:36:33 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "caustic.h"
#include "minirt.h"

void	swap_photons(t_photon *a, t_photon *b)
{
	t_photon	t;

	t = *a;
	*a = *b; *b = t;
}

float	get_pos_on_axis(t_photon *p, int axis)
{
	if (axis == 0)
		return (p->position.x);
	if (axis == 1)
		return (p->position.y);
	return (p->position.z);
}
