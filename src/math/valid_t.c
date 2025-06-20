/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   valid_t.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 17:24:00 by madelvin          #+#    #+#             */
/*   Updated: 2025/06/20 14:39:44 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "maths.h"
#include "minirt.h"

inline char	valid_t(t_quadratic *q)
{
	q->t_hit = q->t0;
	if (q->t_hit < IT_MIN || q->t_hit > IT_MAX)
	{
		q->t_hit = q->t1;
		if (q->t_hit < IT_MIN || q->t_hit > IT_MAX)
			return (0);
	}
	return (1);
}
