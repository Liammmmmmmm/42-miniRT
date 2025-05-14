/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_solid_texture.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 18:19:43 by lilefebv          #+#    #+#             */
/*   Updated: 2025/05/14 11:46:25 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_fcolor	get_solid_texture_default(t_vec3 point, double scale)
{
	int	a;
	int	b;
	int	c;

	point = vec3_multiply_scalar(point, scale);
	a = floor(point.x);
	b = floor(point.y + 0.5);
	c = floor(point.z);
	if ((a + b + c) % 2 == 0)
		return ((t_fcolor){0.0, 0.0, 0.0});
	return (color_to_fcolor((t_color){243, 0, 243}));
}

t_fcolor	get_solid_texture(t_vec3 p, t_color c1, t_color c2, double scale)
{
	int	a;
	int	b;
	int	c;

	p = vec3_multiply_scalar(p, scale);
	a = floor(p.x);
	b = floor(p.y + 0.5);
	c = floor(p.z);
	if ((a + b + c) % 2 == 0)
		return (color_to_fcolor(c1));
	return (color_to_fcolor(c2));
}
