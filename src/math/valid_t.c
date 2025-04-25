/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   valid_t.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 17:24:00 by madelvin          #+#    #+#             */
/*   Updated: 2025/04/25 17:24:37 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "maths.h"

inline char	valid_t(t_quadratic *q, t_interval i)
{
	q->t_hit = q->t0;
	if (q->t_hit < i.min || q->t_hit > i.max)
	{
		q->t_hit = q->t1;
		if (q->t_hit < i.min || q->t_hit > i.max)
			return (0);
	}
	return (1);
}
