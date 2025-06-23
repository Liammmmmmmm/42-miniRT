/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_extract_value_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 16:07:58 by madelvin          #+#    #+#             */
/*   Updated: 2025/06/23 17:11:08 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "obj_parsing.h"
#include "utils.h"

char	*skip_space(char *str)
{
	while (*str == ' ' || *str == '\t')
		str++;
	return (str);
}

char	parse_vec3(char *line, t_vector *vec_array)
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

char	parse_vec2(char *line, t_vector *vec_array)
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

int	parse_int_and_advance(char **line)
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
