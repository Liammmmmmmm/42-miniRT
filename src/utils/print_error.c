/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 17:59:22 by madelvin          #+#    #+#             */
/*   Updated: 2025/06/23 18:04:33 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

int	print_error(char *err)
{
	ft_dprintf(2, RED"[Error]"NC" %s\n", err);
	return (0);
}

int	print_error1(char *err)
{
	ft_dprintf(2, RED"[Error]"NC" %s\n", err);
	return (1);
}
