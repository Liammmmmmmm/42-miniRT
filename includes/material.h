/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   material.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 18:32:20 by madelvin          #+#    #+#             */
/*   Updated: 2025/04/24 12:12:12 by madelvin         ###   ########.fr       */
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

t_ray_data	reflection_color(t_mat_manager *mat_man);
t_color	refracted_color(t_mat_manager *mat_man);

t_ray_data	material_mix(t_mat_manager *mat_man);
t_ray_data	material_default(t_mat_manager *mat_man);

t_ray_data	material_manager(t_mat_manager mat_man);


// v2

t_color	color_lerp(t_color a, t_color b, float t);


t_ray_data	material_mix_v2(t_mat_manager *mat_man);

/**
 * Calc color of a ray hitting a metallic surface
 * 
 * direction : bouncing ray
 * `R = I−2(I⋅N)N`
 * with
 * • `I` : the direction of the ray hitting the object
 * • `N` : normal of the point
 */
t_ray_data	metallic_material(t_mat_manager *mat_man);

t_ray_data	dielectric_non_transmissive_material(t_mat_manager *mat_man);
t_ray_data	dielectric_transmissive_material(t_mat_manager *mat_man);
t_ray_data	reflected_dielectric_color(t_mat_manager *mat_man);
t_color	emissive_material(t_mat_manager *mat_man);

double	get_reflect_value(t_mat_manager *mat_man);

double get_cos_theta(t_vec3 dir_in, t_vec3 normal);

#endif