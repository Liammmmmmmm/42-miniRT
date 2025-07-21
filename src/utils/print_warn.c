/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_warn.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 17:59:22 by madelvin          #+#    #+#             */
/*   Updated: 2025/07/01 13:17:48 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

int	print_warn(char *warn)
{
	ft_dprintf(2, YELLOW"[Warn]"NC" %s\n", warn);
	return (0);
}

int	print_warn_rv(char *warn, char rv)
{
	ft_dprintf(2, YELLOW"[Warn]"NC" %s\n", warn);
	return (rv);
}
