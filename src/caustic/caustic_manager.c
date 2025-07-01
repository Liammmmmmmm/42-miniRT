/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   caustic_manager.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 19:36:31 by madelvin          #+#    #+#             */
/*   Updated: 2025/07/01 19:51:14 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include "structs.h"
#include "maths.h"
#include "minirt.h"
#include "caustic.h"
#include "error_message.h"

static size_t	get_capacity(t_minirt *minirt, int photons_per_light)
{
	size_t	capacity;

	capacity = minirt->scene.obj_lst.light_nb * photons_per_light / 2;
	if (capacity < 1000)
		capacity = 1000;
	return (capacity);
}

static char	emit_photon_from_one_light(int photons_per_light, t_minirt *minirt,
	t_vector *photon_array, int i)
{
	int			j;
	t_light		*light;
	t_photon	new_photon;

	j = -1;
	light = ((t_light *)minirt->scene.obj_lst.light_lst[i]->object);
	while (++j < photons_per_light)
	{
		if (trace_single_photon_path(minirt, light->position,
				(t_fcolor){
				light->color.r * light->brightness / photons_per_light,
				light->color.g * light->brightness / photons_per_light,
				light->color.b * light->brightness / photons_per_light
			}, &new_photon))
			if (vector_add(photon_array, &new_photon) == -1)
				return (1);
		print_progress_bar(i * photons_per_light + j, photons_per_light
			* minirt->scene.obj_lst.light_nb);
	}
	return (0);
}

char	init_photon_map(t_minirt *minirt, int photons_per_light,
	t_vector *photon_array)
{
	int			i;

	if (minirt->scene.obj_lst.light_nb == 0)
		return (print_warn_rv(WARN_CAUSTIC_NO_LIGHT, 2));
	if (vector_init(photon_array, sizeof(t_photon),
			get_capacity(minirt, photons_per_light)) == -1)
		return (print_error1("Init vector in init_photon_map"));
	printf("Start photons emission\n");
	i = -1;
	while (++i < minirt->scene.obj_lst.light_nb)
	{
		if (emit_photon_from_one_light(photons_per_light, minirt,
			photon_array, i))
		{
			free(photon_array);
			return (print_error1("Add photon to vector."));
		}
	}
	print_progress_bar(i * photons_per_light, photons_per_light
		* minirt->scene.obj_lst.light_nb);
	printf("\n%zu photons stored.\n", photon_array->num_elements);
	printf("Start KD Tree build\n");
	return (0);
}

int	caustic_manager(t_minirt *minirt, int photons_to_shoot)
{
	char		rv;
	t_vector	photon_array;

	printf("Init photon map\n");
	kd_tree_destroy(&minirt->scene.photon_map);
	ft_bzero(&minirt->scene.photon_map, sizeof(t_kd_tree));
	rv = init_photon_map(minirt, photons_to_shoot, &photon_array);
	if (rv != 0)
		return (print_warn_rv("caustics will be disabled.", rv == 1));
	if (kd_tree_build(&minirt->scene.photon_map, &photon_array))
	{
		free(photon_array.data);
		return (print_error1(ERROR_CAUSTIC_KD_TREE_NO_BUILD));
	}
	free(photon_array.data);
	printf("Photon map and KD Tree build !\n\n");
	return (0);
}
