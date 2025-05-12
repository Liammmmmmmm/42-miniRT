/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_hdr_header.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 12:08:44 by lilefebv          #+#    #+#             */
/*   Updated: 2025/05/12 16:07:58 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "hdr_parsing.h"

size_t	get_line_bin(t_bin *bin, size_t current_index)
{
	size_t	tmp;

	current_index++;
	tmp = current_index;
	if (current_index >= bin->size)
		return ((size_t)-1);
	while (tmp < bin->size && bin->data[tmp] && bin->data[tmp] != '\n')
		tmp++;
	if (bin->data[tmp] == '\n')
		bin->data[tmp] = '\0';
	return (current_index);
}

int	cmp_prop(char *prop, char *test)
{
	int	i;

	while (prop[i])
	{
		if (test[i] != prop[i])
			return (0);
		i++;
	}
	if (test[i] == '=')
		return (1);
	return (0);
}

void	move_index_end(t_bin *bin, int *index)
{
	while (*index < bin->size && bin->data[*index])
		(*index)++;
}

int	parse_hdr_header(t_hdr *hdr, t_bin *bin)
{
	int	index;
	int	have_format;

	hdr->exposure = 1.0;
	hdr->gamma = 1.0;
	index = get_line_bin(bin, (size_t)-1);
	if (index == (size_t)-1
		|| (ft_strcmp("#?RADIANCE", (char *)&bin->data[index]) != 0
			&& ft_strcmp("#?RGBE", (char *)&bin->data[index]) != 0))
		return (print_err_hdr("Invalid hdr file: not an hdr."));
	have_format = 0;
	while (index != (size_t)-1 && bin->data[index])
	{
		if (cmp_prop("FORMAT", (char *)&bin->data[index]))
		{
			have_format = 1;
			if (ft_strcmp("FORMAT=32-bit_rle_rgbe", (char *)&bin->data[index]) != 0)
				return (print_err_hdr("Unsuported hdr format, only support '32-bit_rle_rgbe'"));
			
		}
		else if (cmp_prop("EXPOSURE", (char *)&bin->data[index]))
		{
			if (is_valid_float((char *)&bin->data[index + 10]))
				hdr->exposure = ft_atof((char *)&bin->data[index + 10]);
		}
		else if (cmp_prop("GAMMA", (char *)&bin->data[index]))
		{
			if (is_valid_float((char *)&bin->data[index + 7]))
				hdr->exposure = ft_atof((char *)&bin->data[index + 7]);
		}
		move_index_end(bin, &index);
		index = get_line_bin(bin, index);
	}
	if (index == (size_t)-1)
		return (-1);
	if (have_format)
		return (index + 1);
	return (print_err_hdr("Unsuported hdr format, only support '32-bit_rle_rgbe'"));
}

void	read_width_or_height(t_hdr *hdr, t_bin *bin, int index)
{
	if (bin->data[index + 1] == 'X')
	{
		if (bin->data[index] == '-')
			hdr->dir_x = -1;
		hdr->width = ft_atoi((char *)&bin->data[index + 3]);
	}
	else if (bin->data[index + 1] == 'Y')
	{
		if (bin->data[index] == '-')
			hdr->dir_y = -1;
		hdr->height = ft_atoi((char *)&bin->data[index + 3]);
	}
}

int	get_hdr_size(t_hdr *hdr, t_bin *bin, int index)
{
	hdr->dir_x = 1;
	hdr->dir_y = 1;
	hdr->width = 0;
	hdr->height = 0;
	index = get_line_bin(bin, index - 1);
	if (index == (size_t)-1 || ft_strlen((char *)&bin->data[index]) < 9)
		return (-1);
	read_width_or_height(hdr, bin, index);
	index += 3;
	while (ft_isdigit(bin->data[index]))
		index++;
	if (bin->data[index] != ' ' || ft_strlen((char *)&bin->data[index]) < 5)
		return (-1);
	index++;
	read_width_or_height(hdr, bin, index);
	if (hdr->width == 0 || hdr->height == 0)
		return (-1);
	return (0);
}
