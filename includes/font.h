/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   font.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 14:47:38 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/10 15:01:04 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef FONT_H
# define FONT_H

# include "libft.h"
# include <stdio.h>

# define FONT_DEBUG 1
# define MAGIC_NUMBER 0x5F0F3CF5

typedef struct s_point2
{
	int32_t	x; 
	int32_t	y;
}	t_point2;

typedef struct s_bezier
{
	t_point2	p1; 
	t_point2	p2;
	t_bool		have_control: 1;
	t_point2	pc;
}	t_bezier;

typedef struct s_segment
{
	t_point2	p1; 
	t_point2	p2;
}	t_segment;

typedef struct s_head
{
	uint32_t	version;
	uint32_t	font_revision;
	uint32_t	check_sum_adjustment;
	uint32_t	magic_number;
	uint16_t	flags;
	uint16_t	units_per_em;
	int64_t		created;
	int64_t		modified;
	int16_t		xmin;
	int16_t		ymin;
	int16_t		xmax;
	int16_t		ymax;
	uint16_t	mac_style;
	uint16_t	lowest_rec_ppem;
	int16_t		font_direction_hint;
	int16_t		index_to_loc_format;
	int16_t		glyph_data_format;
}	t_head;

typedef struct s_random_font_data
{
	uint32_t	cmap_offset;
	uint32_t	uni_f4_offset;
	uint32_t	head_offset;
	uint32_t	loca_offset;
	uint32_t	glyf_offset;
	uint32_t	hhea_offset;
	uint32_t	hmtx_offset;
	uint32_t	maxp_offset;
	t_point2	*points_buff;
	int32_t		estimated_max_seg_intersect;
	int32_t		*seg_intersec;
	float		scale;
	int32_t		xmax;
	int32_t		ytmp;
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
	t_format4					*format4;
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
}	t_font_directory;

typedef union u_glyph_lag
{
	struct
	{
		uint8_t	on_curve: 1;
		uint8_t	x_short: 1;
		uint8_t	y_short: 1;
		uint8_t	repeat: 1;
		uint8_t	x_short_pos: 1;
		uint8_t	y_short_pos: 1;
		uint8_t	reserved1: 1;
		uint8_t	reserved2: 1;
	};
	uint8_t	flag;
}	t_glyph_flag;

typedef struct s_glyph_outline
{
	int16_t			number_of_contours;
	int16_t			xmin;
	int16_t			ymin;
	int16_t			xmax;
	int16_t			ymax;
	uint16_t		instruction_length;
	uint8_t			*instructions;
	t_glyph_flag	*flags;
	int16_t			*x_coordinates;
	int16_t			*y_coordinates;
	uint16_t		*end_pts_of_contours;
	t_bezier		*bezier_lines;
	int32_t			bezier_amount;
	uint16_t		advance_width;
	int16_t			left_side_bearing;
	t_segment		*segments;
	uint32_t		segments_amount;
}	t_glyph_outline;

typedef struct s_hhea
{
	uint32_t	version;
	int16_t		ascent;
	int16_t		descent;
	int16_t		line_gap;
	uint16_t	num_hmetrics;
}	t_hhea;

typedef struct s_maxp
{
	uint16_t	max_contours;
}	t_maxp;

typedef struct s_ttf
{
	t_font_directory	ft_dir;
	t_random_font_data	r_data;
	t_cmap				cmap;
	t_head				head;
	t_hhea				hhea;
	t_maxp				maxp;
	t_glyph_outline		*glyph256;
	int32_t				size;
	uint16_t			bezier_resolution;
	uint8_t				ssp;
}	t_ttf;

# include "./structs.h"

int		get_font(t_ttf *ttf, char *filename);

/*═════════════════════════════════════════════════════════════════════════════╗
║                                   PARSING                                    ║
╚═════════════════════════════════════════════════════════════════════════════*/

t_table_directory	*get_table_directory(t_font_directory *ft_dir, char c[4]);
uint32_t			get_glyph_index(uint16_t code_point, t_format4 *f);
int					cmp_tbl_tag(char c[4], uint32_t tag);
uint32_t			get_glyph_offset(t_bin *bin, t_ttf *ttf, uint32_t glyph_index);

int		read_font_directory(t_bin *bin, t_ttf *ttf);
int		read_cmap(t_bin *bin, size_t *i, t_cmap *cmap);
int		get_cmap(t_bin *bin, t_ttf *ttf);
int		read_format4(t_bin *bin, t_ttf *ttf);
int		read_head(t_bin *bin, t_ttf *ttf);
int		read_hhea(t_bin *bin, t_ttf *ttf);
int		read_hmtx(t_bin *bin, t_ttf *ttf);
int		read_maxp(t_bin *bin, t_ttf *ttf);
int		get_glyph_outline(t_bin *bin, t_ttf* ttf, uint32_t glyph_index,
	t_glyph_outline *outline);
int		init_co_y(t_bin *bin, size_t *i, t_glyph_outline *o, int last_index);
int		init_co_x(t_bin *bin, size_t *i, t_glyph_outline *o, int last_index);
int		get_bezier(t_glyph_outline *o, int pts_am);
int		count_bezier_lines(t_glyph_outline *o, int pts_am);
t_point2	p2co(t_glyph_outline *o, int i1, int i2);
int		get_tmp_next(int i, t_glyph_outline *o, int n_contours);


int		save_glyph256(t_bin *bin, t_ttf *ttf);
int		free_glyphs(t_glyph_outline *glyphs, int i);
int		free_glyph(t_glyph_outline *o);

int		print_err_ttf(char *str);

int		free_ttf(t_ttf *ttf);

int		set_bezier_res(t_ttf *ttf, uint8_t bezier_res);

/*═════════════════════════════════════════════════════════════════════════════╗
║                                    DEBUG                                     ║
╚═════════════════════════════════════════════════════════════════════════════*/

void	print_table_directory(t_table_directory *tbl_dir, uint16_t tbl_size);
void	print_cmap(t_cmap *c);
void	print_format4(t_format4 *f4);
void	print_glyph_outline(t_glyph_outline *outline);
void	print_head(t_head *head);

/*═════════════════════════════════════════════════════════════════════════════╗
║                                    RENDER                                    ║
╚═════════════════════════════════════════════════════════════════════════════*/

void	tessellate_bezier(t_point2 *output, uint32_t output_size, t_bezier *pts);

void	draw_line(t_point *point_a, t_point *point_b, t_img *img);
void	draw_glyph_outline(t_img *img, t_ttf *ttf, t_uchar c, t_point2 pos);

void	draw_glyph(t_img *img, t_ttf *ttf, t_uchar c, t_point2 pos);
void	draw_string(t_img *img, t_ttf *ttf, char *str, t_point2 pos);

#endif
