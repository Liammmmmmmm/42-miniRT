/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dmin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 14:15:05 by madelvin          #+#    #+#             */
/*   Updated: 2025/04/16 12:40:51 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "maths.h"

inline double	ft_dmin(double a, double b)
{
	if (a < b)
		return (a);
	return (b);
}

inline double	ft_dmax(double a, double b)
{
	if (a > b)
		return (a);
	return (b);
}