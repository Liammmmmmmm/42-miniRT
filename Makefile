# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/17 09:42:48 by lilefebv          #+#    #+#              #
#    Updated: 2025/08/06 12:04:21 by lilefebv         ###   ########lyon.fr    #
#                                                                              #
# **************************************************************************** #

# Program name
NAME     = miniRT

# Style
GREY     = \033[0;30m
RED      = \033[0;31m
GREEN    = \033[0;32m
YELLOW   = \033[0;33m
BLUE     = \033[0;34m
PURPLE   = \033[0;35m
CYAN     = \033[0;36m
WHITE    = \033[0;37m

BOLD     = \033[1m
UNDER    = \033[4m
REV      = \033[7m
BLINK    = \033[5m

NC       = \033[0;0m
ERASE    = \033[2K\r
ERASE2   = $(ERASE)\033[F$(ERASE)

SEED_TMP = $(shell if command -v od >/dev/null 2>&1; then od -vAn -N8 -tu8 < /dev/urandom | tr -d ' '; fi)
SEED	 = $(if $(SEED_TMP),$(SEED_TMP),42)

# Compiler and flags
CC       = gcc

DEPFLAGS = -MMD -MP
CFLAGS   = $(DEPFLAGS) -DRANDOM_SEED=$(SEED) -Wall -Wextra #-Werror # -mavx # SIMD flag
LDFLAGS  = -L$(MINILIBXDIR) -lXext -lX11 -lm -lXfixes
GPU_FLAGS = -Wall -Wextra -g3 -DUSE_GPU -lGL -lGLEW -lglfw -ldl $(DEPFLAGS)
DEBUG_FLAGS = -g3
FAST_FLAGS = -O3 -flto -march=native -mtune=native -funroll-loops -ffast-math -falign-functions=32 -falign-loops=16
# -O3 -march=native -mtune=native -flto -funsafe-math-optimizations -ffast-math -fomit-frame-pointer -funroll-loops -fno-exceptions -fno-rtti -fno-stack-protector -DNDEBUG -falign-functions=32 -falign-loops=16

ifeq ($(MAKECMDGOALS), debug)
	CFLAGS += $(DEBUG_FLAGS)
endif
ifeq ($(MAKECMDGOALS), fast)
	CC     = gcc
	CFLAGS   = $(DEPFLAGS) -Wall -Wextra
	CFLAGS += $(FAST_FLAGS)
endif

# libs
LIBFTDIR = libft/
LIBFT    = $(LIBFTDIR)libft.a

MINILIBXDIR = minilibx/
MINILIBX    = $(MINILIBXDIR)libmlx_Linux.a

# Includes
INCLUDES = -I includes/ -I $(LIBFTDIR)includes/ -I $(MINILIBXDIR)

# Source files mandatory
SRC_DIR				= src/
SRC_FILE			= main.c

CAUSTIC_DIR			= src/caustic/
CAUSTIC_FILE		= kd_tree/kd_tree_build_utils.c kd_tree/kd_tree_build.c kd_tree/kd_tree_destroy.c kd_tree/kd_tree_print.c kd_tree/kd_tree_task.c \
					kd_tree/kd_tree_find_near.c caustic_manager.c trace_photon_path.c get_caustic.c

UTILS_DIR			= src/utils/
UTILS_FILE			= utils.c bmp/bmp_parser.c bmp/bmp_extract_header.c \
					bmp/bmp_extract_color.c bmp/bmp_extract_pixel.c \
					bump_to_normal.c progress_bar.c obj/obj_ear_clipping.c \
					obj/obj_extract_value.c obj/obj_finilizer.c \
					obj/obj_parser_utils.c obj/obj_parser.c obj/obj_utils.c \
					obj/obj_vertex_cache.c obj/obj_extract_value_utils.c obj/obj_parse_face.c \
					obj/obj_vertex_cache_utils.c obj/obj_vertex_utils.c print_error.c print_warn.c

UTILS_MLX_DIR		= src/utils/mlx/
UTILS_MLX_FILE		= font.c button.c editable_text.c slider_int.c string.c draw_circles.c \
					conversions.c color_picker.c color_picker2.c color_picker3.c float_input.c \
					float_input2.c float_input3.c expr_float.c

UI_DIR				= src/ui/
UI_FILE				= controls/button_click.c controls/init_buttons.c controls/clear_buttons.c controls/init_float_inputs.c controls/init_float_inputs2.c controls/init_text_inputs.c \
					controls/init_sliders.c controls/init_color_picker.c controls/init_dropdown.c controls/init_dropdown2.c controls/add_material.c controls/add_object.c controls/add_object2.c controls/rm_object.c \
					render/layout.c render/layout_left.c render/material_tab.c render/object_prop2.c render/object_prop3.c render/object_prop.c render/objects_tab.c \
					mouse_events/global.c mouse_events/materials.c mouse_events/objects2.c mouse_events/objects.c mouse_events/utils.c\
					render.c list_objects.c list_materials.c selected_object.c selected_object2.c selected_mat.c \
					dropdown.c dropdown2.c materials_dropdown.c default_dropdown.c textures_dropdown.c

DEBUG_DIR			= src/debug/
DEBUG_FILE			= print_scene.c print_utils.c print_obj1.c print_obj2.c print_obj3.c debug_ray.c

EVENT_DIR			= src/events/
EVENT_FILE			= destroy.c  hooks.c  key_common.c  key_controls.c key_render.c key_render2.c mouse_common.c  mouse_controls.c  mouse_render.c scroll_actions.c

RAY_TRACING_DIR		= src/ray_tracing/
RAY_TRACING_FILE	= render.c manage_movement.c init_animated_items.c focus.c bvh/bvh_manager.c bvh/bvh_math.c bvh/bvh_make_lst.c bvh/bvh_utils.c bvh/qshort_axis.c bvh/bvh_draw.c \
					bvh/bvh_draw_utils.c bvh/bvh_obj_bounds.c setup_scene_obj.c background.c viewport.c path_trace.c hit_register/apply_map.c hit_register/get_hdr_value.c \
					hit_register/get_hit_color.c hit_register/get_tex_color.c hit_register/hit_register.c hit_register/hit_obj.c bvh/bvh_hit.c micrort.c bvh/bvh_build.c \
					importance_sampling/get_dir.c importance_sampling/calc_pdf.c importance_sampling/calc_cdf.c importance_sampling/importance_sampling_manager.c \
					importance_sampling/init_maloc_importance_sampling.c calc_sample.c light/d_light.c light/p_light.c light/light_manager.c

TEXTURES_DIR		= src/ray_tracing/textures/
TEXTURES_FILE		= get_solid_texture.c

RENDERING_DIR		= src/rendering/
RENDERING_FILE		= pixel.c loop.c loop_utils.c no_display.c

ENV_DIR				= src/env/
ENV_FILE			= init_mlx.c free_mlx.c init_controls.c init_ui.c set_dependant_values.c \
					micrort_init.c set_dependant_values_objs.c set_dependant_values_objs2.c \
					set_new_win_size.c

MAT_DIR				= src/material/
MAT_FILE			= utils.c dielectric.c material_default.c metallic.c refraction.c

MATH_DIR			= src/math/
MATH_FILE			= vector/vec3_operations.c ray/ray.c vector/vec3_dot_cross.c vector/vec3_length.c vector/vec3_lerp.c \
					vector/vec3_utils.c vector/vec3_random.c vector/vec3_operation_scalar.c normal/normal.c \
					color/fcolor_operation.c ft_dmin.c plane/hit_plane.c sphere/hit_sphere.c random.c \
					matrix/matrix.c matrix/matrix_calc.c angle/angle_math.c matrix/matrix3.c fresnel_schlick.c \
					cylinder/hit_cylinder.c quadratique/quadratique.c quadratique/quadratique_hyperboloid.c \
					cone/hit_cone.c valid_t.c clamp_int.c hyperboloid/hit_hyperboloid.c triangle/hit_triangle.c \
					cylinder/cylinder_uv.c color/fcolor_clamp.c color/conversions.c clamp_double.c color/get_heat_map_color.c

PARSING_DIR			= src/parsing/
PARSING_FILE		= parse_scene.c errors.c errors2.c errors3.c valid_line.c \
					tranform_line.c verify_elements.c parse_elements.c \
					parse_elements2.c parse_elements3.c parse_elements4.c  parse_elements_utils.c \
					parse_elements_utils2.c parse_elements_utils3.c parse_elements_utils4.c \
					parse_elements_utils5.c parse_elements_utils6.c get_texture.c parse_elements5.c \
					free_scene.c init_scene.c parse_elements6.c

FONT_PARS_DIR		= src/utils/font/parsing/
FONT_PARS_FILE		= free.c get_glyph_outline.c get_glyph_outline_xy.c parse_ttf.c read_cmap.c \
					read_font_directory.c read_format4.c read_glyph.c read_maxp.c read_head.c \
					read_loca.c utils.c get_bezier.c get_bezier_utils.c read_hhea.c read_hmtx.c \
					set_bezier_res.c

FONT_REND_DIR		= src/utils/font/rendering/
FONT_REND_FILE		= draw_glyph_outline.c draw_string.c basic_slope.c tesselate_bezier.c rasterization.c

HDR_PARSING_DIR		= src/utils/hdr/
HDR_PARSING_FILE	= parse_hdr.c parse_hdr_header.c parse_hdr_data.c parse_hdr_utils.c hdr_set_pixel.c

PPM_DIR				= src/utils/ppm/
PPM_FILE			= export_ppm.c

EDIT_MOD_DIR		= src/edition_mod/
EDIT_MOD_FILE		= select_obj.c movements.c movements_utils.c

UPSCALING_DIR		= src/upscaling/
UPSCALING_FILE		= bilinear.c bicubic.c utils.c no_upscaling.c

OPTIONS_DIR			= src/options/
OPTIONS_FILE		= options.c animation.c animation_err.c animation_move_points.c animation_parse.c animation_tesselate.c animation_rotations.c animation_auto_rota.c

PNG_DIR				= src/utils/png/
PNG_FILE			= bit_stream.c dynamic_huffman_block.c parse_png_header.c png_filters.c decode_deflate_utils.c \
					generate_huffman_codes.c parse_png_idat.c read_deflate.c decode_table.c huffman_table.c \
					parse_png_utils.c zlib_block.c decompress_data.c parse_png.c png_debug.c decompress_data_utils.c \
					parse_png_chunk.c png_filter.c

M_FILE	=	$(addprefix $(SRC_DIR), $(SRC_FILE)) \
			$(addprefix $(UTILS_DIR), $(UTILS_FILE)) \
			$(addprefix $(DEBUG_DIR), $(DEBUG_FILE)) \
			$(addprefix $(EVENT_DIR), $(EVENT_FILE)) \
			$(addprefix $(RAY_TRACING_DIR), $(RAY_TRACING_FILE)) \
			$(addprefix $(TEXTURES_DIR), $(TEXTURES_FILE)) \
			$(addprefix $(RENDERING_DIR), $(RENDERING_FILE)) \
			$(addprefix $(MAT_DIR), $(MAT_FILE)) \
			$(addprefix $(MATH_DIR), $(MATH_FILE)) \
			$(addprefix $(UTILS_MLX_DIR), $(UTILS_MLX_FILE)) \
			$(addprefix $(UI_DIR), $(UI_FILE)) \
			$(addprefix $(PARSING_DIR), $(PARSING_FILE)) \
			$(addprefix $(FONT_PARS_DIR), $(FONT_PARS_FILE)) \
			$(addprefix $(FONT_REND_DIR), $(FONT_REND_FILE)) \
			$(addprefix $(ENV_DIR), $(ENV_FILE)) \
			$(addprefix $(EDIT_MOD_DIR), $(EDIT_MOD_FILE)) \
			$(addprefix $(UPSCALING_DIR), $(UPSCALING_FILE)) \
			$(addprefix $(PPM_DIR), $(PPM_FILE)) \
			$(addprefix $(HDR_PARSING_DIR), $(HDR_PARSING_FILE)) \
			$(addprefix $(OPTIONS_DIR), $(OPTIONS_FILE)) \
			$(addprefix $(PNG_DIR), $(PNG_FILE)) \
			$(addprefix $(CAUSTIC_DIR), $(CAUSTIC_FILE))

SRC_DIR_GPU			= src/
SRC_FILE_GPU		= main_shader.c

GPU_DIR				= src/gpu/
GPU_FILE			= init_shader.c compile_shader.c clean_shaders.c \
					use_shader.c gpu_scene.c send_uniforms.c importance_sampling.c \
					convert_utils.c convert_objects.c convert_materials.c convert_textures.c \
					count.c convert_others.c convert_primitives.c convert_caustic.c convert_caustic_utils.c 
					

RAY_TRACING_DIR_GPU		= src/ray_tracing/
RAY_TRACING_FILE_GPU	= render_gpu.c manage_movement.c init_animated_items.c focus.c bvh/bvh_manager.c bvh/bvh_math.c bvh/bvh_make_lst.c bvh/bvh_utils.c bvh/qshort_axis.c bvh/bvh_draw.c \
					bvh/bvh_draw_utils.c bvh/bvh_obj_bounds.c setup_scene_obj.c background.c viewport.c path_trace.c hit_register/apply_map.c hit_register/get_hdr_value.c \
					hit_register/get_hit_color.c hit_register/get_tex_color.c hit_register/hit_register.c hit_register/hit_obj.c bvh/bvh_hit.c micrort.c bvh/bvh_build.c \
					importance_sampling/get_dir.c importance_sampling/calc_pdf.c importance_sampling/calc_cdf.c importance_sampling/importance_sampling_manager.c \
					importance_sampling/init_maloc_importance_sampling.c calc_sample.c light/d_light.c light/p_light.c light/light_manager.c

ENV_DIR_GPU			= src/env/
ENV_FILE_GPU		= init_mlx.c free_mlx.c init_controls.c init_ui.c set_dependant_values.c \
					micrort_init.c set_dependant_values_objs.c set_dependant_values_objs2.c \
					set_new_win_size_gpu.c


NETWORK_DIR			= src/network/
NETWORK_FILE		= client/active_mode.c client/client.c client/passive_mode.c client/send_frame.c \
					server/cli.c server/connect_passive_client.c server/handle_client.c \
					server/info.c server/password.c server/server.c server/signal.c \
					server/send_scene.c server/init_scene.c \
					utils.c

OPTIONS_DIR_GPU		= src/options/
OPTIONS_FILE_GPU	= options_server.c animation.c animation_err.c animation_move_points.c \
					animation_parse.c animation_tesselate.c animation_rotations.c \
					animation_auto_rota.c server.c client.c

RENDERING_DIR_GPU	= src/rendering/
RENDERING_FILE_GPU	= pixel.c loop.c loop_utils.c loop_server.c no_display.c


M_FILE_GPU = $(addprefix $(SRC_DIR_GPU), $(SRC_FILE_GPU)) \
			$(addprefix $(UTILS_DIR), $(UTILS_FILE)) \
			$(addprefix $(DEBUG_DIR), $(DEBUG_FILE)) \
			$(addprefix $(EVENT_DIR), $(EVENT_FILE)) \
			$(addprefix $(RAY_TRACING_DIR_GPU), $(RAY_TRACING_FILE_GPU)) \
			$(addprefix $(TEXTURES_DIR), $(TEXTURES_FILE)) \
			$(addprefix $(RENDERING_DIR_GPU), $(RENDERING_FILE_GPU)) \
			$(addprefix $(MAT_DIR), $(MAT_FILE)) \
			$(addprefix $(MATH_DIR), $(MATH_FILE)) \
			$(addprefix $(UTILS_MLX_DIR), $(UTILS_MLX_FILE)) \
			$(addprefix $(UI_DIR), $(UI_FILE)) \
			$(addprefix $(PARSING_DIR), $(PARSING_FILE)) \
			$(addprefix $(FONT_PARS_DIR), $(FONT_PARS_FILE)) \
			$(addprefix $(FONT_REND_DIR), $(FONT_REND_FILE)) \
			$(addprefix $(ENV_DIR_GPU), $(ENV_FILE_GPU)) \
			$(addprefix $(EDIT_MOD_DIR), $(EDIT_MOD_FILE)) \
			$(addprefix $(UPSCALING_DIR), $(UPSCALING_FILE)) \
			$(addprefix $(PPM_DIR), $(PPM_FILE)) \
			$(addprefix $(HDR_PARSING_DIR), $(HDR_PARSING_FILE)) \
			$(addprefix $(OPTIONS_DIR_GPU), $(OPTIONS_FILE_GPU)) \
			$(addprefix $(PNG_DIR), $(PNG_FILE)) \
			$(addprefix $(CAUSTIC_DIR), $(CAUSTIC_FILE)) \
			$(addprefix $(GPU_DIR), $(GPU_FILE)) \
			$(addprefix $(NETWORK_DIR), $(NETWORK_FILE))

# Source files bonus
SRCS_BONUS = 

# Object files directory
OBJ_DIR   = .obj/
OBJ       = $(M_FILE:%.c=$(OBJ_DIR)%.o)
OBJ_BONUS = $(B_FILE:%.c=$(OBJ_DIR)%.o)

OBJ_GPU = $(M_FILE_GPU:%.c=$(OBJ_DIR)%.o)

# Remake all if modified
REMAKE   = libft/includes/libft.h libft/includes/ft_printf.h \
		libft/includes/get_next_line.h \
		includes/bmp_parsing.h includes/bvh.h includes/font.h includes/font_structs.h \
		includes/material.h includes/maths.h includes/minirt.h \
		includes/mlx_base.h includes/mlx_components.h includes/structs.h includes/ui.h \
		includes/utils.h includes/obj_parsing.h includes/hit_register.h \
		includes/importance_sampling.h includes/caustic.h
		# libft/Makefile  Makefile

# NORMINETTE
NORM_RET = $(RED)[ERROR]$(BOLD) Norminette Disable$(NC)
# NORM	 = $(shell norminette srcs includes | grep -c 'Error!')
# ifeq ($(NORM), 0)
# 	NORM_RET = $(GREEN)[DONE] $(BOLD)$(YELLOW)Norminette.$(NC)
# else
# 	NORM_RET = $(RED)[ERROR] $(BOLD)$(YELLOW)Norminette.$(NC)
# endif

COMPILED_FILES := 0

# Pattern rule for object files
$(OBJ_DIR)%.o : %.c $(REMAKE)
	@if [ $(COMPILED_FILES) -eq 0 ]; then \
		echo "\n$(YELLOW)╔══════════════════════════════════════════════╗$(NC)";                          \
		echo "$(YELLOW)║        Starting $(YELLOW2)$(NAME)$(YELLOW) compilation...        ║$(NC)";           \
		echo "$(YELLOW)╚══════════════════════════════════════════════╝$(NC)";                        \
	fi
	@$(eval COMPILED_FILES := 1)
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES) -g3
	@printf "\n$(GREEN)[Compiling] $(NC)$(shell echo $< | sed 's|^srcs/||')";

all : $(NAME) nothing_to_be_done

nothing_to_be_done:
	@if [ $(COMPILED_FILES) -eq 0 ]; then \
		echo "\n$(YELLOW)╔══════════════════════════════════════════════╗$(NC)";                          \
		echo "$(YELLOW)║        Nothing to be done for $(YELLOW2)$(NAME)$(YELLOW).        ║$(NC)";           \
		echo "$(YELLOW)╚══════════════════════════════════════════════╝$(NC)\n";                          \
	fi

end_message:
	@echo ""
	@./print_image 40 40 < assets/make_image/res
	@echo "\n$(NORM_RET)";

$(NAME) : $(MINILIBX) $(LIBFT) $(OBJ)
	@if [ $(COMPILED_FILES) -eq 0 ]; then \
		echo "\n$(YELLOW)╔══════════════════════════════════════════════╗$(NC)";                          \
		echo "$(YELLOW)║        Starting $(YELLOW2)$(NAME)$(YELLOW) compilation...        ║$(NC)";           \
		echo "$(YELLOW)╚══════════════════════════════════════════════╝$(NC)";                        \
	fi
	@$(eval COMPILED_FILES := 1)
	@echo "\n\n$(GREEN)[Compiling program] $(NC)$(NAME)"
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LIBFT) $(MINILIBX) $(LDFLAGS)
	@make --no-print-directory end_message

$(NAME)_gpu: $(MINILIBX) $(LIBFT) $(OBJ_GPU)
	@if [ $(COMPILED_FILES) -eq 0 ]; then \
		echo "\n$(YELLOW)╔══════════════════════════════════════════════╗$(NC)";                          \
		echo "$(YELLOW)║        Starting $(YELLOW2)$(NAME)_gpu$(YELLOW) compilation...        ║$(NC)";           \
		echo "$(YELLOW)╚══════════════════════════════════════════════╝$(NC)";                        \
	fi
	@$(eval COMPILED_FILES := 1)
	@echo "\n\n$(GREEN)[Compiling program] $(NC)$(NAME)_gpu"
	@$(CC) $(CFLAGS) -o $(NAME)_gpu $(OBJ_GPU) $(LIBFT) $(MINILIBX) $(LDFLAGS) $(GPU_FLAGS)
	@make --no-print-directory end_message

gpu: $(NAME)_gpu nothing_to_be_done

make_libft:
	@make --no-print-directory -C $(LIBFTDIR) all

make_mlx:
	@make --no-print-directory -C $(MINILIBXDIR) all

$(LIBFT): make_libft

$(MINILIBX): make_mlx


clean :
	@make --no-print-directory -C $(LIBFTDIR) clean
	@make --no-print-directory -C $(MINILIBXDIR) clean
	@echo "$(RED)[Removing] $(NC)object files"
	@rm -rf $(OBJ_DIR)

fclean : clean
	@make --no-print-directory -C $(LIBFTDIR) fclean
	@if [ -f $(NAME) ]; then \
		echo "$(RED)[Removing] $(NC)program $(NAME)"; \
		rm -f $(NAME); \
	fi

fcleanp :
	@echo "$(RED)[Removing] $(NC)object files"
	@rm -rf $(OBJ_DIR)
	@if [ -f $(NAME) ]; then \
		echo "$(RED)[Removing] $(NC)program $(NAME)"; \
		rm -f $(NAME); \
	fi

re : fclean
	@make --no-print-directory all

fast: all

debug: all 

ffast: fcleanp
	@make --no-print-directory fast

fdebug: fcleanp
	@make --no-print-directory debug

norminette:
	@norminette src/ libft/ includes/

.PHONY: all clean fclean nothing_to_be_done re end_message norminette fast ffast debug fdebug fcleanp