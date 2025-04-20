/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   qshort_axis.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 12:11:31 by madelvin          #+#    #+#             */
/*   Updated: 2025/04/18 18:54:27 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "bvh.h"

void	swap_uint32(uint32_t *a, uint32_t *b)
{
	uint32_t	temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

void	qsort_axis(uint32_t *arr, t_interval inter, t_bvh *bvh, int axis)
{
	int			i;
	int			j;
	uint32_t	pivot;

	if (inter.min >= inter.max)
		return ;
	pivot = arr[(int)inter.max];
	i = inter.min - 1;
	j = inter.min;
	while (j < inter.max)
	{
		if (compare_obj_axis_with_bvh(arr[j], pivot, bvh, axis) < 0)
		{
			i++;
			swap_uint32(&arr[i], &arr[j]);
		}
		j++;
	}
	swap_uint32(&arr[i + 1], &arr[(int)inter.max]);
	qsort_axis(arr, (t_interval){inter.min, i}, bvh, axis);
	qsort_axis(arr, (t_interval){i + 2, inter.max}, bvh, axis);
}
