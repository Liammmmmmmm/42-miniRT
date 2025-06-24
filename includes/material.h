/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   material.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 18:32:20 by madelvin          #+#    #+#             */
/*   Updated: 2025/06/24 16:40:04 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MATERIAL_H
# define MATERIAL_H

# include "structs.h"

# define SPEC_STRENGTH 1.0
# define SHINE 64.0

t_vec3	reflection_vec(const t_vec3 uv, const t_vec3 n);

/**
 * Calc color of a ray hitting a metallic surface
 * 
 * direction : bouncing ray
 * `R = I−2(I⋅N)N`
 * with
 * • `I` : the direction of the ray hitting the object
 * • `N` : normal of the point
 */
void	metallic_color(t_ray *ray, t_hit_record	*hit_record, t_fcolor *power);

void	default_mat(t_minirt *minirt, t_ray *ray, t_hit_record *hit_record,
	t_ray_data data);
void	refracted_ray(t_minirt *minirt, t_ray *ray, t_hit_record *hit_record,
	t_fcolor *power);
void	dielectric_mat(t_minirt *minirt, t_ray *ray, t_hit_record *hit_record,
	t_ray_data data);
char	dielectric_mat_photon(t_minirt *minirt, t_ray *ray,
	t_hit_record *hit_record, t_fcolor *power);
char	default_mat_photon(t_hit_record *hit_record, t_fcolor *power);
	
double	get_cos_theta(t_vec3 dir_in, t_vec3 normal);
double	get_reflect_value(t_ray *ray_in, t_hit_record *hit_record);

t_vec3	ggx_sample_hemisphere(t_vec3 normal, double roughness);

#endif