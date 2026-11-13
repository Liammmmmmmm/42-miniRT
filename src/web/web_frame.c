/*
** web_frame.c - encode the accumulated render as a 24 bit BMP (every browser
** displays it and it needs no compression library).
*/

#include "minirt.h"
#include "web.h"

static void	put_le32(t_uchar *dst, uint32_t value)
{
	dst[0] = value & 0xFF;
	dst[1] = (value >> 8) & 0xFF;
	dst[2] = (value >> 16) & 0xFF;
	dst[3] = (value >> 24) & 0xFF;
}

static void	bmp_header(t_uchar *bmp, int w, int h, size_t size)
{
	ft_bzero(bmp, 54);
	bmp[0] = 'B';
	bmp[1] = 'M';
	put_le32(bmp + 2, size);
	put_le32(bmp + 10, 54);
	put_le32(bmp + 14, 40);
	put_le32(bmp + 18, w);
	put_le32(bmp + 22, h);
	bmp[26] = 1;
	bmp[28] = 24;
	put_le32(bmp + 34, size - 54);
}

static t_uchar	to_byte(double value, double gamma_corr)
{
	if (!(value > 0.0))
		return (0);
	value = pow(value, gamma_corr);
	if (value >= 1.0)
		return (255);
	return ((t_uchar)(value * 255.0));
}

/* Average of the scale x scale block, written as B G R */
static void	put_block(t_minirt *m, t_uchar *dst, t_point2 p, int scale)
{
	const t_fcolor	*src = m->screen.float_render;
	const double	gamma_corr = 1.0 / m->viewport.gamma;
	double			sum[3];
	double			div;
	t_point2		o;

	ft_bzero(sum, sizeof(sum));
	o.y = -1;
	while (++o.y < scale)
	{
		o.x = -1;
		while (++o.x < scale)
		{
			sum[0] += src[(p.y + o.y) * m->scene.render_width + p.x + o.x].r;
			sum[1] += src[(p.y + o.y) * m->scene.render_width + p.x + o.x].g;
			sum[2] += src[(p.y + o.y) * m->scene.render_width + p.x + o.x].b;
		}
	}
	div = (double)scale * scale;
	if (m->screen.last_sample_am > 0)
		div *= m->screen.last_sample_am;
	dst[0] = to_byte(sum[2] / div, gamma_corr);
	dst[1] = to_byte(sum[1] / div, gamma_corr);
	dst[2] = to_byte(sum[0] / div, gamma_corr);
}

/* scale = n: one output pixel per n x n render pixels. Caller frees */
t_uchar	*web_encode_bmp(t_minirt *minirt, int scale, size_t *size)
{
	t_uchar		*bmp;
	t_point2	p;
	int			w;
	int			h;
	int			stride;

	if (scale < 1)
		scale = 1;
	while (scale > 1 && (minirt->scene.render_width / scale < 1
			|| minirt->scene.render_height / scale < 1))
		scale--;
	w = minirt->scene.render_width / scale;
	h = minirt->scene.render_height / scale;
	stride = (w * 3 + 3) & ~3;
	*size = 54 + (size_t)stride * h;
	bmp = ft_calloc(*size, 1);
	if (!bmp || !minirt->screen.float_render)
		return (free(bmp), NULL);
	bmp_header(bmp, w, h, *size);
	p.y = -1;
	while (++p.y < h)
	{
		p.x = -1;
		while (++p.x < w)
			put_block(minirt, bmp + 54 + (size_t)stride * (h - 1 - p.y)
				+ p.x * 3, (t_point2){p.x * scale, p.y * scale}, scale);
	}
	return (bmp);
}
