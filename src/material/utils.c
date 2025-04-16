/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 12:34:41 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/16 13:07:13 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "material.h"

double get_cos_theta(t_vec3 dir_in, t_vec3 normal)
{
	return (vec3_dot(vec3_negate(vec3_unit(dir_in)), normal));
}
