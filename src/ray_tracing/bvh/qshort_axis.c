/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   qshort_axis.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: delmath <delmath@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 12:11:31 by madelvin          #+#    #+#             */
/*   Updated: 2025/06/04 15:13:45 by delmath          ###   ########.fr       */
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

void	insertion_sort_axis(uint32_t *arr, t_interval inter, t_bvh *bvh, int axis)
{
	int	i;
	int	j;

	i = inter.min + 1;
    while (i <= inter.max)
	{
        uint32_t key = arr[i];
        int j = i - 1;
        
        while (j >= inter.min && compare_obj_axis_with_bvh(arr[j], key, bvh, axis) > 0)
		{
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
		i++;
    }
}

uint32_t	select_pivot(uint32_t *arr, t_interval inter, t_bvh *bvh, int axis)
{
	int 		mid;
	uint32_t	pivot;

	mid = inter.min + (inter.max - inter.min) / 2;
	if (compare_obj_axis_with_bvh(arr[(int)inter.min], arr[mid], bvh, axis) > 0)
		swap_uint32(&arr[(int)inter.min], &arr[mid]);
	if (compare_obj_axis_with_bvh(arr[(int)inter.min], arr[(int)inter.max], bvh, axis) > 0)
		swap_uint32(&arr[(int)inter.min], &arr[(int)inter.max]);
	if (compare_obj_axis_with_bvh(arr[mid], arr[(int)inter.max], bvh, axis) > 0)
		swap_uint32(&arr[mid], &arr[(int)inter.max]);
	pivot = arr[select_pivot(arr, inter, bvh, axis)];
	swap_uint32(&arr[mid], &arr[(int)inter.max - 1]);
	return (pivot);
}

void	qsort_axis(uint32_t *arr, t_interval inter, t_bvh *bvh, int axis)
{
	int			i;
	int			j;
	uint32_t	pivot;

	while (inter.min < inter.max) 
	{
		if (inter.max - inter.min < 16) 
		{
			insertion_sort_axis(arr, inter, bvh, axis);
			return;
		}
		pivot = select_pivot(arr, inter, bvh, axis);
		i = inter.min;
		j = inter.max - 1;
		while (1)
		{
			while (compare_obj_axis_with_bvh(arr[++i], pivot, bvh, axis) < 0)
				;
			while (compare_obj_axis_with_bvh(arr[--j], pivot, bvh, axis) > 0)
				;
			if (i >= j)
				break;
			swap_uint32(&arr[i], &arr[j]);
		}
		swap_uint32(&arr[i], &arr[(int)inter.max - 1]);
		if (i - inter.min < inter.max - i)
		{
			qsort_axis(arr, (t_interval){inter.min, i - 1}, bvh, axis);
			inter.min = i + 1;
		}
		else
		{
			qsort_axis(arr, (t_interval){i + 1, inter.max}, bvh, axis);
			inter.max = i - 1;
		}
	}
}
