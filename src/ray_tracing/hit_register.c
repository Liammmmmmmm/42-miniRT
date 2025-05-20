/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_register.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 18:40:21 by madelvin          #+#    #+#             */
/*   Updated: 2025/05/20 14:55:31 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "structs.h"
#include "maths.h"
#include "bvh.h"
#include <math.h>

t_fcolor	get_hdr_pixel(t_hdr *hdr, int x, int y)
{
	double	scale;
	double	gamma_corr;

	if (hdr->exposure == 0)
		scale = ldexp(1.0, hdr->pixels[y * hdr->width + x].e - 128);
	else
		scale = ldexp(1.0, hdr->pixels[y * hdr->width + x].e - 128) * powf(2.0, hdr->exposure);
	if (hdr->gamma == 1.0)
		return ((t_fcolor){
			.r = hdr->pixels[y * hdr->width + x].r * scale / 255.0,
			.g = hdr->pixels[y * hdr->width + x].g * scale / 255.0,
			.b = hdr->pixels[y * hdr->width + x].b * scale / 255.0
		});
	else
	{
		gamma_corr = 1.0 / (hdr->gamma);
		return ((t_fcolor){
			.r = pow(hdr->pixels[y * hdr->width + x].r * scale / 255.0, gamma_corr),
			.g = pow(hdr->pixels[y * hdr->width + x].g * scale / 255.0, gamma_corr),
			.b = pow(hdr->pixels[y * hdr->width + x].b * scale / 255.0, gamma_corr)
		});
	}
}

t_fcolor	clamp_fcolor(t_fcolor color)
{
	return ((t_fcolor){
		.r = clamp_double(color.r),
		.g = clamp_double(color.g),
		.b = clamp_double(color.b)
	});
}

t_fcolor	get_tex_color(t_tex *tex, double u, double v, t_vec3 hit_point)
{
	if (tex->type == IMAGE)
	{
		if (!tex->img.pixel_data || !tex->img.width || !tex->img.height)
			return (get_solid_texture_default(hit_point, 2));
		else
			return (color_to_fcolor(tex->img.pixel_data[tex->img.width * (int)(v * (tex->img.height - 1)) + (int)(u * (tex->img.width - 1))]));
	}
	else if (tex->type == CHECKER_LOCAL)
	{
		int	a;
		int	b;

		a = floor(u * tex->checker.scale);
		b = floor(v * tex->checker.scale);
		if ((a + b) % 2 == 0)
			return (color_to_fcolor(tex->checker.c1));
		return (color_to_fcolor(tex->checker.c2));
	}
	else if (tex->type == CHECKER_GLOBAL)
		return (get_solid_texture(hit_point, tex->checker.c1, tex->checker.c2, tex->checker.scale));
	else if (tex->type == HDR)
	{
		if (!tex->hdr.pixels || !tex->hdr.width || !tex->hdr.height)
			return (get_solid_texture_default(hit_point, 2));
		else
			return (get_hdr_pixel(&tex->hdr, (u * (tex->img.width - 1)), (v * (tex->img.height - 1))));
	}
	return ((t_fcolor){0.0, 0.0, 0.0});
}


t_fcolor	get_hit_register_color(t_mat *mat, t_color color, t_hit_record *hit, t_bvh *bvh)
{
	if (bvh->normal_mode)
	{
		return ((t_fcolor){
			.r = (hit->normal.x + 1) * 0.5,
			.g = (hit->normal.z + 1) * 0.5,
			.b = (hit->normal.y + 1) * 0.5
		});
	}
	if (mat)
	{
		if (mat->color_tex)
			return (get_tex_color(mat->color_tex, hit->u, hit->v, hit->point));
		else
			return (color_to_fcolor(mat->color_value));
	}
	return (color_to_fcolor(color));
}

void	apply_normal_map(t_hit_record *hit)
{
	t_color	map;

	if (hit->mat == NULL || hit->mat->normal == NULL || hit->mat->normal->img.pixel_data == NULL)
		return ;
	// 	printf("y = %d | x = %d\n image size: %dx%d\n result: %d\n", (int)(hit->v * hit->mat->normal->img.height), (int)(hit->u * hit->mat->normal->img.width), hit->mat->normal->img.height, hit->mat->normal->img.width, (hit->mat->normal->img.width - 1 )
	// * (int)(hit->v * 
	// 	(hit->mat->normal->img.height - 1)) + 
	// 	(int)(hit->u *
	// 		 (hit->mat->normal->img.width - 1)));
	map = hit->mat->normal->img.pixel_data[
		(hit->mat->normal->img.width)
		* (int)(hit->v * 
			(hit->mat->normal->img.height - 1)) + 
			(int)(hit->u *
				 (hit->mat->normal->img.width - 1))];
	
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
	if (hit->mat == NULL || hit->mat->roughness_tex == NULL)
		return ;
	hit->mat->roughness_value = get_tex_color(hit->mat->roughness_tex, hit->u, hit->v, hit->point).r;
}

void	apply_metallic_map(t_hit_record *hit)
{
	if (hit->mat == NULL || hit->mat->metallic_tex == NULL)
		return ;
	hit->mat->metallic_value = get_tex_color(hit->mat->metallic_tex, hit->u, hit->v, hit->point).r;
}

void	apply_ao_map(t_hit_record *hit)
{
	if (hit->mat == NULL || hit->mat->ao_tex == NULL)
		return ;
	hit->mat->ao_value = get_tex_color(hit->mat->ao_tex, hit->u, hit->v, hit->point).r;
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
				hit_record->color = get_hit_register_color(((t_sphere *)obj->object)->material, ((t_sphere *)obj->object)->color, hit_record, bvh);
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
						hit_record->color = get_hit_register_color(((t_cylinder *)obj->object)->material_top, ((t_cylinder *)obj->object)->color, hit_record, bvh);
					else if (hit_record->part == BOTTOM_CAP && ((t_cylinder *)obj->object)->material_bot)
						hit_record->color = get_hit_register_color(((t_cylinder *)obj->object)->material_bot, ((t_cylinder *)obj->object)->color, hit_record, bvh);
					else
						hit_record->color = get_hit_register_color(((t_cylinder *)obj->object)->material, ((t_cylinder *)obj->object)->color, hit_record, bvh);
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
						hit_record->color = get_hit_register_color(((t_cone *)obj->object)->material_top, ((t_cone *)obj->object)->color, hit_record, bvh);
					else
						hit_record->color = get_hit_register_color(((t_cone *)obj->object)->material, ((t_cone *)obj->object)->color, hit_record, bvh);
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
				hit_record->color = get_hit_register_color(((t_hyperboloid *)obj->object)->material, ((t_hyperboloid *)obj->object)->color, hit_record, bvh);
			}
		}
		else if (obj->type == TRIANGLE && hit_triangle(((t_triangle *)obj->object), ray, interval, &temp_hit_record))
		{
			if (temp_hit_record.t < closest_t)
			{
				hit_anything = 1;
				*hit_record = temp_hit_record;
				hit_record->obj = obj;
				hit_record->mat = ((t_triangle *)obj->object)->material;
				apply_normal_map(hit_record);
				apply_roughness_map(hit_record);
				apply_metallic_map(hit_record);
				apply_ao_map(hit_record);
				hit_record->color = get_hit_register_color(hit_record->mat, ((t_triangle *)obj->object)->color, hit_record, bvh);
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
				hit_record->color = get_hit_register_color(plane->material, plane->color, hit_record, &minirt->scene.bvh);
			}
		}
		i++;
	}
	return (hit);
}
