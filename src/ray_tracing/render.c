/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 15:55:21 by lilefebv          #+#    #+#             */
/*   Updated: 2025/05/20 14:20:16 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "bvh.h"
#include "material.h"
#include <math.h>

void	calc_one_sample(t_minirt *minirt, t_vec3 offset)
{
	t_fcolor			color;
	const t_uint	tpi = minirt->viewport.render_w * minirt->viewport.render_h;
	t_uint			i;
	t_ray			ray;

	i = 0;
	while (i < tpi)
	{
		if (minirt->scene.camera.defocus_angle <= 0)
			ray.orig = minirt->scene.camera.position;
		else
			ray.orig = defocus_disk_sample(minirt);
		ray.dir = vec3_subtract(vec3_add(vec3_add(minirt->viewport.pixel00_loc,
						vec3_multiply_scalar(minirt->viewport.pixel_delta_u,
							(i % minirt->viewport.render_w) + offset.x)),
					vec3_multiply_scalar(minirt->viewport.pixel_delta_v,
						(i / minirt->viewport.render_w) + offset.y)), ray.orig);
		color = path_trace(minirt, ray, 2);
		minirt->screen.float_render[i].r += color.r;
		minirt->screen.float_render[i].g += color.g;
		minirt->screen.float_render[i].b += color.b;
		i++;
	}
	if (minirt->controls.selected_x != -1 && minirt->controls.selected_y != -1)
	{
		ft_bzero(minirt->controls.traced_ray, sizeof(t_vec3) * 11);
		if (minirt->scene.camera.defocus_angle <= 0)
			ray.orig = minirt->scene.camera.position;
		else
			ray.orig = defocus_disk_sample(minirt);
		ray.dir = vec3_subtract(vec3_add(vec3_add(minirt->viewport.pixel00_loc,
						vec3_multiply_scalar(minirt->viewport.pixel_delta_u, minirt->controls.selected_x + offset.x)),
					vec3_multiply_scalar(minirt->viewport.pixel_delta_v, minirt->controls.selected_y + offset.y)), ray.orig);
		
		debug_path_trace(minirt, ray, 8);

	}
}

void	draw_pixels(t_minirt *minirt)
{
	t_vec3	offset;

	minirt->screen.last_sample_time = get_cpu_time();
	offset = vec3_random();
	calc_one_sample(minirt, offset);
	minirt->screen.sample++;
	minirt->screen.sample_total_anim++;
	minirt->screen.last_sample_am = minirt->screen.sample;
	put_render_to_buff(minirt);

	if (minirt->options.no_display)
		return ;
		
	if (minirt->controls.selected_x != -1 && minirt->controls.selected_y != -1)
	{
		int	points[11][2];

		int i = 1;
		while (i < 11)
		{
			if (minirt->controls.traced_ray[i].x == 0.0 && minirt->controls.traced_ray[i].y == 0.0 && minirt->controls.traced_ray[i].z == 0.0)
				break ;
			project_vertex(minirt, minirt->controls.traced_ray[i], &points[i][0], &points[i][1]);
			i++;
		}

		t_point		p1;
		t_point		p2;
		i = 1;
		points[0][0] = minirt->scene.win_width / 2;
		points[0][1] = minirt->scene.win_height / 2;
		
		while (i < 11)
		{
			if (minirt->controls.traced_ray[i].x == 0.0 && minirt->controls.traced_ray[i].y == 0.0 && minirt->controls.traced_ray[i].z == 0.0)
				break ;
			p1 = (t_point){points[i - 1][0], points[i - 1][1], 0,
				0xF300F3};
			p2 = (t_point){points[i][0], points[i][1], 0,
				0xF300F3};

			if (i == 1)
			{
				p1.color = 0xFF0000;
				p2.color = 0xFF0000;
			}
			
			printf("draw line [%d %d] [%d %d]\n", p1.x, p1.y, p2.x, p2.y);
			draw_line(&p1, &p2, &minirt->mlx.img, p1.color);
			i++;
		}
	}		

	mlx_put_image_to_window(minirt->mlx.mlx, minirt->mlx.render_win,
		minirt->mlx.img.img, 0, 0);
	printf("Sample %d - %zums\n", minirt->screen.sample, get_cpu_time() - minirt->screen.last_sample_time);
}

void	init_animated_items(t_minirt *minirt)
{
	t_uint	i;
	int		y;
	t_uint	obj_num;

	if (!minirt->options.anim.enabled || minirt->options.anim.frame_i > minirt->options.anim.frames)
		return ;
	i = (t_uint)-1;
	while (++i < minirt->options.anim.nb_objects)
	{
		if (minirt->options.anim.objects[i].obj == CAMERA)
		{
			minirt->scene.camera.position = minirt->options.anim.objects[i].points[minirt->options.anim.frame_i];
		}
		else
		{
			obj_num = 0;
			y = -1;
			while (++y < minirt->scene.el_amount)
			{
				if (minirt->scene.elements[y].type == minirt->options.anim.objects[i].obj)
				{
					if (obj_num == minirt->options.anim.objects[i].obj_num)
					{
						if (minirt->scene.elements[y].type == SPHERE)
						{
							((t_sphere *)minirt->scene.elements[y].object)->position = minirt->options.anim.objects[i].points[minirt->options.anim.frame_i];
							
						}
						else if (minirt->scene.elements[y].type == PLANE)
						{
							((t_plane *)minirt->scene.elements[y].object)->position = minirt->options.anim.objects[i].points[minirt->options.anim.frame_i];
							
						}
						else if (minirt->scene.elements[y].type == CYLINDER)
						{
							((t_cylinder *)minirt->scene.elements[y].object)->position = minirt->options.anim.objects[i].points[minirt->options.anim.frame_i];
							
						}
						else if (minirt->scene.elements[y].type == CONE)
						{
							((t_cone *)minirt->scene.elements[y].object)->position = minirt->options.anim.objects[i].points[minirt->options.anim.frame_i];
							
						}
						else if (minirt->scene.elements[y].type == HYPERBOLOID)
						{
							((t_hyperboloid *)minirt->scene.elements[y].object)->position = minirt->options.anim.objects[i].points[minirt->options.anim.frame_i];
							
						}
						else if (minirt->scene.elements[y].type == CUSTOM)
						{
							((t_custom_object *)minirt->scene.elements[y].object)->position = minirt->options.anim.objects[i].points[minirt->options.anim.frame_i];
							
						}
						else if (minirt->scene.elements[y].type == LIGHT)
						{
							((t_light *)minirt->scene.elements[y].object)->position = minirt->options.anim.objects[i].points[minirt->options.anim.frame_i];
							
						}
					}
					obj_num++;
				}
			}
		}
	}
}

void	check_sample_amount(t_minirt *minirt)
{
	if (minirt->screen.sample == minirt->screen.spp)
	{
		if (minirt->options.auto_export)
		{
			char *filename;

			if (minirt->options.anim.enabled && minirt->options.anim.frame_i < minirt->options.anim.frames)
				filename = ft_sprintf("%sminirt_export_SCENE_NAME.FRAME.%u.SAMPLES.%d.%u.ppm", minirt->options.output_dir, minirt->options.anim.frame_i, minirt->screen.sample, (unsigned int)get_cpu_time());
			else
				filename = ft_sprintf("%sminirt_export_SCENE_NAME.SAMPLES.%d.%u.ppm", minirt->options.output_dir, minirt->screen.sample, (unsigned int)get_cpu_time());
			printf("Start image export\n");
			if (filename)
				export_ppm_p6_minirt(filename, minirt);
			free(filename);
		}
		minirt->screen.sample = 0;
		minirt->screen.start_render = 0;
		if (minirt->options.anim.enabled && minirt->options.anim.frame_i < minirt->options.anim.frames)
		{
			minirt->options.anim.frame_i++;
			minirt->screen.start_render = 1;
		}
	}
}

void	render(t_minirt *minirt)
{
	if (!minirt->screen.start_render || minirt->screen.pause_render)
		return ;
	if (minirt->screen.sample == 0)
	{
		if (minirt->screen.sample_total_anim == 0 || minirt->options.anim.enabled == 0)
			minirt->screen.first_sample_time = get_cpu_time();
		init_animated_items(minirt);
		minirt->viewport = init_viewport(minirt);
		if (!minirt->options.no_display)
			ft_izero(minirt->screen.render, minirt->scene.win_width * minirt->scene.win_height);
		ft_bzero(minirt->screen.float_render, sizeof(t_fcolor) * minirt->viewport.render_w * minirt->viewport.render_h);
	}
	draw_pixels(minirt);
}
