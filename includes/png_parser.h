/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   png_parser.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 16:45:08 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/11 17:42:23 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef PNG_PARSER_H
# define PNG_PARSER_H

# include "structs.h"
# include "libft.h"



typedef struct s_sh_png_chunk {
	uint32_t	data_length;
	union
	{
		char		type_c[4];
		uint32_t	type;
	};
	uint8_t		*data;
	uint32_t	crc32;
}	t_sh_png_chunk;



#endif