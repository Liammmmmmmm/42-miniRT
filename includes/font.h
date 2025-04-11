/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   font.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 14:47:38 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/11 12:55:50 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef FONT_H
# define FONT_H

# include "libft.h"
# include <stdio.h>
# include "mlx_base.h"
# include "font_structs.h"
# include "structs.h"

# define FONT_DEBUG 1
# define MAGIC_NUMBER 0x5F0F3CF5

int		get_font(t_ttf *ttf, char *filename);

/*═════════════════════════════════════════════════════════════════════════════╗
║                                   PARSING                                    ║
╚═════════════════════════════════════════════════════════════════════════════*/

int		read_font_directory(t_bin *bin, t_ttf *ttf);
int		read_cmap(t_bin *bin, size_t *i, t_cmap *cmap);
int		get_cmap(t_bin *bin, t_ttf *ttf);
int		read_format4(t_bin *bin, t_ttf *ttf);
int		read_head(t_bin *bin, t_ttf *ttf);
int		read_hhea(t_bin *bin, t_ttf *ttf);
int		read_hmtx(t_bin *bin, t_ttf *ttf);
int		read_maxp(t_bin *bin, t_ttf *ttf);
int		get_glyph_outline(t_bin *bin, t_ttf *ttf, uint32_t glyph_index,
			t_glyph_outline *outline);
int		init_co_y(t_bin *bin, size_t *i, t_glyph_outline *o, int last_index);
int		init_co_x(t_bin *bin, size_t *i, t_glyph_outline *o, int last_index);
int		get_bezier(t_glyph_outline *o, int pts_am);
int		count_bezier_lines(t_glyph_outline *o, int pts_am);
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

int		calc_gradiant_color(int color_a, int color_b, float ratio);
int		get_pixel_color(t_img *img, int x, int y);
void	tessellate_bezier(t_point2 *output, uint32_t output_size, t_bezier *pts)
		;

void	draw_line(t_point *point_a, t_point *point_b, t_img *img, int c);
void	draw_glyph_outline(t_img *img, t_ttf *ttf, t_uchar c, t_point2 pos);

void	draw_glyph(t_img *img, t_ttf *ttf, t_uchar c, t_point2 pos);
void	draw_string(t_img *img, t_ttf *ttf, char *str, t_point2 pos);

#endif
