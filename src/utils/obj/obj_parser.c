/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_parser.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 17:40:53 by madelvin          #+#    #+#             */
/*   Updated: 2025/06/23 17:04:41 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "obj_parsing.h"

void	free_after_exit(t_parser_temp_data *temp_data,
	t_mesh *mesh, char error)
{
	if (error != 0)
	{
		if (error == 1)
			return ;
		free(temp_data->temp_positions.data);
		free(temp_data->temp_normals.data);
		free(temp_data->temp_uvs.data);
		free(temp_data->temp_faces.data);
		if (temp_data->object_name)
			free(temp_data->object_name);
		if (error == 2)
			return ;
		free(mesh->indices);
		free(mesh->vertices);
		return ;
	}
	free(temp_data->temp_positions.data);
	free(temp_data->temp_normals.data);
	free(temp_data->temp_uvs.data);
	free(temp_data->temp_faces.data);
	free(mesh->vertices);
	free(mesh->indices);
}

int	parse_obj(char *filepath, t_custom_object *out_obj)
{
	t_parser_temp_data	temp_data;
	t_mesh				mesh;

	printf("Start parsing : " YELLOW "%s\n" NC, filepath);
	printf("Start reading file.\n");
	ft_bzero(&mesh, sizeof(t_mesh));
	if (read_file_to_temp_data(filepath, &temp_data) != 0)
	{
		free_after_exit(&temp_data, &mesh, 1);
		return (print_error1("Failed to read and parse file data"));
	}
	printf("File finished being read.\n");
	print_progress_bar(0, temp_data.temp_faces.num_elements);
	if (build_indexed_mesh(&mesh, &temp_data) != 0)
	{
		free_after_exit(&temp_data, &mesh, 2);
		return (print_error1("Failed to build indexed mesh"));
	}
	if (flatten_mesh_to_object(out_obj, &mesh) != 0)
	{
		free_after_exit(&temp_data, &mesh, 3);
		return (print_error("Failed to flatten mesh into final object"));
	}
	free_after_exit(&temp_data, &mesh, 0);
	return (0);
}
