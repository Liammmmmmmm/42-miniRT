/*
** rt_cuda.cu - CUDA backend of miniRT (make CUDA=1).
** The path tracer itself lives in rt_kernel.h, this file only holds the
** device runtime: init, memory, kernel launch and read back.
**
** Environment:
**   MINIRT_CUDA_DEVICE=<n>   GPU index to use (default 0)
*/

#include <cuda_runtime.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rt_cuda.h"

#define RT_DEV __device__ inline
#include "rt_kernel.h"

#define RT_BLOCK 16

static int	g_ready;

static int	rt_check(cudaError_t err, const char *what)
{
	if (err == cudaSuccess)
		return (0);
	fprintf(stderr, "[Error] miniRT CUDA: %s: %s\n", what,
		cudaGetErrorString(err));
	return (1);
}

static void	*rt_dev_alloc(size_t size)
{
	void	*ptr;

	ptr = NULL;
	if (rt_check(cudaMalloc(&ptr, size), "cudaMalloc"))
		return (NULL);
	return (ptr);
}

static void	rt_dev_free(void *ptr)
{
	if (ptr)
		cudaFree(ptr);
}

static int	rt_dev_upload(void *dst, const void *src, size_t size)
{
	return (rt_check(cudaMemcpy(dst, src, size, cudaMemcpyHostToDevice),
			"upload"));
}

static int	rt_dev_zero(void *dst, size_t size)
{
	return (rt_check(cudaMemset(dst, 0, size), "cudaMemset"));
}

__global__ void	rt_render_kernel(t_rt_scene sc, t_cuda_frame fr, float *out)
{
	const uint32_t	x = blockIdx.x * blockDim.x + threadIdx.x;
	const uint32_t	y = blockIdx.y * blockDim.y + threadIdx.y;
	vec3			color;
	float			*px;

	if (x >= fr.width || y >= fr.height)
		return ;
	color = rt_pixel(&sc, &fr, x, y);
	px = out + ((size_t)y * fr.width + x) * 4;
	px[0] = color.x;
	px[1] = color.y;
	px[2] = color.z;
	px[3] = 0.0f;
}

static int	rt_dev_launch(const t_rt_scene *sc, const t_cuda_frame *fr,
	float *dev_out, float *host_out, size_t size)
{
	const dim3	block(RT_BLOCK, RT_BLOCK);
	const dim3	grid((fr->width + RT_BLOCK - 1) / RT_BLOCK,
		(fr->height + RT_BLOCK - 1) / RT_BLOCK);

	if (!g_ready)
		return (1);
	rt_render_kernel<<<grid, block>>>(*sc, *fr, dev_out);
	if (rt_check(cudaGetLastError(), "kernel launch"))
		return (1);
	return (rt_check(cudaMemcpy(host_out, dev_out, size,
				cudaMemcpyDeviceToHost), "render"));
}

#include "rt_runtime.h"

extern "C" int	rt_cuda_init(void)
{
	cudaDeviceProp	prop;
	const char		*env = getenv("MINIRT_CUDA_DEVICE");
	int				count;
	int				device;

	count = 0;
	if (rt_check(cudaGetDeviceCount(&count), "cudaGetDeviceCount")
		|| count == 0)
	{
		fprintf(stderr, "[Error] miniRT CUDA: no CUDA device found\n");
		return (-1);
	}
	device = 0;
	if (env)
		device = atoi(env);
	if (device < 0 || device >= count)
	{
		fprintf(stderr, "[Error] miniRT CUDA: MINIRT_CUDA_DEVICE=%d but %d "
			"device(s) available\n", device, count);
		return (-1);
	}
	if (rt_check(cudaSetDevice(device), "cudaSetDevice")
		|| rt_check(cudaGetDeviceProperties(&prop, device), "device info"))
		return (-1);
	printf("CUDA device %d/%d: %s (%.1f GB, sm_%d%d)\n", device, count,
		prop.name, prop.totalGlobalMem / 1073741824.0, prop.major, prop.minor);
	g_ready = 1;
	return (0);
}

extern "C" void	rt_cuda_shutdown(void)
{
	if (!g_ready)
		return ;
	rt_runtime_release();
	cudaDeviceReset();
	g_ready = 0;
}
