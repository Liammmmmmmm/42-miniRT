/*
** rt_runtime.h - buffer / texture bookkeeping shared by the CUDA backend
** (rt_cuda.cu) and the CPU emulation (rt_emu.cpp).
**
** The file including this one must provide:
**   void *rt_dev_alloc(size_t size);            NULL on failure
**   void  rt_dev_free(void *ptr);
**   int   rt_dev_upload(void *dst, const void *src, size_t size);   0 = ok
**   int   rt_dev_zero(void *dst, size_t size);                      0 = ok
**   int   rt_dev_launch(const t_rt_scene *sc, const t_cuda_frame *fr,
**             float *dev_out, float *host_out, size_t size);        0 = ok
*/

#ifndef RT_RUNTIME_H
# define RT_RUNTIME_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>

# define RT_SLOTS 32
# define RT_TEXS 16

struct RtBuf
{
	void	*dev;
	size_t	size;
};

struct RtTex
{
	float	*dev;
	int		w;
	int		h;
};

static RtBuf		g_buf[RT_SLOTS];
static RtTex		g_tex[RT_TEXS];
static unsigned int	g_unit_tex[3];
static int			g_unit;
static float		*g_result;
static size_t		g_result_size;

extern "C" unsigned int	rt_cuda_buffer_set(long slot, size_t size,
	const void *data)
{
	int	err;

	if (slot < 0 || slot >= RT_SLOTS)
		return (0);
	rt_dev_free(g_buf[slot].dev);
	g_buf[slot].dev = NULL;
	g_buf[slot].size = 0;
	if (size == 0)
		return ((unsigned int)slot + 1);
	g_buf[slot].dev = rt_dev_alloc(size);
	if (!g_buf[slot].dev)
	{
		fprintf(stderr, "[Error] miniRT: cannot allocate %zu bytes on the "
			"device (buffer %ld)\n", size, slot);
		return ((unsigned int)slot + 1);
	}
	if (data)
		err = rt_dev_upload(g_buf[slot].dev, data, size);
	else
		err = rt_dev_zero(g_buf[slot].dev, size);
	if (err)
	{
		rt_dev_free(g_buf[slot].dev);
		g_buf[slot].dev = NULL;
		return ((unsigned int)slot + 1);
	}
	g_buf[slot].size = size;
	return ((unsigned int)slot + 1);
}

extern "C" void	rt_cuda_buffer_free(unsigned int handle)
{
	if (handle == 0 || handle > RT_SLOTS)
		return ;
	rt_dev_free(g_buf[handle - 1].dev);
	g_buf[handle - 1].dev = NULL;
	g_buf[handle - 1].size = 0;
}

extern "C" unsigned int	rt_cuda_tex_create(const float *data, int width,
	int height)
{
	const size_t	size = sizeof(float) * (size_t)width * (size_t)height;
	int				i;

	i = 0;
	while (i < RT_TEXS && g_tex[i].dev)
		i++;
	if (i == RT_TEXS || !data || width <= 0 || height <= 0)
		return (0);
	g_tex[i].dev = (float *)rt_dev_alloc(size);
	if (!g_tex[i].dev)
		return (0);
	if (rt_dev_upload(g_tex[i].dev, data, size))
	{
		rt_dev_free(g_tex[i].dev);
		g_tex[i].dev = NULL;
		return (0);
	}
	g_tex[i].w = width;
	g_tex[i].h = height;
	return ((unsigned int)i + 1);
}

extern "C" void	rt_cuda_tex_delete(unsigned int id)
{
	int	i;

	if (id == 0 || id > RT_TEXS)
		return ;
	rt_dev_free(g_tex[id - 1].dev);
	g_tex[id - 1].dev = NULL;
	i = -1;
	while (++i < 3)
		if (g_unit_tex[i] == id)
			g_unit_tex[i] = 0;
}

extern "C" void	rt_cuda_tex_unit(int unit)
{
	if (unit >= 0 && unit < 3)
		g_unit = unit;
}

extern "C" void	rt_cuda_tex_bind(unsigned int id)
{
	if (id <= RT_TEXS)
		g_unit_tex[g_unit] = id;
}

extern "C" const float	*rt_cuda_result(void)
{
	return (g_result);
}

static const RtTex	*rt_unit(int unit)
{
	static const RtTex	none = {NULL, 0, 0};

	if (g_unit_tex[unit] == 0 || !g_tex[g_unit_tex[unit] - 1].dev)
		return (&none);
	return (&g_tex[g_unit_tex[unit] - 1]);
}

# define RT_BIND(field, count, slot, type) \
	sc->field = (const type *)g_buf[slot].dev; \
	sc->count = (uint32_t)(g_buf[slot].size / sizeof(type));

static void	rt_fill_scene(t_rt_scene *sc)
{
	memset(sc, 0, sizeof(*sc));
	RT_BIND(lights, lights_n, SSBO_BIND_LIGHTS, t_gpu_light)
	RT_BIND(materials, materials_n, SSBO_BIND_MATERIALS, t_gpu_material)
	RT_BIND(bvh, bvh_n, SSBO_BIND_BVH, t_gpu_bvh_node)
	RT_BIND(prim_ti, prim_ti_n, SSBO_BIND_PRIM_TYPE_INDICE, t_type_indice)
	RT_BIND(primitives, primitives_n, SSBO_BIND_PRIMITIVE, t_gpu_primitive)
	RT_BIND(planes, planes_n, SSBO_BIND_PLANE, t_gpu_plane)
	RT_BIND(hypers, hypers_n, SSBO_BIND_HYPERS, t_gpu_hyper)
	RT_BIND(triangles, triangles_n, SSBO_BIND_TRIANGLES, t_gpu_triangle)
	RT_BIND(checkers, checkers_n, SSBO_BIND_CHECKERS, t_gpu_checker)
	RT_BIND(tex_data, tex_data_n, SSBO_BIND_IMAGES, t_gpu_tex_data)
	RT_BIND(tex_ti, tex_ti_n, SSBO_BIND_TEX_TYPE_INDICE, t_type_indice)
	RT_BIND(photon_idx, photon_idx_n, SSBO_BIND_PHOTON_INDICE, uint32_t)
	RT_BIND(photons, photons_n, SSBO_BIND_PHOTON, t_gpu_photon)
	RT_BIND(cells, cells_n, SSBO_BIND_CELL, t_gpu_cell)
	sc->tex_stream = (const float *)g_buf[SSBO_BIND_IMAGES_STREAM].dev;
	sc->tex_stream_n = (uint32_t)(g_buf[SSBO_BIND_IMAGES_STREAM].size
			/ (sizeof(float) * 4));
	sc->cdf_ci = rt_unit(0)->dev;
	sc->cdf_ci_w = rt_unit(0)->w;
	sc->cdf_ci_h = rt_unit(0)->h;
	sc->pdf_joint = rt_unit(1)->dev;
	sc->pdf_w = rt_unit(1)->w;
	sc->pdf_h = rt_unit(1)->h;
	sc->cdf_mi = rt_unit(2)->dev;
	sc->cdf_mi_w = rt_unit(2)->w * rt_unit(2)->h;
}

extern "C" int	rt_cuda_render(const t_cuda_frame *frame)
{
	t_rt_scene		sc;
	t_cuda_frame	fr;
	size_t			need;

	fr = *frame;
	rt_fill_scene(&sc);
	if (fr.planes_am > sc.planes_n)
		fr.planes_am = sc.planes_n;
	if (fr.lights_am > sc.lights_n)
		fr.lights_am = sc.lights_n;
	if (fr.table_size < 0 || (uint32_t)fr.table_size > sc.cells_n)
		fr.table_size = 0;
	need = (size_t)fr.width * (size_t)fr.height * 4 * sizeof(float);
	if (need == 0)
		return (1);
	if (g_buf[RT_SLOT_OUTPUT].size < need)
		rt_cuda_buffer_set(RT_SLOT_OUTPUT, need, NULL);
	if (g_buf[RT_SLOT_OUTPUT].size < need)
		return (1);
	if (g_result_size < need)
	{
		free(g_result);
		g_result = (float *)malloc(need);
		g_result_size = need;
		if (!g_result)
			g_result_size = 0;
	}
	if (!g_result)
		return (1);
	return (rt_dev_launch(&sc, &fr, (float *)g_buf[RT_SLOT_OUTPUT].dev,
			g_result, need));
}

static void	rt_runtime_release(void)
{
	int	i;

	i = -1;
	while (++i < RT_SLOTS)
		rt_cuda_buffer_free((unsigned int)i + 1);
	i = -1;
	while (++i < RT_TEXS)
		rt_cuda_tex_delete((unsigned int)i + 1);
	free(g_result);
	g_result = NULL;
	g_result_size = 0;
}

#endif
