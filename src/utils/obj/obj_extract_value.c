/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_extract_value.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 13:58:05 by madelvin          #+#    #+#             */
/*   Updated: 2025/06/23 16:53:48 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static char	safe_return_parsing(t_parser_temp_data *data)
{
	free(data->temp_positions.data);
	free(data->temp_normals.data);
	free(data->temp_uvs.data);
	free(data->temp_faces.data);
	if (data->object_name)
		free(data->object_name);
	return (1);
}

static char	open_file_and_init_vector(int *fd, t_parser_temp_data *data,
	char *filepath)
{
	*fd = open(filepath, O_RDONLY);
	if (*fd < 0)
	{
		print_error("Failed to open file");
		return (1);
	}
	if (vector_init(&data->temp_positions, sizeof(t_vec3), 1024) == -1 || \
		vector_init(&data->temp_normals, sizeof(t_vec3), 1024) == -1 || \
		vector_init(&data->temp_uvs, sizeof(t_vec2), 1024) == -1 || \
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
		return (print_error1("Init vector in read_file_to_temp_data"));
	}
	data->object_name = NULL;
	return (0);
}

static char	parse_file(char *line, t_parser_temp_data *data)
{
	if (ft_strncmp(line, "v ", 2) == 0)
	{
		if (parse_vec3(line + 2, &data->temp_positions) == 1)
			return (safe_return_parsing(data));
	}
	else if (ft_strncmp(line, "vn ", 3) == 0)
	{
		if (parse_vec3(line + 3, &data->temp_normals) == 1)
			return (safe_return_parsing(data));
	}
	else if (ft_strncmp(line, "vt ", 3) == 0)
	{
		if (parse_vec2(line + 3, &data->temp_uvs) == 1)
			return (safe_return_parsing(data));
	}
	else if (ft_strncmp(line, "f ", 2) == 0)
	{
		if (parse_face(line + 2, &data->temp_faces) == 1)
			return (safe_return_parsing(data));
	}
	return (0);
}

int	read_file_to_temp_data(char *filepath, t_parser_temp_data *data)
{
	int		fd;
	char	*line;
	char	*name_start;
	char	*name_end;

	if (open_file_and_init_vector(&fd, data, filepath))
		return (1);
	line = get_next_line(fd);
	while (line)
	{
		if (parse_file(line, data))
			return (1);
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
