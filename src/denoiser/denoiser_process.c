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
	oidnSetFilterInt(denoiser->filter, "quality", 0);
	oidnCommitFilter(denoiser->filter);
	oidnExecuteFilter(denoiser->filter);
	if (oidnGetDeviceError(denoiser->device, &error_message)
		!= OIDN_ERROR_NONE)
		return (false);
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
	return (force);
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
	}
#endif
	return (denoiser->input_buffer);
}

