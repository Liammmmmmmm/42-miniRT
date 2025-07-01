/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   caustic_manager.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 19:36:31 by madelvin          #+#    #+#             */
/*   Updated: 2025/07/01 13:55:02 by madelvin         ###   ########.fr       */
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

char    init_photon_map(t_minirt *minirt,
		int photons_per_light, t_vector *photon_array)
{
    int         i;
    int         j;
	t_photon	new_photon;
    t_light     *light;

    if (minirt->scene.obj_lst.light_nb == 0)
        return (print_warn_rv(WARN_CAUSTIC_NO_LIGHT, 2));
    if (vector_init(photon_array, sizeof(t_photon),
			get_capacity(minirt, photons_per_light)) == -1)
        return (print_error1("Init vector in init_photon_map"));
    printf("Start photons emission\n");
    i = -1;
    while (++i < minirt->scene.obj_lst.light_nb)
    {
        light = ((t_light *)minirt->scene.obj_lst.light_lst[i]->object);
        j = -1;
        while (++j < photons_per_light)
        {
            if (trace_single_photon_path(minirt, light->position,
                (t_fcolor){
                light->color.r * light->brightness / photons_per_light,
                light->color.g * light->brightness / photons_per_light,
                light->color.b * light->brightness / photons_per_light
                }, &new_photon))
                vector_add(photon_array, &new_photon);
            print_progress_bar(i * photons_per_light + j, photons_per_light * minirt->scene.obj_lst.light_nb);
        }
    }
	print_progress_bar(i * photons_per_light, photons_per_light * minirt->scene.obj_lst.light_nb);
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
