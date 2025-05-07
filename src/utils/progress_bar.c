/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   progress_bar.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 15:52:15 by madelvin          #+#    #+#             */
/*   Updated: 2025/05/07 16:38:07 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void print_progress_bar(size_t actual, size_t max)
{
	const size_t	bar_width = 50;
	const size_t	filled = (int)((double)actual / (double)max * bar_width);
	const size_t	percent = (int)((double)actual / (double)max * 100);
	static size_t	last_filled = (size_t)-1;
	size_t			i;

	if (filled == last_filled)
		return ;
	write(1, "\e[?25l", 7);
	if (filled != 0)
		ft_printf("\r");
	write(1, "[", 1);
	i = 0;
	while (i < bar_width)
	{
		if (i < filled)
			write(1, YELLOW"="NC, 15);
		else
			write(1, " ", 1);
		i++;
	}
	ft_printf("] %d%% (%d/%d)", percent, actual, max);
	last_filled = filled;
	if (last_filled == 100)
		last_filled = (size_t)-1;
	write(1, "\e[?25h", 7);
}