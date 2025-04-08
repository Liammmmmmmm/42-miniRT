/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reflection_math.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 18:56:23 by madelvin          #+#    #+#             */
/*   Updated: 2025/04/08 19:47:49 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "maths.h"

t_vec3	reflection_vec(const t_vec3 uv, const t_vec3 n)
{
	t_vec3	direction;

	direction = vec3_subtract(uv, vec3_multiply_scalar(n, 2 * vec3_dot(uv, n)));
	return (direction);
}
