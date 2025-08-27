/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 14:08:52 by lilefebv          #+#    #+#             */
/*   Updated: 2025/08/27 13:06:58 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "error_message.h"

static void	more_types(t_objects type)
{
	if (type == MATERIAL)
	{
		ft_dprintf(2, RED"[Error]"NC" Invalid material. Expected: mat "BLUE);
		ft_dprintf(2, "<name> <albedo> <metallic> <ior> <transmission> ");
		ft_dprintf(2, "<emission_strength> <emission_color> [tex_scale] ");
		ft_dprintf(2, "[normal_map] [normal_intensity]\n"NC);
	}
	else if (type == TEXTURE)
	{
		ft_dprintf(2, RED"[Error]"NC" Invalid texture. Expected: tex "BLUE);
		ft_dprintf(2, "<name> <type> <filepath>\n"NC);
	}
	else if (type == WINDOW)
	{
		ft_dprintf(2, RED"[Error]"NC" Invalid window size. Expected: W "BLUE);
		ft_dprintf(2, "<width_win> <height_win> [width_render]");
		ft_dprintf(2, " [height_render]\n"NC);
	}
}

int	invalid_struct_error(t_objects type, char **splited)
{
	if (type == AMBIANT_LIGHT)
		print_error(ERR_AMBIENT_LF BLUE ERR_AMBIENT_LFS);
	else if (type == CAMERA)
		print_error(ERR_CF BLUE ERR_CFS);
	else if (type == LIGHT)
		print_error(ERR_LF BLUE ERR_LFS);
	else if (type == DIRECTIONAL_LIGHT)
		print_error(ERR_DLF BLUE ERR_DLFS);
	else if (type == SPHERE)
		print_error(ERR_SF BLUE ERR_SFS);
	else if (type == PLANE)
		print_error(ERR_PF BLUE ERR_PFS);
	else if (type == HYPERBOLOID)
		print_error(ERR_HF BLUE ERR_HFS ERR_HFT);
	else if (type == CUSTOM)
		print_error(ERR_CUF BLUE ERR_CUFS);
	else if (type == CYLINDER)
		print_error(ERR_CYF BLUE ERR_CYFS ERR_CFT);
	else if (type == CONE)
		print_error(ERR_COF BLUE ERR_COFS ERR_COFT);
	more_types(type);
	free(splited);
	return (0);
}
