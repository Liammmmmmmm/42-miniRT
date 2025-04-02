/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   font.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 14:47:38 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/02 19:14:39 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef FONT_H
# define FONT_H

# include "libft.h"
# include <stdio.h>

# define FONT_DEBUG 1


typedef struct s_random_font_data
{
	uint16_t	cmap_offset;
	uint16_t	uni_f4_offset;
 }	t_random_font_data;


typedef struct s_format4
{
	uint16_t  format;
 	uint16_t  length;
 	uint16_t  language;
 	uint16_t  seg_count_x2;
 	uint16_t  search_range;
 	uint16_t  entry_selector;
 	uint16_t  range_shift;
	uint16_t  reserved_pad;
	uint16_t  *end_code;
	uint16_t  *start_code;
	uint16_t  *id_delta;
	uint16_t  *id_range_offset;
	uint16_t  *glyph_id_array;
 }	t_format4;

typedef struct s_cmap_encoding_subtable
{
	uint16_t platform_id;
	uint16_t platform_specific_id;
	uint32_t offset;
}	t_cmap_encoding_subtable;

typedef struct s_cmap
{
	uint16_t					version;
	uint16_t					number_subtables;
	t_cmap_encoding_subtable*	subtables;
	t_format4					*format4; // Just transform it into an union if I want to accept other formats
} 	t_cmap;



typedef struct s_offset_subtable
{
	uint32_t	scaler_type;
	uint16_t	num_tables;
	uint16_t	search_range;
	uint16_t	entry_selector;
	uint16_t	range_shift;
}	t_offset_subtable;

typedef struct t_table_directory
{
	union
	{
		char		tag_c[4];
		uint32_t	tag;
	};
	uint32_t	check_sum;
	uint32_t	offset;
	uint32_t	length;
}	t_table_directory;

typedef struct s_font_directoy
{
	t_offset_subtable	off_sub;
	t_table_directory	*tbl_dir;
}	t_font_directoy;




typedef struct s_ttf
{
	t_font_directoy		ft_dir;
	t_cmap				cmap;
	t_random_font_data	r_data;
}	t_ttf;





/*═════════════════════════════════════════════════════════════════════════════╗
║                                   PARSING                                    ║
╚═════════════════════════════════════════════════════════════════════════════*/

t_table_directory	*get_table_directory(t_font_directoy *ft_dir, char c[4]);

int		read_font_directory(t_bin *bin, t_font_directoy *ft_dir);
int		read_cmap(t_bin *bin, size_t *i, t_cmap *cmap);
int		get_cmap(t_bin *bin, t_ttf *ttf);
int		read_format4(t_bin *bin, t_ttf *ttf);

/*═════════════════════════════════════════════════════════════════════════════╗
║                                    DEBUG                                     ║
╚═════════════════════════════════════════════════════════════════════════════*/

void	print_table_directory(t_table_directory *tbl_dir, uint16_t tbl_size);
void	print_cmap(t_cmap *c);
void	print_format4(t_format4 *f4);

#endif
