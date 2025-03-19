/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 09:40:06 by lilefebv          #+#    #+#             */
/*   Updated: 2025/03/19 15:11:36 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# include "structs.h"
# include "libft.h"
# include <fcntl.h>
# include <stdio.h>
# include <string.h>
# include <errno.h>

int		print_error(char *err);

/*═════════════════════════════════════════════════════════════════════════════╗
║                                    UTILS                                     ║
╚═════════════════════════════════════════════════════════════════════════════*/

void	print_scene(t_scene *scene);


int		char_tab_len(char **tab);
char	**free_ret_null(char **fre);

/*═════════════════════════════════════════════════════════════════════════════╗
║                                   PARSING                                    ║
╚═════════════════════════════════════════════════════════════════════════════*/

int		not_enough_el_error(char *el);
int		to_many_el_error(char *el);
int		print_line_error(int nb, char *line);
int		invalid_struct_error(t_objects type, char **splited);
int		invalid_float_error(char **splited, int i);
int		invalid_size_error(char **splited);

/**
 * @return 1 if the line start by `type` folowed by a space or tab, 0 otherwise
 */
int		cmp_type(const char *type, const char *line);

int		is_empty_line(char *line);

/**
 * @return -1 for an invalid line, 0 for an empty line, 1 for a valid line
 */
int 	is_valid_line(char *line);

int		count_valid_lines(char *filename);
char	*remove_useless_spaces(char *line);
char	*remove_comments(char *line);
int		valid_elements_amount(char **lines);

int		free_scene(t_scene *scene, char **lines);
int		parse_elements(t_scene *scene, char **lines, int ln_amount);

int		is_valid_brightness(char *str, float *co);
int		is_valid_size(char *str, float *co);
int		is_valid_fov(char *str, unsigned char *fov);
int		parse_color(char *color, t_color *store);
int		parse_vector(char *str, t_vec3 *vec);
int		parse_vector_normalized(char *str, t_vec3 *vec);

int		parse_ambiant_light(t_scene *scene, char *line);
int		parse_camera(t_scene *scene, char *line);
int		parse_light(t_scene *scene, char *line);
int		parse_sphere(t_scene *scene, char *line);
int		parse_plane(t_scene *scene, char *line);
int		parse_cylinder(t_scene *scene, char *line);

int		parse_scene(t_minirt *minirt, char *filename);

#endif