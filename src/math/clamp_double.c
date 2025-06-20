/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clamp_double.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 15:49:44 by madelvin          #+#    #+#             */
/*   Updated: 2025/06/20 18:58:07 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "maths.h"

inline double	clamp_double_val(double v, double min, double max)
{
	if (v <= min)
		return (min);
	else if (v >= max)
		return (max);
	return (v);
}
