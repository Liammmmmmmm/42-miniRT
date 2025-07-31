/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert_caustic.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 14:39:40 by madelvin          #+#    #+#             */
/*   Updated: 2025/07/31 18:30:51 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gpu.h"

static uint32_t hash_cell(int ix, int iy, int iz, int table_size)
{
    const uint32_t p1 = 73856093;
    const uint32_t p2 = 19349663;
    const uint32_t p3 = 83492791;
    uint32_t n = (ix * p1) ^ (iy * p2) ^ (iz * p3);
    return (n % table_size);
}

static int compare_hash_entries(const void *a, const void *b)
{
    t_hash_entry *entry_a = (t_hash_entry *)a;
    t_hash_entry *entry_b = (t_hash_entry *)b;
    if (entry_a->hash < entry_b->hash) return -1;
    if (entry_a->hash > entry_b->hash) return 1;
    return (0);
}

static void compute_photons_aabb(t_photon *photons, int count,
    float *min, float *max)
{
    if (count == 0) return;
    vec3_to_float3(&photons[0].position, min);
    vec3_to_float3(&photons[0].position, max);
    for (int i = 1; i < count; i++)
    {
        min[0] = fmin(min[0], photons[i].position.x);
        min[1] = fmin(min[1], photons[i].position.y);
        min[2] = fmin(min[2], photons[i].position.z);
        max[0] = fmax(max[0], photons[i].position.x);
        max[1] = fmax(max[1], photons[i].position.y);
        max[2] = fmax(max[2], photons[i].position.z);
    }
}

int build_caustic_grid(t_scene *scene, t_gpu_structs *gpu, float cell_size, int table_size)
{
    if (gpu->photon_am == 0)
        return (0);
    gpu->photons = malloc(sizeof(t_gpu_photon) * gpu->photon_am);
    gpu->cells_am = table_size;
    gpu->cells = malloc(sizeof(t_gpu_cell) * table_size);
    gpu->photon_indices = malloc(sizeof(uint32_t) * gpu->photon_am);
    t_hash_entry *hash_entries = malloc(sizeof(t_hash_entry) * gpu->photon_am);

    if (!gpu->photons || !gpu->cells || !gpu->photon_indices || !hash_entries)
    {
        free(gpu->photons);
        free(gpu->cells);
        free(gpu->photon_indices);
        free(hash_entries);
        return (1);
    }
    compute_photons_aabb(scene->photon_map.photons, gpu->photon_am, gpu->grid_world_min, gpu->grid_world_max);
    gpu->cell_size = cell_size;
    gpu->table_size = table_size;

    for (uint32_t i = 0; i < gpu->photon_am; i++)
    {
        t_vec3 p = scene->photon_map.photons[i].position;
        int ix = floor((p.x - gpu->grid_world_min[0]) / cell_size);
        int iy = floor((p.y - gpu->grid_world_min[1]) / cell_size);
        int iz = floor((p.z - gpu->grid_world_min[2]) / cell_size);

        hash_entries[i].hash = hash_cell(ix, iy, iz, table_size);
        hash_entries[i].photon_index = i;
    }
    qsort(hash_entries, gpu->photon_am, sizeof(t_hash_entry), compare_hash_entries);

    ft_memset(gpu->cells, 0, sizeof(t_gpu_cell) * table_size);
    for (uint32_t i = 0; i < gpu->photon_am; i++)
    {
        uint32_t hash = hash_entries[i].hash;
        gpu->photon_indices[i] = hash_entries[i].photon_index;

        if (i == 0 || hash != hash_entries[i - 1].hash)
        {
            gpu->cells[hash].start_index = i;
        }
        gpu->cells[hash].count++;
    }

    for (uint32_t i = 0; i < gpu->photon_am; i++)
    {
        gpu->photons[i].position[0] = scene->photon_map.photons[i].position.x;
        gpu->photons[i].position[1] = scene->photon_map.photons[i].position.y;
        gpu->photons[i].position[2] = scene->photon_map.photons[i].position.z;
        gpu->photons[i].position[3] = 1.0f;
        gpu->photons[i].color[0] = scene->photon_map.photons[i].power.r;
        gpu->photons[i].color[1] = scene->photon_map.photons[i].power.g;
        gpu->photons[i].color[2] = scene->photon_map.photons[i].power.b;
        gpu->photons[i].color[3] = 1.0f;
    }

    free(hash_entries);
    printf("--- Analyse de la grille ---\n");
    int non_empty_cells = 0;
    int total_photons_in_cells = 0;
    for (int i = 0; i < table_size; i++)
    {
        if (gpu->cells[i].count > 0)
        {
            non_empty_cells++;
            total_photons_in_cells += gpu->cells[i].count;
        }
    }

    if (non_empty_cells > 0)
    {
        float avg_photons_per_cell = (float)total_photons_in_cells / (float)non_empty_cells;
        printf("-> Nombre total de photons: %d\n", gpu->photon_am);
        printf("-> Taille de la table de hachage: %d\n", table_size);
        printf("-> Cellules non-vides trouvées: %d\n", non_empty_cells);
        printf("-> Moyenne de photons par cellule non-vide: %.2f\n", avg_photons_per_cell);
    }
    else
    {
        printf("Aucune cellule non-vide trouvée. Problème possible avec la grille.\n");
    }
    printf("--------------------------\n");

    return (0);
}