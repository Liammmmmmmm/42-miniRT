/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_hdr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 11:21:41 by lilefebv          #+#    #+#             */
/*   Updated: 2025/05/12 16:09:24 by lilefebv         ###   ########lyon.fr   */
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
		return (print_err_hdr("Failed to read the file."));
	ft_bzero(hdr, sizeof(t_hdr));
	index = parse_hdr_header(hdr, &bin);
	if (index == -1)
	{
		free(bin.data);
		return (0);
	}
	if (get_hdr_size(hdr, &bin, index) == -1)
	{
		free(bin.data);
		return (0);
	}
	printf("pas mal en vrai %d %d\n", hdr->width * hdr->dir_x, hdr->height * hdr->dir_y);
	free(bin.data);
	return (1);
}



int main()
{
	t_hdr hdr;

	printf("Retours : %d\n", parse_hdr(&hdr, "brown_photostudio_02_2k_greyscale.hdr"));
}