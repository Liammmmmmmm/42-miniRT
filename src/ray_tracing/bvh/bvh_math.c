/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh_math.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 12:31:37 by madelvin          #+#    #+#             */
/*   Updated: 2025/04/20 15:53:01 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "maths.h"
#include "bvh.h"

static inline t_aabb	compute_cylinder_bounds(t_cylinder *cyl)
{
	t_vec3	rvec;
	t_vec3	half_height_vec;
	t_vec3	top;
	t_vec3	bottom;

	double	radius = cyl->diameter * 0.5;
	double	half_height = cyl->height * 0.5;

	// Normalisée si besoin, pour la direction du cylindre
	t_vec3	dir = vec3_unit(cyl->orientation);

	// Les extrémités du cylindre (centre de chaque base)
	half_height_vec = vec3_multiply_scalar(dir, half_height);
	top = vec3_add(cyl->position, half_height_vec);
	bottom = vec3_subtract(cyl->position, half_height_vec);

	// Le rayon s’applique perpendiculairement à l'axe
	rvec = (t_vec3){radius, radius, radius};

	// Le AABB doit inclure les deux extrémités + rayon
	t_vec3 min = {
		ft_dmin(top.x, bottom.x) - radius,
		ft_dmin(top.y, bottom.y) - radius,
		ft_dmin(top.z, bottom.z) - radius
	};

	t_vec3 max = {
		ft_dmax(top.x, bottom.x) + radius,
		ft_dmax(top.y, bottom.y) + radius,
		ft_dmax(top.z, bottom.z) + radius
	};

	return ((t_aabb){.min = min, .max = max});
}


static inline t_aabb	compute_sphere_bounds(t_sphere *s)
{
	t_vec3	rvec;

	rvec = (t_vec3){s->radius, s->radius, s->radius};
	return ((t_aabb){.min = vec3_subtract(s->position, rvec), \
					.max = vec3_add(s->position, rvec)});
}

inline t_aabb	compute_object_bounds(t_object *obj)
{
	if (obj->type == SPHERE)
		return (compute_sphere_bounds(obj->object));
	return (compute_cylinder_bounds(obj->object));
}

int	compare_obj_axis_with_bvh(uint32_t a, uint32_t b, t_bvh *bvh, int axis)
{
	t_vec3	pos_a;
	t_vec3	pos_b;

	if (bvh->obj_list[a].type == SPHERE)
		pos_a = ((t_sphere *)bvh->obj_list[a].object)->position;
	else if (bvh->obj_list[a].type == CYLINDER)
		pos_a = ((t_cylinder *)bvh->obj_list[a].object)->position;

	if (bvh->obj_list[b].type == SPHERE)
		pos_b = ((t_sphere *)bvh->obj_list[b].object)->position;
	else if (bvh->obj_list[b].type == CYLINDER)
		pos_b = ((t_cylinder *)bvh->obj_list[b].object)->position;

	float va = (axis == 0) ? pos_a.x : (axis == 1) ? pos_a.y : pos_a.z;
	float vb = (axis == 0) ? pos_b.x : (axis == 1) ? pos_b.y : pos_b.z;

	if (va < vb)
		return (-1);
	return (va > vb);
}
