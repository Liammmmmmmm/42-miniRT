/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_object.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 11:46:29 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/04 15:33:32 by lilefebv         ###   ########lyon.fr   */
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

void	create_default_light(t_object *obj)
{
	t_light	*sub;
	
	sub = malloc(sizeof(t_light));
	if (!sub)
		return ;
	ft_bzero(sub, sizeof(t_light));
	obj->object = sub;
	sub->position = (t_vec3){0, 0, 0};
	sub->brightness = 1;
	sub->color = (t_color){255, 255, 255};
}

void	create_default_directional_light(t_object *obj)
{
	t_dlight	*sub;
	
	sub = malloc(sizeof(t_dlight));
	if (!sub)
		return ;
	ft_bzero(sub, sizeof(t_dlight));
	obj->object = sub;
	sub->orientation = (t_vec3){0, -1, 0};
	sub->brightness = 1;
	sub->color = (t_color){255, 255, 255};
}

void	create_default_obj(t_object *obj)
{
	obj->object = NULL;
	if (obj->type == SPHERE)
		create_default_sphere(obj);
	else if (obj->type == CYLINDER)
		create_default_cyl(obj);
	else if (obj->type == CONE)
		create_default_cone(obj);
	else if (obj->type == HYPERBOLOID)
		create_default_hyperboloid(obj);
	else if (obj->type == PLANE)
		create_default_plane(obj);
	else if (obj->type == LIGHT)
		create_default_light(obj);
	else if (obj->type == DIRECTIONAL_LIGHT)
		create_default_directional_light(obj);
}

void	add_object(void *vparam)
{
	t_minirt	*minirt;
	t_object	*new;
	int			i;

	minirt = ((t_btn_param *)vparam)->minirt;
	if ((long)minirt->controls.ui_infos.add_object == NULL_OBJ)
		return ;
	new = malloc((minirt->scene.el_amount + 1) * sizeof(t_object));
	if (!new)
		return ((void)print_error("Can allocate more space for an object"));
	i = -1;
	while (++i < minirt->scene.el_amount)
		new[i + 1] = minirt->scene.elements[i];
	new[0].type = (long)minirt->controls.ui_infos.add_object;
	create_default_obj(&new[0]);
	if (!new[0].object)
	{
		free(new);
		return ((void)print_error("Can allocate more space for an object"));
	}
	stop_minirt(minirt);
	minirt->scene.build_bvh = 1;
	minirt->scene.el_amount++;
	free(minirt->scene.elements);
	minirt->scene.elements = new;
	minirt->controls.ui_infos.selected_object = &new[0];
	set_selected_object(minirt, &new[0]);
}
