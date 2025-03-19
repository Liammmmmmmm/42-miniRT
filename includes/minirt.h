/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 09:40:06 by lilefebv          #+#    #+#             */
/*   Updated: 2025/03/19 10:59:58 by lilefebv         ###   ########lyon.fr   */
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
║                                   PARSING                                    ║
╚═════════════════════════════════════════════════════════════════════════════*/

int		not_enough_el_error(char *el);
int		to_many_el_error(char *el);
int		print_line_error(int nb, char *line);

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

int		parse_scene(t_minirt *minirt, char *filename);

#endif