/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert_caustic.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 14:39:40 by madelvin          #+#    #+#             */
/*   Updated: 2025/08/04 16:50:04 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "gpu.h"

void	copy_photons_to_gpu(t_scene *scene, t_gpu_structs *gpu)
{
	t_vec3		pos;
	t_fcolor	power;
	uint32_t	i;

	i = 0;
	while (i < gpu->photon_am)
	{
		pos = scene->photon_map.photons[i].position;
		power = scene->photon_map.photons[i].power;
		gpu->photons[i].position[0] = pos.x;
		gpu->photons[i].position[1] = pos.y;
		gpu->photons[i].position[2] = pos.z;
		gpu->photons[i].position[3] = 1.0f;
		gpu->photons[i].color[0] = power.r;
		gpu->photons[i].color[1] = power.g;
		gpu->photons[i].color[2] = power.b;
		gpu->photons[i].color[3] = 1.0f;
		i++;
	}
}

void	assign_cells_and_indices(t_gpu_structs *gpu, t_hash_entry *entries)
{
	uint32_t	i;
	uint32_t	hash;

	i = 0;
	ft_memset(gpu->cells, 0, sizeof(t_gpu_cell) * gpu->table_size);
	while (i < gpu->photon_am)
	{
		hash = entries[i].hash;
		gpu->photon_indices[i] = entries[i].photon_index;
		if (i == 0 || hash != entries[i - 1].hash)
			gpu->cells[hash].start_index = i;
		gpu->cells[hash].count++;
		i++;
	}
}

void	fill_hash_entries(t_scene *scene, t_gpu_structs *gpu,
	t_hash_entry *entries)
{
	uint32_t	i;
	int			axe[3];

	i = 0;
	while (i < gpu->photon_am)
	{
		t_vec3 p = scene->photon_map.photons[i].position;
		axe[0] = floor((p.x - gpu->grid_world_min[0]) / gpu->cell_size);
		axe[1] = floor((p.y - gpu->grid_world_min[1]) / gpu->cell_size);
		axe[2] = floor((p.z - gpu->grid_world_min[2]) / gpu->cell_size);
		entries[i].hash = hash_cell(axe[0], axe[1], axe[2], gpu->table_size);
		entries[i].photon_index = i;
		i++;
	}
}

void	setup_grid_bounds_and_params(t_scene *scene, t_gpu_structs *gpu,
	float cell_size, int table_size)
{
	compute_photons_aabb(scene->photon_map.photons, gpu->photon_am,
		gpu->grid_world_min, gpu->grid_world_max);
	gpu->cell_size = cell_size;
	gpu->table_size = table_size;
}

int	build_caustic_grid(t_scene *scene, t_gpu_structs *gpu, float cell_size,
	int table_size)
{
	t_hash_entry	*hash_entries;

	if (gpu->photon_am == 0)
		return (0);
	hash_entries = malloc(sizeof(t_hash_entry) * gpu->photon_am);
	if (!hash_entries)
		return (print_warn_rv("Hash entries malloc error, Caustic disable", 1));
	setup_grid_bounds_and_params(scene, gpu, cell_size, table_size);
	fill_hash_entries(scene, gpu, hash_entries);
	ft_qsort_caustic(hash_entries, 0, gpu->photon_am - 1);
	assign_cells_and_indices(gpu, hash_entries);
	copy_photons_to_gpu(scene, gpu);
	free(hash_entries);
	return (0);
}
