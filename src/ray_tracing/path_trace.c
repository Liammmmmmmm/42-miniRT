/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_trace.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 11:48:23 by lilefebv          #+#    #+#             */
/*   Updated: 2025/05/08 12:57:12 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_fcolor	color_to_fcolor(t_color color)
{
	return ((t_fcolor){color.r / 255.0, color.g / 255.0, color.b / 255.0});
}

t_fcolor	add_fcolor(t_fcolor c1, t_fcolor c2)
{
	return ((t_fcolor){
		.r = c1.r + c2.r,
		.g = c1.g + c2.g,
		.b = c1.b + c2.b
	});
}

t_fcolor	multiply_fcolor(t_fcolor c1, t_fcolor c2)
{
	return ((t_fcolor){
		.r = c1.r * c2.r,
		.g = c1.g * c2.g,
		.b = c1.b * c2.b
	});
}

t_fcolor	multiply_scalar_fcolor(t_fcolor c1, double c)
{
	return ((t_fcolor){
		.r = c1.r * c,
		.g = c1.g * c,
		.b = c1.b * c
	});
}
/*



t_ray_data	dielectric_material(t_mat_manager *mat_man)
{
	if (hit_record->mat->ior > 0)
	{
		if (hit_record->mat->transmission == 1.0)
			return (dielectric_transmissive_material(mat_man));
		else if (hit_record->mat->transmission == 0.0)
			return (dielectric_non_transmissive_material(mat_man));
		else
		{
			if (random_double() > hit_record->mat->transmission)
				return (dielectric_non_transmissive_material(mat_man));
			else
				return (dielectric_transmissive_material(mat_man));
		}
	}
	else
		return (material_default(mat_man));
}

t_ray_data	material_mix_v2(t_mat_manager *mat_man)
{
	if (hit_record->mat->emission_strength > 0.0)
		return ((t_ray_data){emissive_material(mat_man), EMISSIVE});
	else
	{
		if (hit_record->mat->metallic_value == 1.0)
			return (metallic_material(mat_man));
		else if (hit_record->mat->metallic_value == 0.0)
			return (dielectric_material(mat_man));
		else
		{
			if (random_double() > hit_record->mat->metallic_value)
				return (dielectric_material(mat_man));
			else
				return (metallic_material(mat_man));
		}
	}
	return ((t_ray_data){(t_color){0, 0, 0}, DEFFAULT});
}



*/

void	reflected_dielectric_color_itt(t_ray *ray, t_hit_record	*hit_record, t_fcolor *throughput)
{
	t_vec3	direction;

	direction = vec3_subtract(ray->dir, \
		vec3_multiply_scalar(hit_record->normal, 2 * \
			vec3_dot(ray->dir, hit_record->normal)));
	if (hit_record->mat->roughness_value > 0.0)
		direction = vec3_add(vec3_unit(direction), vec3_multiply_scalar(\
			vec3_random_unit(), hit_record->mat->roughness_value));
	ray->dir = vec3_unit(direction);

	*throughput = multiply_fcolor(*throughput, color_to_fcolor(hit_record->color));
}

void	metallic_color_itt(t_ray *ray, t_hit_record	*hit_record, t_fcolor *throughput)
{
	t_vec3			direction;
	const double	cos_theta = get_cos_theta(ray->dir, \
		hit_record->normal);

	direction = vec3_subtract(ray->dir, \
		vec3_multiply_scalar(hit_record->normal, 2 * \
			vec3_dot(ray->dir, hit_record->normal)));
	if (hit_record->mat->roughness_value > 0.0)
		direction = vec3_add(vec3_unit(direction), \
			vec3_multiply_scalar(vec3_random_unit(), \
			hit_record->mat->roughness_value));
	ray->dir = vec3_unit(direction);
	*throughput = multiply_fcolor(*throughput, color_to_fcolor(fresnel_schlick_color(cos_theta, hit_record->color)));
}

static inline t_vec3	refracted_v(const t_vec3 uv, const t_vec3 n, double ri)
{
	t_vec3	r_out_perp;

	r_out_perp = vec3_multiply_scalar(vec3_add(uv, \
		vec3_multiply_scalar(n, fmin(vec3_dot(vec3_negate(uv), n), 1.0))), ri);
	return (vec3_add(vec3_multiply_scalar(n, \
		-sqrt(fabs(1.0 - vec3_length_squared(r_out_perp)))), r_out_perp));
}

static inline void	refracted_value_itt(t_ray *ray, t_hit_record *hit_record, t_fcolor *throughput, double eta)
{
	t_vec3		direction;

	direction = refracted_v(ray->dir, \
		hit_record->normal, eta);
	if (hit_record->mat->roughness_value > 0.0)
		direction = vec3_add(vec3_unit(direction), \
			vec3_multiply_scalar(vec3_random_unit(), \
			hit_record->mat->roughness_value));
	ray->dir = vec3_unit(direction);
	*throughput = multiply_fcolor(*throughput, color_to_fcolor(hit_record->color));
}

static inline void	refracted_ray_itt(t_minirt *minirt, t_ray *ray, t_hit_record *hit_record, t_fcolor *throughput)
{
	double		eta;
	double		cos_theta;
	double		sin_theta;

	if (hit_record->front_face)
		eta = minirt->scene.ior_all / hit_record->mat->ior;
	else
		eta = hit_record->mat->ior / minirt->scene.ior_all;
	ray->dir = vec3_unit(ray->dir);
	cos_theta = fmin(-vec3_dot(ray->dir, \
		hit_record->normal), 1.0);
	sin_theta = sqrt(1.0 - cos_theta * cos_theta);
	if (eta * sin_theta > 1.0)
		metallic_color_itt(ray, hit_record, throughput);
	else
		refracted_value_itt(ray, hit_record, throughput, eta);
}

static inline void	default_mat_itt(t_minirt *minirt, t_ray *ray, t_hit_record *hit_record, t_fcolor *throughput, t_fcolor *accumulated_color)
{
	t_vec3 direction = vec3_add(hit_record->normal, vec3_random_unit());
	if (fabs(direction.x) < 1e-8 && fabs(direction.y) < 1e-8 && fabs(direction.z) < 1e-8)
		direction = hit_record->normal; // A changer plus tard selon le materiau, la on revoie juste une direction rand en mode materiau mat

	ray->dir = direction;
	*accumulated_color = add_fcolor(*accumulated_color,
		multiply_fcolor(multiply_fcolor(color_to_fcolor(hit_record->color), *throughput), compute_light_v2(hit_record, minirt))
	);
	*throughput = multiply_fcolor(*throughput, color_to_fcolor(hit_record->color));
}

static inline void	dielectric_mat_itt(t_minirt *minirt, t_ray *ray, t_hit_record *hit_record, t_fcolor *throughput, t_fcolor *accumulated_color)
{
	if (hit_record->mat->ior > 0)
	{
		if (get_reflect_value_v2(ray, hit_record) >= random_double())
			reflected_dielectric_color_itt(ray, hit_record, throughput);
		else
		{
			if (hit_record->mat->transmission == 1.0)
				refracted_ray_itt(minirt, ray, hit_record, throughput);
			else if (hit_record->mat->transmission == 1.0)
				default_mat_itt(minirt, ray, hit_record, throughput, accumulated_color);
			else
			{
				if (hit_record->mat->transmission < random_double())
					default_mat_itt(minirt, ray, hit_record, throughput, accumulated_color);
				else
					refracted_ray_itt(minirt, ray, hit_record, throughput);
			}
		}
	}
	else
	{
		default_mat_itt(minirt, ray, hit_record, throughput, accumulated_color);
	}
}


t_fcolor	path_trace(t_minirt *minirt, t_ray ray, int max_depth)
{
	t_fcolor		accumulated_color;
	t_fcolor		throughput;
	t_hit_record	hit_record; 
	int				bounce;

	bounce = 0;
	accumulated_color = (t_fcolor) {0.0, 0.0, 0.0};
	throughput = (t_fcolor) {1.0, 1.0, 1.0};
	while (bounce < max_depth)
	{
		if (hit_register_all(minirt, &ray, &hit_record) == 1)
		{
			// ray.orig = vec3_add(hit_record.point, vec3_multiply_scalar(ray.dir, -1e-4));
			ray.orig = hit_record.point;

			if (hit_record.mat)
			{
				
				if (hit_record.mat->emission_strength > 0)
				{
					accumulated_color = add_fcolor(
						accumulated_color,
						multiply_fcolor(multiply_scalar_fcolor(color_to_fcolor(hit_record.mat->emission_color), hit_record.mat->emission_strength), throughput)
					);
				}

				if (hit_record.mat->metallic_value == 1.0)
					metallic_color_itt(&ray, &hit_record, &throughput);
				else if (hit_record.mat->metallic_value == 0.0)
					dielectric_mat_itt(minirt, &ray, &hit_record, &throughput, &accumulated_color);
				else
				{
					if (hit_record.mat->metallic_value > random_double())
						metallic_color_itt(&ray, &hit_record, &throughput);
					else
						dielectric_mat_itt(minirt, &ray, &hit_record, &throughput, &accumulated_color);
				}
			}
			else
			{
				default_mat_itt(minirt, &ray, &hit_record, &throughput, &accumulated_color);
			}
		}
		else
		{
			accumulated_color = add_fcolor(accumulated_color, 
				multiply_fcolor(
					multiply_scalar_fcolor(color_to_fcolor(get_background_color(minirt, ray)), minirt->scene.amb_light.ratio),
					throughput)
			); // accumulated += throughtput * background * ambiant;
			break ;
		}
		
		bounce++;
	}
	return (accumulated_color);
}





t_fcolor	debug_path_trace(t_minirt *minirt, t_ray ray, int max_depth)
{
	t_fcolor		accumulated_color;
	t_fcolor		throughput;
	t_hit_record	hit_record; 
	int				bounce;

	bounce = 0;
	accumulated_color = (t_fcolor) {0.0, 0.0, 0.0};
	throughput = (t_fcolor) {1.0, 1.0, 1.0};
	while (bounce < max_depth && bounce < 10)
	{
		minirt->controls.traced_ray[bounce] = ray.orig;
		printf("hit : ");
		print_vec3(minirt->controls.traced_ray[bounce]);
		printf(" - dir: ");
		print_vec3(ray.dir);
		printf("\n");

		if (hit_register_all(minirt, &ray, &hit_record) == 1)
		{
			// ray.orig = vec3_add(hit_record.point, vec3_multiply_scalar(ray.dir, -1e-4));
			ray.orig = hit_record.point;

			if (hit_record.mat)
			{
				
				if (hit_record.mat->emission_strength > 0)
				{
					accumulated_color = add_fcolor(
						accumulated_color,
						multiply_fcolor(multiply_scalar_fcolor(color_to_fcolor(hit_record.mat->emission_color), hit_record.mat->emission_strength), throughput)
					);
				}

				if (hit_record.mat->metallic_value == 1.0)
					metallic_color_itt(&ray, &hit_record, &throughput);
				else if (hit_record.mat->metallic_value == 0.0)
					dielectric_mat_itt(minirt, &ray, &hit_record, &throughput, &accumulated_color);
				else
				{
					if (hit_record.mat->metallic_value > random_double())
						metallic_color_itt(&ray, &hit_record, &throughput);
					else
						dielectric_mat_itt(minirt, &ray, &hit_record, &throughput, &accumulated_color);
				}
			}
			else
			{
				default_mat_itt(minirt, &ray, &hit_record, &throughput, &accumulated_color);
			}
		}
		else
		{
			accumulated_color = add_fcolor(accumulated_color, 
				multiply_fcolor(
					multiply_scalar_fcolor(color_to_fcolor(get_background_color(minirt, ray)), minirt->scene.amb_light.ratio),
					throughput)
			); // accumulated += throughtput * background * ambiant;
			minirt->controls.traced_ray[bounce + 1] = vec3_add(ray.orig, vec3_multiply_scalar(ray.dir, 4));
			printf("skybox : ");
			print_vec3(minirt->controls.traced_ray[bounce + 1]);
			printf(" - dir: ");
			print_vec3(ray.dir);
			printf("\n");

			break ;
		}
		
		bounce++;
	}
	return (accumulated_color);
}
