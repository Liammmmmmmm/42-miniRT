/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 12:34:41 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/27 15:32:20 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "material.h"

double	get_cos_theta(t_vec3 dir_in, t_vec3 normal)
{
	return (-vec3_dot(vec3_unit(dir_in), normal));
}
