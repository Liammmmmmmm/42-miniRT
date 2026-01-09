#include "minirt.h"
#include <stdlib.h>
#include <string.h>

static void	fcolor_to_rgb_float(t_minirt *minirt, float *rgb_buffer)
{
	int			i;
	int			total;
	int			divide;
	t_fcolor	*src;

	divide = minirt->screen.last_sample_am;
	if (divide == 0)
		divide = 1;
	total = minirt->scene.render_width * minirt->scene.render_height;
	src = minirt->screen.float_render;
	i = 0;
	while (i < total)
	{
		rgb_buffer[i * 3 + 0] = (float)(src[i].r / divide);
		rgb_buffer[i * 3 + 1] = (float)(src[i].g / divide);
		rgb_buffer[i * 3 + 2] = (float)(src[i].b / divide);
		i++;
	}
}

static void	fcolor_to_rgb_float_from_display(t_minirt *minirt, float *rgb_buffer)
{
	int			i;
	int			total;
	int			divide;
	t_fcolor	*src;

	divide = minirt->screen.last_sample_am;
	if (divide == 0)
		divide = 1;
	total = minirt->scene.render_width * minirt->scene.render_height;
	src = minirt->screen.float_render_backup;
	i = 0;
	while (i < total)
	{
		rgb_buffer[i * 3 + 0] = (float)(src[i].r / divide);
		rgb_buffer[i * 3 + 1] = (float)(src[i].g / divide);
		rgb_buffer[i * 3 + 2] = (float)(src[i].b / divide);
		i++;
	}
}

static void	copy_albedo_buffer(t_minirt *minirt)
{
	int			i;
	int			total;
	int			divide;
	t_fcolor	*src;
	float		*dst;

	if (!minirt->denoiser || !minirt->denoiser->albedo_buffer)
		return ;
	divide = minirt->screen.last_sample_am;
	if (divide == 0)
		divide = 1;
	total = minirt->scene.render_width * minirt->scene.render_height;
	src = minirt->screen.albedo_buffer;
	dst = minirt->denoiser->albedo_buffer;
	i = 0;
	while (i < total)
	{
		dst[i * 3 + 0] = (float)(src[i].r / divide);
		dst[i * 3 + 1] = (float)(src[i].g / divide);
		dst[i * 3 + 2] = (float)(src[i].b / divide);
		i++;
	}
}

static void	copy_normal_buffer(t_minirt *minirt)
{
	int			i;
	int			total;
	int			divide;
	t_fcolor	*src;
	float		*dst;

	if (!minirt->denoiser || !minirt->denoiser->normal_buffer)
		return ;
	divide = minirt->screen.last_sample_am;
	if (divide == 0)
		divide = 1;
	total = minirt->scene.render_width * minirt->scene.render_height;
	src = minirt->screen.normal_buffer;
	dst = minirt->denoiser->normal_buffer;
	i = 0;
	while (i < total)
	{
		dst[i * 3 + 0] = (float)(src[i].r / divide);
		dst[i * 3 + 1] = (float)(src[i].g / divide);
		dst[i * 3 + 2] = (float)(src[i].b / divide);
		i++;
	}
}

static void	copy_depth_buffer(t_minirt *minirt)
{
	int		i;
	int		total;
	int		divide;
	float	*src;
	float	*dst;

	if (!minirt->denoiser || !minirt->denoiser->depth_buffer)
		return ;
	divide = minirt->screen.last_sample_am;
	if (divide == 0)
		divide = 1;
	total = minirt->scene.render_width * minirt->scene.render_height;
	src = minirt->screen.depth_buffer_denoise;
	dst = minirt->denoiser->depth_buffer;
	i = 0;
	while (i < total)
	{
		dst[i] = src[i] / divide;
		i++;
	}
}

static void	rgb_float_to_fcolor(t_minirt *minirt, const float *rgb_buffer)
{
	int			i;
	int			total;
	t_fcolor	*dst;
	int			divide;

	divide = minirt->screen.last_sample_am;
	if (divide == 0)
		divide = 1;
	total = minirt->scene.render_width * minirt->scene.render_height;
	dst = minirt->screen.float_render;
	i = 0;
	while (i < total)
	{
		dst[i].r = (double)(rgb_buffer[i * 3 + 0] * divide);
		dst[i].g = (double)(rgb_buffer[i * 3 + 1] * divide);
		dst[i].b = (double)(rgb_buffer[i * 3 + 2] * divide);
		i++;
	}
}

static void	rgb_float_to_display_buffer(t_minirt *minirt, const float *rgb_buffer)
{
	int			i;
	int			total;
	t_fcolor	*dst;
	int			divide;

	divide = minirt->screen.last_sample_am;
	if (divide == 0)
		divide = 1;
	total = minirt->scene.render_width * minirt->scene.render_height;
	dst = minirt->screen.float_render_backup;
	i = 0;
	while (i < total)
	{
		dst[i].r = (double)(rgb_buffer[i * 3 + 0] * divide);
		dst[i].g = (double)(rgb_buffer[i * 3 + 1] * divide);
		dst[i].b = (double)(rgb_buffer[i * 3 + 2] * divide);
		i++;
	}
}

static void	backup_float_render(t_minirt *minirt)
{
	int	total;

	total = minirt->scene.render_width * minirt->scene.render_height;
	memcpy(minirt->screen.float_render_backup, minirt->screen.float_render,
		total * sizeof(t_fcolor));
}

static void	restore_float_render(t_minirt *minirt)
{
	int	total;

	total = minirt->scene.render_width * minirt->scene.render_height;
	memcpy(minirt->screen.float_render, minirt->screen.float_render_backup,
		total * sizeof(t_fcolor));
}

bool	apply_denoising(t_minirt *minirt)
{
	float	*input_rgb;
	float	*denoised_rgb;
	bool	force;

	if (!minirt->denoiser || !minirt->denoiser->enabled
		|| !minirt->denoiser->available)
		return (false);
	if (!minirt->show_denoised)
	{
		restore_float_render(minirt);
		return (false);
	}
	backup_float_render(minirt);
	input_rgb = malloc(minirt->scene.render_width
			* minirt->scene.render_height * 3 * sizeof(float));
	if (!input_rgb)
		return (false);
	fcolor_to_rgb_float(minirt, input_rgb);
	copy_albedo_buffer(minirt);
	copy_normal_buffer(minirt);
	copy_depth_buffer(minirt);
	force = (minirt->screen.sample >= minirt->screen.spp)
		|| (minirt->screen.sample % 100 == 0);
	denoised_rgb = denoiser_process(minirt->denoiser, input_rgb, force);
	if (denoised_rgb && denoised_rgb != input_rgb)
		rgb_float_to_fcolor(minirt, denoised_rgb);
	free(input_rgb);
	return (denoised_rgb && denoised_rgb != input_rgb);
}

bool	apply_denoising_forced(t_minirt *minirt)
{
	float	*input_rgb;
	float	*denoised_rgb;

	if (!minirt->denoiser->available)
		return (false);
	input_rgb = malloc(minirt->scene.render_width
			* minirt->scene.render_height * 3 * sizeof(float));
	if (!input_rgb)
		return (false);
	fcolor_to_rgb_float_from_display(minirt, input_rgb);
	copy_albedo_buffer(minirt);
	copy_normal_buffer(minirt);
	copy_depth_buffer(minirt);
	denoised_rgb = denoiser_process(minirt->denoiser, input_rgb, true);
	if (denoised_rgb && denoised_rgb != input_rgb)
		rgb_float_to_display_buffer(minirt, denoised_rgb);
	free(input_rgb);
	return (denoised_rgb && denoised_rgb != input_rgb);
}
