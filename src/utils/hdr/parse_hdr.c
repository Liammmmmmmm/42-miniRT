/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_hdr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 11:21:41 by lilefebv          #+#    #+#             */
/*   Updated: 2025/05/14 12:15:42 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "hdr_parsing.h"

int	print_err_hdr(char *str)
{
	ft_dprintf(2, RED"[Error]"NC" %s\n", str);
	return (-1);
}

int	print_err_hdr_free(char *str, t_bin *bin)
{
	free(bin->data);
	if (str)
		ft_dprintf(2, RED"[Error]"NC" %s\n", str);
	return (0);
}

int	parse_hdr(t_hdr *hdr, char *filename)
{
	t_bin	bin;
	int		index;

	if (!hdr)
		return (-1);
	if (!read_bin_file(&bin, filename))
		return (print_err_hdr_free("Failed to read the file.", &(t_bin){0, 0}));
	ft_bzero(hdr, sizeof(t_hdr));
	index = parse_hdr_header(hdr, &bin);
	if (index == -1)
		return (print_err_hdr_free(NULL, &bin));
	index = get_hdr_size(hdr, &bin, index);
	if (index == -1)
		return (print_err_hdr_free("Can't get the size of the image. "
				"Is the file corrupted ?", &bin));
	hdr->pixels = ft_calloc(hdr->width * hdr->height, sizeof(t_rgbe));
	if (!hdr->pixels)
		return (print_err_hdr_free("Malloc failled.", &bin));
	if (read_hdr_data(hdr, &bin, (size_t)index) == -1)
	{
		free(hdr->pixels);
		return (print_err_hdr_free("Failed to read pixels.", &bin));
	}
	free(bin.data);
	return (1);
}
