/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_extract_value.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 13:58:05 by madelvin          #+#    #+#             */
/*   Updated: 2025/06/11 22:12:42 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static char	*skip_space(char *str)
{
	while (*str == ' ' || *str == '\t')
		str++;
	return (str);
}

static void parse_vec(char *line, t_vector *vec_array, int dim)
{
	t_vec3 v = {0};
	char *ptr = line;

	v.x = ft_atod(ptr);
	ptr += ft_strlen_to(ptr, ' ') + 1;
	v.y = ft_atod(ptr);
	ptr += ft_strlen_to(ptr, ' ') + 1;
	if (dim == 3)
		v.z = ft_atod(ptr);
	
	if (dim == 2)
	{
		t_vec2 uv = {v.x, v.y};
		vector_add(vec_array, &uv);
	}
	else
		vector_add(vec_array, &v);
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

static void parse_face(char *line, t_vector *face_indices)
{
    t_vector            corner_indices;
    char                *ptr;
    t_face_idx_triplet  triplet;

    vector_init(&corner_indices, sizeof(t_face_idx_triplet), 4);
    ptr = line;
    while (*ptr)
    {
        ptr = skip_space(ptr);
        if (!isdigit(*ptr) && *ptr != '-' && *ptr != '+')
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
        vector_add(&corner_indices, &triplet);
    }
    
    if (corner_indices.num_elements >= 3)
    {
        for (size_t i = 1; i < corner_indices.num_elements - 1; ++i)
        {
            vector_add(face_indices, vector_get(&corner_indices, 0));
            vector_add(face_indices, vector_get(&corner_indices, i));
            vector_add(face_indices, vector_get(&corner_indices, i + 1));
        }
    }
    free(corner_indices.data);
}

int read_file_to_temp_data(char *filepath, t_parser_temp_data *data)
{
	int		fd;
	char	*line;
	char	*name_start;
	char	*name_end;

	if (vector_init(&data->temp_positions, sizeof(t_vec3), 1024) == -1 ||
		vector_init(&data->temp_normals, sizeof(t_vec3), 1024) == -1 ||
		vector_init(&data->temp_uvs, sizeof(t_vec2), 1024) == -1 ||
		vector_init(&data->temp_faces, sizeof(t_face_idx_triplet), 3072) == -1)
	{
		print_error("Failed to init vector in read_file_to_temp_data");
		return (1);
	}
	fd = open(filepath, O_RDONLY);
	if (fd < 0)
	{
		print_error("Failed to open file.");
		return (1);
	}
	data->object_name = NULL;
	line = get_next_line(fd);
	while (line)
	{
		if (strncmp(line, "v ", 2) == 0)
			parse_vec(line + 2, &data->temp_positions, 3);
		else if (ft_strncmp(line, "vn ", 3) == 0)
			parse_vec(line + 3, &data->temp_normals, 3);
		else if (ft_strncmp(line, "vt ", 3) == 0)
			parse_vec(line + 3, &data->temp_uvs, 2);
		else if (ft_strncmp(line, "f ", 2) == 0)
			parse_face(line + 2, &data->temp_faces);
		else if (ft_strncmp(line, "o ", 2) == 0 && !data->object_name)
		{
			name_start = skip_space(line + 2);
			name_end = ft_strchr(name_start, '\n');
			if (name_end)
				*name_end = '\0';
			data->object_name = ft_strdup(name_start);
		}
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	return (0);
}
