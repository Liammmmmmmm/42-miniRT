/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_hdr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 11:21:41 by lilefebv          #+#    #+#             */
/*   Updated: 2025/05/13 09:23:50 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "hdr_parsing.h"

int	print_err_hdr(char *str)
{
	ft_dprintf(2, RED"[Error]"NC" %s\n", str);
	return (-1);
}
#include <stdio.h>

int	parse_hdr(t_hdr *hdr, char *filename)
{
	t_bin	bin;
	int		index;

	if (!hdr)
		return (-1);
	if (!read_bin_file(&bin, filename))
	{
		print_err_hdr("Failed to read the file.");
		return (0);
	}
	ft_bzero(hdr, sizeof(t_hdr));
	index = parse_hdr_header(hdr, &bin);
	if (index == -1)
	{
		free(bin.data);
		return (0);
	}
	index = get_hdr_size(hdr, &bin, index);
	if (index == -1)
	{
		free(bin.data);
		print_err_hdr("Can't get the size of the image. Is the file corrupted ?");
		return (0);
	}
	hdr->pixels = ft_calloc(hdr->width * hdr->height, sizeof(t_rgbe));
	if (!hdr->pixels)
	{
		free(bin.data);
		print_err_hdr("Malloc failled.");
		return (0);
	}
	printf("pas mal en vrai %d %d\n", hdr->width * hdr->dir_x, hdr->height * hdr->dir_y);
	if (read_hdr_data(hdr, &bin, (size_t)index) == -1)
	{
		free(bin.data);
		// free(hdr->pixels);
		print_err_hdr("Failed to read pixels.");
		return (0);
	}
	free(bin.data);
	return (1);
}



// int main()
// {
// 	t_hdr hdr;

// 	printf("Retours : %d\n", parse_hdr(&hdr, "brown_photostudio_02_2k_greyscale.hdr"));
// }