/*
** rt_kernel.h - path tracing kernel, port of src/shaders/ (GLSL compute) to
** plain C++ that compiles both as CUDA device code (rt_cuda.cu) and as host
** code (rt_emu.cpp, CPU emulation used to debug without an NVIDIA GPU).
**
** The structure and the function names follow the .comp files one to one so
** the two implementations can be diffed. Deliberate differences with the GLSL:
**  - every buffer access is bound checked (an out of range read is harmless in
**    OpenGL but kills a CUDA context),
**  - texture coordinates are clamped to the image instead of being undefined
**    when u/v fall outside [0, 1],
**  - image textures are looked up through tex_ti[].indice,
**  - the BVH traversal skips nodes that are farther than the closest hit.
*/

#ifndef RT_KERNEL_H
# define RT_KERNEL_H

# include <stdint.h>
# include <math.h>

# ifndef RT_DEV
#  define RT_DEV static inline
# endif

# define RT_PI 3.14159265358979323846f
# define RT_IT_MAX 1000.0f
# define RT_IT_MIN 0.001f
# define RT_SPEC_STRENGTH 1.0f
# define RT_SHINE 64.0f
# define RT_BVH_STACK 128
# define RT_KNN_K 10

typedef struct s_rt_scene
{
	const t_gpu_light		*lights;
	const t_gpu_material	*materials;
	const t_gpu_bvh_node	*bvh;
	const t_type_indice		*prim_ti;
	const t_gpu_primitive	*primitives;
	const t_gpu_plane		*planes;
	const t_gpu_hyper		*hypers;
	const t_gpu_triangle	*triangles;
	const t_gpu_checker		*checkers;
	const t_gpu_tex_data	*tex_data;
	const float				*tex_stream;
	const t_type_indice		*tex_ti;
	const uint32_t			*photon_idx;
	const t_gpu_photon		*photons;
	const t_gpu_cell		*cells;
	const float				*cdf_ci;
	const float				*pdf_joint;
	const float				*cdf_mi;
	uint32_t				lights_n;
	uint32_t				materials_n;
	uint32_t				bvh_n;
	uint32_t				prim_ti_n;
	uint32_t				primitives_n;
	uint32_t				planes_n;
	uint32_t				hypers_n;
	uint32_t				triangles_n;
	uint32_t				checkers_n;
	uint32_t				tex_data_n;
	uint32_t				tex_stream_n;
	uint32_t				tex_ti_n;
	uint32_t				photon_idx_n;
	uint32_t				photons_n;
	uint32_t				cells_n;
	int						cdf_ci_w;
	int						cdf_ci_h;
	int						pdf_w;
	int						pdf_h;
	int						cdf_mi_w;
}	t_rt_scene;

/* ------------------------------------------------------------------ vec3 */

struct vec3
{
	float	x;
	float	y;
	float	z;
};

struct vec4
{
	float	x;
	float	y;
	float	z;
	float	w;
};

RT_DEV vec3	v3(float x, float y, float z)
{
	vec3	r;

	r.x = x;
	r.y = y;
	r.z = z;
	return (r);
}

RT_DEV vec3	v3s(float s) { return (v3(s, s, s)); }
RT_DEV vec3	v3p(const float *p) { return (v3(p[0], p[1], p[2])); }
RT_DEV vec3	operator+(vec3 a, vec3 b) { return (v3(a.x + b.x, a.y + b.y, a.z + b.z)); }
RT_DEV vec3	operator-(vec3 a, vec3 b) { return (v3(a.x - b.x, a.y - b.y, a.z - b.z)); }
RT_DEV vec3	operator*(vec3 a, vec3 b) { return (v3(a.x * b.x, a.y * b.y, a.z * b.z)); }
RT_DEV vec3	operator/(vec3 a, vec3 b) { return (v3(a.x / b.x, a.y / b.y, a.z / b.z)); }
RT_DEV vec3	operator*(vec3 a, float s) { return (v3(a.x * s, a.y * s, a.z * s)); }
RT_DEV vec3	operator*(float s, vec3 a) { return (v3(a.x * s, a.y * s, a.z * s)); }
RT_DEV vec3	operator/(vec3 a, float s) { return (v3(a.x / s, a.y / s, a.z / s)); }
RT_DEV vec3	operator-(vec3 a) { return (v3(-a.x, -a.y, -a.z)); }
RT_DEV vec3	&operator+=(vec3 &a, vec3 b) { a = a + b; return (a); }
RT_DEV vec3	&operator*=(vec3 &a, vec3 b) { a = a * b; return (a); }
RT_DEV vec3	&operator*=(vec3 &a, float s) { a = a * s; return (a); }
RT_DEV float	dot(vec3 a, vec3 b) { return (a.x * b.x + a.y * b.y + a.z * b.z); }
RT_DEV float	length(vec3 a) { return (sqrtf(dot(a, a))); }
RT_DEV vec3	normalize(vec3 a) { return (a / length(a)); }

RT_DEV vec3	cross(vec3 a, vec3 b)
{
	return (v3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z,
			a.x * b.y - a.y * b.x));
}

RT_DEV float	clampf(float x, float lo, float hi)
{
	return (fminf(fmaxf(x, lo), hi));
}

RT_DEV vec3	clampv(vec3 a, float lo, float hi)
{
	return (v3(clampf(a.x, lo, hi), clampf(a.y, lo, hi), clampf(a.z, lo, hi)));
}

RT_DEV vec3	mixv(vec3 a, vec3 b, float t) { return (a * (1.0f - t) + b * t); }
RT_DEV float	fractf(float x) { return (x - floorf(x)); }

/* --------------------------------------------------------------- structs */

struct MaterialValues
{
	float	metallic;
	float	roughness;
	float	ior;
	float	transmission;
	float	ao;
	float	emission_strength;
	float	scale;
	float	normal_intensity;
	vec3	emission_color;
};

struct hit_record
{
	vec3			point;
	float			t;
	vec3			normal;
	int				material_id;
	vec3			color;
	bool			front_face;
	uint32_t		object_index;
	uint32_t		object_type;
	float			u;
	float			v;
	int				part_id;
	MaterialValues	mat_val;
};

struct quadratic
{
	float	a;
	float	b;
	float	c;
	float	disc;
	float	t0;
	float	t1;
	float	t_hit;
	float	dd;
	float	oo;
};

struct KNNResult
{
	vec3	power;
	float	dist_sq;
};

struct KNNSearch
{
	KNNResult	results[RT_KNN_K];
	uint32_t	count;
	uint32_t	k;
	float		farthest_dist_sq;
};

/* Per pixel state: scene, uniforms and the xorshift seed (g_seed in GLSL) */
struct Ctx
{
	const t_rt_scene	*sc;
	const t_cuda_frame	*fr;
	uint32_t			seed;
};

RT_DEV hit_record	get_default_hit_record(void)
{
	hit_record	h;

	h.point = v3s(0.0f);
	h.t = 10000000.0f;
	h.normal = v3s(0.0f);
	h.material_id = -1;
	h.color = v3s(0.0f);
	h.front_face = false;
	h.object_index = 0;
	h.object_type = 0;
	h.u = 0.0f;
	h.v = 0.0f;
	h.part_id = 0;
	h.mat_val.metallic = 0.0f;
	h.mat_val.roughness = 0.0f;
	h.mat_val.ior = 0.0f;
	h.mat_val.transmission = 0.0f;
	h.mat_val.ao = 0.0f;
	h.mat_val.emission_strength = 0.0f;
	h.mat_val.scale = 0.0f;
	h.mat_val.normal_intensity = 0.0f;
	h.mat_val.emission_color = v3s(0.0f);
	return (h);
}

RT_DEV quadratic	get_default_quadratic(void)
{
	quadratic	q;

	q.a = 0.0f;
	q.b = 0.0f;
	q.c = 0.0f;
	q.disc = 0.0f;
	q.t0 = 0.0f;
	q.t1 = 0.0f;
	q.t_hit = 0.0f;
	q.dd = 0.0f;
	q.oo = 0.0f;
	return (q);
}

/* -------------------------------------------------- utils/random.comp */

RT_DEV uint32_t	rand_xorshift(Ctx &c)
{
	c.seed ^= c.seed << 13;
	c.seed ^= c.seed >> 17;
	c.seed ^= c.seed << 5;
	return (c.seed);
}

RT_DEV float	rt_rand(Ctx &c)
{
	return ((float)rand_xorshift(c) / 4294967296.0f);
}

RT_DEV float	random_double_neg(Ctx &c)
{
	return ((float)rand_xorshift(c) / 2147483648.0f - 1.0f);
}

RT_DEV vec3	random_in_unit_sphere(Ctx &c)
{
	vec3	p;

	p.x = random_double_neg(c);
	p.y = random_double_neg(c);
	p.z = random_double_neg(c);
	if (dot(p, p) >= 1.0f)
		p = normalize(p) * powf(rt_rand(c), 1.0f / 3.0f);
	return (p);
}

/* ------------------------------------------------- utils/defocus.comp */

RT_DEV vec3	defocus_disk_sample(Ctx &c)
{
	const float	angle = rt_rand(c) * 2.0f * RT_PI;
	const float	radius = sqrtf(rt_rand(c));

	return (v3p(c.fr->camera.position)
		+ v3p(c.fr->viewport.defocus_disk_u) * (cosf(angle) * radius)
		+ v3p(c.fr->viewport.defocus_disk_v) * (sinf(angle) * radius));
}

/* ------------------------------------ path_trace/importance_sampling.comp */

/* texture() on a GL_NEAREST / GL_CLAMP_TO_EDGE R32F texture */
RT_DEV float	tex_nearest(const float *data, int w, int h, float u, float v)
{
	int	x;
	int	y;

	if (!data || w <= 0 || h <= 0)
		return (0.0f);
	x = (int)fminf(fmaxf(floorf(u * (float)w), 0.0f), (float)(w - 1));
	y = (int)fminf(fmaxf(floorf(v * (float)h), 0.0f), (float)(h - 1));
	return (data[(size_t)y * (size_t)w + (size_t)x]);
}

RT_DEV void	calc_inverse_transform_sampling_uv(Ctx &c, float &nx, float &ny)
{
	const float	rx = rt_rand(c);
	const float	ry = rt_rand(c);

	ny = tex_nearest(c.sc->cdf_mi, c.sc->cdf_mi_w, 1, rx, 0.0f);
	nx = tex_nearest(c.sc->cdf_ci, c.sc->cdf_ci_w, c.sc->cdf_ci_h, ry, ny);
}

RT_DEV vec3	calc_inverse_transform_sampling_dir(float u, float v)
{
	const float	theta = (v - 0.5f) * RT_PI;
	const float	phi = (0.5f - u) * 2.0f * RT_PI;

	return (v3(cosf(theta) * cosf(phi), sinf(theta), cosf(theta) * sinf(phi)));
}

/* ------------------------------------ path_trace/textures/ */

/* Fetch one RGBA texel. Returns false when the image is missing (size 0). */
RT_DEV bool	fetch_texel(const Ctx &c, int tex_index, float u, float v,
	vec4 &px, float &exposure)
{
	const t_rt_scene	*s = c.sc;
	uint32_t			img;
	uint32_t			x;
	uint32_t			y;
	size_t				idx;

	px.x = 0.0f;
	px.y = 0.0f;
	px.z = 0.0f;
	px.w = 0.0f;
	exposure = 0.0f;
	if (tex_index < 0 || (uint32_t)tex_index >= s->tex_ti_n)
		return (false);
	img = s->tex_ti[tex_index].indice;
	if (img >= s->tex_data_n || s->tex_data[img].width == 0
		|| s->tex_data[img].height == 0)
		return (false);
	exposure = s->tex_data[img].exposure;
	x = (uint32_t)clampf(u * (float)(s->tex_data[img].width - 1), 0.0f,
			(float)(s->tex_data[img].width - 1));
	y = (uint32_t)clampf(v * (float)(s->tex_data[img].height - 1), 0.0f,
			(float)(s->tex_data[img].height - 1));
	idx = (size_t)s->tex_data[img].offset
		+ (size_t)y * s->tex_data[img].width + x;
	if (idx >= s->tex_stream_n)
		return (true);
	px.x = s->tex_stream[idx * 4];
	px.y = s->tex_stream[idx * 4 + 1];
	px.z = s->tex_stream[idx * 4 + 2];
	px.w = s->tex_stream[idx * 4 + 3];
	return (true);
}

RT_DEV vec4	sample_texture(const Ctx &c, int tex_index, float u, float v)
{
	vec4	px;
	float	exposure;

	if (!fetch_texel(c, tex_index, u, v, px, exposure))
	{
		px.x = 0.0f;
		px.y = 0.0f;
		px.z = 0.0f;
		px.w = 0.0f;
		if (((int)floorf(u * 4.0f) + (int)floorf(v * 4.0f)) % 2 != 0)
		{
			px.x = 0.95294117647f;
			px.z = 0.95294117647f;
		}
	}
	return (px);
}

/* exposure_add: added to the exposure stored with the texture */
RT_DEV vec3	sample_hdr_texture(const Ctx &c, int tex_index, float u, float v,
	float exposure_add)
{
	vec4	px;
	float	exposure;
	float	scale;

	if (!fetch_texel(c, tex_index, u, v, px, exposure))
	{
		if (((int)floorf(u * 10.0f) + (int)floorf(v * 10.0f)) % 2 == 0)
			return (v3s(0.0f));
		return (v3(0.95294117647f, 0.0f, 0.95294117647f));
	}
	scale = ldexpf(1.0f, (int)px.w - 128) * powf(2.0f, exposure + exposure_add)
		/ 255.0f;
	return (v3(px.x * scale, px.y * scale, px.z * scale));
}

RT_DEV vec3	get_solid_texture(vec3 p, vec3 c1, vec3 c2, float scale)
{
	int	a;
	int	b;
	int	cc;

	p = p * scale;
	a = (int)floorf(p.x);
	b = (int)floorf(p.y + 0.5f);
	cc = (int)floorf(p.z);
	if ((a + b + cc) % 2 == 0)
		return (c1);
	return (c2);
}

RT_DEV vec3	get_tex_color(const Ctx &c, int tex_index, float u, float v,
	vec3 hit_point)
{
	const t_rt_scene	*s = c.sc;
	uint32_t			type;
	uint32_t			ci;

	if (tex_index < 0 || (uint32_t)tex_index >= s->tex_ti_n)
		return (v3s(0.0f));
	type = s->tex_ti[tex_index].type;
	if (type == 0)
	{
		const vec4 px = sample_texture(c, tex_index, u, v);
		return (v3(px.x, px.y, px.z));
	}
	if (type == 1)
		return (sample_hdr_texture(c, tex_index, u, v, 0.0f));
	if (type != 3 && type != 4)
		return (v3s(0.0f));
	ci = s->tex_ti[tex_index].indice;
	if (ci >= s->checkers_n)
		return (v3s(0.0f));
	if (type == 4)
		return (get_solid_texture(hit_point, v3p(s->checkers[ci].c1),
				v3p(s->checkers[ci].c2), s->checkers[ci].scale));
	if (((int)floorf(u * s->checkers[ci].scale)
			+ (int)floorf(v * s->checkers[ci].scale)) % 2 == 0)
		return (v3p(s->checkers[ci].c1));
	return (v3p(s->checkers[ci].c2));
}

RT_DEV float	get_transparency(const Ctx &c, int tex_index, float u, float v)
{
	if (tex_index < 0 || (uint32_t)tex_index >= c.sc->tex_ti_n)
		return (0.0f);
	if (c.sc->tex_ti[tex_index].type == 0)
		return (sample_texture(c, tex_index, u, v).w);
	return (0.0f);
}

RT_DEV void	apply_normal_map(const Ctx &c, hit_record &hit)
{
	const t_gpu_material	*m;
	vec3					map;
	vec3					tangent;
	vec3					bi_tangent;
	vec3					mapped;

	if (hit.material_id == -1)
		return ;
	m = &c.sc->materials[hit.material_id];
	if (m->normal_tex_index == -1)
		return ;
	map = get_tex_color(c, m->normal_tex_index, hit.u, hit.v, hit.point);
	map = map * 2.0f - v3s(1.0f);
	tangent = cross(hit.normal, v3(0.0f, -1.0f, 0.0f));
	if (length(tangent) == 0.0f)
		tangent = cross(hit.normal, v3(0.0f, 0.0f, 1.0f));
	tangent = normalize(tangent);
	bi_tangent = normalize(cross(hit.normal, tangent));
	mapped = tangent * map.x + bi_tangent * map.y + hit.normal * map.z;
	hit.normal = normalize(mixv(hit.normal, normalize(mapped),
				m->normal_intensity));
}

RT_DEV float	map_value(const Ctx &c, const hit_record &hit, int tex,
	float value)
{
	if (tex == -1)
		return (value);
	return (get_tex_color(c, tex, hit.u, hit.v, hit.point).x);
}

RT_DEV void	apply_all_map(const Ctx &c, hit_record &hit)
{
	const t_gpu_material	*m;

	if (hit.material_id == -1)
		return ;
	m = &c.sc->materials[hit.material_id];
	apply_normal_map(c, hit);
	hit.mat_val.metallic = map_value(c, hit, m->metallic_tex_index,
			m->metallic_value);
	hit.mat_val.roughness = map_value(c, hit, m->roughness_tex_index,
			m->roughness_value);
	hit.mat_val.ior = m->ior;
	hit.mat_val.transmission = map_value(c, hit, m->transmission_tex_index,
			m->transmission_value);
	hit.mat_val.ao = map_value(c, hit, m->ao_tex_index, m->ao_value);
	hit.mat_val.emission_strength = map_value(c, hit,
			m->emission_strength_tex_index, m->emission_strength);
	hit.mat_val.scale = m->scale;
	hit.mat_val.normal_intensity = m->normal_intensity;
	if (m->emission_color_tex_index == -1)
		hit.mat_val.emission_color = v3p(m->emission_color);
	else
		hit.mat_val.emission_color = get_tex_color(c,
				m->emission_color_tex_index, hit.u, hit.v, hit.point);
}

/* ------------------------------------ path_trace/hit_register/ */

RT_DEV vec3	ray_at(vec3 ro, vec3 rd, float t) { return (ro + rd * t); }

RT_DEV vec3	set_normal_face(vec3 rd, vec3 outward_normal, hit_record &hit)
{
	if (dot(rd, outward_normal) < 0.0f)
	{
		hit.front_face = true;
		return (outward_normal);
	}
	hit.front_face = false;
	return (-outward_normal);
}

RT_DEV bool	solve_quadratic(quadratic &q)
{
	float	sqrt_disc;
	float	denom;

	q.disc = q.b * q.b - 4.0f * q.a * q.c;
	if (q.disc < 0.0f)
		return (false);
	denom = 2.0f * q.a;
	sqrt_disc = sqrtf(q.disc);
	q.t0 = (-q.b - sqrt_disc) / denom;
	q.t1 = (-q.b + sqrt_disc) / denom;
	return (true);
}

RT_DEV bool	valid_t(quadratic &q)
{
	q.t_hit = q.t0;
	if (q.t_hit < RT_IT_MIN || q.t_hit > RT_IT_MAX)
	{
		q.t_hit = q.t1;
		if (q.t_hit < RT_IT_MIN || q.t_hit > RT_IT_MAX)
			return (false);
	}
	return (true);
}

/* Shared by every get_*_uv(): apply the material tiling scale */
RT_DEV void	scale_uv(const Ctx &c, hit_record &hit, int material_id)
{
	float	scale;

	if (material_id == -1)
		return ;
	scale = c.sc->materials[material_id].scale;
	if (scale == 1.0f)
		return ;
	hit.u = fractf(hit.u * scale);
	hit.v = fractf(hit.v * scale);
}

RT_DEV vec3	get_right(vec3 orientation)
{
	if (fabsf(orientation.y) < 0.99f)
		return (normalize(cross(v3(0.0f, 1.0f, 0.0f), orientation)));
	return (normalize(cross(v3(1.0f, 0.0f, 0.0f), orientation)));
}

/* --- sphere */

RT_DEV void	get_sphere_uv(const Ctx &c, hit_record &hit,
	const t_gpu_primitive &sp)
{
	const vec3	pos = v3p(sp.position);

	hit.u = clampf((atan2f((-hit.point.z + pos.z) / sp.radius,
					(hit.point.x - pos.x) / sp.radius) + RT_PI)
			/ (2.0f * RT_PI), 0.0f, 1.0f);
	hit.v = clampf(acosf((-hit.point.y + pos.y) / sp.radius) / RT_PI,
			0.0f, 1.0f);
	scale_uv(c, hit, hit.material_id);
}

RT_DEV bool	hit_sphere(const t_gpu_primitive &sp, vec3 ro, vec3 rd,
	hit_record &hit)
{
	const vec3	pos = v3p(sp.position);
	const vec3	oc = ro - pos;
	quadratic	q;

	q = get_default_quadratic();
	q.a = dot(rd, rd);
	q.b = 2.0f * dot(rd, oc);
	q.c = dot(oc, oc) - sp.radius * sp.radius;
	if (!solve_quadratic(q) || !valid_t(q))
		return (false);
	hit.t = q.t_hit;
	hit.point = ray_at(ro, rd, hit.t);
	hit.normal = set_normal_face(rd, (hit.point - pos) / sp.radius, hit);
	return (true);
}

/* --- cylinder */

RT_DEV void	get_cylinder_lateral_uv(const Ctx &c, hit_record &hit,
	const t_gpu_primitive &cyl)
{
	const vec3	ori = v3p(cyl.orientation);
	const vec3	rel = hit.point - v3p(cyl.position);
	const vec3	right = get_right(ori);

	hit.u = clampf((atan2f(dot(rel, cross(ori, right)), dot(rel, right))
				+ RT_PI) / (2.0f * RT_PI), 0.0f, 1.0f);
	hit.v = clampf(dot(rel, ori) / cyl.height + 0.5f, 0.0f, 1.0f);
	scale_uv(c, hit, hit.material_id);
}

RT_DEV void	get_cylinder_cap_uv(const Ctx &c, hit_record &hit,
	const t_gpu_primitive &cyl, float top)
{
	const vec3	ori = v3p(cyl.orientation);
	const vec3	rel = hit.point - v3p(cyl.position)
		+ ori * (cyl.height * 0.5f * top);
	const vec3	right = get_right(ori);

	hit.u = clampf(dot(rel, right) / (cyl.radius / 2.0f) + 0.5f, 0.0f, 1.0f);
	hit.v = clampf(dot(rel, cross(ori, right)) / (cyl.radius / 2.0f) + 0.5f,
			0.0f, 1.0f);
	scale_uv(c, hit, hit.material_id);
}

RT_DEV bool	handle_cylinder_hit(const t_gpu_primitive &cyl, vec3 ori, vec3 ro,
	vec3 rd, hit_record &hit, const quadratic &q)
{
	const vec3	axis = normalize(ori);
	const vec3	p = ray_at(ro, rd, q.t_hit);
	const vec3	base = v3p(cyl.position) - axis * (cyl.height * 0.5f);
	const float	proj = dot(p - base, axis);

	if (proj < 0.0f || proj > cyl.height)
		return (false);
	hit.t = q.t_hit;
	hit.point = p;
	hit.normal = set_normal_face(rd, normalize(p - base - axis * proj), hit);
	hit.material_id = cyl.material_id;
	hit.part_id = 0;
	return (true);
}

/* side = -1 bottom cap, +1 top cap */
RT_DEV bool	cylinder_cap(const t_gpu_primitive &cyl, vec3 ori, vec3 ro,
	vec3 rd, hit_record &hit, float side)
{
	const vec3	plane_origin = v3p(cyl.position)
		+ normalize(ori) * (cyl.height * 0.5f * side);
	const float	d = dot(rd, ori);
	float		t;
	vec3		hit_point;

	if (fabsf(d) < 1e-6f)
		return (false);
	t = -dot(ro - plane_origin, ori) / d;
	if (t < RT_IT_MIN || t > RT_IT_MAX)
		return (false);
	hit_point = ray_at(ro, rd, t);
	if (length(hit_point - plane_origin) > cyl.radius)
		return (false);
	hit.t = t;
	hit.point = hit_point;
	hit.normal = ori * side;
	hit.front_face = dot(rd, hit.normal) < 0.0f;
	if (side < 0.0f)
		hit.material_id = cyl.material_id_bot;
	else
		hit.material_id = cyl.material_id_top;
	if (hit.material_id == -1)
		hit.material_id = cyl.material_id;
	hit.part_id = 1;
	if (side < 0.0f)
		hit.part_id = 2;
	return (true);
}

RT_DEV bool	hit_cylinder(const t_gpu_primitive &cyl, vec3 ro, vec3 rd,
	hit_record &hit)
{
	quadratic	q;
	hit_record	tmp_rec;
	bool		hit_any;
	vec3		ori;
	vec3		oc;

	q = get_default_quadratic();
	tmp_rec = get_default_hit_record();
	ori = v3p(cyl.orientation);
	if (ori.x == 0.0f && ori.y == 0.0f && ori.z == 0.0f)
		ori.y = 1.0f;
	hit_any = false;
	oc = ro - v3p(cyl.position);
	q.dd = dot(rd, ori);
	q.oo = dot(oc, ori);
	q.a = dot(rd, rd) - q.dd * q.dd;
	q.b = 2.0f * (dot(rd, oc) - q.dd * q.oo);
	q.c = dot(oc, oc) - q.oo * q.oo - cyl.radius * cyl.radius;
	if (solve_quadratic(q) && valid_t(q)
		&& handle_cylinder_hit(cyl, ori, ro, rd, hit, q))
		hit_any = true;
	if (cylinder_cap(cyl, ori, ro, rd, tmp_rec, -1.0f)
		&& (!hit_any || tmp_rec.t < hit.t))
	{
		hit = tmp_rec;
		hit_any = true;
	}
	if (cylinder_cap(cyl, ori, ro, rd, tmp_rec, 1.0f)
		&& (!hit_any || tmp_rec.t < hit.t))
	{
		hit = tmp_rec;
		hit_any = true;
	}
	if (!hit_any)
		return (false);
	hit.normal = set_normal_face(rd, hit.normal, hit);
	return (true);
}

/* --- cone */

RT_DEV void	get_cone_lateral_uv(const Ctx &c, hit_record &hit,
	const t_gpu_primitive &cone)
{
	const vec3	ori = v3p(cone.orientation);
	const vec3	rel = hit.point - v3p(cone.position);
	const vec3	right = get_right(ori);

	hit.u = clampf((atan2f(dot(rel, cross(ori, right)), dot(rel, right))
				+ RT_PI) / (2.0f * RT_PI), 0.0f, 1.0f);
	hit.v = clampf(dot(rel, ori) / cone.height, 0.0f, 1.0f);
	scale_uv(c, hit, hit.material_id);
}

RT_DEV void	get_cone_cap_uv(const Ctx &c, hit_record &hit,
	const t_gpu_primitive &cone)
{
	const vec3	ori = v3p(cone.orientation);
	const vec3	rel = hit.point - v3p(cone.position)
		+ ori * (cone.height * 0.5f);
	const vec3	right = get_right(ori);

	hit.u = clampf(dot(rel, right) / (cone.radius * 2.0f) + 0.5f, 0.0f, 1.0f);
	hit.v = clampf(dot(rel, cross(ori, right)) / (cone.radius * 2.0f) + 0.5f,
			0.0f, 1.0f);
	scale_uv(c, hit, hit.material_id);
}

RT_DEV bool	handle_cone_hit(const t_gpu_primitive &cone, vec3 ori, vec3 ro,
	vec3 rd, hit_record &hit, const quadratic &q)
{
	const vec3	axis = normalize(ori);
	const vec3	p = ray_at(ro, rd, q.t_hit);
	const vec3	apex = v3p(cone.position);
	const float	proj = dot(p - apex, axis);
	const float	k = (cone.radius / cone.height) * (cone.radius / cone.height);

	if (proj < 0.0f || proj > cone.height)
		return (false);
	hit.t = q.t_hit;
	hit.point = p;
	hit.normal = set_normal_face(rd,
			normalize(p - apex - axis * ((1.0f + k) * proj)), hit);
	hit.material_id = cone.material_id;
	hit.part_id = 0;
	return (true);
}

RT_DEV bool	cone_cap(const t_gpu_primitive &cone, vec3 ori, vec3 ro, vec3 rd,
	hit_record &hit)
{
	const vec3	normal = normalize(ori);
	const vec3	cap_c = v3p(cone.position) + normal * cone.height;
	const float	denom = dot(rd, normal);
	float		t;
	vec3		hit_point;

	if (fabsf(denom) < 1e-6f)
		return (false);
	t = dot(cap_c - ro, normal) / denom;
	if (t < RT_IT_MIN || t > RT_IT_MAX)
		return (false);
	hit_point = ray_at(ro, rd, t);
	if (length(hit_point - cap_c) > cone.radius)
		return (false);
	hit.t = t;
	hit.point = hit_point;
	hit.normal = normal;
	hit.front_face = dot(rd, hit.normal) < 0.0f;
	hit.part_id = 1;
	hit.material_id = cone.material_id_top;
	if (hit.material_id == -1)
		hit.material_id = cone.material_id;
	return (true);
}

RT_DEV bool	hit_cone(const t_gpu_primitive &cone, vec3 ro, vec3 rd,
	hit_record &hit)
{
	quadratic	q;
	vec3		ori;
	vec3		oc;
	float		k;

	q = get_default_quadratic();
	ori = v3p(cone.orientation);
	if (ori.x == 0.0f && ori.y == 0.0f && ori.z == 0.0f)
		ori.y = 1.0f;
	oc = ro - v3p(cone.position);
	k = powf(cone.radius / cone.height, 2.0f);
	q.dd = dot(rd, ori);
	q.oo = dot(oc, ori);
	q.a = dot(rd, rd) - (1.0f + k) * q.dd * q.dd;
	q.b = 2.0f * (dot(rd, oc) - (1.0f + k) * q.dd * q.oo);
	q.c = dot(oc, oc) - (1.0f + k) * q.oo * q.oo;
	if (!solve_quadratic(q) || !valid_t(q))
		return (false);
	if (handle_cone_hit(cone, ori, ro, rd, hit, q))
		return (true);
	if (!cone_cap(cone, ori, ro, rd, hit))
		return (false);
	hit.normal = set_normal_face(rd, hit.normal, hit);
	return (true);
}

/* --- hyperboloid */

RT_DEV void	get_hyperboloid_uv(const Ctx &c, hit_record &hit,
	const t_gpu_hyper &hyper)
{
	const vec3	ori = v3p(hyper.orientation);
	const vec3	rel = hit.point - v3p(hyper.position);
	const vec3	right = get_right(ori);

	hit.u = clampf((atan2f(dot(rel, cross(ori, right)), dot(rel, right))
				+ RT_PI) / (2.0f * RT_PI), 0.0f, 1.0f);
	hit.v = clampf(dot(rel, ori) / (hyper.height * 0.5f) * 0.5f + 0.5f,
			0.0f, 1.0f);
	scale_uv(c, hit, hit.material_id);
}

RT_DEV bool	handle_hyperboloid_hit(const t_gpu_hyper &hyp, vec3 ori, vec3 ro,
	vec3 rd, hit_record &hit, float t_hit)
{
	const vec3	axis = normalize(ori);
	const vec3	p = ray_at(ro, rd, t_hit);
	const vec3	to_p = p - v3p(hyp.position);
	const vec3	u = get_right(axis);
	const vec3	v = cross(axis, u);
	vec3		normal;

	if (!(fabsf(dot(to_p, axis)) <= hyp.height * 0.5f))
		return (false);
	normal = normalize(u * (2.0f * dot(to_p, u) / (hyp.a * hyp.a))
			+ v * (2.0f * dot(to_p, v) / (hyp.b * hyp.b))
			+ axis * (-2.0f * dot(to_p, axis) / (hyp.c * hyp.c)));
	hit.t = t_hit;
	hit.point = p;
	hit.normal = set_normal_face(rd, normal, hit);
	hit.front_face = dot(rd, normal) < 0.0f;
	return (true);
}

RT_DEV bool	hit_hyperboloid(const t_gpu_hyper &hyp, vec3 ro, vec3 rd,
	hit_record &hit, float interval_max)
{
	quadratic	q;
	vec3		ori;
	vec3		x;
	vec3		y;
	vec3		z;
	vec3		lrd;
	vec3		lro;
	bool		hit_anything;

	q = get_default_quadratic();
	ori = v3p(hyp.orientation);
	if (dot(ori, ori) < 1e-6f)
		ori = v3(0.0f, 0.0f, 1.0f);
	z = normalize(ori);
	if (fabsf(z.x) > 0.9f)
		x = normalize(cross(v3(0.0f, 1.0f, 0.0f), z));
	else
		x = normalize(cross(v3(1.0f, 0.0f, 0.0f), z));
	y = cross(z, x);
	lro = ro - v3p(hyp.position);
	lrd = v3(dot(rd, x), dot(rd, y), dot(rd, z));
	lro = v3(dot(lro, x), dot(lro, y), dot(lro, z));
	q.a = (lrd.x * lrd.x) / (hyp.a * hyp.a) + (lrd.y * lrd.y) / (hyp.b * hyp.b)
		- (lrd.z * lrd.z) / (hyp.c * hyp.c);
	q.b = 2.0f * ((lrd.x * lro.x) / (hyp.a * hyp.a)
			+ (lrd.y * lro.y) / (hyp.b * hyp.b)
			- (lrd.z * lro.z) / (hyp.c * hyp.c));
	q.c = (lro.x * lro.x) / (hyp.a * hyp.a) + (lro.y * lro.y) / (hyp.b * hyp.b)
		- (lro.z * lro.z) / (hyp.c * hyp.c) - hyp.shape;
	if (!solve_quadratic(q))
		return (false);
	hit_anything = false;
	if (q.t0 > RT_IT_MIN && q.t0 < interval_max
		&& handle_hyperboloid_hit(hyp, ori, ro, rd, hit, q.t0))
	{
		hit_anything = true;
		interval_max = q.t0;
	}
	if (q.t1 > RT_IT_MIN && q.t1 < interval_max
		&& handle_hyperboloid_hit(hyp, ori, ro, rd, hit, q.t1))
		hit_anything = true;
	return (hit_anything);
}

/* --- triangle */

RT_DEV bool	hit_triangle(const Ctx &c, const t_gpu_triangle &t, vec3 ro,
	vec3 rd, hit_record &hit)
{
	const int	one_sided = c.fr->viewport.t_one_sided;
	const vec3	p0 = v3p(t.v0.pos);
	const vec3	e1 = v3p(t.v1.pos) - p0;
	const vec3	e2 = v3p(t.v2.pos) - p0;
	const vec3	h = cross(rd, e2);
	const float	a = dot(e1, h);
	float		f;
	float		u;
	float		v;
	float		w;
	float		hit_t;
	vec3		s;
	vec3		q;
	vec3		outward_normal;

	if (one_sided == 0 && fabsf(a) < 1e-6f)
		return (false);
	if (one_sided != 0 && a <= 1e-6f)
		return (false);
	f = 1.0f / a;
	s = ro - p0;
	u = f * dot(s, h);
	if (u < 0.0f || u > 1.0f)
		return (false);
	q = cross(s, e1);
	v = f * dot(rd, q);
	if (v < 0.0f || u + v > 1.0f)
		return (false);
	hit_t = f * dot(e2, q);
	if (!(hit_t > 1e-4f && hit_t < hit.t))
		return (false);
	hit.t = hit_t;
	hit.point = ro + rd * hit_t;
	w = 1.0f - u - v;
	outward_normal = normalize(v3p(t.v0.normal) * w + v3p(t.v1.normal) * u
			+ v3p(t.v2.normal) * v);
	if (one_sided == 0 && dot(rd, outward_normal) > 0.0f)
	{
		hit.front_face = false;
		hit.normal = -outward_normal;
	}
	else
	{
		hit.front_face = true;
		hit.normal = outward_normal;
	}
	hit.u = w * t.v0.u + u * t.v1.u + v * t.v2.u;
	hit.v = w * t.v0.v + u * t.v1.v + v * t.v2.v;
	return (true);
}

/* --- plane */

RT_DEV void	get_uv_plane(const Ctx &c, hit_record &hit, const t_gpu_plane &pl)
{
	const vec3	n = v3p(pl.normal);
	vec3		up;
	vec3		tangent;
	vec3		local;
	float		u;
	float		v;

	up = v3(1.0f, 0.0f, 0.0f);
	if (fabsf(n.y) < 0.99f)
		up = v3(0.0f, 1.0f, 0.0f);
	tangent = normalize(cross(up, n));
	local = hit.point - v3p(pl.position);
	u = dot(local, tangent);
	v = dot(local, cross(n, tangent));
	if (pl.material_id != -1
		&& (uint32_t)pl.material_id < c.sc->materials_n)
	{
		u *= c.sc->materials[pl.material_id].scale * 0.1f;
		v *= c.sc->materials[pl.material_id].scale * 0.1f;
	}
	hit.u = u - floorf(u);
	hit.v = v - floorf(v);
}

RT_DEV bool	hit_plane(const t_gpu_plane &pl, vec3 ro, vec3 rd, hit_record &hit)
{
	const vec3	n = v3p(pl.normal);
	const float	denom = dot(n, rd);
	float		t;

	if (fabsf(denom) < 1e-7f)
		return (false);
	t = (pl.d - dot(n, ro)) / denom;
	if (t <= RT_IT_MIN || t > RT_IT_MAX)
		return (false);
	hit.t = t;
	hit.point = ro + rd * t;
	hit.front_face = denom < 0.0f;
	hit.normal = -n;
	if (denom < 0.0f)
		hit.normal = n;
	return (true);
}

/* --- bvh */

RT_DEV int	safe_mat(const Ctx &c, int material_id)
{
	if (material_id < 0 || (uint32_t)material_id >= c.sc->materials_n)
		return (-1);
	return (material_id);
}

RT_DEV void	set_rec(hit_record &rec, const hit_record &tmp, int material_id,
	uint32_t type, uint32_t index)
{
	rec = tmp;
	rec.material_id = material_id;
	rec.object_type = type;
	rec.object_index = index;
}

RT_DEV bool	hit_register_loop(Ctx &c, vec3 ro, vec3 rd,
	const t_gpu_bvh_node &node, hit_record &rec)
{
	const t_rt_scene	*s = c.sc;
	hit_record			hit_tmp;
	bool				hit;
	uint32_t			i;
	uint32_t			type;
	uint32_t			index;
	int					mat;

	hit = false;
	hit_tmp = get_default_hit_record();
	i = (uint32_t)-1;
	while (++i < node.prim_count && node.first_prim + i < s->prim_ti_n)
	{
		type = s->prim_ti[node.first_prim + i].type;
		index = s->prim_ti[node.first_prim + i].indice;
		if (type == 4 && index < s->triangles_n)
		{
			if (!hit_triangle(c, s->triangles[index], ro, rd, hit_tmp)
				|| !(hit_tmp.t < rec.t))
				continue ;
			mat = safe_mat(c, s->triangles[index].material_id);
			if (mat != -1 && s->materials[mat].color_tex_index != -1
				&& !(rt_rand(c) < get_transparency(c,
						s->materials[mat].color_tex_index, hit_tmp.u,
						hit_tmp.v)))
				continue ;
			set_rec(rec, hit_tmp, mat, 4, index);
			hit = true;
		}
		else if (type <= 2 && index < s->primitives_n)
		{
			if (type == 0 && !hit_sphere(s->primitives[index], ro, rd, hit_tmp))
				continue ;
			if (type == 1
				&& !hit_cylinder(s->primitives[index], ro, rd, hit_tmp))
				continue ;
			if (type == 2 && !hit_cone(s->primitives[index], ro, rd, hit_tmp))
				continue ;
			if (!(hit_tmp.t < rec.t))
				continue ;
			set_rec(rec, hit_tmp, safe_mat(c, s->primitives[index].material_id),
				type, index);
			hit = true;
		}
		else if (type == 3 && index < s->hypers_n)
		{
			if (!hit_hyperboloid(s->hypers[index], ro, rd, hit_tmp, RT_IT_MAX)
				|| !(hit_tmp.t < rec.t))
				continue ;
			set_rec(rec, hit_tmp, safe_mat(c, s->hypers[index].material_id),
				3, index);
			hit = true;
		}
	}
	return (hit);
}

/* Returns the entry distance through tmin */
RT_DEV bool	intersect_aabb(vec3 ro, vec3 inv_rd, const t_gpu_aabb &box,
	float &tmin)
{
	const vec3	t1 = (v3p(box.min) - ro) * inv_rd;
	const vec3	t2 = (v3p(box.max) - ro) * inv_rd;
	float		tmax;

	tmin = fmaxf(fmaxf(fminf(t1.x, t2.x), fminf(t1.y, t2.y)),
			fminf(t1.z, t2.z));
	tmax = fminf(fminf(fmaxf(t1.x, t2.x), fmaxf(t1.y, t2.y)),
			fmaxf(t1.z, t2.z));
	return (tmax >= fmaxf(tmin, 0.0f) && tmin < 1000.0f);
}

RT_DEV bool	hit_bvh(Ctx &c, vec3 ro, vec3 rd, hit_record &rec)
{
	const t_rt_scene	*s = c.sc;
	const vec3			inv_rd = v3(1.0f / rd.x, 1.0f / rd.y, 1.0f / rd.z);
	uint32_t			stack[RT_BVH_STACK];
	int					stack_ptr;
	uint32_t			node_index;
	bool				hit_anything;
	float				tmin;
	hit_record			rec_tmp;

	hit_anything = false;
	if (s->bvh_n == 0)
		return (false);
	stack_ptr = 0;
	stack[stack_ptr++] = 0;
	while (stack_ptr > 0)
	{
		node_index = stack[--stack_ptr];
		if (node_index >= s->bvh_n)
			continue ;
		const t_gpu_bvh_node &node = s->bvh[node_index];
		if (!intersect_aabb(ro, inv_rd, node.node_bounds, tmin))
			continue ;
		if (hit_anything && tmin > rec.t)
			continue ;
		if (node.is_leaf)
		{
			rec_tmp = get_default_hit_record();
			if (hit_register_loop(c, ro, rd, node, rec_tmp))
			{
				if (!hit_anything || rec_tmp.t < rec.t)
					rec = rec_tmp;
				hit_anything = true;
			}
		}
		else
		{
			if (stack_ptr + 2 > RT_BVH_STACK)
				return (hit_anything);
			stack[stack_ptr++] = node.right_child;
			stack[stack_ptr++] = node.left_child;
		}
	}
	return (hit_anything);
}

RT_DEV bool	hit_bvh_depth(const Ctx &c, vec3 ro, vec3 rd, int &depth)
{
	const t_rt_scene	*s = c.sc;
	const vec3			inv_rd = v3(1.0f / rd.x, 1.0f / rd.y, 1.0f / rd.z);
	uint32_t			stack[RT_BVH_STACK];
	int					stack_ptr;
	uint32_t			node_index;
	bool				hit_anything;
	float				tmin;

	hit_anything = false;
	if (s->bvh_n == 0)
		return (false);
	stack_ptr = 0;
	stack[stack_ptr++] = 0;
	while (stack_ptr > 0)
	{
		node_index = stack[--stack_ptr];
		if (node_index >= s->bvh_n)
			continue ;
		const t_gpu_bvh_node &node = s->bvh[node_index];
		if (!intersect_aabb(ro, inv_rd, node.node_bounds, tmin))
			continue ;
		depth++;
		if (node.is_leaf)
			hit_anything = true;
		else
		{
			if (stack_ptr + 2 > RT_BVH_STACK)
				return (hit_anything);
			stack[stack_ptr++] = node.right_child;
			stack[stack_ptr++] = node.left_child;
		}
	}
	return (hit_anything);
}

/* --- hit_register.comp */

RT_DEV vec3	get_hit_register_color(const Ctx &c, vec3 obj_color,
	const hit_record &hit)
{
	const t_gpu_material	*m;

	if (hit.material_id == -1)
		return (obj_color);
	m = &c.sc->materials[hit.material_id];
	if (m->color_tex_index == -1)
		return (v3p(m->color_value));
	return (get_tex_color(c, m->color_tex_index, hit.u, hit.v, hit.point));
}

RT_DEV void	hit_planes(const Ctx &c, hit_record &hit, bool &hit_something,
	vec3 ro, vec3 rd)
{
	hit_record	hit_tmp;
	uint32_t	i;

	i = (uint32_t)-1;
	while (++i < c.fr->planes_am)
	{
		hit_tmp = get_default_hit_record();
		if (hit_plane(c.sc->planes[i], ro, rd, hit_tmp) && hit_tmp.t < hit.t)
		{
			hit_something = true;
			set_rec(hit, hit_tmp, safe_mat(c, c.sc->planes[i].material_id),
				5, i);
		}
	}
}

RT_DEV void	set_final_values(const Ctx &c, hit_record &hit)
{
	const t_rt_scene	*s = c.sc;
	const uint32_t		i = hit.object_index;
	vec3				obj_color;

	if (hit.object_type <= 2 && i < s->primitives_n)
	{
		if (hit.object_type == 0)
			get_sphere_uv(c, hit, s->primitives[i]);
		else if (hit.object_type == 1 && hit.part_id == 0)
			get_cylinder_lateral_uv(c, hit, s->primitives[i]);
		else if (hit.object_type == 1 && hit.part_id == 1)
			get_cylinder_cap_uv(c, hit, s->primitives[i], 1.0f);
		else if (hit.object_type == 1 && hit.part_id == 2)
			get_cylinder_cap_uv(c, hit, s->primitives[i], -1.0f);
		else if (hit.object_type == 2 && hit.part_id == 0)
			get_cone_lateral_uv(c, hit, s->primitives[i]);
		else if (hit.object_type == 2 && hit.part_id == 1)
			get_cone_cap_uv(c, hit, s->primitives[i]);
		obj_color = v3p(s->primitives[i].color);
	}
	else if (hit.object_type == 3 && i < s->hypers_n)
	{
		get_hyperboloid_uv(c, hit, s->hypers[i]);
		obj_color = v3p(s->hypers[i].color);
	}
	else if (hit.object_type == 4 && i < s->triangles_n)
		obj_color = v3p(s->triangles[i].color);
	else if (hit.object_type == 5 && i < s->planes_n)
	{
		get_uv_plane(c, hit, s->planes[i]);
		obj_color = v3p(s->planes[i].color);
	}
	else
		return ;
	apply_all_map(c, hit);
	hit.color = get_hit_register_color(c, obj_color, hit);
}

RT_DEV bool	hit_register_all(Ctx &c, hit_record &hit, vec3 ro, vec3 rd,
	bool calc_uv)
{
	bool	hit_something;

	hit_something = hit_bvh(c, ro, rd, hit);
	hit_planes(c, hit, hit_something, ro, rd);
	if (calc_uv)
		set_final_values(c, hit);
	return (hit_something);
}

/* ------------------------------------------ path_trace/skybox.comp */

RT_DEV vec3	get_background_color(const Ctx &c, vec3 rd)
{
	const t_rt_scene	*s = c.sc;
	const int			tex = c.fr->ambiant.skybox_tex_index;
	const float			ratio = c.fr->ambiant.ratio;
	float				u;
	float				v;
	uint32_t			type;
	uint32_t			ci;

	if (tex == -1)
		return (v3p(c.fr->ambiant.skybox_color) * ratio);
	if (tex < 0 || (uint32_t)tex >= s->tex_ti_n)
		return (v3s(0.0f));
	u = 0.5f - atan2f(rd.z, rd.x) / (2.0f * RT_PI);
	v = 0.5f + asinf(rd.y) / RT_PI;
	type = s->tex_ti[tex].type;
	if (type == 1)
		return (sample_hdr_texture(c, tex, u, v, ratio - 1.0f));
	if (type == 0)
	{
		const vec4 px = sample_texture(c, tex, u, v);
		return (v3(px.x, px.y, px.z) * ratio);
	}
	ci = s->tex_ti[tex].indice;
	if ((type != 3 && type != 4) || ci >= s->checkers_n)
		return (v3s(0.0f));
	if (((int)floorf(u * s->checkers[ci].scale)
			+ (int)floorf(v * s->checkers[ci].scale)) % 2 == 0)
		return (v3p(s->checkers[ci].c1));
	return (v3p(s->checkers[ci].c2));
}

/* ------------------------------- path_trace/material/manager_opti.comp */

RT_DEV uint32_t	hash_cell(int x, int y, int z, int table_size)
{
	const uint32_t	n = ((uint32_t)x * 73856093u) ^ ((uint32_t)y * 19349663u)
		^ ((uint32_t)z * 83492791u);

	return (n % (uint32_t)table_size);
}

RT_DEV void	knn_refresh_farthest(KNNSearch &search, uint32_t &farthest_idx)
{
	uint32_t	i;

	farthest_idx = 0;
	search.farthest_dist_sq = search.results[0].dist_sq;
	i = 0;
	while (++i < search.k)
	{
		if (search.results[i].dist_sq > search.farthest_dist_sq)
		{
			farthest_idx = i;
			search.farthest_dist_sq = search.results[i].dist_sq;
		}
	}
}

RT_DEV void	knn_add_result(vec3 photon_power, KNNSearch &search, float dist_sq)
{
	uint32_t	farthest_idx;

	if (search.count < search.k)
	{
		search.results[search.count].power = photon_power;
		search.results[search.count].dist_sq = dist_sq;
		if (dist_sq > search.farthest_dist_sq)
			search.farthest_dist_sq = dist_sq;
		search.count++;
	}
	else if (dist_sq < search.farthest_dist_sq)
	{
		knn_refresh_farthest(search, farthest_idx);
		search.results[farthest_idx].power = photon_power;
		search.results[farthest_idx].dist_sq = dist_sq;
		knn_refresh_farthest(search, farthest_idx);
	}
}

RT_DEV vec3	get_caustic(const Ctx &c, const hit_record &hit)
{
	const t_rt_scene	*s = c.sc;
	const int			table_size = c.fr->table_size;
	const vec3			rel = (hit.point - v3p(c.fr->grid_world_min))
		/ c.fr->cell_size;
	const int			cx = (int)floorf(rel.x);
	const int			cy = (int)floorf(rel.y);
	const int			cz = (int)floorf(rel.z);
	KNNSearch			search;
	uint32_t			tested;
	uint32_t			i;
	uint32_t			idx;
	int					n;
	vec3				final_color;

	if (table_size <= 0)
		return (v3s(0.0f));
	search.k = RT_KNN_K;
	search.count = 0;
	search.farthest_dist_sq = 10000000.0f;
	tested = 0;
	n = -1;
	while (++n < 27 && tested < 500)
	{
		const t_gpu_cell cell = s->cells[hash_cell(cx + n % 3 - 1,
				cy + (n / 3) % 3 - 1, cz + n / 9 - 1, table_size)];
		i = (uint32_t)-1;
		while (++i < cell.count && tested < 500)
		{
			if (cell.start_index + i >= s->photon_idx_n)
				break ;
			idx = s->photon_idx[cell.start_index + i];
			if (idx >= s->photons_n)
				break ;
			final_color = v3p(s->photons[idx].position) - hit.point;
			knn_add_result(v3p(s->photons[idx].color), search,
				dot(final_color, final_color));
			tested++;
		}
	}
	if (search.count < search.k || !(search.farthest_dist_sq > 0.0001f))
		return (v3s(0.0f));
	final_color = v3s(0.0f);
	i = (uint32_t)-1;
	while (++i < search.k)
		final_color += search.results[i].power;
	return (final_color * (1.0f / (3.14159265f * search.farthest_dist_sq)));
}

/* GGX lobe around `normal` (not normalized, as in the GLSL) */
RT_DEV vec3	ggx_sample(Ctx &c, vec3 normal, float roughness)
{
	const float	u2 = rt_rand(c);
	const float	phi = 2.0f * RT_PI * rt_rand(c);
	const float	r4 = (roughness * roughness) * (roughness * roughness);
	const float	cos_theta = sqrtf((1.0f - u2) / (1.0f + (r4 - 1.0f) * u2));
	const float	sin_theta = sqrtf(1.0f - cos_theta * cos_theta);
	vec3		tangent;

	tangent = normalize(cross(normal, v3(1.0f, 0.0f, 0.0f)));
	if (dot(tangent, tangent) < 0.0001f)
		tangent = normalize(cross(normal, v3(0.0f, 1.0f, 0.0f)));
	return (normal * cos_theta + tangent * (cosf(phi) * sin_theta)
		+ normalize(cross(normal, tangent)) * (sinf(phi) * sin_theta));
}

/* Is the point visible from `dir`, up to max_dist (< 0: infinite) */
RT_DEV bool	rt_visible(Ctx &c, hit_record &shadow_hit, vec3 origin, vec3 dir,
	float max_dist)
{
	shadow_hit.t = RT_IT_MAX;
	if (!hit_register_all(c, shadow_hit, origin, dir, false))
		return (true);
	return (max_dist >= 0.0f && shadow_hit.t >= max_dist);
}

/* Blinn-less phong of the shader: diffuse + specular for one light.
** flip_spec is true on the material path, where the GLSL reflects +light_dir */
RT_DEV vec3	light_contribution(const t_gpu_light &l, const hit_record &hit,
	vec3 light_dir, vec3 view_dir, float shadow_factor, bool flip_spec)
{
	const float	n_dot_l = dot(hit.normal, light_dir);
	vec3		reflection;
	vec3		res;
	float		spec;

	if (n_dot_l <= 0.0f)
		return (v3s(0.0f));
	res = v3p(l.color) * (l.brightness * n_dot_l * shadow_factor);
	reflection = hit.normal * (2.0f * n_dot_l) - light_dir;
	if (flip_spec)
		reflection = -reflection;
	spec = powf(fmaxf(0.0f, dot(reflection, view_dir)), RT_SHINE)
		* RT_SPEC_STRENGTH * shadow_factor;
	return (res + v3p(l.color) * (l.brightness * spec));
}

RT_DEV float	point_light_shadow(Ctx &c, const t_gpu_light &l,
	const hit_record &hit, hit_record &shadow_hit)
{
	vec3		to_light;
	uint32_t	hits_count;
	uint32_t	s;

	if (l.radius == 0.0f || l.shadow_sample <= 1)
	{
		to_light = v3p(l.pos) - hit.point;
		if (rt_visible(c, shadow_hit, hit.point, normalize(to_light),
				length(to_light)))
			return (1.0f);
		return (0.0f);
	}
	hits_count = 0;
	s = (uint32_t)-1;
	while (++s < l.shadow_sample)
	{
		to_light = v3p(l.pos) + random_in_unit_sphere(c) * l.radius
			- hit.point;
		if (rt_visible(c, shadow_hit, hit.point, normalize(to_light),
				length(to_light)))
			hits_count++;
	}
	return ((float)hits_count / (float)l.shadow_sample);
}

RT_DEV vec3	direct_lighting(Ctx &c, const hit_record &hit, bool flip_spec)
{
	const vec3	view_dir = normalize(v3p(c.fr->camera.position) - hit.point);
	hit_record	shadow_hit;
	vec3		total;
	vec3		light_dir;
	float		shadow_factor;
	uint32_t	i;

	total = v3s(0.0f);
	shadow_hit = get_default_hit_record();
	i = (uint32_t)-1;
	while (++i < c.fr->lights_am)
	{
		const t_gpu_light &l = c.sc->lights[i];
		if (l.type == 0)
		{
			shadow_factor = point_light_shadow(c, l, hit, shadow_hit);
			if (shadow_factor > 0.0f)
				total += light_contribution(l, hit,
						normalize(v3p(l.pos) - hit.point), view_dir,
						shadow_factor, flip_spec);
		}
		else if (l.type == 1)
		{
			light_dir = -v3p(l.pos);
			if (rt_visible(c, shadow_hit, hit.point, light_dir, -1.0f))
				total += light_contribution(l, hit, light_dir, view_dir, 1.0f,
						flip_spec);
		}
	}
	return (total);
}

/* Skybox next event estimation through the HDR importance sampling tables */
RT_DEV void	sample_skybox(Ctx &c, vec3 ro, const hit_record &hit, vec3 &power,
	vec3 &accumulation)
{
	hit_record	tmp_hit;
	float		u;
	float		v;
	float		costheta;
	float		pdf;
	vec3		rd_sky;
	vec3		radiance;

	if (c.fr->ambiant.skybox_tex_index == -1)
		return ;
	tmp_hit = get_default_hit_record();
	calc_inverse_transform_sampling_uv(c, u, v);
	rd_sky = calc_inverse_transform_sampling_dir(u, v);
	if (hit_register_all(c, tmp_hit, ro, rd_sky, false))
		return ;
	costheta = dot(hit.normal, rd_sky);
	pdf = tex_nearest(c.sc->pdf_joint, c.sc->pdf_w, c.sc->pdf_h, u, v);
	if (!(pdf > 0.0f))
		return ;
	radiance = get_background_color(c, rd_sky);
	if (hit.material_id != -1)
		radiance *= hit.mat_val.ao;
	radiance *= power;
	radiance *= costheta / pdf;
	accumulation += radiance;
	power *= (1.0f - costheta);
}

/* The "becomes diffuse" block of material_manager_v4 */
RT_DEV void	diffuse_bounce(Ctx &c, vec3 ro, vec3 &rd, const hit_record &hit,
	vec3 &power, vec3 &accumulation)
{
	vec3	total_light;
	vec3	tangent;
	vec3	bitangent;
	float	r1;
	float	r2;
	float	phi;

	total_light = direct_lighting(c, hit, hit.material_id != -1);
	power *= hit.color;
	accumulation += (total_light + get_caustic(c, hit)) * power;
	sample_skybox(c, ro, hit, power, accumulation);
	r1 = rt_rand(c);
	r2 = rt_rand(c);
	phi = 2.0f * RT_PI * r1;
	if (fabsf(hit.normal.y) < 0.999f)
		tangent = normalize(cross(v3(0.0f, 1.0f, 0.0f), hit.normal));
	else
		tangent = normalize(cross(v3(1.0f, 0.0f, 0.0f), hit.normal));
	bitangent = cross(hit.normal, tangent);
	rd = normalize(tangent * (cosf(phi) * sqrtf(r2))
			+ hit.normal * sqrtf(1.0f - r2)
			+ bitangent * (sinf(phi) * sqrtf(r2)));
}

RT_DEV void	metallic_bounce(Ctx &c, vec3 &rd, const hit_record &hit,
	vec3 &power)
{
	const float	cos_theta = -dot(normalize(rd), hit.normal);
	vec3		micro_normal;

	micro_normal = hit.normal;
	if (hit.mat_val.roughness > 0.0f)
		micro_normal = ggx_sample(c, hit.normal, hit.mat_val.roughness);
	rd = normalize(rd - micro_normal * (2.0f * dot(rd, micro_normal)));
	power *= (hit.color + (v3s(1.0f) - hit.color)
			* powf(1.0f - cos_theta, 5.0f));
}

RT_DEV void	specular_reflect(Ctx &c, vec3 &rd, const hit_record &hit)
{
	vec3	micro_normal;

	micro_normal = hit.normal;
	if (hit.mat_val.roughness > 0.0f)
		micro_normal = normalize(ggx_sample(c, hit.normal,
					hit.mat_val.roughness));
	rd = normalize(rd - micro_normal * (2.0f * dot(rd, micro_normal)));
}

RT_DEV void	refract_bounce(Ctx &c, vec3 &rd, const hit_record &hit,
	vec3 &power)
{
	const vec3	incident = normalize(rd);
	const float	cos_theta = fminf(-dot(incident, hit.normal), 1.0f);
	const float	sin_theta = sqrtf(1.0f - cos_theta * cos_theta);
	float		eta;
	vec3		r_out_perp;
	vec3		refracted;

	if (hit.front_face)
		eta = c.fr->viewport.ior_global / hit.mat_val.ior;
	else
		eta = hit.mat_val.ior / c.fr->viewport.ior_global;
	if (eta * sin_theta > 1.0f)
		return (specular_reflect(c, rd, hit));
	r_out_perp = (incident + hit.normal * cos_theta) * eta;
	refracted = normalize(r_out_perp + hit.normal
			* -sqrtf(fabsf(1.0f - dot(r_out_perp, r_out_perp))));
	if (hit.mat_val.roughness > 0.0f)
		refracted = ggx_sample(c, refracted, hit.mat_val.roughness);
	rd = refracted;
	power *= hit.color;
}

RT_DEV void	dielectric_bounce(Ctx &c, vec3 ro, vec3 &rd, const hit_record &hit,
	vec3 &power, vec3 &accumulation)
{
	const float	ior = hit.mat_val.ior;
	bool		becomes_diffuse;
	bool		is_reflected;
	float		f0;
	float		reflect_prob;

	becomes_diffuse = (ior <= 0.0f);
	is_reflected = false;
	if (!becomes_diffuse)
	{
		f0 = powf((ior - 1.0f) / (ior + 1.0f), 2.0f);
		reflect_prob = f0 + (1.0f - f0)
			* powf(1.0f + dot(normalize(rd), hit.normal), 5.0f);
		if (reflect_prob >= rt_rand(c) && ior != 1.0f)
			is_reflected = true;
		else if (hit.mat_val.transmission < 1.0f
			&& (hit.mat_val.transmission == 0.0f
				|| hit.mat_val.transmission < rt_rand(c)))
			becomes_diffuse = true;
	}
	if (is_reflected)
		specular_reflect(c, rd, hit);
	else if (!becomes_diffuse)
		refract_bounce(c, rd, hit, power);
	else
		diffuse_bounce(c, ro, rd, hit, power, accumulation);
}

RT_DEV void	material_manager_v4(Ctx &c, vec3 ro, vec3 &rd,
	const hit_record &hit, vec3 &power, vec3 &accumulation)
{
	if (hit.material_id == -1)
		return (diffuse_bounce(c, ro, rd, hit, power, accumulation));
	if (hit.mat_val.emission_strength > 0.0f)
		accumulation += hit.mat_val.emission_color
			* hit.mat_val.emission_strength * power;
	if (hit.mat_val.metallic == 1.0f)
		metallic_bounce(c, rd, hit, power);
	else if (hit.mat_val.metallic == 0.0f)
		dielectric_bounce(c, ro, rd, hit, power, accumulation);
	else if (hit.mat_val.metallic > rt_rand(c))
		metallic_bounce(c, rd, hit, power);
	else
		dielectric_bounce(c, ro, rd, hit, power, accumulation);
}

/* --------------------------------------- path_trace/path_trace.comp */

RT_DEV vec3	path_trace(Ctx &c, vec3 ro, vec3 rd)
{
	vec3		accumulation;
	vec3		power;
	hit_record	hit;
	uint32_t	depth;

	accumulation = v3s(0.0f);
	power = v3s(1.0f);
	hit = get_default_hit_record();
	depth = 0;
	while (depth < c.fr->max_bounces)
	{
		hit.t = RT_IT_MAX;
		if (!hit_register_all(c, hit, ro, rd, true))
			return (accumulation
				+ power * clampv(get_background_color(c, rd), 0.0f, 16.0f));
		ro = hit.point;
		material_manager_v4(c, ro, rd, hit, power, accumulation);
		depth++;
	}
	return (accumulation);
}

RT_DEV vec3	path_trace_normal(Ctx &c, vec3 ro, vec3 rd)
{
	hit_record	hit;

	hit = get_default_hit_record();
	hit.t = RT_IT_MAX;
	if (hit_register_all(c, hit, ro, rd, true))
		return ((hit.normal + v3s(1.0f)) * 0.5f);
	return (v3s(0.0f));
}

RT_DEV vec3	path_trace_bvh(const Ctx &c, vec3 ro, vec3 rd)
{
	int	depth;

	depth = -1;
	if (hit_bvh_depth(c, ro, rd, depth))
		return (v3((float)depth, 0.0f, 0.0f));
	return (v3s(0.0f));
}

/* ------------------------------------------------------- shader.comp */

/* One sample for pixel (x, y): main() of the compute shader */
RT_DEV vec3	rt_pixel(const t_rt_scene *sc, const t_cuda_frame *fr, uint32_t x,
	uint32_t y)
{
	Ctx		c;
	vec3	ro;
	vec3	rd;
	vec3	pixel_pos;
	vec3	color;

	c.sc = sc;
	c.fr = fr;
	c.seed = (x + 1) * 1973u * (y + 1) * 9277u + (fr->sample_count + 1) * 6541u;
	rt_rand(c);
	rt_rand(c);
	if (fr->camera.defocus_angle <= 0.0f)
		ro = v3p(fr->camera.position);
	else
		ro = defocus_disk_sample(c);
	pixel_pos = v3p(fr->viewport.upper_left);
	pixel_pos += v3p(fr->viewport.pixel_delta_u)
		* ((float)x + rt_rand(c) - 0.5f);
	pixel_pos += v3p(fr->viewport.pixel_delta_v)
		* ((float)y + rt_rand(c) - 0.5f);
	rd = normalize(pixel_pos - ro);
	color = v3s(0.0f);
	if (fr->render_mode == 0)
		color = path_trace(c, ro, rd);
	else if (fr->render_mode == 1)
		color = path_trace_bvh(c, ro, rd);
	else if (fr->render_mode == 2)
		color = path_trace_normal(c, ro, rd);
	if (isnan(color.x) || isnan(color.y) || isnan(color.z))
		color = v3s(0.0f);
	return (color);
}

#endif
