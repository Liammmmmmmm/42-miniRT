/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 12:49:02 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/09 17:00:54 by lilefebv         ###   ########lyon.fr   */
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
		printf("%d)\t%c%c%c%c\t%d\t%d\n", i + 1, tbl->tag_c[3], tbl->tag_c[2],
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
	while (i < c->number_subtables)
	{
		es = c->subtables + i;
		printf("%d)\t%d\t%d\t%d\t", i + 1, es->platform_id,
			es->platform_specific_id, es->offset);
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

void	print_format4(t_format4 *f4)
{
	int	i;

	i = 0;
	printf("Format: %d, Length: %d, Language: %d, Segment Count: %d\n",
		f4->format, f4->length, f4->language, f4->seg_count_x2 / 2);
	printf("Search Params: (searchRange: %d, entrySelector: %d, rangeShift: %d)"
		"\n", f4->search_range, f4->entry_selector, f4->range_shift);
	printf("Segment Ranges:\tstartCode\tendCode\tidDelta\tidRangeOffset\n");
	while (i < f4->seg_count_x2 / 2)
	{
		printf("--------------:\t% 9d\t% 7d\t% 7d\t% 12d\n", f4->start_code[i],
			f4->end_code[i], (int16_t)f4->id_delta[i], f4->id_range_offset[i]);
		i++;
	}
}

void	print_glyph_outline(t_glyph_outline *outline)
{
	int	last_index;
	int	i;

	printf("#contours\t(xMin,yMin)\t(xMax,yMax)\tinst_length\n");
	printf("%9d\t(%d,%d)\t\t(%d,%d)\t%d\n", outline->number_of_contours,
		outline->xmin, outline->ymin, outline->xmax, outline->ymax,
		outline->instruction_length);
	printf("advance : %d\n", outline->advance_width);
	printf("lsb     : %d\n", outline->left_side_bearing);
	printf("#)\t(  x  ,  y  )\n");
	if (outline->number_of_contours < 1)
		return ;
	last_index = outline->end_pts_of_contours[outline->number_of_contours - 1];
	i = -1;
	while (++i <= last_index)
		printf("%d)\t(%5d,%5d) -> on curve: %d\n", i, outline->x_coordinates[i], outline->y_coordinates[i], outline->flags[i].on_curve);
	i = -1;
	while (++i <= last_index)
	{
		if (i + 'A' <= 'Z')
			printf("%c=(%d,%d)\n", 'A' + i, outline->x_coordinates[i], outline->y_coordinates[i]);
		else
			printf("%c=(%d,%d)\n", 'a' - 26 + i, outline->x_coordinates[i], outline->y_coordinates[i]);
	}
	printf("\nOn curve only\n");
	i = -1;
	while (++i <= last_index)
	{
		if (!outline->flags[i].on_curve)
			continue ;
		if (i + 'A' <= 'Z')
			printf("%c=(%d,%d)\n", 'A' + i, outline->x_coordinates[i], outline->y_coordinates[i]);
		else
			printf("%c=(%d,%d)\n", 'a' - 26 + i, outline->x_coordinates[i], outline->y_coordinates[i]);
	}
	printf("\nBezier\n");
	i = -1;
	while (++i < outline->bezier_amount)
	{
		if (outline->bezier_lines[i].have_control)
			printf("%d) (%d,%d - %d,%d) + control %d,%d\n", i, outline->bezier_lines[i].p1.x, outline->bezier_lines[i].p1.y, outline->bezier_lines[i].p2.x, outline->bezier_lines[i].p2.y, outline->bezier_lines[i].pc.x, outline->bezier_lines[i].pc.y);
		else
			printf("%d) (%d,%d - %d,%d)\n", i, outline->bezier_lines[i].p1.x, outline->bezier_lines[i].p1.y, outline->bezier_lines[i].p2.x, outline->bezier_lines[i].p2.y);
	}
}

void	print_head(t_head *head)
{
	printf("\n--- HEAD ---");
	printf("Version:              %u\n", head->version);
    printf("Font Revision:        %u\n", head->font_revision);
    printf("Check Sum Adjustment: %u\n", head->check_sum_adjustment);
    printf("Magic Number:        %u\n", head->magic_number);
    printf("Flags:               %u\n", head->flags);
    printf("Units per EM:        %u\n", head->units_per_em);
    printf("Created:             %ld\n", head->created);
    printf("Modified:            %ld\n", head->modified);
    printf("xMin:                %d\n", head->xmin);
    printf("yMin:                %d\n", head->ymin);
    printf("xMax:                %d\n", head->xmax);
    printf("yMax:                %d\n", head->ymax);
    printf("Mac Style:           %u\n", head->mac_style);
    printf("Lowest Rec PPEM:     %u\n", head->lowest_rec_ppem);
    printf("Font Direction Hint: %d\n", head->font_direction_hint);
    printf("Index to Loc Format: %d\n", head->index_to_loc_format);
    printf("Glyph Data Format:   %d\n", head->glyph_data_format);
}
