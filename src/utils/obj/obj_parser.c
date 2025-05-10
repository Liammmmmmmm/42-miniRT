/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_parser.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 17:40:53 by madelvin          #+#    #+#             */
/*   Updated: 2025/05/08 12:57:12 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "obj_parsing.h"

void	init_obj_data_function(int *fd, char *error, t_obj_temp *tmp, \
	char **line)
{
	*error = 0;
	*fd = open(tmp->name, O_RDONLY);
	if (*fd < 0)
	{
		print_error("Error opening .obj file");
		*error = 1;
		return ;
	}
	*line = get_next_line(*fd);
	if (!line)
	{
		print_error("Empty or unreadable .obj file");
		*error = 1;
		return ;
	}
	tmp->v_count = 0;
	tmp->vn_count = 0;
	tmp->vt_count = 0;
	tmp->face_count = 0;
}

int	extract_obj_data(t_obj_temp *tmp, size_t *size)
{
	char	*line;
	int		fd;
	char	error;
	size_t	i;

	line = NULL;
	init_obj_data_function(&fd, &error, tmp, &line);
	tmp->name = NULL;
	i = 0;
	while (line && i < *size)
	{
		get_value_v(line, tmp, &error);
		get_value_vn(line, tmp, &error);
		get_value_vt(line, tmp, &error);
		get_value_face(line, tmp, &error);
		get_value_name(line, &tmp->name, &error);
		free(line);
		if (error == 1)
			break ;
		line = get_next_line(fd);
		i++;
	}
	close(fd);
	return (error);
}

int	init_tmp_obj(t_obj_temp *tmp, char *file)
{
	tmp->name = file;
	tmp->vn = malloc(sizeof(t_vec3) * tmp->vn_count);
	tmp->v = malloc(sizeof(t_vec3) * tmp->v_count);
	tmp->vt = malloc(sizeof(t_vec3) * tmp->vt_count);
	tmp->face = malloc(sizeof(t_face) * tmp->face_count);
	if (!tmp->face || !tmp->v || !tmp->vn || !tmp->vt)
	{
		print_error("malloc error in obj parser");
		tmp->name = NULL;
		return (1);
	}
	return (0);
}

int	setup_obj_lst(t_custom_object *obj)
{
	size_t	i;

	obj->obj_list = malloc(sizeof(t_object) * obj->triangle_count);
	if (obj->obj_list == NULL)
	{
		free(obj->triangles);
		print_error("Error of malloc in setup obj lst");
		return (1);
	}
	i  = 0;
	while (i < obj->triangle_count)
	{
		obj->obj_list[i].object = &obj->triangles[i];
		obj->obj_list[i].type = TRIANGLE;
		i++;
	}
	return (0);
}

int	parse_obj(char *file, t_custom_object *obj)
{
	t_obj_temp	tmp;
	size_t		size;

	ft_bzero(&tmp, sizeof(t_obj_temp));
	if (count_obj_data(file, &tmp, &size) || init_tmp_obj(&tmp, file) || \
		extract_obj_data(&tmp, &size) || \
		extract_all_triangle(&tmp, obj, file))
	{
		free(tmp.v);
		free(tmp.vn);
		free(tmp.vt);
		ft_free_tab_face(tmp.face, tmp.face_count);
		free(tmp.face);
		free(tmp.name);
		printf(RED"[Error]"NC" Error of parsing in object :"YELLOW"%s"NC"\n", \
			file);
		return (0);
	}
	free(tmp.v);
	free(tmp.vn);
	free(tmp.vt);
	ft_free_tab_face(tmp.face, tmp.face_count);
	if (setup_obj_lst(obj) == 1)
		return (0);
	return (1);
}
