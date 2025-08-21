/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_message.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 17:15:04 by madelvin          #+#    #+#             */
/*   Updated: 2025/08/21 20:47:11 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_MESSAGE_H
# define ERROR_MESSAGE_H

# define ERR_INVALID_FOV "Invalid FOV. Expected a value between 0 and 180"
# define ERR_ILR "Invalid light ratio. Expected a value between 0.0 and 1.0"
# define ERR_ILRADIUS "Invalid light radius. Expected a value greater than 0.0"
# define ER_ILSS "Invalid light shadow samples. Expected a value greater than 0"
# define ERR_AMBIENT_LF "Invalid ambient lighting. Expected: A "
# define ERR_AMBIENT_LFS "<ratio|exposure> <r,g,b|skybox>"
# define ERR_CF "Invalid camera. Expected: C "
# define ERR_CFS "<x,y,z> <x,y,z> <fov>"
# define ERR_LF "Invalid light. Expected: L or l "
# define ERR_LFS "<x,y,z> <brightness ratio> <r,g,b>"
# define ERR_DLF "Invalid directional light. Expected: dl "
# define ERR_DLFS "<x,y,z> <brightness ratio> <r,g,b>"
# define ERR_SF "Invalid sphere. Expected: sp "
# define ERR_SFS "<x,y,z> <diameter> <r,g,b|material>"
# define ERR_PF "Invalid plane. Expected: pl "
# define ERR_PFS "<x,y,z> <x,y,z> <r,g,b|material>"
# define ERR_CYF "Invalid cylinder. Expected: cy "
# define ERR_CYFS "<x,y,z> <x,y,z> <diameter> <height> <r,g,b|material> "
# define ERR_CFT "[top_material] [bottom_material]"
# define ERR_HF "Invalid hyperboloid. Expected: hb "
# define ERR_HFS  "<x,y,z> <x,y,z> "
# define ERR_HFT  "<height> <a> <b> <c> <shape> <r,g,b|material>"
# define ERR_CUF "Invalid 3d obj. Expected: cu "
# define ERR_CUFS "<x,y,z> <x,y,z> <x,y,z> <r,g,b|material>"
# define ERR_COF "Invalid cone. Expected: co "
# define ERR_COFS "<x,y,z> <x,y,z> <diameter> "
# define ERR_COFT "<height> <r,g,b|material> [top_material]"
# define ERR_BMP_P "Error: palette too small for expected color count"
# define ERR_BMP_PATION "Error: unable to allocate memory for palette"
# define ERR_BMP_P_READ_COLOR "Error: unable to read color"
# define ERR_MAIN "Missing scene file. Usage: ./miniRT <scene.rt> [options]"
# define WARN_CAUSTIC_NO_L "No lights found: skipping photon map generation."
# define ERR_C_KD_TREE_NO_B "KD Tree not build: caustics will be disabled."
# define HEAT_MBE "Failed to allocate memory for depth buffer | no heat map"

#endif