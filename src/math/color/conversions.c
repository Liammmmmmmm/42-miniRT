/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conversions.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 12:25:19 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/17 12:25:39 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "maths.h"

inline t_fcolor	color_to_fcolor(t_color color)
{
	return ((t_fcolor){color.r / 255.0, color.g / 255.0, color.b / 255.0});
}

inline t_fcolor	rgba_to_fcolor(t_rgba color)
{
	return ((t_fcolor){color.r / 255.0, color.g / 255.0, color.b / 255.0});
}
