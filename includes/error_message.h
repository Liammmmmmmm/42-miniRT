/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_message.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 17:15:04 by madelvin          #+#    #+#             */
/*   Updated: 2025/06/20 18:53:13 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_MESSAGE_H
# define ERROR_MESSAGE_H

# define MSG_ERR_AMBIENT_LIGHT_RATIO "Invalid ambient light ratio. Expected a value upper than 0.0"
# define ERR_INVALID_FOV "Invalid FOV. Expected a value between 0 and 180"
# define ERR_INVALID_LIGHT_RATIO "Invalid light ratio. Expected a value between 0.0 and 1.0"
# define ERR_AMBIENT_LIGHT_FORMAT "Invalid ambient lighting. Expected: A "BLUE"<ratio|exposure> <r,g,b|skybox>"NC
# define ERR_CAMERA_FORMAT "Invalid camera. Expected: C "BLUE"<x,y,z> <x,y,z> <fov>"NC
# define ERR_LIGHT_FORMAT "Invalid light. Expected: L or l "BLUE"<x,y,z> <brightness ratio> <r,g,b>"NC
# define ERR_DLIGHT_FORMAT "Invalid directional light. Expected: dl "BLUE"<x,y,z> <brightness ratio> <r,g,b>"NC
# define ERR_SPHERE_FORMAT "Invalid sphere. Expected: sp "BLUE"<x,y,z> <diameter> <r,g,b|material>"NC
# define ERR_PLANE_FORMAT "Invalid plane. Expected: pl "BLUE"<x,y,z> <x,y,z> <r,g,b|material>"NC
# define ERR_CYLINDER_FORMAT "Invalid cylinder. Expected: cy "BLUE"<x,y,z> <x,y,z> <diameter> <height> <r,g,b|material> [top_material] [bottom_material]"NC
# define ERR_HYPERBOLOID_FORMAT "Invalid hyperboloid. Expected: hb "BLUE"<x,y,z> <x,y,z> <height> <a> <b> <c> <shape> <r,g,b|material>"NC
# define ERR_CUSTOM_FORMAT "Invalid 3d obj. Expected: cu "BLUE"<x,y,z> <x,y,z> <x,y,z> <r,g,b|material>"NC
# define ERR_CONE_FORMAT "Invalid cone. Expected: co "BLUE"<x,y,z> <x,y,z> <diameter> <height> <r,g,b|material> [top_material]"NC
# define ERR_MATERIAL_FORMAT "Invalid material. Expected: mat "BLUE"<name> <albedo> <metallic> <roughness> <ior> <transmission> <emission_strength> <emission_color> [tex_scale] [normal_map] [normal_intensity]"NC
# define ERR_TEXTURE_FORMAT "Invalid texture. Expected: tex "BLUE"<name> <filepath>"NC
# define ERR_WINDOW_FORMAT "Invalid window size. Expected: W "BLUE"<width_win> <height_win> [width_render] [height_render]"NC
# define ERR_BMP_PALETTE RED "Error: palette too small for expected color count"NC
# define ERR_BMP_PALETTE_ALLOCATION RED "Error: unable to allocate memory for palette"NC
# define ERR_BMP_PALETTE_READ_COLOR RED "Error: unable to read color"NC
# define ERR_MAIN "Missing scene file. Usage: ./miniRT <scene.rt> [options]"

#endif