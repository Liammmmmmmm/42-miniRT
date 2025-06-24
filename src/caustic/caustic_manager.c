/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   caustic_manager.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 19:36:31 by madelvin          #+#    #+#             */
/*   Updated: 2025/06/24 16:46:11 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include "structs.h"
#include "maths.h"
#include "minirt.h"
#include "caustic.h"

char	init_photon_map(t_kd_tree *photon_map, t_minirt *minirt, int photons_per_light)
{
	int			i;
	int			j;
	t_vector	all_photon;
	t_light		*light;
	size_t		capacity;
	size_t		count;

	if (!photon_map || !minirt || minirt->scene.obj_lst.light_nb == 0)
		return (print_error1("No light or no photon map init"));
	capacity = minirt->scene.obj_lst.light_nb * photons_per_light / 2;
	if (capacity < 1000)
		capacity = 1000;
	count = 0;
	if (vector_init(&all_photon, sizeof(t_photon *), capacity) == -1)
		return (print_error1("Init vector in init_photon_map"));
	printf("Photon emission...\n");
	i = -1;
	while (++i < minirt->scene.obj_lst.light_nb)
	{
		light = ((t_light *)minirt->scene.obj_lst.light_lst[i]->object);
		j = -1;
		while (++j < photons_per_light)
		{
			t_photon new_photon;
			if (trace_single_photon_path(minirt, light->position,
				(t_fcolor){
				light->color.r * light->brightness / photons_per_light,
				light->color.g * light->brightness / photons_per_light,
				light->color.b * light->brightness / photons_per_light
				}, &new_photon))
			{
				vector_add(&all_photon, &new_photon);
				count++;
			}
		}
	}
	printf("%zu photons store.\n", count);
	printf("K-D Tree build...\n");
	if (kd_tree_build(photon_map, (t_photon *)all_photon.data, count))
	{
		printf("K-D Tree not build.\n");
		free(all_photon.data);
		return (1);
	}
	else
		printf("K-D Tree build.\n");
	free(all_photon.data);
	return (0);
}

int	caustic_manager(t_minirt *minirt, int photons_to_shoot)
{
	kd_tree_destroy(&minirt->scene.photon_map);
	minirt->scene.photon_map = (t_kd_tree){0};
	if (init_photon_map(&minirt->scene.photon_map, minirt, photons_to_shoot))
		return (1);
	if (minirt->scene.photon_map.root)
	{
		printf("\nPhoton map created.\n");
		printf("totas photons in K-D Tree: %zu\n", minirt->scene.photon_map.photon_count);
	}
	else
	{
		kd_tree_destroy(&minirt->scene.photon_map);
		return (print_error1("K-D Tree not existing"));
	}
	return (0);
}
