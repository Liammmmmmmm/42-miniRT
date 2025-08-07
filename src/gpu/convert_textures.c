/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert_textures.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 19:28:11 by lilefebv          #+#    #+#             */
/*   Updated: 2025/08/07 16:29:12 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "gpu.h"

void	convert_checker(t_scene *scene, t_gpu_textures *gpu, int *check_i,
	int i)
{
	gpu->textures_types_indices[i].indice = *check_i;
	gpu->checkers[*check_i].scale = scene->textures[i].checker.scale;
	color_to_float3(&scene->textures[i].checker.c1, gpu->checkers[*check_i].c1);
	color_to_float3(&scene->textures[i].checker.c2, gpu->checkers[*check_i].c2);
	(*check_i)++;
}

void	convert_image(t_scene *scene, t_gpu_textures *gpu, t_point2 i,
	uint32_t *image_offset)
{
	t_uint	y;
	t_uint	tpx;

	if (scene->textures[i.x].img.rgba == NULL)
		return ;
	gpu->images[i.y].height = scene->textures[i.x].img.height;
	gpu->images[i.y].width = scene->textures[i.x].img.width;
	gpu->images[i.y].offset = *image_offset;
	y = 0;
	tpx = scene->textures[i.x].img.height * scene->textures[i.x].img.width;
	while (y < tpx)
	{
		gpu->images_stream[*image_offset * 4 + y * 4]
			= scene->textures[i.x].img.rgba[y].r / 255.0;
		gpu->images_stream[*image_offset * 4 + y * 4 + 1]
			= scene->textures[i.x].img.rgba[y].g / 255.0;
		gpu->images_stream[*image_offset * 4 + y * 4 + 2]
			= scene->textures[i.x].img.rgba[y].b / 255.0;
		gpu->images_stream[*image_offset * 4 + y * 4 + 3]
			= scene->textures[i.x].img.rgba[y].a / 255.0;
		y++;
	}
	*image_offset += tpx;
}

void	convert_hdr(t_scene *scene, t_gpu_textures *gpu, t_point2 i,
	uint32_t *image_offset)
{
	t_uint	y;
	t_uint	tpx;

	if (scene->textures[i.x].hdr.pixels == NULL)
		return ;
	gpu->images[i.y].height = scene->textures[i.x].hdr.height;
	gpu->images[i.y].width = scene->textures[i.x].hdr.width;
	gpu->images[i.y].offset = *image_offset;
	gpu->images[i.y].exposure = scene->textures[i.x].hdr.exposure;
	y = 0;
	tpx = scene->textures[i.x].hdr.height * scene->textures[i.x].hdr.width;
	while (y < tpx)
	{
		gpu->images_stream[*image_offset * 4 + y * 4]
			= scene->textures[i.x].hdr.pixels[y].r;
		gpu->images_stream[*image_offset * 4 + y * 4 + 1]
			= scene->textures[i.x].hdr.pixels[y].g;
		gpu->images_stream[*image_offset * 4 + y * 4 + 2]
			= scene->textures[i.x].hdr.pixels[y].b;
		gpu->images_stream[*image_offset * 4 + y * 4 + 3]
			= scene->textures[i.x].hdr.pixels[y].e;
		y++;
	}
	*image_offset += tpx;
}

void	convert_textures(t_scene *scene, t_gpu_textures *gpu)
{
	int			i;
	int			img_i;
	int			check_i;
	uint32_t	image_offset;

	i = -1;
	img_i = 0;
	check_i = 0;
	image_offset = 0;
	while (++i < scene->tex_amount)
	{
		gpu->textures_types_indices[i].type = scene->textures[i].type;
		if (scene->textures[i].type == CHECKER_GLOBAL
			|| scene->textures[i].type == CHECKER_LOCAL)
			convert_checker(scene, gpu, &check_i, i);
		if (scene->textures[i].type == HDR || scene->textures[i].type == IMAGE)
		{
			gpu->textures_types_indices[i].indice = img_i;
			if (scene->textures[i].type == IMAGE)
				convert_image(scene, gpu, (t_point2){i, img_i}, &image_offset);
			else
				convert_hdr(scene, gpu, (t_point2){i, img_i}, &image_offset);
			img_i++;
		}
	}
}

int	convert_textures_init(t_scene *scene, t_gpu_textures *gtx)
{
	count_tex(scene, gtx);
	gtx->checkers = ft_calloc(gtx->checkers_am, sizeof(t_gpu_checker));
	gtx->images = ft_calloc(gtx->images_am, sizeof(t_gpu_tex_data));
	gtx->images_stream = ft_calloc(gtx->total_pixel_images * 4, sizeof(float));
	gtx->textures_types_indices = ft_calloc(scene->tex_amount,
			sizeof(t_type_indice));
	if (!gtx->checkers || !gtx->images || !gtx->images_stream
		|| !gtx->textures_types_indices)
	{
		free(gtx->checkers);
		free(gtx->images);
		free(gtx->images_stream);
		free(gtx->textures_types_indices);
		return (print_errorm1("Malloc failed, textures not transmitted."));
	}
	convert_textures(scene, gtx);
	create_ssbo(&gtx->checkers_ssbo, sizeof(t_gpu_checker) * gtx->checkers_am,
		gtx->checkers, SSBO_BIND_CHECKERS);
	create_ssbo(&gtx->images_ssbo, sizeof(t_gpu_tex_data) * gtx->images_am,
		gtx->images, SSBO_BIND_IMAGES);
	create_ssbo(&gtx->images_stream_ssbo,
		sizeof(float) * gtx->total_pixel_images * 4, gtx->images_stream,
		SSBO_BIND_IMAGES_STREAM);
	create_ssbo(&gtx->textures_types_indices_ssbo,
		sizeof(t_type_indice) * scene->tex_amount, gtx->textures_types_indices,
		SSBO_BIND_TEX_TYPE_INDICE);
	free(gtx->checkers);
	free(gtx->images);
	free(gtx->images_stream);
	free(gtx->textures_types_indices);
	gtx->checkers = NULL;
	gtx->images = NULL;
	gtx->images_stream = NULL;
	gtx->textures_types_indices = NULL;
	return (0);
}
