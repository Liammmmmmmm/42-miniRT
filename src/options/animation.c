/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   animation.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 15:00:59 by lilefebv          #+#    #+#             */
/*   Updated: 2025/05/20 10:00:44 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	free_anim(t_animation *anim)
{
	t_uint	i;

	i = (t_uint)-1;
	if (!anim->objects)
		return ;
	while (++i < anim->nb_objects)
	{
		free(anim->objects[i].points);
		free(anim->objects[i].orientations);
	}
	free(anim->objects);
}

int	anim_print_error_f(char **parts, char *err)
{
	free(parts);
	return (print_error(err));
}

int	parse_object_type_and_num(char *str, t_obj_anim *obj)
{
	char	**parts;
	int		tab_size;

	parts = ft_split_in_line(str, ".");
	if (!parts)
		return (print_error("Malloc error"));
	tab_size = char_tab_len(parts);
	if (tab_size != 2 && tab_size != 1)
		return (anim_print_error_f(parts, "Invalid object name. Usage: OBJ[.num]"));
	obj->obj = get_object_type(parts[0]);
	if (obj->obj == NULL_OBJ)
		return (anim_print_error_f(parts, "Invalid object type. Usage: OBJ[.num]"));
	if (tab_size == 1)
		obj->obj_num = 0;
	else
		obj->obj_num = ft_atoi(parts[1]);
	if (obj->obj_num < 0)
		obj->obj_num = 0;
	free(parts);
	return (1);
}

int	parse_point_frame(char *str, t_tmp_obj_anim *pts, int i)
{
	char	**co;
	int		tab_size;

	co = ft_split_in_line(str, ",");
	if (!co)
		return (print_error(strerror(errno)));
	tab_size = char_tab_len(co);
	if (tab_size != 3 && tab_size != 4)
		return (anim_print_error_f(co, "Animation point must have 3 coordinates, and can have a frame index : <x,y,z[,F]>"));
	if (!is_valid_double_el_no_bordered(co[0], &pts->points[i].x)
		|| !is_valid_double_el_no_bordered(co[1], &pts->points[i].y)
		|| !is_valid_double_el_no_bordered(co[2], &pts->points[i].z))
		return (anim_print_error_f(co, "Invalid vector format. Expected format: x,y,z"));
	pts->frames[i] = -1;
	if (tab_size == 4)
	{
		if (!is_valid_positive_int(co[3], &pts->frames[i]))
			return (anim_print_error_f(co, "Invalid frame index."));
	}
	free(co);
	return (1);
}

void	tessellate_bezier3(t_vec3 *output, uint32_t output_size, t_bezier3 b)
{
	const float	step_per_iter = 1.0 / output_size;
	uint32_t	i;
	float		t;
	float		t1;

	i = -1;
	while (++i <= output_size)
	{
		t = i * step_per_iter;
		t1 = 1.0f - t;
		output[i].x = t1 * t1 * b.p1.x + 2 * t1 * t * b.pc.x + t * t * b.p2.x;
		output[i].y = t1 * t1 * b.p1.y + 2 * t1 * t * b.pc.y + t * t * b.p2.y;
		output[i].z = t1 * t1 * b.p1.z + 2 * t1 * t * b.pc.z + t * t * b.p2.z;
	}
}

void	tessellate_straight_line(t_vec3 *output, uint32_t output_size, t_vec3 p1, t_vec3 p2)
{
	const float	step_per_iter = 1.0 / output_size;
	uint32_t	i;

	i = -1;
	while (++i <= output_size)
	{
		output[i] = vec3_lerp(p1, p2, i * step_per_iter);
	}
}

int	get_frame_index_of_created_point(t_tmp_obj_anim *pts, int index)
{
	int	step;
	int	size;
	int	i;

	i = index;
	while (pts->frames[i] == -1)
		i--;
	size = 1;
	while (pts->frames[i + size] == -1)
		size++;
	step = index - i;
	return (pts->frames[i] + step * ((pts->frames[i + size] - pts->frames[i]) / (size - 1)));
}

int	tesselate_everything(int tab_size, t_tmp_obj_anim *pts, t_obj_anim *obj)
{
	int			i;
	t_bool		pc;
	t_bezier3	b;
	int			frame1;
	int			frame2;
	
	i = 0;
	while (++i < tab_size)
	{
		pc = 0;
		if (pts->frames[i] != -1 && pts->frames[i - 1] == -1)
			continue ;
		if (pts->frames[i] != -1)
		{
			b.p2 = pts->points[i];
			frame2 = pts->frames[i];
		}
		else
		{
			pc = 1;
			b.pc = pts->points[i];
			if (pts->frames[i + 1] != -1)
			{
				b.p2 = pts->points[i + 1];
				frame2 = pts->frames[i + 1];
			}
			else
			{
				b.p2 = vec3_lerp(pts->points[i + 1], pts->points[i], 0.5);
				frame2 = get_frame_index_of_created_point(pts, i);
			}
		}
		if (pts->frames[i - 1] != -1)
		{
			b.p1 = pts->points[i - 1];
			frame1 = pts->frames[i - 1];
		}
		else
		{
			b.p1 = vec3_lerp(pts->points[i - 1], pts->points[i], 0.5);
			frame1 = get_frame_index_of_created_point(pts, i - 1);
		}
		if (pc)
			tessellate_bezier3(&obj->points[frame1], frame2 - frame1, b);
		else
			tessellate_straight_line(&obj->points[frame1], frame2 - frame1, b.p1, b.p2);
	}
	return (1);
}

int	compute_movement_pts(int tab_size, t_tmp_obj_anim *pts, t_obj_anim *obj)
{
	int	i;
	int	last_frame;

	if (pts->frames[0] == -1 || pts->frames[tab_size - 1] == -1)
		return (print_error("First and last points must have a frame index"));
	if (pts->frames[0] != 0)
		return (print_error("First point must be the frame 0"));
	i = -1;
	last_frame = 0;
	while (++i < tab_size)
	{
		if (pts->frames[i] == -1)
			continue ;
		if (i != 0 && last_frame >= pts->frames[i])
			return (print_error("Points not ordered by frame index"));
		last_frame = pts->frames[i];
	}
	obj->frames = pts->frames[tab_size - 1] + 1;
	obj->points = ft_calloc(obj->frames, sizeof(t_vec3));
	obj->orientations = ft_calloc(obj->frames, sizeof(t_vec3));
	if (!obj->orientations || !obj->points || !tesselate_everything(tab_size, pts, obj))
	{
		free(obj->orientations);
		free(obj->points);
		return (0);
	}
	return (1);
}

int	parse_and_compute_pts(char **parts, int tab_size, t_tmp_obj_anim *pts, t_obj_anim *obj)
{
	int	i;

	i = -1;
	while (++i < tab_size)
		if (!parse_point_frame(parts[i], pts, i))
			return (0);
	if (!compute_movement_pts(tab_size, pts, obj))
		return (0);

	
	// a partir de maintenant les erreurs doivent free orientations et points
	return (1);
}

int	parse_movement_points(char *str, t_obj_anim *obj)
{
	t_tmp_obj_anim	points;
	char			**parts;
	int				tab_size;
	int				res;

	printf("OEOE : %s\n", str);
	parts = ft_split_in_line(str, " ");
	if (!parts)
		return (print_error("Malloc error"));
	tab_size = char_tab_len(parts);
	printf("OEOE2 : %d\n", tab_size);
	if (tab_size < 2)
		return (anim_print_error_f(parts, "Not enough movement points"));
	points.points = ft_calloc(tab_size, sizeof(t_vec3));
	points.frames = ft_calloc(tab_size, sizeof(int));
	if (!points.points || !points.frames)
	{
		free(points.points);
		free(points.frames);
		return (anim_print_error_f(parts, "Malloc error"));
	}
	res = parse_and_compute_pts(parts, tab_size, &points, obj);
	free(points.points);
	free(points.frames);
	free(parts);
	return (res);
}

int	parse_object_points(char *str, t_obj_anim *obj)
{
	char	**parts;
	int		tab_size;

	parts = ft_split_in_line(str, "|");
	if (!parts)
		return (print_error("Malloc error"));
	tab_size = char_tab_len(parts);
	if (tab_size != 2 && tab_size != 1)
		return (anim_print_error_f(parts, "Invalid animation parameter. Usage: Movement points [| orientation vectors]"));
	if (!parse_movement_points(parts[0], obj))
	{
		free(parts);
		return (0);
	}
	// if (!parse_orientation_points(parts[1], obj))
	// {
	// 	free(parts);
	// 	return (0);
	// }
	free(parts);
	return (1);
}

int	parse_one_anim_obj(char *str, t_obj_anim *obj)
{
	char	**parts;

	parts = ft_split_in_line(str, ":");
	if (!parts)
		return (print_error("Malloc error"));
	if (char_tab_len(parts) != 2)
		return (anim_print_error_f(parts, "Invalid animation parameter. Usage: OBJ[.num]:POINTS"));
	if (!parse_object_type_and_num(parts[0], obj))
	{
		free(parts);
		return (0);
	}
	if (!parse_object_points(parts[1], obj))
	{
		free(parts);
		return (0);
	}
	free(parts);
	return (1);
}

void	debug_print_animation(t_animation *anim)
{
	int			i;
	int			y;
	t_obj_anim	obj;
	
	i = -1;
	while (++i < (int)anim->nb_objects)
	{
		obj = anim->objects[i];
		y = -1;
		printf("Object : %s.%u\n", get_object_name(obj.obj), obj.obj_num);
		while (++y < (int)obj.frames)
		{
			printf("  Frame %d : Pos -> %f,%f,%f\n", y, obj.points[y].x, obj.points[y].y, obj.points[y].z);
			
		}
	}
}

int	animate_option_parsing(char *str, t_animation *anim)
{
	char	**parts;
	int		tab_size;
	int		i;

	parts = ft_split_in_line(str, ";");
	if (!parts)
		return (print_error("Malloc error"));
	tab_size = char_tab_len(parts);
	anim->nb_objects = tab_size;
	anim->objects = ft_calloc(tab_size, sizeof(t_obj_anim));
	i = -1;
	while (++i < tab_size)
	{
		if (!parse_one_anim_obj(parts[i], &anim->objects[i]))
		{
			free_anim(anim);
			free(parts);
			return (0);
		}
	}
	debug_print_animation(anim);
	i = -1;
	while (++i < tab_size)
	{
		if (i != 0 && anim->frames != anim->objects[i].frames)
		{
			free_anim(anim);
			return (anim_print_error_f(parts, "Every object should have the same amount of frame"));
		}
		anim->frames = anim->objects[i].frames;
	}
	free(parts);
	return (1);
}

int	is_animate_option(t_minirt *minirt, char *argvi, int *y)
{
	if (ft_strcmp(argvi, "--animate") == 0)
	{
		*y = 0;		
		return (print_error1("Animate option need a value"));
	}
	else if (ft_strncmp(argvi, "--animate=", 10) == 0)
	{
		if (ft_strlen(argvi + 10) > 16)
		{
			minirt->options.anim.enabled = 1;
			if (!animate_option_parsing(argvi + 10, &minirt->options.anim))
				*y = 0;
			return (1);
		}
		*y = 0;
		return (print_error1("Invalid animation value"));
	}
	return (0);
}
