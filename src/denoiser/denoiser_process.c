#include "denoiser.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifdef USE_OIDN

static bool	execute_oidn_filter(t_denoiser *denoiser)
{
	const char	*error_message;

	oidnSetSharedFilterImage(denoiser->filter, "color",
		denoiser->input_buffer, OIDN_FORMAT_FLOAT3,
		denoiser->width, denoiser->height, 0, 0, 0);
	oidnSetSharedFilterImage(denoiser->filter, "output",
		denoiser->output_buffer, OIDN_FORMAT_FLOAT3,
		denoiser->width, denoiser->height, 0, 0, 0);
	if (denoiser->use_albedo && denoiser->albedo_buffer)
		oidnSetSharedFilterImage(denoiser->filter, "albedo",
			denoiser->albedo_buffer, OIDN_FORMAT_FLOAT3,
			denoiser->width, denoiser->height, 0, 0, 0);
	if (denoiser->use_normal && denoiser->normal_buffer)
		oidnSetSharedFilterImage(denoiser->filter, "normal",
			denoiser->normal_buffer, OIDN_FORMAT_FLOAT3,
			denoiser->width, denoiser->height, 0, 0, 0);
	if (denoiser->use_depth && denoiser->depth_buffer)
		oidnSetSharedFilterImage(denoiser->filter, "depth",
			denoiser->depth_buffer, OIDN_FORMAT_FLOAT,
			denoiser->width, denoiser->height, 0, 0, 0);
	oidnSetFilterBool(denoiser->filter, "hdr", denoiser->quality.hdr);
	oidnSetFilterBool(denoiser->filter, "srgb", denoiser->quality.srgb);
	oidnSetFilterBool(denoiser->filter, "cleanAux", denoiser->quality.clean_aux);
	if (denoiser->quality.max_memory_mb > 0)
		oidnSetFilterInt(denoiser->filter, "maxMemoryMB",
			denoiser->quality.max_memory_mb);
	oidnCommitFilter(denoiser->filter);
	oidnExecuteFilter(denoiser->filter);
	if (oidnGetDeviceError(denoiser->device, &error_message)
		!= OIDN_ERROR_NONE)
	{
		fprintf(stderr, "[OIDN] Filter error: %s\n", error_message);
		return (false);
	}
	return (true);
}

#endif

static void	copy_render_to_input(t_denoiser *denoiser,
	const float *render_buffer)
{
	size_t	size;

	size = denoiser->width * denoiser->height * 3 * sizeof(float);
	memcpy(denoiser->input_buffer, render_buffer, size);
}

static bool	should_denoise(t_denoiser *denoiser, bool force)
{
	if (!denoiser || !denoiser->enabled || !denoiser->available)
		return (false);
	if (force)
		return (true);
	denoiser->frame_counter++;
	if (denoiser->update_interval == 0)
		return (true);
	if (denoiser->frame_counter >= denoiser->update_interval)
	{
		denoiser->frame_counter = 0;
		return (true);
	}
	return (false);
}

float	*denoiser_process(t_denoiser *denoiser, const float *render_buffer,
	bool force)
{
	if (!denoiser || !render_buffer)
		return ((float *)render_buffer);
	if (!should_denoise(denoiser, force))
	{
		copy_render_to_input(denoiser, render_buffer);
		return (denoiser->input_buffer);
	}
	copy_render_to_input(denoiser, render_buffer);
#ifdef USE_OIDN
	if (denoiser->device && denoiser->filter)
	{
		if (execute_oidn_filter(denoiser))
			return (denoiser->output_buffer);
		fprintf(stderr, "[OIDN] Denoising failed, using noisy buffer\n");
	}
#endif
	return (denoiser->input_buffer);
}

void	denoiser_rgba_to_float(const unsigned int *rgba_buffer,
	float *rgb_float, int width, int height)
{
	int				i;
	int				total;
	unsigned int	pixel;

	total = width * height;
	i = 0;
	while (i < total)
	{
		pixel = rgba_buffer[i];
		rgb_float[i * 3 + 0] = ((pixel >> 16) & 0xFF) / 255.0f;
		rgb_float[i * 3 + 1] = ((pixel >> 8) & 0xFF) / 255.0f;
		rgb_float[i * 3 + 2] = (pixel & 0xFF) / 255.0f;
		i++;
	}
}

void	denoiser_float_to_rgba(const float *rgb_float,
	unsigned int *rgba_buffer, int width, int height)
{
	int				i;
	int				total;
	unsigned char	r;
	unsigned char	g;
	unsigned char	b;

	total = width * height;
	i = 0;
	while (i < total)
	{
		r = (unsigned char)(rgb_float[i * 3 + 0] * 255.0f);
		g = (unsigned char)(rgb_float[i * 3 + 1] * 255.0f);
		b = (unsigned char)(rgb_float[i * 3 + 2] * 255.0f);
		if (rgb_float[i * 3 + 0] > 1.0f)
			r = 255;
		if (rgb_float[i * 3 + 1] > 1.0f)
			g = 255;
		if (rgb_float[i * 3 + 2] > 1.0f)
			b = 255;
		rgba_buffer[i] = (0xFF << 24) | (r << 16) | (g << 8) | b;
		i++;
	}
}