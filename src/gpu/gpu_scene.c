/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gpu_scene.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 16:03:21 by lilefebv          #+#    #+#             */
/*   Updated: 2025/07/23 15:52:56 by lilefebv         ###   ########lyon.fr   */
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
			gpu_scene->prim_indice[i] = sphere;
			gpu_scene->prim_type[i] = 0;
			gpu_scene->spheres[sphere].radius = ((t_sphere *)cpu_scene->bvh.obj_list[i]->object)->radius;
			vec3_to_float3(&((t_sphere *)cpu_scene->bvh.obj_list[i]->object)->position, gpu_scene->spheres[sphere].position);
			color_to_float3(&((t_sphere *)cpu_scene->bvh.obj_list[i]->object)->color, gpu_scene->spheres[sphere].color);
			if (((t_sphere *)cpu_scene->bvh.obj_list[i]->object)->material == NULL)
				gpu_scene->spheres[sphere].material_id = -1;
			else
				gpu_scene->spheres[sphere].material_id = ((t_sphere *)cpu_scene->bvh.obj_list[i]->object)->material - cpu_scene->materials;
			sphere++;
		}
		if (cpu_scene->bvh.obj_list[i]->type == CYLINDER)
		{
			gpu_scene->prim_indice[i] = cylindre;
			gpu_scene->prim_type[i] = 1;
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
		if (cpu_scene->bvh.obj_list[i]->type == CONE)
		{
			gpu_scene->prim_indice[i] = cone;
			gpu_scene->prim_type[i] = 2;
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
		if (cpu_scene->bvh.obj_list[i]->type == HYPERBOLOID)
		{
			gpu_scene->prim_indice[i] = hyper;
			gpu_scene->prim_type[i] = 3;
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
		if (cpu_scene->bvh.obj_list[i]->type == TRIANGLE)
		{
			gpu_scene->prim_indice[i] = triangle;
			gpu_scene->prim_type[i] = 4;
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
		bvh_node[i].first_prim = cpu_scene->bvh.bvh_nodes[i].first_prim;
		bvh_node[i].is_leaf = cpu_scene->bvh.bvh_nodes[i].is_leaf;
		bvh_node[i].right_child = cpu_scene->bvh.bvh_nodes[i].right_child;
		bvh_node[i].left_child = cpu_scene->bvh.bvh_nodes[i].left_child;
		vec3_to_float3(&cpu_scene->bvh.bvh_nodes[i].node_bounds.max, bvh_node[i].node_bounds.max);
		vec3_to_float3(&cpu_scene->bvh.bvh_nodes[i].node_bounds.min, bvh_node[i].node_bounds.min);
		bvh_node[i].prim_count = cpu_scene->bvh.bvh_nodes[i].prim_count;
		// printf("%d | %d\n", bvh_node[i].first_prim, bvh_node[i].prim_count);
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
		if (scene->bvh.obj_list[i]->type == CYLINDER)
			gpu_structs->cylinders_am++;	
		if (scene->bvh.obj_list[i]->type == CONE)
			gpu_structs->cones_am++;
		if (scene->bvh.obj_list[i]->type == HYPERBOLOID)
			gpu_structs->hypers_am++;
		if (scene->bvh.obj_list[i]->type == TRIANGLE)
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

void	convert_textures(t_scene *scene, t_gpu_structs *gpu_structs)
{
	int	i;
	int	img_i;
	int	check_i;

	i = 0;
	img_i = 0;
	check_i = 0;
	while (i < scene->tex_amount)
	{
		gpu_structs->textures_types[i] = scene->textures[i].type;
		printf("Type %d : %d\n", i, gpu_structs->textures_types[i]);
		if (scene->textures[i].type == CHECKER_GLOBAL || scene->textures[i].type == CHECKER_LOCAL)
		{
			gpu_structs->textures_indices[i] = check_i;
			gpu_structs->checkers[check_i].scale = scene->textures[i].checker.scale;
			color_to_float3(&scene->textures[i].checker.c1, gpu_structs->checkers[check_i].c1);
			color_to_float3(&scene->textures[i].checker.c2, gpu_structs->checkers[check_i].c2);
			check_i++;
		}
		if (scene->textures[i].type == HDR || scene->textures[i].type == IMAGE)
		{
			gpu_structs->textures_indices[i] = img_i;
			img_i++;
			printf("Ntm on gere pas ca encore\n");
		}
		i++;
	}
}

void	count_tex(t_scene *scene, t_gpu_structs *gpu_structs)
{
	int	i;

	i = 0;
	while (i < scene->tex_amount)
	{
		if (scene->textures[i].type == CHECKER_GLOBAL || scene->textures[i].type == CHECKER_LOCAL)
			gpu_structs->checkers_am++;
		if (scene->textures[i].type == HDR || scene->textures[i].type == IMAGE)
			gpu_structs->images_am++;	
		i++;
	}
}

void	create_ssbo(GLuint *ssbo, size_t size, void *data, long bind)
{
	glGenBuffers(1, ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, *ssbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, GL_STATIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bind, *ssbo);
}

int	convert_scene(t_scene *scene, t_viewport *viewport, t_gpu_structs *gpu_structs)
{
	clean_scene(gpu_structs);

	convert_viewport(&gpu_structs->viewport, viewport, scene->ior_all);
	create_ssbo(&gpu_structs->viewport_ssbo, sizeof(t_gpu_viewport), &gpu_structs->viewport, SSBO_BIND_VIEWPORT);
	convert_camera(scene, &gpu_structs->camera);
	create_ssbo(&gpu_structs->camera_ssbo, sizeof(t_gpu_camera), &gpu_structs->camera, SSBO_BIND_CAMERA);
	
	// ---------------- TEXTURES ---------------- //
	count_tex(scene, gpu_structs);
	gpu_structs->checkers = ft_calloc(gpu_structs->checkers_am, sizeof(t_gpu_checker));
	gpu_structs->images = ft_calloc(gpu_structs->images_am, sizeof(GLuint64));
	gpu_structs->textures_types = ft_calloc(scene->tex_amount, sizeof(t_uint));
	gpu_structs->textures_indices = ft_calloc(scene->tex_amount, sizeof(t_uint));
	// SECURE LES MALLOC
	convert_textures(scene, gpu_structs);
	create_ssbo(&gpu_structs->checkers_ssbo, sizeof(t_gpu_checker) * gpu_structs->checkers_am, gpu_structs->checkers, SSBO_BIND_CHECKERS);
	create_ssbo(&gpu_structs->images_ssbo, sizeof(GLuint64) * gpu_structs->images_am, gpu_structs->images, SSBO_BIND_IMAGES);
	create_ssbo(&gpu_structs->textures_types_ssbo, sizeof(t_uint) * scene->tex_amount, gpu_structs->textures_types, SSBO_BIND_TEX_TYPE);
	create_ssbo(&gpu_structs->textures_indices_ssbo, sizeof(t_uint) * scene->tex_amount, gpu_structs->textures_indices, SSBO_BIND_TEX_INDICE);
	
	// ---------------- MATERIALS ---------------- //
	gpu_structs->mat_am = scene->mat_amount;
	gpu_structs->mat = ft_calloc(gpu_structs->mat_am, sizeof(t_gpu_material));
	if (!gpu_structs->mat)
		return (-1); // Tout free correctement
	convert_materials(scene, gpu_structs->mat);
	create_ssbo(&gpu_structs->mat_ssbo, sizeof(t_gpu_material) * gpu_structs->mat_am, gpu_structs->mat, SSBO_BIND_MATERIALS);

	// ---------------- OBJECTS ---------------- //
	count_obj(scene, gpu_structs);
	gpu_structs->spheres = ft_calloc(gpu_structs->spheres_am, sizeof(t_gpu_sphere));
	gpu_structs->hypers = ft_calloc(gpu_structs->hypers_am, sizeof(t_gpu_hyper));
	gpu_structs->cylinders = ft_calloc(gpu_structs->cylinders_am, sizeof(t_gpu_cylinder));
	gpu_structs->cones = ft_calloc(gpu_structs->cones_am, sizeof(t_gpu_cone));
	gpu_structs->triangles = ft_calloc(gpu_structs->triangles_am, sizeof(t_gpu_triangle));
	gpu_structs->prim_indice = ft_calloc(scene->bvh.size, sizeof(uint32_t));
	gpu_structs->prim_type = ft_calloc(scene->bvh.size, sizeof(uint32_t));
	// if (!gpu_structs->spheres)
	// 	return (-1); // Tout free correctement
	convert_all_obj(scene, gpu_structs);
	create_ssbo(&gpu_structs->spheres_ssbo, sizeof(t_gpu_sphere) * gpu_structs->spheres_am, gpu_structs->spheres, SSBO_BIND_SPHERES);
	create_ssbo(&gpu_structs->hypers_ssbo, sizeof(t_gpu_hyper) * gpu_structs->hypers_am, gpu_structs->hypers, SSBO_BIND_HYPERS);
	create_ssbo(&gpu_structs->cylinders_ssbo, sizeof(t_gpu_cylinder) * gpu_structs->cylinders_am, gpu_structs->cylinders, SSBO_BIND_CYLINDERS);
	create_ssbo(&gpu_structs->cones_ssbo, sizeof(t_gpu_cone) * gpu_structs->cones_am, gpu_structs->cones, SSBO_BIND_CONES);
	create_ssbo(&gpu_structs->triangles_ssbo, sizeof(t_gpu_triangle) * gpu_structs->triangles_am, gpu_structs->triangles, SSBO_BIND_TRIANGLES);

	gpu_structs->bvh_node_am = scene->bvh.bvh_nodes_used;
	gpu_structs->bvh_node = ft_calloc(gpu_structs->bvh_node_am, sizeof(t_gpu_bvh_node));
	if (!gpu_structs->bvh_node)
		return (-1); // Tout free correctement
	convert_bvh_node(scene, gpu_structs->bvh_node);
	create_ssbo(&gpu_structs->bvh_node_ssbo, sizeof(t_gpu_bvh_node) * gpu_structs->bvh_node_am, gpu_structs->bvh_node, SSBO_BIND_BVH);
	
	gpu_structs->prim_indice_am = scene->bvh.size;
	create_ssbo(&gpu_structs->prim_indice_ssbo, sizeof(uint32_t) * gpu_structs->prim_indice_am, gpu_structs->prim_indice, SSBO_BIND_PRIM_INDICE);

	create_ssbo(&gpu_structs->prim_type_ssbo, sizeof(uint32_t) * gpu_structs->prim_indice_am, gpu_structs->prim_type, SSBO_BIND_PRIM_TYPE);

	gpu_structs->planes_am = scene->obj_lst.plane_nb;
	gpu_structs->planes = ft_calloc(gpu_structs->planes_am, sizeof(t_gpu_plane));
	if (!gpu_structs->planes)
		return (-1); // Tout free correctement
	convert_plane(scene, gpu_structs->planes);
	create_ssbo(&gpu_structs->planes_ssbo, sizeof(t_gpu_plane) * gpu_structs->planes_am, gpu_structs->planes, SSBO_BIND_PLANE);
	
	return (0);
}
