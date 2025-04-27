/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_solid_texture.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 18:19:43 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/27 17:42:40 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_color	get_solid_texture(t_vec3 point, double scale)
{
	int	a;
	int	b;
	int	c;

	point = vec3_multiply_scalar(point, scale);
	a = floor(point.x);
	b = floor(point.y + 0.5);
	c = floor(point.z);
	if ((a + b + c) % 2 == 0)
		return ((t_color){0, 0, 0});
	return ((t_color){243, 0, 243});
}
