/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 09:40:06 by lilefebv          #+#    #+#             */
/*   Updated: 2025/03/18 12:27:42 by lilefebv         ###   ########lyon.fr   */
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

int	print_error(char *err);

/*═════════════════════════════════════════════════════════════════════════════╗
║                                   PARSING                                    ║
╚═════════════════════════════════════════════════════════════════════════════*/

int	print_line_error(int nb, char *line);

/**
 * @return 1 if the line start by `type` folowed by a space or tab, 0 otherwise
 */
int	cmp_type(const char *type, const char *line);

int	is_empty_line(char *line);

int	count_valid_lines(char *filename);

int	parse_scene(t_minirt *minirt, char *filename);

#endif