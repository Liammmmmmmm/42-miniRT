/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fcolor_clamp.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 17:07:01 by madelvin          #+#    #+#             */
/*   Updated: 2025/05/30 16:00:07 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "maths.h"

inline t_fcolor	clamp_fcolor(t_fcolor color)
{
	return ((t_fcolor){
		.r = clamp_double(color.r),
		.g = clamp_double(color.g),
		.b = clamp_double(color.b)
	});
}


inline t_fcolor	clamp_fcolor_val(t_fcolor color, double min, double max)
{
	return ((t_fcolor){
		.r = clamp_double_val(color.r, min, max),
		.g = clamp_double_val(color.g, min, max),
		.b = clamp_double_val(color.b, min, max)
	});
}
