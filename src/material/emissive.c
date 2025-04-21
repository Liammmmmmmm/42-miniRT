/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   emissive.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 10:52:06 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/21 11:32:14 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_color	emissive_material(t_mat_manager *mat_man)
{

	if (mat_man->hit_record.mat->emission_strength == 1)
		return (mat_man->hit_record.mat->emission_color);
	return (color_scale(mat_man->hit_record.mat->emission_color, mat_man->hit_record.mat->emission_strength));
}


/*

t_color emissive_material(t_mat_manager *mat_man)
{
    t_vec3 normal = mat_man->hit_record.normal;
    t_vec3 view_dir = vec3_negate(mat_man->ray_in.dir);

    double glow_factor = fmax(0.0, vec3_dot(normal, view_dir));
    t_color emission = vec3_multiply_scalar(
        mat_man->hit_record.mat->emission_color,
        mat_man->hit_record.mat->emission_strength * glow_factor
    );
    return emission;
}

*/