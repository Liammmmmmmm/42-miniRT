/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 14:40:16 by lilefebv          #+#    #+#             */
/*   Updated: 2025/05/21 12:37:25 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include "structs.h"

const char	*get_object_name(t_objects obj);
t_objects	get_object_type(char *str);
char		*empty_static_string(void);
int			print_error(char *err);
int			print_error1(char *err);
int			char_tab_len(char **tab);
char		**free_ret_null(char **fre);
void		ft_izero(int *pointer, size_t n);

#endif