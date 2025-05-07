/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_extract_value.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 13:58:05 by madelvin          #+#    #+#             */
/*   Updated: 2025/05/07 18:33:51 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	get_value_v(char *line, t_obj_temp *tmp, char *error)
{
	t_vec3	vec;
	char	**co;

	if (*error != 0 || ft_strncmp(line, "v ", 2) != 0)
		return ;
	co = ft_split_in_line(line + 2, " ");
	if (!co)
	{
		print_error("malloc error in obj parser");
		*error = 1;
		return ;
	}
	if (char_tab_len(co) != 3)
	{
		print_error("malloc error in obj parser");
		free(co);
		*error = 1;
		return ;
	}
	vec.x = ft_atod(co[0]);
	vec.y = ft_atod(co[1]);
	vec.z = ft_atod(co[2]);
	free(co);
	tmp->v[tmp->v_count++] = vec;
	return ;
}

void	get_value_vn(char *line, t_obj_temp *tmp, char *error)
{
	t_vec3	vec;
	char	**co;

	if (*error != 0 || ft_strncmp(line, "vn ", 3) != 0)
		return ;
	co = ft_split_in_line(line + 3, " ");
	if (!co)
	{
		print_error("malloc error in obj parser");
		*error = 1;
		return ;
	}
	if (char_tab_len(co) != 3)
	{
		print_error("malloc error in obj parser");
		free(co);
		*error = 1;
		return ;
	}
	vec.x = ft_atod(co[0]);
	vec.y = ft_atod(co[1]);
	vec.z = ft_atod(co[2]);
	free(co);
	tmp->vn[tmp->vn_count++] = vec;
	return ;
}

void	get_value_vt(char *line, t_obj_temp *tmp, char *error)
{
	t_vec3	uv;
	char	**co;

	if (*error != 0 || ft_strncmp(line, "vt ", 3) != 0)
		return ;
	co = ft_split_in_line(line + 3, " ");
	if (!co)
	{
		print_error("malloc error in obj parser");
		*error = 1;
		return ;
	}
	if (char_tab_len(co) < 2)
	{
		print_error("malloc error in obj parser");
		free(co);
		*error = 1;
		return ;
	}
	uv.x = ft_atod(co[0]);
	uv.y = ft_atod(co[1]);
	uv.z = 0;
	free(co);
	tmp->vt[tmp->vt_count++] = uv;
	return ;
}

void	get_value_name(char *line, char **out, char *error)
{
	if (*error != 0 || ft_strncmp(line, "o ", 2) != 0)
		return ;
	if (out != NULL)
		return ;
	*out = ft_strdup(line + 2);
	if (out == NULL)
	{
		*error = 1;
		return ;
	}
	return ;
}
