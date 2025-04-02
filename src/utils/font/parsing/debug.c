/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 12:49:02 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/02 16:00:02 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "font.h"

void	print_table_directory(t_table_directory *tbl_dir, uint16_t tbl_size)
{
	uint16_t			i;
	t_table_directory	*tbl;

	printf("\n#)\ttag\tlen\toffset\n");
	i = 0;
	while (i < tbl_size)
	{
		tbl = tbl_dir + i;
		printf("%d)\t%c%c%c%c\t%d\t%d\n", i+1, tbl->tag_c[3], tbl->tag_c[2],
			tbl->tag_c[1], tbl->tag_c[0], tbl->length, tbl->offset);
		i++;
	}
}

void	print_cmap(t_cmap *c)
{
	uint16_t					i;
	t_cmap_encoding_subtable	*es;

	printf("\n#)\tpId\tpsID\toffset\ttype\n");
	i = 0;
	while (i < c->number_subtables) {
		es = c->subtables + i;
		printf("%d)\t%d\t%d\t%d\t", i+1, es->platform_id, es->platform_specific_id, es->offset);
		if (es->platform_id == 0)
			printf("Unicode");
		else if (es->platform_id == 1)
			printf("Mac");
		else if (es->platform_id == 2)
			printf("Not Supported");
		else if (es->platform_id == 3)
			printf("Microsoft");
		printf("\n");
		i++;
	}
}
