/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   qshort_axis.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 12:11:31 by madelvin          #+#    #+#             */
/*   Updated: 2025/06/03 17:33:36 by madelvin         ###   ########.fr       */
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

// void	qsort_axis(uint32_t *arr, t_interval inter, t_bvh *bvh, int axis)
// {
// 	int			i;
// 	int			j;
// 	uint32_t	pivot;

// 	if (inter.min >= inter.max)
// 		return ;
// 	pivot = arr[(int)inter.max];
// 	i = inter.min - 1;
// 	j = inter.min;
// 	while (j < inter.max)
// 	{
// 		if (compare_obj_axis_with_bvh(arr[j], pivot, bvh, axis) < 0)
// 		{
// 			i++;
// 			swap_uint32(&arr[i], &arr[j]);
// 		}
// 		j++;
// 	}
// 	swap_uint32(&arr[i + 1], &arr[(int)inter.max]);
// 	qsort_axis(arr, (t_interval){inter.min, i}, bvh, axis);
// 	qsort_axis(arr, (t_interval){i + 2, inter.max}, bvh, axis);
// }

void insertion_sort_axis(uint32_t *arr, t_interval inter, t_bvh *bvh, int axis)
{
    for (int i = inter.min + 1; i <= inter.max; i++) {
        uint32_t key = arr[i];
        int j = i - 1;
        
        // Déplacer les éléments plus grands que la clé vers la droite
        while (j >= inter.min && compare_obj_axis_with_bvh(arr[j], key, bvh, axis) > 0) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void qsort_axis(uint32_t *arr, t_interval inter, t_bvh *bvh, int axis)
{
    while (inter.min < inter.max) {
        if (inter.max - inter.min < 16) {
            insertion_sort_axis(arr, inter, bvh, axis);
            return;
        }
        
        // Choix du pivot médiane de trois
        int mid = inter.min + (inter.max - inter.min) / 2;
        if (compare_obj_axis_with_bvh(arr[(int)inter.min], arr[mid], bvh, axis) > 0)
            swap_uint32(&arr[(int)inter.min], &arr[mid]);
        if (compare_obj_axis_with_bvh(arr[(int)inter.min], arr[(int)inter.max], bvh, axis) > 0)
            swap_uint32(&arr[(int)inter.min], &arr[(int)inter.max]);
        if (compare_obj_axis_with_bvh(arr[mid], arr[(int)inter.max], bvh, axis) > 0)
            swap_uint32(&arr[mid], &arr[(int)inter.max]);
        
        uint32_t pivot = arr[mid];
        swap_uint32(&arr[mid], &arr[(int)inter.max - 1]);
        
        // Partition
        int i = inter.min;
        int j = inter.max - 1;
        for (;;) {
            while (compare_obj_axis_with_bvh(arr[++i], pivot, bvh, axis) < 0);
            while (compare_obj_axis_with_bvh(arr[--j], pivot, bvh, axis) > 0);
            if (i >= j) break;
            swap_uint32(&arr[i], &arr[j]);
        }
        swap_uint32(&arr[i], &arr[(int)inter.max - 1]);
        
        // Trier la plus petite partition en premier
        if (i - inter.min < inter.max - i) {
            qsort_axis(arr, (t_interval){inter.min, i - 1}, bvh, axis);
            inter.min = i + 1;
        } else {
            qsort_axis(arr, (t_interval){i + 1, inter.max}, bvh, axis);
            inter.max = i - 1;
        }
    }
}
