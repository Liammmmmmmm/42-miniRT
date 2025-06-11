/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_parser.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 17:40:53 by madelvin          #+#    #+#             */
/*   Updated: 2025/06/11 22:12:46 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "obj_parsing.h"

void	free_after_exit(t_custom_object *out_obj, t_parser_temp_data *temp_data,
	t_mesh *mesh, char error)
{
	if (error != 0)
	{
		if (error == 1)
			return ;
		if (error == 2)
			return ;
		if (error == 3)
			return ;
	}
	free(temp_data->temp_positions.data);
	free(temp_data->temp_normals.data);
	free(temp_data->temp_uvs.data);
	free(temp_data->temp_faces.data);
	free(temp_data->object_name);
	free(mesh->vertices);
	free(mesh->indices);
	(void)out_obj; //finir de proteger
}

int	parse_obj(char *filepath, t_custom_object *out_obj)
{
	t_parser_temp_data	temp_data;
	t_mesh				mesh;

	ft_bzero(&mesh, sizeof(t_mesh));
	if (read_file_to_temp_data(filepath, &temp_data) != 0)
	{
		free_after_exit(out_obj, &temp_data, &mesh, 1);
		print_error("Failed to read and parse file data.");
		return (1);
	}
	if (build_indexed_mesh(&mesh, &temp_data) != 0)
	{
		free_after_exit(out_obj, &temp_data, &mesh, 2);
		print_error("Failed to build indexed mesh.");
		return (1);
	}
	if (flatten_mesh_to_object(out_obj, &mesh) != 0)
	{
		free_after_exit(out_obj, &temp_data, &mesh, 3);
		print_error("Failed to flatten mesh into final object.");
		return (1);
	}
	free_after_exit(out_obj, &temp_data, &mesh, 0);
	return (0);
}
