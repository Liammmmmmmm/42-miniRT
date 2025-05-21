/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_elements_utils6.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 11:48:01 by lilefebv          #+#    #+#             */
/*   Updated: 2025/05/19 12:38:40 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_objects	get_object_type(char *str)
{
	if (ft_strcmp(str, "C") == 0)
		return (CAMERA);
	if (ft_strcmp(str, "sp") == 0)
		return (SPHERE);
	if (ft_strcmp(str, "pl") == 0)
		return (PLANE);
	if (ft_strcmp(str, "cy") == 0)
		return (CYLINDER);
	if (ft_strcmp(str, "co") == 0)
		return (CONE);
	if (ft_strcmp(str, "hb") == 0)
		return (HYPERBOLOID);
	if (ft_strcmp(str, "cu") == 0)
		return (CUSTOM);
	if (ft_strcmp(str, "l") == 0 || ft_strcmp(str, "L") == 0)
		return (LIGHT);
	if (ft_strcmp(str, "tr") == 0)
		return (TRIANGLE);
	return (NULL_OBJ);
}
