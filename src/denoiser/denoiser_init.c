#include "denoiser.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

bool	denoiser_is_available(void)
{
#ifdef USE_OIDN
	return (true);
#else
	return (false);
#endif
}

#ifdef USE_OIDN

static bool	init_oidn_device(t_denoiser *denoiser)
{
	const char	*error_message;

	denoiser->device = oidnNewDevice(OIDN_DEVICE_TYPE_DEFAULT);
	if (!denoiser->device)
		return (false);
	oidnCommitDevice(denoiser->device);
	if (oidnGetDeviceError(denoiser->device, &error_message) != OIDN_ERROR_NONE)
		return (false);
	denoiser->filter = oidnNewFilter(denoiser->device,
			denoiser->quality.filter_type);
	if (!denoiser->filter)
		return (false);
	return (true);
}

#endif

static void	init_default_quality(t_denoiser_quality *quality)
{
	quality->hdr = true;
	quality->srgb = false;
	quality->clean_aux = true;
	quality->max_memory_mb = 0;
	quality->filter_type = "RT";
}

t_denoiser	*denoiser_init(int width, int height)
{
	t_denoiser	*denoiser;
	size_t		buffer_size;

	denoiser = (t_denoiser *)malloc(sizeof(t_denoiser));
	if (!denoiser)
		return (NULL);
	memset(denoiser, 0, sizeof(t_denoiser));
	denoiser->width = width;
	denoiser->height = height;
	denoiser->enabled = false;
	denoiser->available = denoiser_is_available();
	init_default_quality(&denoiser->quality);
	buffer_size = width * height * 3 * sizeof(float);
	denoiser->input_buffer = (float *)malloc(buffer_size);
	denoiser->output_buffer = (float *)malloc(buffer_size);
	denoiser->albedo_buffer = (float *)malloc(buffer_size);
	denoiser->normal_buffer = (float *)malloc(buffer_size);
	denoiser->depth_buffer = (float *)malloc(width * height * sizeof(float));
	if (!denoiser->input_buffer || !denoiser->output_buffer
		|| !denoiser->albedo_buffer || !denoiser->normal_buffer
		|| !denoiser->depth_buffer)
	{
		denoiser_cleanup(denoiser);
		return (NULL);
	}
	memset(denoiser->input_buffer, 0, buffer_size);
	memset(denoiser->output_buffer, 0, buffer_size);
	memset(denoiser->albedo_buffer, 0, buffer_size);
	memset(denoiser->normal_buffer, 0, buffer_size);
	memset(denoiser->depth_buffer, 0, width * height * sizeof(float));
	denoiser->use_albedo = true;
	denoiser->use_normal = true;
	denoiser->use_depth = false;
#ifdef USE_OIDN
	if (!init_oidn_device(denoiser))
	{
		denoiser->available = false;
		denoiser->device = NULL;
		denoiser->filter = NULL;
	}
#endif
	return (denoiser);
}

void	denoiser_cleanup(t_denoiser *denoiser)
{
	if (!denoiser)
		return ;
#ifdef USE_OIDN
	if (denoiser->filter)
		oidnReleaseFilter(denoiser->filter);
	if (denoiser->device)
		oidnReleaseDevice(denoiser->device);
#endif
	if (denoiser->input_buffer)
		free(denoiser->input_buffer);
	if (denoiser->output_buffer)
		free(denoiser->output_buffer);
	if (denoiser->albedo_buffer)
		free(denoiser->albedo_buffer);
	if (denoiser->normal_buffer)
		free(denoiser->normal_buffer);
	if (denoiser->depth_buffer)
		free(denoiser->depth_buffer);
	free(denoiser);
}

void	denoiser_set_enabled(t_denoiser *denoiser, bool enabled)
{
	if (!denoiser)
		return ;
	if (enabled && !denoiser->available)
		return ;
	denoiser->enabled = enabled;
}

void	denoiser_set_quality(t_denoiser *denoiser, bool hdr,
	bool clean_aux, const char *filter_type)
{
	if (!denoiser)
		return ;
	denoiser->quality.hdr = hdr;
	denoiser->quality.clean_aux = clean_aux;
	if (filter_type)
		denoiser->quality.filter_type = (char *)filter_type;
}
