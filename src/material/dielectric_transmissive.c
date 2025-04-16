/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dielectric_transmissive.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 10:44:50 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/16 10:48:31 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_color	dielectric_transmissive_material(t_mat_manager *mat_man)
{
	(void)mat_man;
	return ((t_color){0, 0, 0});
}
