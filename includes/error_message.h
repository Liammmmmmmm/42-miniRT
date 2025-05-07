/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_message.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 17:15:04 by madelvin          #+#    #+#             */
/*   Updated: 2025/05/07 13:49:17 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_MESSAGE_H
# define ERROR_MESSAGE_H

# define MSG_ERR_AMBIENT_LIGHT_RATIO "Invalid ambient light ratio. Expected a value between 0.0 and 1.0"
# define ERR_INVALID_FOV "Invalid FOV. Expected a value between 0 and 180"
# define ERR_INVALID_LIGHT_RATIO "Invalid light ratio. Expected a value between 0.0 and 1.0"
# define ERR_AMBIENT_LIGHT_FORMAT "Invalid ambient lighting. Expected: A "BLUE"<ratio> <r,g,b> [r,g,b|texture]"NC
# define ERR_CAMERA_FORMAT "Invalid camera. Expected: C "BLUE"<x,y,z> <x,y,z> <fov>"NC
# define ERR_LIGHT_FORMAT "Invalid light. Expected: L "BLUE"<x,y,z> <brightness ratio> <r,g,b>"NC
# define ERR_SPHERE_FORMAT "Invalid sphere. Expected: sp "BLUE"<x,y,z> <diameter> <r,g,b|material>"NC
# define ERR_PLANE_FORMAT "Invalid plane. Expected: pl "BLUE"<x,y,z> <x,y,z> <r,g,b|material>"NC
# define ERR_CYLINDER_FORMAT "Invalid cylinder. Expected: cy "BLUE"<x,y,z> <x,y,z> <diameter> <height> <r,g,b|material> [top_material] [bottom_material]"NC
# define ERR_HYPERBOLOID_FORMAT "Invalid hyperboloid. Expected: hb "BLUE"<x,y,z> <x,y,z> <height> <a> <b> <c> <shape> <r,g,b|material>"NC
# define ERR_CONE_FORMAT "Invalid cone. Expected: co "BLUE"<x,y,z> <x,y,z> <diameter> <height> <r,g,b|material> [top_material]"NC
# define ERR_MATERIAL_FORMAT "Invalid material. Expected: mat "BLUE"<name> <albedo> <metallic> <roughness> <ior> <transmission> <emission_strength> <emission_color> [tex_scale] [normal_map] [normal_intensity]"NC
# define ERR_TEXTURE_FORMAT "Invalid texture. Expected: tex "BLUE"<name> <filepath>"NC


#endif