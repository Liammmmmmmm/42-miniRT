/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   material.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 18:32:20 by madelvin          #+#    #+#             */
/*   Updated: 2025/06/20 10:30:12 by lilefebv         ###   ########lyon.fr   */
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

	
double	get_cos_theta(t_vec3 dir_in, t_vec3 normal);
double	get_reflect_value(t_ray *ray_in, t_hit_record *hit_record);

t_vec3	ggx_sample_hemisphere(t_vec3 normal, double roughness);

#endif