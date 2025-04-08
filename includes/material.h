/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   material.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 18:32:20 by madelvin          #+#    #+#             */
/*   Updated: 2025/04/08 20:57:08 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MATERIAL_H
# define MATERIAL_H

# include "structs.h"

# define SPEC_STRENGTH 1.0
# define SHINE 64.0

t_vec3  refracted_vec(const t_vec3 uv, const t_vec3 n, double ri);
double	reflectance(double cosine, double refraction_index);
t_color	fresnel_schlick_blend_color(double cos_theta, double refract_index, \
	t_color c1, t_color c2);

t_vec3	reflection_vec(const t_vec3 uv, const t_vec3 n);

t_color	reflection_color(t_mat_manager *mat_man);
t_color	refracted_color(t_mat_manager *mat_man);

t_color	material_mix(t_mat_manager *mat_man);
t_color	material_default(t_mat_manager *mat_man);

t_color	material_manager(t_mat_manager mat_man);

#endif