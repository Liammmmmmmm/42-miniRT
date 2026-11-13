/*
** rt_emu.cpp - CPU emulation of the CUDA backend (make CUDA=emu).
** Runs the exact same kernel (rt_kernel.h) on the host with OpenMP. It is
** slow and only meant to debug / validate the kernel on a machine without an
** NVIDIA GPU.
*/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rt_cuda.h"

#define RT_DEV static inline
#include "rt_kernel.h"

static void	*rt_dev_alloc(size_t size)
{
	return (malloc(size));
}

static void	rt_dev_free(void *ptr)
{
	free(ptr);
}

static int	rt_dev_upload(void *dst, const void *src, size_t size)
{
	memcpy(dst, src, size);
	return (0);
}

static int	rt_dev_zero(void *dst, size_t size)
{
	memset(dst, 0, size);
	return (0);
}

static int	rt_dev_launch(const t_rt_scene *sc, const t_cuda_frame *fr,
	float *dev_out, float *host_out, size_t size)
{
	const int	height = (int)fr->height;
	int			y;

	#pragma omp parallel for schedule(dynamic, 4)
	for (y = 0; y < height; y++)
	{
		for (uint32_t x = 0; x < fr->width; x++)
		{
			const vec3 color = rt_pixel(sc, fr, x, (uint32_t)y);
			float *px = dev_out + ((size_t)y * fr->width + x) * 4;
			px[0] = color.x;
			px[1] = color.y;
			px[2] = color.z;
			px[3] = 0.0f;
		}
	}
	memcpy(host_out, dev_out, size);
	return (0);
}

#include "rt_runtime.h"

extern "C" int	rt_cuda_init(void)
{
	printf("CUDA backend running in CPU emulation (make CUDA=emu)\n");
	return (0);
}

extern "C" void	rt_cuda_shutdown(void)
{
	rt_runtime_release();
}
