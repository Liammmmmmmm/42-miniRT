/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_object2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 11:46:29 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/10 15:26:03 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	create_default_sphere(t_object *obj)
{
	t_sphere	*sub;

	sub = malloc(sizeof(t_sphere));
	if (!sub)
		return ;
	ft_bzero(sub, sizeof(t_sphere));
	obj->object = sub;
	sub->position = (t_vec3){0, 0, 0};
	sub->radius = 1;
	sub->diameter = 2 * sub->radius;
	sub->sqrt_radius = sub->radius * sub->radius;
	sub->color = (t_color){255, 255, 255};
}

void	create_default_cyl(t_object *obj)
{
	t_cylinder	*sub;

	sub = malloc(sizeof(t_cylinder));
	if (!sub)
		return ;
	ft_bzero(sub, sizeof(t_cylinder));
	obj->object = sub;
	sub->position = (t_vec3){0, 0, 0};
	sub->orientation = (t_vec3){0, 1, 0};
	sub->diameter = 1;
	sub->height = 2;
	sub->color = (t_color){255, 255, 255};
}

void	create_default_cone(t_object *obj)
{
	t_cone	*sub;

	sub = malloc(sizeof(t_cone));
	if (!sub)
		return ;
	ft_bzero(sub, sizeof(t_cone));
	obj->object = sub;
	sub->position = (t_vec3){0, 0, 0};
	sub->orientation = (t_vec3){0, 1, 0};
	sub->diameter = 1;
	sub->height = 2;
	sub->color = (t_color){255, 255, 255};
}

void	create_default_hyperboloid(t_object *obj)
{
	t_hyperboloid	*sub;

	sub = malloc(sizeof(t_hyperboloid));
	if (!sub)
		return ;
	ft_bzero(sub, sizeof(t_hyperboloid));
	obj->object = sub;
	sub->position = (t_vec3){0, 0, 0};
	sub->orientation = (t_vec3){0, 1, 0};
	sub->height = 2;
	sub->shape = 0.2;
	sub->a = 1;
	sub->b = 1;
	sub->c = 1.1;
	sub->color = (t_color){255, 255, 255};
}

void	create_default_plane(t_object *obj)
{
	t_plane	*sub;

	sub = malloc(sizeof(t_plane));
	if (!sub)
		return ;
	ft_bzero(sub, sizeof(t_plane));
	obj->object = sub;
	sub->position = (t_vec3){0, 0, 0};
	sub->normal = (t_vec3){0, 1, 0};
	sub->color = (t_color){255, 255, 255};
}
