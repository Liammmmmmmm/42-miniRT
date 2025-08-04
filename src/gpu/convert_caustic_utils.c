/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert_caustic_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: delmath <delmath@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 14:39:40 by madelvin          #+#    #+#             */
/*   Updated: 2025/08/01 17:12:02 by delmath          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gpu.h"

uint32_t	hash_cell(int ix, int iy, int iz, int table_size)
{
	const uint32_t	p1 = 73856093;
	const uint32_t	p2 = 19349663;
	const uint32_t	p3 = 83492791;
	uint32_t		n;

	n = (ix * p1) ^ (iy * p2) ^ (iz * p3);
	return (n % table_size);
}

void	compute_photons_aabb(t_photon *photons, int count,
	float *min, float *max)
{
	int	i;

	if (count == 0)
		return ;
	vec3_to_float3(&photons[0].position, min);
	vec3_to_float3(&photons[0].position, max);
	i = 1;
	while (i < count)
	{
		min[0] = fmin(min[0], photons[i].position.x);
		min[1] = fmin(min[1], photons[i].position.y);
		min[2] = fmin(min[2], photons[i].position.z);
		max[0] = fmax(max[0], photons[i].position.x);
		max[1] = fmax(max[1], photons[i].position.y);
		max[2] = fmax(max[2], photons[i].position.z);
		i++;
	}
}

void	ft_qsort_caustic(t_hash_entry *arr, int left, int right)
{
	const uint32_t	pivot = arr[left + (right - left) / 2].hash;
	t_hash_entry	tmp;
	int				i;
	int				j;

	if (left >= right)
		return ;
	i = left;
	j = right;
	while (i <= j)
	{
		while (arr[i].hash < pivot)
			i++;
		while (arr[j].hash > pivot)
			j--;
		if (i <= j)
		{
			tmp = arr[i];
			arr[i++] = arr[j];
			arr[j--] = tmp;
		}
	}
	ft_qsort_caustic(arr, left, j);
	ft_qsort_caustic(arr, i, right);
}
