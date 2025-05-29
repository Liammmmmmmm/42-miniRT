/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_parser_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 14:42:04 by madelvin          #+#    #+#             */
/*   Updated: 2025/05/23 16:21:44 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "obj_parsing.h"
#include "minirt.h"

static int	count_while(char *line, t_obj_temp *tmp, int fd)
{
	size_t	i;

	i = 0;
	while (line)
	{
		if (ft_strncmp(line, "v ", 2) == 0)
			tmp->v_count++;
		else if (ft_strncmp(line, "vn ", 3) == 0)
			tmp->vn_count++;
		else if (ft_strncmp(line, "vt ", 3) == 0)
			tmp->vt_count++;
		else if (ft_strncmp(line, "f ", 2) == 0)
			tmp->face_count++;
		free(line);
		line = get_next_line(fd);
		i++;
	}
	return (i);
}

char	count_obj_data(const char *filename, t_obj_temp *tmp, size_t *size)
{
	char	*line;
	int		fd;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		print_error("Could not open file");
		return (1);
	}
	line = get_next_line(fd);
	if (!line)
	{
		print_error("Empty or unreadable .obj file");
		return (1);
	}
	*size = count_while(line, tmp, fd);
	close(fd);
	return (0);
}

void	ft_free_tab_face(t_face *tab, size_t i)
{
	size_t	c;

	c = 0;
	while (c < i)
	{
		free(tab[c].v_idx);
		free(tab[c].vn_idx);
		free(tab[c].vt_idx);
		c++;
	}
	free(tab);
}

void	ft_free_tab_triangle(t_triangle **tab, size_t i)
{
	size_t	c;

	c = 0;
	while (c < i)
	{
		free(tab[c]);
		c++;
	}
	free(tab);
}

static inline void	update_position_scale(t_triangle *t, t_custom_object *obj)
{
	t_vec3	delta_scale;

	t->v0.pos = vec3_subtract(t->v0.pos, obj->prev_position);
	t->v1.pos = vec3_subtract(t->v1.pos, obj->prev_position);
	t->v2.pos = vec3_subtract(t->v2.pos, obj->prev_position);
	t->v0.pos = rotate_around_axis(t->v0.pos, obj->prev_orientation, -1.0f);
	t->v1.pos = rotate_around_axis(t->v1.pos, obj->prev_orientation, -1.0f);
	t->v2.pos = rotate_around_axis(t->v2.pos, obj->prev_orientation, -1.0f);
	delta_scale = vec3_divide_safe(obj->scale, obj->prev_scale, MIN_SCALE);
	t->v0.pos = vec3_multiply(t->v0.pos, delta_scale);
	t->v1.pos = vec3_multiply(t->v1.pos, delta_scale);
	t->v2.pos = vec3_multiply(t->v2.pos, delta_scale);
	t->v0.pos = rotate_around_axis(t->v0.pos, obj->orientation, 1.0f);
	t->v1.pos = rotate_around_axis(t->v1.pos, obj->orientation, 1.0f);
	t->v2.pos = rotate_around_axis(t->v2.pos, obj->orientation, 1.0f);
	t->v0.pos = vec3_add(t->v0.pos, obj->position);
	t->v1.pos = vec3_add(t->v1.pos, obj->position);
	t->v2.pos = vec3_add(t->v2.pos, obj->position);
}

void	ft_update_obj_position(t_custom_object *obj)
{
	size_t	i;

	obj->orientation = vec3_unit(obj->orientation);
	if (obj->scale.x < MIN_SCALE)
		obj->scale.x = MIN_SCALE;
	if (obj->scale.y < MIN_SCALE)
		obj->scale.y = MIN_SCALE;
	if (obj->scale.z < MIN_SCALE)
		obj->scale.z = MIN_SCALE;
	i = 0;
	while (i < obj->triangle_count)
	{
		update_position_scale(&obj->triangles[i], obj);
		i++;
	}
	obj->prev_orientation = obj->orientation;
	obj->prev_position = obj->position;
	obj->prev_scale = obj->scale;
	obj->aabb = compute_custom_obj(obj, 1);
}
