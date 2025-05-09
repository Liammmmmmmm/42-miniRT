/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_register.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 18:40:21 by madelvin          #+#    #+#             */
/*   Updated: 2025/05/09 15:42:45 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "structs.h"
#include "maths.h"
#include "bvh.h"
#include <math.h>


t_color	get_hit_register_color(t_mat *mat, t_color color, t_hit_record *hit)
{
	if (mat)
	{
		if (mat->color_tex)
		{
			if (!mat->color_tex->img.pixel_data || !mat->color_tex->img.width || !mat->color_tex->img.height)
				return (get_solid_texture(hit->point, 2));
			else
				return (mat->color_tex->img.pixel_data[mat->color_tex->img.width * (int)(hit->v * mat->color_tex->img.height) + (int)(hit->u * mat->color_tex->img.width)]);
		}
		else
			return (mat->color_value);
	}
	return (color);
}

void	apply_normal_map(t_hit_record *hit)
{
	t_color	map;

	if (hit->mat == NULL || hit->mat->normal == NULL || hit->mat->normal->img.pixel_data == NULL)
		return ;
	
	map = hit->mat->normal->img.pixel_data[
		hit->mat->normal->img.width 
		* (int)(hit->v * 
			hit->mat->normal->img.height) + 
			(int)(hit->u *
				 hit->mat->normal->img.width)];
	
	t_vec3 normal_map;
	normal_map.x = (map.r / 127.5f) - 1.0f;
	normal_map.y = (map.g / 127.5f) - 1.0f;
	normal_map.z = (map.b / 127.5f) - 1.0f;

	double	tbn[3][3];
	t_vec3	tangent;
	t_vec3	bi_tangent;
	
	tangent = vec3_cross(hit->normal, (t_vec3){0, -1, 0});
	if (vec3_length(tangent) == 0)
		tangent = vec3_cross(hit->normal, (t_vec3){0, 0, 1});
	tangent = vec3_unit(tangent);
	bi_tangent = vec3_unit(vec3_cross(hit->normal, tangent));
	tbn[0][0] = tangent.x;
	tbn[1][0] = tangent.y;
	tbn[2][0] = tangent.z;
	tbn[0][1] = bi_tangent.x;
	tbn[1][1] = bi_tangent.y;
	tbn[2][1] = bi_tangent.z;
	tbn[0][2] = hit->normal.x;
	tbn[1][2] = hit->normal.y;
	tbn[2][2] = hit->normal.z;
	if (hit->mat->normal_intensity == 1.0)
		hit->normal = vec3_unit(matrix3_dot_vec3(tbn, normal_map));
	else
	{
		t_vec3 mapped_normal = vec3_unit(matrix3_dot_vec3(tbn, normal_map));
		hit->normal = vec3_unit(vec3_lerp(hit->normal, mapped_normal, hit->mat->normal_intensity));
	}
}

void	apply_roughness_map(t_hit_record *hit)
{
	t_color	map;

	if (hit->mat == NULL || hit->mat->roughness_tex == NULL || hit->mat->roughness_tex->img.pixel_data == NULL)
		return ;
	map = hit->mat->roughness_tex->img.pixel_data[hit->mat->roughness_tex->img.width * (int)(hit->v * hit->mat->roughness_tex->img.height) + (int)(hit->u * hit->mat->roughness_tex->img.width)];
	hit->mat->roughness_value = map.r / 255.0; // potentiellement remplacer map.r par (map.r + map.g + map.b) / 3 pour une secu en plus
}

void	apply_metallic_map(t_hit_record *hit)
{
	t_color	map;

	if (hit->mat == NULL || hit->mat->metallic_tex == NULL || hit->mat->metallic_tex->img.pixel_data == NULL)
		return ;
	map = hit->mat->metallic_tex->img.pixel_data[hit->mat->metallic_tex->img.width * (int)(hit->v * hit->mat->metallic_tex->img.height) + (int)(hit->u * hit->mat->metallic_tex->img.width)];
	hit->mat->metallic_value = map.r / 255.0; // potentiellement remplacer map.r par (map.r + map.g + map.b) / 3 pour une secu en plus
}

void	apply_ao_map(t_hit_record *hit)
{
	t_color	map;

	if (hit->mat == NULL || hit->mat->ao_tex == NULL || hit->mat->ao_tex->img.pixel_data == NULL)
		return ;
	else
	{
		map = hit->mat->ao_tex->img.pixel_data[hit->mat->ao_tex->img.width * (int)(hit->v * hit->mat->ao_tex->img.height) + (int)(hit->u * hit->mat->ao_tex->img.width)];
		hit->mat->ao_value = map.r / 255.0;
	}
}


char	hit_register_bvh(t_bvh *bvh, t_bvh_node *node, t_ray *ray, t_hit_record *hit_record)
{
	t_hit_record	temp_hit_record;
	t_interval		interval;
	char			hit_anything;
	uint32_t		i;
	uint32_t		prim_index;
	double			closest_t;
	t_object		*obj;

	i = 0;
	hit_anything = 0;
	interval.min = 0.001;
	interval.max = 1000;
	closest_t = 1000;
	temp_hit_record.part = DEFAULT;
	while (i < node->prim_count)
	{
		prim_index = bvh->prim_indices[node->first_prim + i];
		obj = bvh->obj_list[prim_index];
		if (obj->type == SPHERE && hit_sphere(((t_sphere *)obj->object), ray, interval, &temp_hit_record))
		{			
			if (temp_hit_record.t < closest_t)
			{
				hit_anything = 1;
				closest_t = temp_hit_record.t;
				*hit_record = temp_hit_record;
				hit_record->obj = obj;
				hit_record->mat = ((t_sphere *)obj->object)->material;
				apply_normal_map(hit_record);
				apply_roughness_map(hit_record);
				apply_metallic_map(hit_record);
				apply_ao_map(hit_record);
				hit_record->color = get_hit_register_color(((t_sphere *)obj->object)->material, ((t_sphere *)obj->object)->color, hit_record);
			}
		}
		else if (obj->type == CYLINDER && hit_cylinder(((t_cylinder *)obj->object), ray, interval, &temp_hit_record))
		{
			if (temp_hit_record.t < closest_t)
			{
					hit_anything = 1;
					closest_t = temp_hit_record.t;
					*hit_record = temp_hit_record;
					hit_record->obj = obj;
					apply_normal_map(hit_record);
					apply_roughness_map(hit_record);
					apply_metallic_map(hit_record);
					apply_ao_map(hit_record);
					if (hit_record->part == TOP_CAP && ((t_cylinder *)obj->object)->material_top)
						hit_record->color = get_hit_register_color(((t_cylinder *)obj->object)->material_top, ((t_cylinder *)obj->object)->color, hit_record);
					else if (hit_record->part == BOTTOM_CAP && ((t_cylinder *)obj->object)->material_bot)
						hit_record->color = get_hit_register_color(((t_cylinder *)obj->object)->material_bot, ((t_cylinder *)obj->object)->color, hit_record);
					else
						hit_record->color = get_hit_register_color(((t_cylinder *)obj->object)->material, ((t_cylinder *)obj->object)->color, hit_record);
			}
		}
		else if (obj->type == CONE && hit_cone(((t_cone *)obj->object), ray, interval, &temp_hit_record))
		{
			if (temp_hit_record.t < closest_t)
			{
					hit_anything = 1;
					closest_t = temp_hit_record.t;
					*hit_record = temp_hit_record;
					hit_record->obj = obj;
					apply_normal_map(hit_record);
					apply_roughness_map(hit_record);
					apply_metallic_map(hit_record);
					apply_ao_map(hit_record);
					if (hit_record->part == TOP_CAP && ((t_cone *)obj->object)->material_top)
						hit_record->color = get_hit_register_color(((t_cone *)obj->object)->material_top, ((t_cone *)obj->object)->color, hit_record);
					else
						hit_record->color = get_hit_register_color(((t_cone *)obj->object)->material, ((t_cone *)obj->object)->color, hit_record);
			}
		}
		else if (obj->type == HYPERBOLOID && hit_hyperboloid(((t_hyperboloid *)obj->object), ray, interval, &temp_hit_record))
		{
			if (temp_hit_record.t < closest_t)
			{
				hit_anything = 1;
				closest_t = temp_hit_record.t;
				*hit_record = temp_hit_record;
				hit_record->obj = obj;
				hit_record->mat = ((t_hyperboloid *)obj->object)->material;
				apply_normal_map(hit_record);
				apply_roughness_map(hit_record);
				apply_metallic_map(hit_record);
				apply_ao_map(hit_record);
				hit_record->color = get_hit_register_color(((t_hyperboloid *)obj->object)->material, ((t_hyperboloid *)obj->object)->color, hit_record);
			}
		}
		else if (obj->type == TRIANGLE && hit_triangle(((t_triangle *)obj->object), ray, interval, &temp_hit_record))
		{
			if (temp_hit_record.t < closest_t)
			{
				hit_anything = 1;
				*hit_record = temp_hit_record;
				// hit_record->mat = NULL;
				// hit_record->color.r = hit_record->u * 255;
				// hit_record->color.g = hit_record->v * 255;
				// hit_record->color.b = 0;
				// hit_record->color.r = (hit_record->normal.x + 1) * 127.5;
				// hit_record->color.g = (hit_record->normal.y + 1) * 127.5;
				// hit_record->color.b = (hit_record->normal.z + 1) * 127.5;
				hit_record->obj = obj;
				hit_record->mat = ((t_triangle *)obj->object)->material;
				apply_normal_map(hit_record);
				apply_roughness_map(hit_record);
				apply_metallic_map(hit_record);
				apply_ao_map(hit_record);
				hit_record->color = get_hit_register_color(hit_record->mat, ((t_triangle *)obj->object)->color, hit_record);
			}
		}
		i++;
	}
	return (hit_anything);
}

char	hit_register_all(t_minirt *minirt, t_ray *ray, t_hit_record *hit_record)
{
	char			hit;
	int				i;
	t_plane			*plane;
	t_object		**plane_lst;
	t_hit_record	temp_hit_record;

	hit = 0;
	temp_hit_record.part = DEFAULT;
	if (minirt->scene.bvh.valid == 1)
		hit = hit_bvh(&minirt->scene.bvh, 0, ray, hit_record);
	i = 0;
	plane_lst = minirt->scene.obj_lst.plane_lst;
	while (i < minirt->scene.obj_lst.plane_nb)
	{
		plane = (t_plane *)plane_lst[i]->object;
		if (hit_plane(plane, ray, (t_interval){0.001, 1000}, &temp_hit_record))
		{
			if (hit == 0 || temp_hit_record.t < hit_record->t)
			{
				hit = 1;
				*hit_record = temp_hit_record;
				hit_record->mat = plane->material;
				hit_record->obj = plane_lst[i];
				apply_normal_map(hit_record);
				apply_roughness_map(hit_record);
				apply_metallic_map(hit_record);
				hit_record->color = get_hit_register_color(plane->material, plane->color, hit_record);
			}
		}
		i++;
	}
	i = 0;
	while ((size_t)i < minirt->obj.triangle_count)
	{
		// printf("%f\n", minirt->obj.triangles[i].v0.pos.x);
		t_triangle triangle = minirt->obj.triangles[i];
		if (hit_triangle(&triangle, ray, (t_interval){0.001, 1000}, &temp_hit_record))
		{
			if (hit == 0 || temp_hit_record.t < hit_record->t)
			{
				hit = 1;
				*hit_record = temp_hit_record;
				hit_record->mat = NULL;
				hit_record->color.r = (hit_record->normal.x + 1) * 127.5;
				hit_record->color.g = (hit_record->normal.y + 1) * 127.5;
				hit_record->color.b = (hit_record->normal.z + 1) * 127.5;
				// hit_record->obj = plane_lst[i];
				// apply_normal_map(hit_record);
				// apply_roughness_map(hit_record);
				// apply_metallic_map(hit_record);
				// hit_record->color = get_hit_register_color(plane->material, plane->color, hit_record);
			}
		}
		i++;
	}
	return (hit);
}
