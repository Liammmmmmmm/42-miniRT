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
	{
		fprintf(stderr, "[OIDN] Failed to create device\n");
		return (false);
	}
	oidnCommitDevice(denoiser->device);
	if (oidnGetDeviceError(denoiser->device, &error_message) != OIDN_ERROR_NONE)
	{
		fprintf(stderr, "[OIDN] Device error: %s\n", error_message);
		oidnReleaseDevice(denoiser->device);
		return (false);
	}
	denoiser->filter = oidnNewFilter(denoiser->device,
			denoiser->quality.filter_type);
	if (!denoiser->filter)
	{
		fprintf(stderr, "[OIDN] Failed to create filter\n");
		oidnReleaseDevice(denoiser->device);
		return (false);
	}
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
	denoiser->update_interval = 5;
	denoiser->frame_counter = 0;
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
	denoiser->use_depth = true;
#ifdef USE_OIDN
	if (!init_oidn_device(denoiser))
	{
		fprintf(stderr, "[OIDN] Failed to initialize, running without denoising\n");
		denoiser->available = false;
		denoiser->device = NULL;
		denoiser->filter = NULL;
	}
	else
	{
		printf("[OIDN] Initialized successfully (%dx%d)\n", width, height);
	}
#else
	printf("[Denoiser] Compiled without OIDN support\n");
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
	{
		fprintf(stderr, "[Denoiser] Cannot enable: OIDN not available\n");
		return ;
	}
	denoiser->enabled = enabled;
	printf("[Denoiser] %s\n", enabled ? "Enabled" : "Disabled");
}

void	denoiser_set_update_interval(t_denoiser *denoiser, int interval)
{
	if (!denoiser)
		return ;
	if (interval < 0)
		interval = 0;
	denoiser->update_interval = interval;
}

void	denoiser_print_info(t_denoiser *denoiser)
{
	if (!denoiser)
		return ;
	printf("\n=== Denoiser Information ===\n");
	printf("Available: %s\n", denoiser->available ? "Yes" : "No");
	printf("Enabled: %s\n", denoiser->enabled ? "Yes" : "No");
	printf("Resolution: %dx%d\n", denoiser->width, denoiser->height);
	printf("Update interval: %d frames\n", denoiser->update_interval);
#ifdef USE_OIDN
	if (denoiser->device)
		printf("OIDN Device: Active\n");
#else
	printf("OIDN: Not compiled\n");
#endif
	printf("Quality settings:\n");
	printf("  HDR: %s\n", denoiser->quality.hdr ? "Yes" : "No");
	printf("  Clean Aux: %s\n", denoiser->quality.clean_aux ? "Yes" : "No");
	printf("  Filter: %s\n", denoiser->quality.filter_type);
	printf("Auxiliary buffers:\n");
	printf("  Albedo: %s\n", denoiser->use_albedo ? "Yes" : "No");
	printf("  Normal: %s\n", denoiser->use_normal ? "Yes" : "No");
	printf("  Depth: %s\n", denoiser->use_depth ? "Yes" : "No");
	printf("===========================\n\n");
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
	printf("[Denoiser] Quality updated: HDR=%s, CleanAux=%s, Filter=%s\n",
		hdr ? "ON" : "OFF", clean_aux ? "ON" : "OFF",
		denoiser->quality.filter_type);
}