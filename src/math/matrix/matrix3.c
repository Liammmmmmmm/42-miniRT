/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 10:55:50 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/15 10:58:36 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_vec3	matrix3_dot_vec3(double matrix[3][3], const t_vec3 v)
{
	t_vec3	res;

	res.x = v.x * matrix[0][0] + v.y * matrix[0][1] + v.z * matrix[0][2];
	res.y = v.x * matrix[1][0] + v.y * matrix[1][1] + v.z * matrix[1][2];
	res.z = v.x * matrix[2][0] + v.y * matrix[2][1] + v.z * matrix[2][2];
	return (res);
}
