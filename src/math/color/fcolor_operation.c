/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fcolor_operation.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 12:21:52 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/17 12:25:17 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "maths.h"

inline t_fcolor	add_fcolor(t_fcolor c1, t_fcolor c2)
{
	return ((t_fcolor){
		.r = c1.r + c2.r,
		.g = c1.g + c2.g,
		.b = c1.b + c2.b
	});
}

inline t_fcolor	sub_fcolor(t_fcolor c1, t_fcolor c2)
{
	return ((t_fcolor){
		.r = c1.r - c2.r,
		.g = c1.g - c2.g,
		.b = c1.b - c2.b
	});
}

inline t_fcolor	multiply_fcolor(t_fcolor c1, t_fcolor c2)
{
	return ((t_fcolor){
		.r = c1.r * c2.r,
		.g = c1.g * c2.g,
		.b = c1.b * c2.b
	});
}

inline t_fcolor	multiply_scalar_fcolor(t_fcolor c1, double c)
{
	return ((t_fcolor){
		.r = c1.r * c,
		.g = c1.g * c,
		.b = c1.b * c
	});
}
