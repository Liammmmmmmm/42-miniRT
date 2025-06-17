/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_extract_value.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 13:58:05 by madelvin          #+#    #+#             */
/*   Updated: 2025/06/12 16:02:11 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static char	*skip_space(char *str)
{
	while (*str == ' ' || *str == '\t')
		str++;
	return (str);
}

static char parse_vec3(char *line, t_vector *vec_array)
{
	t_vec3	v;
	char	*ptr;

	ptr = line;
	v.x = ft_atod(ptr);
	ptr += ft_strlen_to(ptr, ' ') + 1;
	v.y = ft_atod(ptr);
	ptr += ft_strlen_to(ptr, ' ') + 1;
	v.z = ft_atod(ptr);
	if (vector_add(vec_array, &v) == -1)
	{
		print_error("Vector add in parse_vec (v).");
		return (1);
	}
	return (0);
}


static char parse_vec2(char *line, t_vector *vec_array)
{
	t_vec2	uv;
	char	*ptr;

	ptr = line;
	uv.x = ft_atod(ptr);
	ptr += ft_strlen_to(ptr, ' ') + 1;
	uv.y = ft_atod(ptr);
	ptr += ft_strlen_to(ptr, ' ') + 1;
	if (uv.x > 1 || uv.x < 0)
		uv.x = uv.x - floor(uv.x);
	if (uv.y > 1 || uv.y < 0)
		uv.y = uv.y - floor(uv.y);
	if (vector_add(vec_array, &uv) == -1)
	{
		print_error("Vector add in parse_vec (uv).");
		return (1);
	}
	return (0);
}

static int	parse_int_and_advance(char **line)
{
	int		val;
	char	*p;

	p = *line;
	val = ft_atoi(p);
	if (*p == '+' || *p == '-')
		p++;
	while (ft_isdigit(*p))
		p++;
	*line = p;
	return (val);
}

static char parse_face(char *line, t_vector *face_indices)
{
	t_face_idx_triplet  triplet;
    t_vector            corner_indices;
    char                *ptr;
	size_t				i;

    if (vector_init(&corner_indices, sizeof(t_face_idx_triplet), 4) == -1)
	{
		print_error("Vector init in parse_face.");
		return (1);
	}
    ptr = line;
    while (*ptr)
    {
        ptr = skip_space(ptr);
        if (!ft_isdigit(*ptr) && *ptr != '-' && *ptr != '+')
            break ;
        triplet = (t_face_idx_triplet){0, 0, 0};
        triplet.v_idx = parse_int_and_advance(&ptr);
        if (*ptr == '/')
        {
            ptr++;
            if (*ptr != '/')
                triplet.vt_idx = parse_int_and_advance(&ptr);
            if (*ptr == '/')
            {
                ptr++;
                triplet.vn_idx = parse_int_and_advance(&ptr);
            }
        }
		if (vector_add(&corner_indices, &triplet) == -1)
		{
			print_error("Vector add in parse_face.");
			free(corner_indices.data);
			return (1);
		}
    }
	if (corner_indices.num_elements >= 3)
	{
		i = 0;
		while (++i < corner_indices.num_elements - 1)
		{
			if (vector_add(face_indices, vector_get(&corner_indices, 0)) == -1 \
		|| vector_add(face_indices, vector_get(&corner_indices, i)) == -1 \
		|| vector_add(face_indices, vector_get(&corner_indices, i + 1)) == -1)
			{
				print_error("Vector add in parse_face.");
				free(corner_indices.data);
				return (1);
			}
		}
    }
    free(corner_indices.data);
	return (0);
}

int read_file_to_temp_data(char *filepath, t_parser_temp_data *data)
{
	int		fd;
	char	*line;
	char	*name_start;
	char	*name_end;

	fd = open(filepath, O_RDONLY);
	if (fd < 0)
	{
		print_error("Failed to open file.");
		return (1);
	}
	if (vector_init(&data->temp_positions, sizeof(t_vec3), 1024) == -1 ||
		vector_init(&data->temp_normals, sizeof(t_vec3), 1024) == -1 ||
		vector_init(&data->temp_uvs, sizeof(t_vec2), 1024) == -1 ||
		vector_init(&data->temp_faces, sizeof(t_face_idx_triplet), 3072) == -1)
	{
		if (data->temp_positions.data)
			free(data->temp_positions.data);
		if (data->temp_normals.data)
			free(data->temp_normals.data);
		if (data->temp_uvs.data)
			free(data->temp_uvs.data);
		if (data->temp_faces.data)
			free(data->temp_faces.data);
		print_error("Failed to init vector in read_file_to_temp_data.");
		return (1);
	}
	data->object_name = NULL;
	line = get_next_line(fd);
	while (line)
	{
		if (strncmp(line, "v ", 2) == 0)
		{
			if (parse_vec3(line + 2, &data->temp_positions) == 1)
			{
				free(data->temp_positions.data);
				free(data->temp_normals.data);
				free(data->temp_uvs.data);
				free(data->temp_faces.data);
				if (data->object_name)
					free(data->object_name);
				return (1);
			}
		}
		else if (ft_strncmp(line, "vn ", 3) == 0)
		{
			if (parse_vec3(line + 3, &data->temp_normals) == 1)
			{
				free(data->temp_positions.data);
				free(data->temp_normals.data);
				free(data->temp_uvs.data);
				free(data->temp_faces.data);
				if (data->object_name)
					free(data->object_name);
				return (1);
			}
		}
		else if (ft_strncmp(line, "vt ", 3) == 0)
		{
			if (parse_vec2(line + 3, &data->temp_uvs) == 1)
			{
				free(data->temp_positions.data);
				free(data->temp_normals.data);
				free(data->temp_uvs.data);
				free(data->temp_faces.data);
				if (data->object_name)
					free(data->object_name);
				return (1);
			}

		}
		else if (ft_strncmp(line, "f ", 2) == 0)
		{
			if (parse_face(line + 2, &data->temp_faces) == 1)
			{
				free(data->temp_positions.data);
				free(data->temp_normals.data);
				free(data->temp_uvs.data);
				free(data->temp_faces.data);
				if (data->object_name)
					free(data->object_name);
				return (1);
			}
		}
		else if (ft_strncmp(line, "o ", 2) == 0 && !data->object_name)
		{
			name_start = skip_space(line + 2);
			name_end = ft_strchr(name_start, '\n');
			if (name_end)
				*name_end = '\0';
			data->object_name = ft_strdup(name_start); // a voir si c'est une erreur critique pour le moment balec
		}
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	return (0);
}
