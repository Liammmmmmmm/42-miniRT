/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:05:12 by lilefebv          #+#    #+#             */
/*   Updated: 2025/05/05 11:08:51 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	invalid_size_error(char **splited)
{
	print_error("Invalid size, must be a positive value.");
	free(splited);
	return (0);
}

int	texture_error(int error, char **parts)
{
	if (error == 0)
		ft_dprintf(2, RED"[Error]"NC" Invalid texture name: `%s'\n", parts[1]);
	else if (error == 1)
		ft_dprintf(2, RED"[Error]"NC" texture name too long: `%s'\n", parts[1]);
	else if (error == 2)
		ft_dprintf(2, RED"[Error]"NC" Name already used: `%s'\n", parts[1]);
	else if (error == 3)
		ft_dprintf(2, RED"[Error]"NC" %s: %s\n", parts[2], strerror(errno));
	free(parts);
	return (0);
}

int	material_error(int error, char **parts)
{
	if (error == 0)
		ft_dprintf(2, RED"[Error]"NC" Invalid material name: `%s'\n", parts[1]);
	else if (error == 1)
		ft_dprintf(2, RED"[Error]"NC" material name too long: `%s'\n", parts[1])
		;
	else if (error == 2)
		ft_dprintf(2, RED"[Error]"NC" Name already used: `%s'\n", parts[1]);
	else if (error == 3)
		ft_dprintf(2, RED"[Error]"NC" Material not found: `%s'\n", parts[1]);
	free(parts);
	return (0);
}

int	material_item_error(int error, char *part)
{
	if (error == 0)
		ft_dprintf(2, RED"[Error]"NC" Material not found: `%s'\n", part);
	else if (error == 1)
		ft_dprintf(2, RED"[Error]"NC" Invalid material name: `%s'\n", part);
	else if (error == 2)
		ft_dprintf(2, RED"[Error]"NC" Invalid metallic value for: `%s'\n", part)
		;
	else if (error == 3)
		ft_dprintf(2, RED"[Error]"NC" Invalid roughness value for: `%s'\n",
			part);
	else if (error == 4)
		ft_dprintf(2, RED"[Error]"NC" Invalid ior value for: `%s'\n", part);
	else if (error == 5)
		ft_dprintf(2, RED"[Error]"NC" Invalid transmission value for: `%s'\n",
			part);
	else if (error == 6)
		ft_dprintf(2, RED"[Error]"NC" Invalid emission_strength value for: "
			"`%s'\n", part);
	else if (error == 7)
		ft_dprintf(2, RED"[Error]"NC" Invalid normal map for: `%s'\n", part);
	else if (error == 8)
		ft_dprintf(2, RED"[Error]"NC" Invalid ambient occlusion for: `%s'\n",
			part);
	else if (error == 9)
		ft_dprintf(2, RED"[Error]"NC" Invalid normal intensity: `%s'\n", part);
	return (0);
}

int	texture_item_error(int error, char *part)
{
	if (error == 0)
		ft_dprintf(2, RED"[Error]"NC" Texture not found: `%s'\n", part);
	else if (error == 1)
		ft_dprintf(2, RED"[Error]"NC" Invalid texture name: `%s'\n", part);
	return (0);
}
