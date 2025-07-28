/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gpu_scene.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 16:03:21 by lilefebv          #+#    #+#             */
/*   Updated: 2025/07/28 16:47:04 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "gpu.h"
#include "gpu_scene.h"
#include <stdio.h>
#include "maths.h"

void	vec3_to_float3(t_vec3 *vec, float *tab)
{
	tab[0] = vec->x;
	tab[1] = vec->y;
	tab[2] = vec->z;
}

void	vert_to_gpu_vert(t_vertex *ver, t_gpu_vertex *gpu_ver)
{
	vec3_to_float3(&ver->normal, gpu_ver->normal);
	vec3_to_float3(&ver->pos, gpu_ver->pos);
	gpu_ver->u = ver->u;
	gpu_ver->v = ver->v;
}

void	color_to_float3(t_color *vec, float *tab)
{
	tab[0] = vec->r / 255.0;
	tab[1] = vec->g / 255.0;
	tab[2] = vec->b / 255.0;
}

void	fcolor_to_float3(t_fcolor *vec, float *tab)
{
	tab[0] = vec->r;
	tab[1] = vec->g;
	tab[2] = vec->b;
}

void convert_all_obj(t_scene *cpu_scene, t_gpu_structs *gpu_scene)
{
	uint32_t	i;
	uint32_t	sphere;
	uint32_t	cone;
	uint32_t	cylindre;
	uint32_t	hyper;
	uint32_t	triangle;

	i = 0;
	sphere = 0;
	cone = 0;
	cylindre = 0;
	hyper = 0;
	triangle = 0;
	while (i < cpu_scene->bvh.size)
	{
		if (cpu_scene->bvh.obj_list[i]->type == SPHERE)
		{
			gpu_scene->prim_types_indices[i].indice = sphere;
			gpu_scene->prim_types_indices[i].type = 0;
			gpu_scene->spheres[sphere].radius = ((t_sphere *)cpu_scene->bvh.obj_list[i]->object)->radius;
			vec3_to_float3(&((t_sphere *)cpu_scene->bvh.obj_list[i]->object)->position, gpu_scene->spheres[sphere].position);
			color_to_float3(&((t_sphere *)cpu_scene->bvh.obj_list[i]->object)->color, gpu_scene->spheres[sphere].color);
			if (((t_sphere *)cpu_scene->bvh.obj_list[i]->object)->material == NULL)
				gpu_scene->spheres[sphere].material_id = -1;
			else
				gpu_scene->spheres[sphere].material_id = ((t_sphere *)cpu_scene->bvh.obj_list[i]->object)->material - cpu_scene->materials;
			sphere++;
		}
		else if (cpu_scene->bvh.obj_list[i]->type == CYLINDER)
		{
			gpu_scene->prim_types_indices[i].indice = cylindre;
			gpu_scene->prim_types_indices[i].type = 1;
			gpu_scene->cylinders[cylindre].height = ((t_cylinder *)cpu_scene->bvh.obj_list[i]->object)->height;
			gpu_scene->cylinders[cylindre].diameter = ((t_cylinder *)cpu_scene->bvh.obj_list[i]->object)->diameter;
			vec3_to_float3(&((t_cylinder *)cpu_scene->bvh.obj_list[i]->object)->position, gpu_scene->cylinders[cylindre].position);
			vec3_to_float3(&((t_cylinder *)cpu_scene->bvh.obj_list[i]->object)->orientation, gpu_scene->cylinders[cylindre].orientation);
			color_to_float3(&((t_cylinder *)cpu_scene->bvh.obj_list[i]->object)->color, gpu_scene->cylinders[cylindre].color);
			if (((t_cylinder *)cpu_scene->bvh.obj_list[i]->object)->material == NULL)
				gpu_scene->cylinders[cylindre].material_id = -1;
			else
				gpu_scene->cylinders[cylindre].material_id = ((t_cylinder *)cpu_scene->bvh.obj_list[i]->object)->material - cpu_scene->materials;
			if (((t_cylinder *)cpu_scene->bvh.obj_list[i]->object)->material_bot == NULL)
				gpu_scene->cylinders[cylindre].material_id_top = -1;
			else
				gpu_scene->cylinders[cylindre].material_id_top = ((t_cylinder *)cpu_scene->bvh.obj_list[i]->object)->material_top - cpu_scene->materials;
			if (((t_cylinder *)cpu_scene->bvh.obj_list[i]->object)->material_bot == NULL)
				gpu_scene->cylinders[cylindre].material_id_bot = -1;
			else
				gpu_scene->cylinders[cylindre].material_id_bot = ((t_cylinder *)cpu_scene->bvh.obj_list[i]->object)->material_bot - cpu_scene->materials;
			cylindre++;
		}
		else if (cpu_scene->bvh.obj_list[i]->type == CONE)
		{
			gpu_scene->prim_types_indices[i].indice = cone;
			gpu_scene->prim_types_indices[i].type = 2;
			gpu_scene->cones[cone].height = ((t_cone *)cpu_scene->bvh.obj_list[i]->object)->height;
			gpu_scene->cones[cone].diameter = ((t_cone *)cpu_scene->bvh.obj_list[i]->object)->diameter;
			vec3_to_float3(&((t_cone *)cpu_scene->bvh.obj_list[i]->object)->orientation, gpu_scene->cones[cone].orientation);
			vec3_to_float3(&((t_cone *)cpu_scene->bvh.obj_list[i]->object)->position, gpu_scene->cones[cone].position);
			color_to_float3(&((t_cone *)cpu_scene->bvh.obj_list[i]->object)->color, gpu_scene->cones[cone].color);
			if (((t_cone *)cpu_scene->bvh.obj_list[i]->object)->material == NULL)
				gpu_scene->cones[cone].material_id = -1;
			else
				gpu_scene->cones[cone].material_id = ((t_cone *)cpu_scene->bvh.obj_list[i]->object)->material - cpu_scene->materials;
			if (((t_cone *)cpu_scene->bvh.obj_list[i]->object)->material_top == NULL)
				gpu_scene->cones[cone].material_id_top = -1;
			else
				gpu_scene->cones[cone].material_id_top = ((t_cone *)cpu_scene->bvh.obj_list[i]->object)->material_top - cpu_scene->materials;
			cone++;
		}
		else if (cpu_scene->bvh.obj_list[i]->type == HYPERBOLOID)
		{
			gpu_scene->prim_types_indices[i].indice = hyper;
			gpu_scene->prim_types_indices[i].type = 3;
			gpu_scene->hypers[hyper].a = ((t_hyperboloid *)cpu_scene->bvh.obj_list[i]->object)->a;
			gpu_scene->hypers[hyper].b = ((t_hyperboloid *)cpu_scene->bvh.obj_list[i]->object)->b;
			gpu_scene->hypers[hyper].c = ((t_hyperboloid *)cpu_scene->bvh.obj_list[i]->object)->c;
			gpu_scene->hypers[hyper].shape = ((t_hyperboloid *)cpu_scene->bvh.obj_list[i]->object)->shape;
			gpu_scene->hypers[hyper].height = ((t_hyperboloid *)cpu_scene->bvh.obj_list[i]->object)->height;
			vec3_to_float3(&((t_hyperboloid *)cpu_scene->bvh.obj_list[i]->object)->orientation, gpu_scene->hypers[hyper].orientation);
			vec3_to_float3(&((t_hyperboloid *)cpu_scene->bvh.obj_list[i]->object)->position, gpu_scene->hypers[hyper].position);
			color_to_float3(&((t_hyperboloid *)cpu_scene->bvh.obj_list[i]->object)->color, gpu_scene->hypers[hyper].color);
			if (((t_hyperboloid *)cpu_scene->bvh.obj_list[i]->object)->material == NULL)
				gpu_scene->hypers[hyper].material_id = -1;
			else
				gpu_scene->hypers[hyper].material_id = ((t_hyperboloid *)cpu_scene->bvh.obj_list[i]->object)->material - cpu_scene->materials;
			hyper++;
		}
		else if (cpu_scene->bvh.obj_list[i]->type == TRIANGLE)
		{
			gpu_scene->prim_types_indices[i].indice = triangle;
			gpu_scene->prim_types_indices[i].type = 4;
			vert_to_gpu_vert(&((t_triangle *)cpu_scene->bvh.obj_list[i]->object)->v0, &gpu_scene->triangles[triangle].v0);
			vert_to_gpu_vert(&((t_triangle *)cpu_scene->bvh.obj_list[i]->object)->v1, &gpu_scene->triangles[triangle].v1);
			vert_to_gpu_vert(&((t_triangle *)cpu_scene->bvh.obj_list[i]->object)->v2, &gpu_scene->triangles[triangle].v2);
			vec3_to_float3(&((t_triangle *)cpu_scene->bvh.obj_list[i]->object)->center, gpu_scene->triangles[triangle].center);
			gpu_scene->triangles[triangle].obj_id = -1;
			color_to_float3(&((t_triangle *)cpu_scene->bvh.obj_list[i]->object)->color,	gpu_scene->triangles[triangle].color);
			if (((t_triangle *)cpu_scene->bvh.obj_list[i]->object)->material == NULL)
				gpu_scene->triangles[triangle].material_id = -1;
			else
				gpu_scene->triangles[triangle].material_id = ((t_triangle *)cpu_scene->bvh.obj_list[i]->object)->material - cpu_scene->materials;
			triangle++;
		}
		i++;
	}
}

void	convert_lights(t_scene *cpu_scene, t_gpu_structs *gpu_scene)
{
	int	lights;

	lights = 0;
	while (lights < cpu_scene->obj_lst.light_nb)
	{
		if (cpu_scene->obj_lst.light_lst[lights]->type == LIGHT)
		{
			vec3_to_float3(&((t_light *)cpu_scene->obj_lst.light_lst[lights]->object)->position, gpu_scene->lights[lights].pos);
			color_to_float3(&((t_light *)cpu_scene->obj_lst.light_lst[lights]->object)->color, gpu_scene->lights[lights].color);
			gpu_scene->lights[lights].type = 0;
			gpu_scene->lights[lights].brightness = ((t_light *)cpu_scene->obj_lst.light_lst[lights]->object)->brightness;
			gpu_scene->lights[lights].radius = ((t_light *)cpu_scene->obj_lst.light_lst[lights]->object)->radius;
			gpu_scene->lights[lights].shadow_sample = (t_uint)((t_light *)cpu_scene->obj_lst.light_lst[lights]->object)->shadow_sample;

			printf("Color %u : %f %f %f\n", lights, gpu_scene->lights[lights].color[0], gpu_scene->lights[lights].color[1], gpu_scene->lights[lights].color[2]);
	
			lights++;
		}
		else if (cpu_scene->obj_lst.light_lst[lights]->type == DIRECTIONAL_LIGHT)
		{
			vec3_to_float3(&((t_dlight *)cpu_scene->obj_lst.light_lst[lights]->object)->orientation, gpu_scene->lights[lights].pos);
			color_to_float3(&((t_dlight *)cpu_scene->obj_lst.light_lst[lights]->object)->color, gpu_scene->lights[lights].color);
			gpu_scene->lights[lights].type = 1;
			gpu_scene->lights[lights].brightness = ((t_dlight *)cpu_scene->obj_lst.light_lst[lights]->object)->brightness;
			gpu_scene->lights[lights].radius = 0;
			gpu_scene->lights[lights].shadow_sample = 0;
			lights++;
		}
	}
}

void convert_plane(t_scene *cpu_scene, t_gpu_plane *gpu)
{
    int i;

    i = 0;
    while (i < cpu_scene->obj_lst.plane_nb)
    {
        t_plane* p = (t_plane*)cpu_scene->obj_lst.plane_lst[i]->object;
        vec3_to_float3(&p->position, gpu[i].position);
        vec3_to_float3(&p->normal, gpu[i].normal);
        color_to_float3(&p->color, gpu[i].color);
        if (p->material == NULL)
            gpu[i].material_id = -1;
        else
            gpu[i].material_id = p->material - cpu_scene->materials;
        gpu[i].d = (p->position.x * p->normal.x +
                    p->position.y * p->normal.y +
                    p->position.z * p->normal.z);
        i++;
    }
}

void	convert_bvh_node(t_scene *cpu_scene, t_gpu_bvh_node *bvh_node)
{
	uint32_t	i;

	i = 0;
	while (i < (cpu_scene->bvh.bvh_nodes_used))
	{
		bvh_node[i].first_prim = cpu_scene->bvh.prim_indices[cpu_scene->bvh.bvh_nodes[i].first_prim];
		bvh_node[i].is_leaf = cpu_scene->bvh.bvh_nodes[i].is_leaf;
		bvh_node[i].right_child = cpu_scene->bvh.bvh_nodes[i].right_child;
		bvh_node[i].left_child = cpu_scene->bvh.bvh_nodes[i].left_child;
		vec3_to_float3(&cpu_scene->bvh.bvh_nodes[i].node_bounds.max, bvh_node[i].node_bounds.max);
		vec3_to_float3(&cpu_scene->bvh.bvh_nodes[i].node_bounds.min, bvh_node[i].node_bounds.min);
		bvh_node[i].prim_count = cpu_scene->bvh.bvh_nodes[i].prim_count;
		i++;
	}
}

void	convert_camera(t_scene *cpu_scene, t_gpu_camera *cam)
{
	vec3_to_float3(&cpu_scene->camera.position, cam->position);
	vec3_to_float3(&cpu_scene->camera.orientation, cam->orientation);
	cam->fov = (float)cpu_scene->camera.fov;
	cam->defocus_angle = (float)cpu_scene->camera.defocus_angle;
	cam->focus_dist = (float)cpu_scene->camera.focus_dist;
}

void	convert_ambiant(t_scene *cpu_scene, t_gpu_amb_light *amb, t_scene *scene)
{
	color_to_float3(&cpu_scene->amb_light.skybox_c, amb->skybox_color);
	amb->ratio = (float)cpu_scene->amb_light.ratio;
	amb->skybox_tex_index = cpu_scene->amb_light.skybox_t - scene->textures;
	if (cpu_scene->amb_light.skybox_t == NULL)
		amb->skybox_tex_index = -1;
}

void	convert_viewport(t_gpu_viewport *dst, t_viewport *src, double ior)
{
	dst->focal_length = src->focal_length;
	dst->height = src->height;
	dst->width = src->width;
	dst->render_w = src->render_w;
	dst->render_h = src->render_h;
	dst->gamma = (float)src->gamma;
	dst->defocus_radius = (float)src->defocus_radius;
	dst->max_bounces = src->max_bounces;
	dst->ior_global = ior;
	vec3_to_float3(&src->u, dst->u);
	vec3_to_float3(&src->v, dst->v);
	vec3_to_float3(&src->pixel_delta_u, dst->pixel_delta_u);
	vec3_to_float3(&src->pixel_delta_v, dst->pixel_delta_v);
	vec3_to_float3(&src->upper_left, dst->upper_left);
	vec3_to_float3(&src->pixel00_loc, dst->pixel00_loc);
	vec3_to_float3(&src->defocus_disk_u, dst->defocus_disk_u);
	vec3_to_float3(&src->defocus_disk_v, dst->defocus_disk_v);
}

void	count_obj(t_scene *scene, t_gpu_structs *gpu_structs)
{
	uint32_t	i;

	i = 0;
	while (i < scene->bvh.size)
	{
		if (scene->bvh.obj_list[i]->type == SPHERE)
			gpu_structs->spheres_am++;
		else if (scene->bvh.obj_list[i]->type == CYLINDER)
			gpu_structs->cylinders_am++;	
		else if (scene->bvh.obj_list[i]->type == CONE)
			gpu_structs->cones_am++;
		else if (scene->bvh.obj_list[i]->type == HYPERBOLOID)
			gpu_structs->hypers_am++;
		else if (scene->bvh.obj_list[i]->type == TRIANGLE)
			gpu_structs->triangles_am++;
		i++;
	}
}

void	convert_material(t_gpu_material *dst, t_mat *src, t_scene *scene)
{
	color_to_float3(&src->color_value, dst->color_value);
	dst->color_tex_index = src->color_tex - scene->textures;
	if (src->color_tex == NULL)
		dst->color_tex_index = -1;
	dst->metallic_value = src->metallic_value;
	dst->metallic_tex_index = src->metallic_tex - scene->textures;
	if (src->metallic_tex == NULL)
		dst->metallic_tex_index = -1;
	dst->roughness_value = src->roughness_value;
	dst->roughness_tex_index = src->roughness_tex - scene->textures;
	if (src->roughness_tex == NULL)
		dst->roughness_tex_index = -1;
	dst->ior = src->ior;
	dst->transmission_value = src->transmission_value;
	dst->transmission_tex_index = src->transmission_tex - scene->textures;
	if (src->transmission_tex == NULL)
		dst->transmission_tex_index = -1;
	dst->ao_value = src->ao_value;
	dst->ao_tex_index = src->ao_tex - scene->textures;
	if (src->ao_tex == NULL)
		dst->ao_tex_index = -1;
	dst->emission_strength = src->emission_strength;
	dst->emission_strength_tex_index = src->emission_strength_tex - scene->textures;
	if (src->emission_strength_tex == NULL)
		dst->emission_strength_tex_index = -1;
	dst->scale = src->scale;
	fcolor_to_float3(&src->emission_color, dst->emission_color);
	dst->emission_color_tex_index = src->emission_color_tex - scene->textures;
	if (src->emission_color_tex == NULL)
		dst->emission_color_tex_index = -1;
	dst->normal_tex_index = src->normal - scene->textures;
	if (src->normal == NULL)
		dst->normal_tex_index = -1;
	dst->normal_intensity = src->normal_intensity;
}

void	convert_materials(t_scene *cpu_scene, t_gpu_material *gpu)
{
	int	i;

	i = -1;
	while (++i < cpu_scene->mat_amount)
	{
		convert_material(&gpu[i], &cpu_scene->materials[i], cpu_scene);
	}
}

void	convert_textures(t_scene *scene, t_gpu_textures *gpu_textures)
{
	int		i;
	int		img_i;
	int		check_i;
	uint32_t	image_offset;

	i = -1;
	img_i = 0;
	check_i = 0;
	image_offset = 0;
	while (++i < scene->tex_amount)
	{
		gpu_textures->textures_types_indices[i].type = scene->textures[i].type;
		printf("Type %d : %d\n", i, gpu_textures->textures_types_indices[i].type);
		if (scene->textures[i].type == CHECKER_GLOBAL || scene->textures[i].type == CHECKER_LOCAL)
		{
			gpu_textures->textures_types_indices[i].indice = check_i;
			gpu_textures->checkers[check_i].scale = scene->textures[i].checker.scale;
			color_to_float3(&scene->textures[i].checker.c1, gpu_textures->checkers[check_i].c1);
			color_to_float3(&scene->textures[i].checker.c2, gpu_textures->checkers[check_i].c2);
			check_i++;
		}
		if (scene->textures[i].type == HDR || scene->textures[i].type == IMAGE)
		{
			gpu_textures->textures_types_indices[i].indice = img_i;
			
			if (scene->textures[i].type == IMAGE)
			{
				if (scene->textures[i].img.rgba != NULL)
				{
					gpu_textures->images[img_i].height = scene->textures[i].img.height;
					gpu_textures->images[img_i].width = scene->textures[i].img.width;
					gpu_textures->images[img_i].offset = image_offset;
	
					t_uint	y = 0;
					t_uint	tpx = scene->textures[i].img.height * scene->textures[i].img.width;
					while (y < tpx)
					{
						gpu_textures->images_stream[image_offset * 4 + y * 4] = scene->textures[i].img.rgba[y].r / 255.0;
						gpu_textures->images_stream[image_offset * 4 + y * 4 + 1] = scene->textures[i].img.rgba[y].g / 255.0;
						gpu_textures->images_stream[image_offset * 4 + y * 4 + 2] = scene->textures[i].img.rgba[y].b / 255.0;
						gpu_textures->images_stream[image_offset * 4 + y * 4 + 3] = scene->textures[i].img.rgba[y].a / 255.0;
						y++;
					}
					image_offset += tpx;
				}
			}
			else
			{
				if (scene->textures[i].hdr.pixels != NULL)
				{
					gpu_textures->images[img_i].height = scene->textures[i].hdr.height;
					gpu_textures->images[img_i].width = scene->textures[i].hdr.width;
					gpu_textures->images[img_i].offset = image_offset;
					gpu_textures->images[img_i].exposure = scene->textures[i].hdr.exposure;
	
					t_uint	y = 0;
					t_uint	tpx = scene->textures[i].hdr.height * scene->textures[i].hdr.width;
					while (y < tpx)
					{
						gpu_textures->images_stream[image_offset * 4 + y * 4] = scene->textures[i].hdr.pixels[y].r;
						gpu_textures->images_stream[image_offset * 4 + y * 4 + 1] = scene->textures[i].hdr.pixels[y].g;
						gpu_textures->images_stream[image_offset * 4 + y * 4 + 2] = scene->textures[i].hdr.pixels[y].b;
						gpu_textures->images_stream[image_offset * 4 + y * 4 + 3] = scene->textures[i].hdr.pixels[y].e;
						y++;
					}
					image_offset += tpx;
				}
			}
			img_i++;
		}
	}
}

void	count_tex(t_scene *scene, t_gpu_textures *gpu_textures)
{
	int	i;

	i = -1;
	while (++i < scene->tex_amount)
	{
		if (scene->textures[i].type == CHECKER_GLOBAL || scene->textures[i].type == CHECKER_LOCAL)
			gpu_textures->checkers_am++;
		if (scene->textures[i].type == HDR || scene->textures[i].type == IMAGE)
		{
			gpu_textures->images_am++;
			if (scene->textures[i].type == IMAGE)
			{
				if (scene->textures[i].img.rgba == NULL)
					continue ;
				gpu_textures->total_pixel_images += scene->textures[i].img.height * scene->textures[i].img.width;
			}
			else
			{
				if (scene->textures[i].hdr.pixels == NULL)
					continue ;
				gpu_textures->total_pixel_images += scene->textures[i].hdr.height * scene->textures[i].hdr.width;
			}
		}
	}
}

void	create_ssbo(GLuint *ssbo, size_t size, void *data, long bind)
{
	glGenBuffers(1, ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, *ssbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, GL_STATIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bind, *ssbo);
}

int	convert_textures_init(t_scene *scene, t_gpu_textures *gpu_tex)
{
	// ---------------- TEXTURES ---------------- //
	count_tex(scene, gpu_tex);
	gpu_tex->checkers = ft_calloc(gpu_tex->checkers_am, sizeof(t_gpu_checker));
	gpu_tex->images = ft_calloc(gpu_tex->images_am, sizeof(t_gpu_tex_data));
	gpu_tex->images_stream = ft_calloc(gpu_tex->total_pixel_images * 4, sizeof(float));
	gpu_tex->textures_types_indices = ft_calloc(scene->tex_amount, sizeof(t_type_indice));
	// SECURE LES MALLOC
	convert_textures(scene, gpu_tex);
	create_ssbo(&gpu_tex->checkers_ssbo, sizeof(t_gpu_checker) * gpu_tex->checkers_am, gpu_tex->checkers, SSBO_BIND_CHECKERS);
	create_ssbo(&gpu_tex->images_ssbo, sizeof(t_gpu_tex_data) * gpu_tex->images_am, gpu_tex->images, SSBO_BIND_IMAGES);
	create_ssbo(&gpu_tex->images_stream_ssbo, sizeof(float) * gpu_tex->total_pixel_images * 4, gpu_tex->images_stream, SSBO_BIND_IMAGES_STREAM);
	create_ssbo(&gpu_tex->textures_types_indices_ssbo, sizeof(t_type_indice) * scene->tex_amount, gpu_tex->textures_types_indices, SSBO_BIND_TEX_TYPE_INDICE);

	free(gpu_tex->checkers);
	free(gpu_tex->images);
	free(gpu_tex->images_stream);
	free(gpu_tex->textures_types_indices);

	return (0);
}

GLuint create_texture2d_from_data(const float* data, int width, int height)
{
	GLuint texture_id;
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);

	glTexImage2D(
		GL_TEXTURE_2D,		// Target
		0,					// Mipmap level
		GL_R32F,			// Format interne (1 canal float)
		width, height,		// Dimensions
		0,					// Border (doit être 0)
		GL_RED,				// Format des données (1 canal)
		GL_FLOAT,			// Type des données
		data				// Pointeur vers les données
	);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	return (texture_id);
}

GLuint create_texture1d_from_data(const float* data, int width)
{
    GLuint texture_id;
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_1D, texture_id);

    glTexImage1D(
        GL_TEXTURE_1D,      // Target
        0,                  // Mipmap level
        GL_R32F,            // Format interne (1 canal float)
        width,              // Taille de la texture (1 dimension)
        0,                  // Border (doit être 0)
        GL_RED,             // Format des données (1 canal)
        GL_FLOAT,           // Type des données
        data                // Pointeur vers les données
    );

    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

    return texture_id;
}

int	convert_scene(t_minirt *minirt, t_scene *scene, t_viewport *viewport, t_gpu_structs *gpu_structs)
{
	clean_scene(gpu_structs);

	convert_viewport(&gpu_structs->viewport, viewport, scene->ior_all);
	convert_camera(scene, &gpu_structs->camera);
	convert_ambiant(scene, &gpu_structs->ambiant, scene);

	
	// ---------------- MATERIALS ---------------- //
	gpu_structs->mat_am = scene->mat_amount;
	gpu_structs->mat = ft_calloc(gpu_structs->mat_am, sizeof(t_gpu_material));
	if (!gpu_structs->mat)
		return (-1); // Tout free correctement
	convert_materials(scene, gpu_structs->mat);
	create_ssbo(&gpu_structs->mat_ssbo, sizeof(t_gpu_material) * gpu_structs->mat_am, gpu_structs->mat, SSBO_BIND_MATERIALS);

	// ---------------- OBJECTS ---------------- //
	count_obj(scene, gpu_structs);
	gpu_structs->lights_am = scene->obj_lst.light_nb;
	gpu_structs->spheres = ft_calloc(gpu_structs->spheres_am, sizeof(t_gpu_sphere));
	gpu_structs->hypers = ft_calloc(gpu_structs->hypers_am, sizeof(t_gpu_hyper));
	gpu_structs->cylinders = ft_calloc(gpu_structs->cylinders_am, sizeof(t_gpu_cylinder));
	gpu_structs->cones = ft_calloc(gpu_structs->cones_am, sizeof(t_gpu_cone));
	gpu_structs->triangles = ft_calloc(gpu_structs->triangles_am, sizeof(t_gpu_triangle));
	gpu_structs->lights = ft_calloc(gpu_structs->lights_am, sizeof(t_gpu_light));
	gpu_structs->prim_types_indices = ft_calloc(scene->bvh.size, sizeof(t_type_indice));
	// if (!gpu_structs->spheres)
	// 	return (-1); // Tout free correctement
	convert_all_obj(scene, gpu_structs);
	convert_lights(scene, gpu_structs);
	create_ssbo(&gpu_structs->spheres_ssbo, sizeof(t_gpu_sphere) * gpu_structs->spheres_am, gpu_structs->spheres, SSBO_BIND_SPHERES);
	create_ssbo(&gpu_structs->hypers_ssbo, sizeof(t_gpu_hyper) * gpu_structs->hypers_am, gpu_structs->hypers, SSBO_BIND_HYPERS);
	create_ssbo(&gpu_structs->cylinders_ssbo, sizeof(t_gpu_cylinder) * gpu_structs->cylinders_am, gpu_structs->cylinders, SSBO_BIND_CYLINDERS);
	create_ssbo(&gpu_structs->cones_ssbo, sizeof(t_gpu_cone) * gpu_structs->cones_am, gpu_structs->cones, SSBO_BIND_CONES);
	create_ssbo(&gpu_structs->triangles_ssbo, sizeof(t_gpu_triangle) * gpu_structs->triangles_am, gpu_structs->triangles, SSBO_BIND_TRIANGLES);
	create_ssbo(&gpu_structs->lights_ssbo, sizeof(t_gpu_light) * gpu_structs->lights_am, gpu_structs->lights, SSBO_BIND_LIGHTS);

	gpu_structs->bvh_node_am = scene->bvh.bvh_nodes_used;
	gpu_structs->bvh_node = ft_calloc(gpu_structs->bvh_node_am, sizeof(t_gpu_bvh_node));
	if (!gpu_structs->bvh_node)
		return (-1); // Tout free correctement
	convert_bvh_node(scene, gpu_structs->bvh_node);
	create_ssbo(&gpu_structs->bvh_node_ssbo, sizeof(t_gpu_bvh_node) * gpu_structs->bvh_node_am, gpu_structs->bvh_node, SSBO_BIND_BVH);
	
	gpu_structs->prim_indice_am = scene->bvh.size;
	create_ssbo(&gpu_structs->prim_types_indices_ssbo, sizeof(t_type_indice) * gpu_structs->prim_indice_am, gpu_structs->prim_types_indices, SSBO_BIND_PRIM_TYPE_INDICE);

	gpu_structs->planes_am = scene->obj_lst.plane_nb;
	gpu_structs->planes = ft_calloc(gpu_structs->planes_am, sizeof(t_gpu_plane));
	if (!gpu_structs->planes)
		return (-1); // Tout free correctement
	convert_plane(scene, gpu_structs->planes);
	create_ssbo(&gpu_structs->planes_ssbo, sizeof(t_gpu_plane) * gpu_structs->planes_am, gpu_structs->planes, SSBO_BIND_PLANE);
	

	glUseProgram(minirt->shaders_data.program);
	GLuint width_loc = glGetUniformLocation(minirt->shaders_data.program, "width_render");
	glUniform1ui(width_loc, minirt->scene.render_width);
	GLuint height_loc = glGetUniformLocation(minirt->shaders_data.program, "height_render");
	glUniform1ui(height_loc, minirt->scene.render_height);
	GLuint max_bounces = glGetUniformLocation(minirt->shaders_data.program, "max_bounces");
	glUniform1ui(max_bounces, (t_uint)minirt->controls.max_bounces);
	GLuint spheres_am = glGetUniformLocation(minirt->shaders_data.program, "spheres_am");
	glUniform1ui(spheres_am, (t_uint)minirt->shaders_data.scene.spheres_am);
	GLuint planes_am = glGetUniformLocation(minirt->shaders_data.program, "planes_am");
	glUniform1ui(planes_am, (t_uint)minirt->shaders_data.scene.planes_am);
	GLuint cylinders_am = glGetUniformLocation(minirt->shaders_data.program, "cylinders_am");
	glUniform1ui(cylinders_am, (t_uint)minirt->shaders_data.scene.cylinders_am);
	GLuint cones_am = glGetUniformLocation(minirt->shaders_data.program, "cones_am");
	glUniform1ui(cones_am, (t_uint)minirt->shaders_data.scene.cones_am);
	GLuint hypers_am = glGetUniformLocation(minirt->shaders_data.program, "hypers_am");
	glUniform1ui(hypers_am, (t_uint)minirt->shaders_data.scene.hypers_am);
	GLuint triangles_am = glGetUniformLocation(minirt->shaders_data.program, "triangles_am");
	glUniform1ui(triangles_am, (t_uint)minirt->shaders_data.scene.triangles_am);

	GLuint lights_am = glGetUniformLocation(minirt->shaders_data.program, "lights_am");
	glUniform1ui(lights_am, (t_uint)minirt->shaders_data.scene.lights_am);


	glUniform3fv(glGetUniformLocation(minirt->shaders_data.program, "cam.position"), 1, minirt->shaders_data.scene.camera.position);
	glUniform3fv(glGetUniformLocation(minirt->shaders_data.program, "cam.orientation"), 1, minirt->shaders_data.scene.camera.orientation);
	glUniform1f(glGetUniformLocation(minirt->shaders_data.program, "cam.fov"), minirt->shaders_data.scene.camera.fov);
	glUniform1f(glGetUniformLocation(minirt->shaders_data.program, "cam.defocus_angle"), minirt->shaders_data.scene.camera.defocus_angle);
	glUniform1f(glGetUniformLocation(minirt->shaders_data.program, "cam.focus_dist"), minirt->shaders_data.scene.camera.focus_dist);

	glUniform3fv(glGetUniformLocation(minirt->shaders_data.program, "viewport.u"), 1, minirt->shaders_data.scene.viewport.u);
	glUniform3fv(glGetUniformLocation(minirt->shaders_data.program, "viewport.v"), 1, minirt->shaders_data.scene.viewport.v);
	glUniform3fv(glGetUniformLocation(minirt->shaders_data.program, "viewport.pixel_delta_u"), 1, minirt->shaders_data.scene.viewport.pixel_delta_u);
	glUniform3fv(glGetUniformLocation(minirt->shaders_data.program, "viewport.pixel_delta_v"), 1, minirt->shaders_data.scene.viewport.pixel_delta_v);
	glUniform3fv(glGetUniformLocation(minirt->shaders_data.program, "viewport.defocus_disk_u"), 1, minirt->shaders_data.scene.viewport.defocus_disk_u);
	glUniform3fv(glGetUniformLocation(minirt->shaders_data.program, "viewport.defocus_disk_v"), 1, minirt->shaders_data.scene.viewport.defocus_disk_v);
	glUniform3fv(glGetUniformLocation(minirt->shaders_data.program, "viewport.upper_left"), 1, minirt->shaders_data.scene.viewport.upper_left);
	glUniform3fv(glGetUniformLocation(minirt->shaders_data.program, "viewport.pixel00_loc"), 1, minirt->shaders_data.scene.viewport.pixel00_loc);
	glUniform1f(glGetUniformLocation(minirt->shaders_data.program, "viewport.height"), minirt->shaders_data.scene.viewport.height);
	glUniform1f(glGetUniformLocation(minirt->shaders_data.program, "viewport.width"), minirt->shaders_data.scene.viewport.width);
	glUniform1i(glGetUniformLocation(minirt->shaders_data.program, "viewport.render_w"), minirt->shaders_data.scene.viewport.render_w);
	glUniform1i(glGetUniformLocation(minirt->shaders_data.program, "viewport.render_h"), minirt->shaders_data.scene.viewport.render_h);
	glUniform1f(glGetUniformLocation(minirt->shaders_data.program, "viewport.focal_length"), minirt->shaders_data.scene.viewport.focal_length);
	glUniform1f(glGetUniformLocation(minirt->shaders_data.program, "viewport.gamma"), minirt->shaders_data.scene.viewport.gamma);
	glUniform1f(glGetUniformLocation(minirt->shaders_data.program, "viewport.defocus_radius"), minirt->shaders_data.scene.viewport.defocus_radius);
	glUniform1i(glGetUniformLocation(minirt->shaders_data.program, "viewport.max_bounces"), minirt->shaders_data.scene.viewport.max_bounces);
	glUniform1f(glGetUniformLocation(minirt->shaders_data.program, "viewport.ior_global"), minirt->shaders_data.scene.viewport.ior_global);

	glUniform3fv(glGetUniformLocation(minirt->shaders_data.program, "ambiant.skybox_color"), 1, minirt->shaders_data.scene.ambiant.skybox_color);
	glUniform1f(glGetUniformLocation(minirt->shaders_data.program, "ambiant.ratio"), minirt->shaders_data.scene.ambiant.ratio);
	glUniform1i(glGetUniformLocation(minirt->shaders_data.program, "ambiant.skybox_tex_index"), minirt->shaders_data.scene.ambiant.skybox_tex_index);


	// -------- IMPORTANCE SAMPLING --------- //

	// cdf_conditional_inverse 2d
	// cdf_marginal_inverse 1d height
	// pdf_joint 2d

	clean_importance_sampling(&minirt->shaders_data.tex);
	
	if (scene->amb_light.skybox_t && scene->amb_light.skybox_t->hdr.pixels)
	{
		minirt->shaders_data.tex.cdf_ci_texture2d = create_texture2d_from_data(scene->amb_light.cdf_conditional_inverse, scene->amb_light.skybox_t->hdr.width, scene->amb_light.skybox_t->hdr.height);
		GLuint cdf_ci_location = glGetUniformLocation(minirt->shaders_data.program, "cdf_conditional_inverse");
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, minirt->shaders_data.tex.cdf_ci_texture2d);
		glUniform1i(cdf_ci_location, 0);

		minirt->shaders_data.tex.cdf_mi_texture2d = create_texture1d_from_data(scene->amb_light.cdf_marginal_inverse, scene->amb_light.skybox_t->hdr.height);
		GLuint cdf_mi_location = glGetUniformLocation(minirt->shaders_data.program, "cdf_marginal_inverse");
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_1D, minirt->shaders_data.tex.cdf_mi_texture2d);
		glUniform1i(cdf_mi_location, 2);

		minirt->shaders_data.tex.pdf_joint_texture2d = create_texture2d_from_data(scene->amb_light.pdf_joint, scene->amb_light.skybox_t->hdr.width, scene->amb_light.skybox_t->hdr.height);
		GLuint pdf_joint_location = glGetUniformLocation(minirt->shaders_data.program, "pdf_joint");
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, minirt->shaders_data.tex.pdf_joint_texture2d);
		glUniform1i(pdf_joint_location, 1);
	}

	// int    i;
	// i = 0;
	// while (i < gpu_structs->spheres_am)
	// {
	// 	 printf("sphere %d | color %f/%f/%f | radius %f\n\n\n", i, gpu_structs->spheres[i].color[0], gpu_structs->spheres[i].color[1], gpu_structs->spheres[i].color[2], gpu_structs->spheres[i].radius);
	// 	i++;
	// }
    // i = 0;
    // while (i < gpu_structs->bvh_node_am)
    // {
    //     if (gpu_structs->bvh_node[i].is_leaf)
    //         printf("%d | prim pos %d | prim count %d | min %f/%f/%f | max %f/%f/%f\n", i, gpu_structs->bvh_node[i].first_prim, gpu_structs->bvh_node[i].prim_count, gpu_structs->bvh_node[i].node_bounds.min[0], gpu_structs->bvh_node[i].node_bounds.min[1], gpu_structs->bvh_node[i].node_bounds.min[2]);
    //     else
    //         printf("%d | left_child %d | right child %d | min %f/%f/%f | max %f/%f/%f\n", i, gpu_structs->bvh_node[i].left_child, gpu_structs->bvh_node[i].right_child, gpu_structs->bvh_node[i].node_bounds.min[1], gpu_structs->bvh_node[i].node_bounds.min[2]);
    //     i++;
    // }
    // printf("\n\n");
    // i = 0;
    // while (i < gpu_structs->prim_indice_am)
    // {
    //     printf("%d | prim indice %d | prim type %d \n", i, gpu_structs->prim_types_indices[i].indice, gpu_structs->prim_types_indices[i].type);
    //     i++;
    // }


	return (0);
}
