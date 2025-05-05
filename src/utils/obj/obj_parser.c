/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_parser.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 17:40:53 by madelvin          #+#    #+#             */
/*   Updated: 2025/05/05 13:24:23 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int is_convex(t_vec3 prev, t_vec3 curr, t_vec3 next)
{
    double cross = (curr.x - prev.x) * (next.y - curr.y) -
                   (curr.y - prev.y) * (next.x - curr.x);
    return (cross < 0);
}

int point_in_triangle(t_vec3 p, t_vec3 a, t_vec3 b, t_vec3 c)
{
    double d1 = (p.x - b.x) * (a.y - b.y) - (a.x - b.x) * (p.y - b.y);
    double d2 = (p.x - c.x) * (b.y - c.y) - (b.x - c.x) * (p.y - c.y);
    double d3 = (p.x - a.x) * (c.y - a.y) - (c.x - a.x) * (p.y - a.y);
    int has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    int has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);
    return (!(has_neg && has_pos));
}

void	add_triangle(t_triangle *output, size_t i, t_triangle new)
{
	t_triangle	*t;
	size_t		j;

	t = malloc(sizeof(t_triangle) * (i + 1));
	if (output)
	{
		j = 0;
		while (j < (i - 1))
			t[j] = output[j];
	}
	t[i] = new;
	if (output)
		free(output);
	output = t;
}

size_t ear_clipping(t_vertex *vertices, size_t vertex_count, t_triangle *output)
{
    size_t	n = vertex_count;
    size_t	e = 0;
	char	ear_found;
	// char	is_ear;

    while (n > 3)
	{
        ear_found = 0;
    }
    if (n == 3)
		add_triangle(output, e++, (t_triangle){vertices[0], vertices[1], vertices[2]});
	printf("\n\n");
    return (e);
}

size_t	clip_one_face(t_obj_temp *obj, t_triangle *out, size_t i)
{
	t_vertex	*vertex;
	size_t		j;

	vertex = malloc(sizeof(t_vertex) * obj->face[i].vertex_count);
	j = 0;
	printf("triangle %zu:\n", i);
	while (j < obj->face[i].vertex_count)
	{
		vertex[j].normal = obj->vn[obj->face[i].vn_idx[j] - 1];
		vertex[j].pos = obj->v[obj->face[i].v_idx[j] - 1];
		vertex[j].u = obj->vt[obj->face[i].vt_idx[j] - 1].x;
		vertex[j].v = obj->vt[obj->face[i].vt_idx[j] - 1].y;
		printf("%zu: %f/%f/%f | %f/%f | %f%f%f\n", j, vertex[j].pos.x, vertex[j].pos.y, vertex[j].pos.z, vertex[j].u, vertex[j].v, vertex[j].normal.x, vertex[j].normal.y, vertex[j].normal.z);
		j++;
	}
	printf("\n");
	if (obj->face[i].vertex_count == 3)
	{
		out = malloc(sizeof(t_triangle) * 1);
		out[0].v0 = vertex[0];
		out[1].v0 = vertex[1];
		out[2].v0 = vertex[2];
		printf("\n\n");
		return (1);
	}
	return (ear_clipping(vertex, obj->face[i].vertex_count, out));
}

void	extract_all_triangle(t_obj_temp	*obj, t_triangle *out, size_t *triangle_count)
{
	size_t		i;
	size_t		j;
	size_t		size;
	t_triangle	*tmp = NULL;
	t_triangle	*tmp2;

	i = 0;
	size = 0;
	*triangle_count = 0;
	while (i < obj->face_count)
	{
		size += clip_one_face(obj, tmp, i);
		tmp2 = malloc(sizeof(t_triangle) * size);
		j = 0;
		if (out)
		{
			while (j < *triangle_count)
				tmp2[j] = out[j];
			free(out);
		}
		while (j < size)
			tmp2[j] = tmp[j];
		free(tmp);
		out = tmp2;
		*triangle_count = size;
		i++;
	}
}






char	get_value_v(char *line, t_vec3 *out)
{
	t_vec3	vec;
	char	**co;

	co = ft_split_in_line(line, " ");
	if (!co)
		return (1);
	if (char_tab_len(co) != 3)
	{
		free(co);
		return (1);
	}
	vec.x = ft_atod(co[0]);
	vec.y = ft_atod(co[1]);
	vec.z = ft_atod(co[2]);
	free(co);
	*out = vec;
	return (0);
}


char	get_value_vn(char *line, t_vec3 *out)
{
	t_vec3	vec;
	char	**co;

	co = ft_split_in_line(line, " ");
	if (!co)
		return (1);
	if (char_tab_len(co) != 3)
	{
		free(co);
		return (1);
	}
	vec.x = ft_atod(co[0]);
	vec.y = ft_atod(co[1]);
	vec.z = ft_atod(co[2]);
	free(co);
	*out = vec;
	return (0);
}


char	get_value_vt(char *line, t_vec3 *out)
{
	t_vec3	uv;
	char	**co;

	co = ft_split_in_line(line, " ");
	if (!co)
		return (1);
	if (char_tab_len(co) != 2)
	{
		free(co);
		return (1);
	}
	uv.x = ft_atod(co[0]);
	uv.y = ft_atod(co[1]);
	uv.z = 0;
	free(co);
	*out = uv;
	return (0);
}

char get_value_face(char *line, t_face *out)
{
	t_face	face;
	char	**tokens;
	char	**vertex_data;
	int		i = 0;

	tokens = ft_split_in_line(line, " ");
	if (!tokens)
		return (1);
	while (tokens[i])
		i++;
	face.vertex_count = i;
	face.v_idx = malloc(sizeof(size_t) * i);
	face.vn_idx = malloc(sizeof(size_t) * i);
	face.vt_idx = malloc(sizeof(size_t) * i);
	i = 0;
	while (tokens[i])
	{
		vertex_data = ft_split(tokens[i], "/");
		if (vertex_data)
		{
			if (vertex_data[0])
				face.v_idx[i] = ft_atoi(vertex_data[0]);
			if (vertex_data[1])
				face.vt_idx[i] = ft_atoi(vertex_data[1]);
			if (vertex_data[2])
				face.vn_idx[i] = ft_atoi(vertex_data[2]);
			ft_free_tab_null_term(vertex_data);
		}
		i++;
	}
	*out = face;
	return (0);
}


char	count_obj_data(const char *filename, t_obj_temp *tmp, int obj_index)
{
	char	*line;
	int		fd;
	int		current_obj_index = 0;
	char	in_target = 0;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (print_error("Could not open file"));

	line = get_next_line(fd);
	while (line)
	{
		if (ft_strncmp(line, "o ", 2) == 0)
		{
			current_obj_index++;
			in_target = (current_obj_index == obj_index);
		}
		else if (in_target)
		{
			if (ft_strncmp(line, "v ", 2) == 0)
				tmp->v_count++;
			else if (ft_strncmp(line, "vn ", 3) == 0)
				tmp->vn_count++;
			else if (ft_strncmp(line, "vt ", 3) == 0)
				tmp->vt_count++;
			else if (ft_strncmp(line, "f ", 2) == 0)
				tmp->face_count++;
		}
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	if (!in_target)
		return (print_error("Object index not found"));
	return (0);
}


int	extract_obj_data(const char *filename, t_obj_temp *tmp, int obj_index)
{
	char	*line;
	int		fd;
	int		current_obj_index = 0;
	char	in_target_object = 0;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (print_error("Error opening .obj file"));
	line = get_next_line(fd);
	if (!line)
		return (print_error("Empty or unreadable .obj file"));
	while (line)
	{
		if (ft_strncmp(line, "o ", 2) == 0)
		{
			if (current_obj_index == obj_index)
				break;
			current_obj_index++;
			in_target_object = (current_obj_index == obj_index);
			if (in_target_object)
			{
				free(tmp->name);
				tmp->name = ft_strdup(line + 2);
			}
		}
		else if (in_target_object)
		{
			if (ft_strncmp(line, "v ", 2) == 0)
				get_value_v(line + 2, &tmp->v[tmp->v_count++]);
			else if (ft_strncmp(line, "vn ", 3) == 0)
				get_value_vn(line + 2, &tmp->vn[tmp->vn_count++]);
			else if (ft_strncmp(line, "vt ", 3) == 0)
				get_value_vt(line + 2, &tmp->vt[tmp->vt_count++]);
			else if (ft_strncmp(line, "f ", 2) == 0)
				get_value_face(line + 2, &tmp->face[tmp->face_count++]);
		}
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	if (!in_target_object)
		return (print_error("Object index not found in .obj file"));

	printf("=== OBJ DEBUG: \"%s\" ===\n", tmp->name);

	printf("\n[v] Vertices (%zu):\n", tmp->v_count);
	for (size_t i = 0; i < tmp->v_count; i++)
		printf("%zu: %f %f %f\n", i+1, tmp->v[i].x, tmp->v[i].y, tmp->v[i].z);

	printf("\n[vt] UVs (%zu):\n", tmp->vt_count);
	for (size_t i = 0; i < tmp->vt_count; i++)
		printf("%zu: %f %f\n", i+1, tmp->vt[i].x, tmp->vt[i].y);

	printf("\n[vn] Normals (%zu):\n", tmp->vn_count);
	for (size_t i = 0; i < tmp->vn_count; i++)
		printf("%zu: %f %f %f\n", i+1, tmp->vn[i].x, tmp->vn[i].y, tmp->vn[i].z);

	printf("\n[f] Faces (%zu):\n", tmp->face_count);
	for (size_t i = 0; i < tmp->face_count; i++)
	{
		printf("%zu: ", i);
		for (int j = 0; j < 4; j++)
			printf("v/vt/vn: %zu/%zu/%zu  ", tmp->face[i].v_idx[j], tmp->face[i].vt_idx[j], tmp->face[i].vn_idx[j]);
		printf("\n");
	}	
	return (0);
}

int	parse_obj(const char *file, t_custom_object *obj, int obj_index)
{
	t_obj_temp	tmp;

	ft_bzero(&tmp, sizeof(t_obj_temp));
	if (count_obj_data(file, &tmp, obj_index))
		return (1);
	tmp.vn = malloc(sizeof(t_vec3) * tmp.vn_count);
	tmp.v = malloc(sizeof(t_vec3) * tmp.v_count);
	tmp.vt = malloc(sizeof(t_vec3) * tmp.vt_count);
	tmp.face = malloc(sizeof(t_face) * tmp.face_count);
	tmp.name = NULL;
	tmp.v_count = 0;
	tmp.vn_count = 0;
	tmp.vt_count = 0;
	tmp.face_count = 0;
	extract_obj_data(file, &tmp, obj_index);
	// extract_all_triangle(&tmp, obj->triangles, &obj->triangle_count);
	// exit(0);
	(void)obj;
	free(tmp.v);
	free(tmp.vn);
	free(tmp.vt);
	free(tmp.name);
	return (0);
}
