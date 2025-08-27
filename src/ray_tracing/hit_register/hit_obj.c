/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_obj.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 11:52:51 by madelvin          #+#    #+#             */
/*   Updated: 2025/08/27 11:53:56 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "hit_register.h"
#include "maths.h"
#include "minirt.h"

static inline char	update_hit_cylinder(t_object *obj, t_hit_record *tmp,
	double *closest_t, t_hit_record *out)
{
	const t_cylinder	*c = obj->object;

	if (tmp->t >= *closest_t)
		return (0);
	if (tmp->part == TOP_CAP && c->material_top)
		tmp->mat = c->material_top;
	else if (tmp->part == BOTTOM_CAP && c->material_bot)
		tmp->mat = c->material_bot;
	else
		tmp->mat = c->material;
	*out = *tmp;
	*closest_t = tmp->t;
	return (1);
}

static inline char	update_hit_cone(t_object *obj, t_hit_record *tmp,
	double *closest_t, t_hit_record *out)
{
	const t_cone	*c = obj->object;

	if (tmp->t >= *closest_t)
		return (0);
	if (tmp->part == TOP_CAP && c->material_top)
		tmp->mat = c->material_top;
	else
		tmp->mat = c->material;
	*out = *tmp;
	*closest_t = tmp->t;
	return (1);
}

static inline char	update_hit_default(t_object *obj, t_hit_record *tmp,
	double *closest_t, t_hit_record *out)
{
	if (tmp->t >= *closest_t)
		return (0);
	if (obj->type == HYPERBOLOID)
		tmp->mat = ((t_hyperboloid *)obj->object)->material;
	if (obj->type == SPHERE)
		tmp->mat = ((t_sphere *)obj->object)->material;
	if (obj->type == TRIANGLE)
		tmp->mat = ((t_triangle *)obj->object)->material;
	*out = *tmp;
	*closest_t = tmp->t;
	return (1);
}

char	handle_hit(t_object *obj, t_hit_register_data *data,
	double *closest_t, t_hit_record *out)
{
	t_hit_record	tmp;

	tmp.part = DEFAULT;
	tmp.one_sided = out->one_sided;
	if (obj->type == SPHERE
		&& hit_sphere(obj->object, data->ray, &tmp))
		return (update_hit_default(obj, &tmp, closest_t, out));
	if (obj->type == HYPERBOLOID
		&& hit_hyperboloid(obj->object, data->ray, &tmp, IT_MAX))
		return (update_hit_default(obj, &tmp, closest_t, out));
	if (obj->type == TRIANGLE
		&& hit_triangle(obj->object, data->ray, &tmp))
		return (update_hit_default(obj, &tmp, closest_t, out));
	if (obj->type == CYLINDER
		&& hit_cylinder(obj->object, data->ray, &tmp))
		return (update_hit_cylinder(obj, &tmp, closest_t, out));
	if (obj->type == CONE
		&& hit_cone(obj->object, data->ray, &tmp))
		return (update_hit_cone(obj, &tmp, closest_t, out));
	return (0);
}
