#include "denoiser.h"
#include <stdlib.h>
#include <string.h>

float	*denoiser_buffer_copy(const float *src, int width, int height)
{
	float	*dst;
	size_t	size;

	if (!src)
		return (NULL);
	size = width * height * 3 * sizeof(float);
	dst = (float *)malloc(size);
	if (!dst)
		return (NULL);
	memcpy(dst, src, size);
	return (dst);
}

static float	clamp_float(float value, float min, float max)
{
	if (value < min)
		return (min);
	if (value > max)
		return (max);
	return (value);
}

static float	lerp(float a, float b, float t)
{
	return (a + (b - a) * t);
}

void	denoiser_lerp_buffers(float *output, const float *buffer_a,
	const float *buffer_b, float alpha, int width, int height)
{
	int		i;
	int		total;
	float	clamped_alpha;

	if (!output || !buffer_a || !buffer_b)
		return ;
	clamped_alpha = clamp_float(alpha, 0.0f, 1.0f);
	total = width * height * 3;
	i = 0;
	while (i < total)
	{
		output[i] = lerp(buffer_a[i], buffer_b[i], clamped_alpha);
		i++;
	}
}