/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_parser.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 17:40:53 by madelvin          #+#    #+#             */
/*   Updated: 2025/05/05 21:06:38 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"


static inline void compute_triangle_center(t_triangle *t)
{
	t->center = vec3_divide_scalar(
		vec3_add(vec3_add(t->v0.pos, t->v1.pos), t->v2.pos),
		3.0
	);
}

size_t ear_clipping(t_vertex *vertices, size_t vertex_count, t_triangle **output)
{
	t_vector v;
	size_t n = vertex_count;
	size_t e = 0;
	t_triangle t;
	char ear_found;

	vector_init(&v, sizeof(t_triangle), 2);

	// Processus de triangulation
	while (n > 3)
	{
		ear_found = 0;

		for (size_t i = 0; i < n; ++i)
		{
			// t_vec3 prev = vertices[(i + n - 1) % n].pos;
			// t_vec3 curr = vertices[i].pos;
			// t_vec3 next = vertices[(i + 1) % n].pos;

			// Vérification de la convexité
			// if (!is_convex(prev, curr, next))
			// {
			//     printf("NON-CONVEX: [%zu] prev(%.2f,%.2f,%.2f) curr(%.2f,%.2f,%.2f) next(%.2f,%.2f,%.2f)\n",
			//         i, prev.x, prev.y, prev.z, curr.x, curr.y, curr.z, next.x, next.y, next.z);
			//     continue;
			// }

			// Vérification si l'oreille est valide
			// is_ear = 1;
			// for (size_t j = 0; j < n; ++j)
			// {
			//     if (j == (i + n - 1) % n || j == i || j == (i + 1) % n)
			//         continue;

			//     if (point_in_triangle(vertices[j].pos, prev, curr, next))
			//     {
			//         printf("▶ Point %.2f,%.2f,%.2f est dans le triangle [%zu]\n",
			//             vertices[j].pos.x, vertices[j].pos.y, vertices[j].pos.z, i);
			//         is_ear = 0;
			//         break;
			//     }
			// }

			// if (is_ear)
			// {
				printf("- Ear trouvé à %zu : ajout triangle\n", i);
				t = (t_triangle){
					vertices[(i + n - 1) % n],
					vertices[i],
					vertices[(i + 1) % n],
					(t_vec3){0, 0, 0},
					NULL,
					(t_color){0, 0, 0}
				};
				compute_triangle_center(&t);
				vector_add(&v, &t);
				e++;

				// Suppression du sommet triangulé
				for (size_t j = i; j < n - 1; ++j)
					vertices[j] = vertices[j + 1];
				n--;
				ear_found = 1;
				break;
			// }
		}

		// Gestion des erreurs de triangulation
		if (!ear_found)
		{
			fprintf(stderr, "Erreur : polygone non triangulable (trous ? self-intersection ?)\n");
			break;
		}
	}

	// Ajout du dernier triangle si nécessaire
	if (n == 3)
	{
		t = (t_triangle){vertices[0], vertices[1], vertices[2], .center = (t_vec3){0, 0, 0}};
		compute_triangle_center(&t);
		vector_add(&v, &t);
		e++;
	}

	printf(">> Triangulation terminée : %zu triangles\n", e);

	// Retourne le résultat de la triangulation
	*output = (t_triangle *)v.data;
	return e;
}

size_t	clip_one_face(t_obj_temp *obj, t_triangle **out, size_t i)
{
	t_vertex	*vertex;
	size_t		j;

	vertex = malloc(sizeof(t_vertex) * obj->face[i].vertex_count);
	j = 0;
	printf("\n\n\nFace %zu:\n", i);
	while (j < obj->face[i].vertex_count)
	{
		vertex[j].normal = obj->vn[obj->face[i].vn_idx[j] - 1];
		vertex[j].pos = obj->v[obj->face[i].v_idx[j] - 1];
		vertex[j].u = obj->vt[obj->face[i].vt_idx[j] - 1].x;
		vertex[j].v = obj->vt[obj->face[i].vt_idx[j] - 1].y;
		printf("%zu: %f/%f/%f | %f/%f | %f/%f/%f\n", j, vertex[j].pos.x, vertex[j].pos.y, vertex[j].pos.z, vertex[j].u, vertex[j].v, vertex[j].normal.x, vertex[j].normal.y, vertex[j].normal.z);
		j++;
	}
	printf("\n");
	// if (obj->face[i].vertex_count == 3)
	// {
	// 	out = malloc(sizeof(t_triangle) * 1);
	// 	out[0].v0 = vertex[0];
	// 	out[1].v0 = vertex[1];
	// 	out[2].v0 = vertex[2];
	// 	printf("\n\n");
	// 	return (1);
	// }
	return (ear_clipping(vertex, obj->face[i].vertex_count, out));
}

void extract_all_triangle(t_obj_temp *obj, t_triangle **out, size_t *triangle_count)
{
	size_t i, j;
	size_t size = 0;
	size_t new_size;
	t_triangle *tmp = NULL;
	t_triangle *tmp2 = NULL;

	*triangle_count = 0;
	*out = NULL;

	for (i = 0; i < obj->face_count; i++)
	{
		size_t clipped = clip_one_face(obj, &tmp, i);
		if (clipped == 0 || tmp == NULL)
			continue;
		new_size = size + clipped;
		tmp2 = malloc(sizeof(t_triangle) * new_size);
		if (!tmp2)
		{
			free(tmp);
			return ;
		}
		j = 0;
		while (j < size)
		{
			tmp2[j] = (*out)[j];
			j++;
		}
		j = 0;
		while (j < clipped)
		{
			tmp2[size + j] = tmp[j];
			j++;
		}		
		free(tmp);
		free(*out);
		*out = tmp2;
		size = new_size;
		printf("\n");
	}
	*triangle_count = size;
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
		vertex_data = ft_split_in_line(tokens[i], "/");
		if (vertex_data)
		{
			if (vertex_data[0])
				face.v_idx[i] = ft_atoi(vertex_data[0]);
			if (vertex_data[1])
				face.vt_idx[i] = ft_atoi(vertex_data[1]);
			if (vertex_data[2])
				face.vn_idx[i] = ft_atoi(vertex_data[2]);
			free(vertex_data);
		}
		i++;
	}
	*out = face;
	free(tokens);
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

int	parse_obj(const char *file, t_custom_object *obj)
{
	t_obj_temp	tmp;

	ft_bzero(&tmp, sizeof(t_obj_temp));
	if (count_obj_data(file, &tmp, 1))
		return (0);
	tmp.vn = malloc(sizeof(t_vec3) * tmp.vn_count);
	tmp.v = malloc(sizeof(t_vec3) * tmp.v_count);
	tmp.vt = malloc(sizeof(t_vec3) * tmp.vt_count);
	tmp.face = malloc(sizeof(t_face) * tmp.face_count);
	tmp.name = NULL;
	tmp.v_count = 0;
	tmp.vn_count = 0;
	tmp.vt_count = 0;
	tmp.face_count = 0;
	extract_obj_data(file, &tmp, 1);
	extract_all_triangle(&tmp, &obj->triangles, &obj->triangle_count);
	// exit(0);
	// (void)obj;
	free(tmp.v);
	free(tmp.vn);
	free(tmp.vt);
	free(tmp.name);
	return (1);
}
