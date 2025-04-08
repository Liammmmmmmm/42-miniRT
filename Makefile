# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/17 09:42:48 by lilefebv          #+#    #+#              #
#    Updated: 2025/04/08 19:53:49 by lilefebv         ###   ########lyon.fr    #
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

# Compiler and flags
CC       = cc
CFLAGS   = -Wall -Wextra -Werror
LDFLAGS  = -L$(MINILIBXDIR) -lXext -lX11 -lm
DEBUG_FLAGS = -g3
FAST_FLAGS = -O3 -flto -march=native -funroll-loops

ifeq ($(MAKECMDGOALS), debug)
	CFLAGS += $(DEBUG_FLAGS)
endif
ifeq ($(MAKECMDGOALS), fast)
	CFLAGS += $(FAST_FLAGS)
endif

# libs
LIBFTDIR = libft/
LIBFT    = $(LIBFTDIR)libft.a

MINILIBXDIR = minilibx/
MINILIBX    = $(MINILIBXDIR)libmlx.a

# Includes
INCLUDES = -I includes/ -I $(LIBFTDIR)includes/ -I $(MINILIBXDIR)

# Source files mandatory
SRC_DIR				= src/
SRC_FILE			= main.c

UTILS_DIR			= src/utils/
UTILS_FILE			= utils.c

UTILS_MLX_DIR		= src/utils/mlx/
UTILS_MLX_FILE		= font.c button.c editable_text.c slider_int.c string.c draw_circles.c

CONTROLS_DIR		= src/controls/
CONTROLS_FILE		= button_click.c init_buttons.c init_sliders.c

DEBUG_DIR			= src/debug/
DEBUG_FILE			= print_scene.c

EVENT_DIR			= src/events/
EVENT_FILE			= basic.c

RAY_TRACING_DIR		= src/ray_tracing/
RAY_TRACING_FILE	= render.c hit_register.c random.c material_reflection.c

RENDERING_DIR		= src/rendering/
RENDERING_FILE		= pixel.c loop.c

ENV_DIR				= src/env/
ENV_FILE			= init_mlx.c init_controls.c init_ui.c

MATH_DIR			= src/math/
MATH_FILE			= vector/vec3_operations.c ray/ray.c vector/vec3_dot_cross.c vector/vec3_length.c \
					vector/vec3_utils.c normal/normal.c\
					plane/hit_plane.c sphere/hit_sphere.c

PARSING_DIR			= src/parsing/
PARSING_FILE		= parse_scene.c errors.c errors2.c valid_line.c \
					tranform_line.c verify_elements.c parse_elements.c \
					parse_elements2.c parse_elements3.c parse_elements_utils.c \
					parse_elements_utils2.c parse_elements_utils3.c parse_elements_utils4.c

FONT_PARS_DIR		= src/utils/font/parsing/
FONT_PARS_FILE		= debug.c free.c get_glyph_outline.c get_glyph_outline_xy.c parse_ttf.c read_cmap.c \
					read_font_directory.c read_format4.c read_glyph.c read_head.c \
					read_loca.c utils.c get_bezier.c

FONT_REND_DIR		= src/utils/font/rendering/
FONT_REND_FILE		= get_points.c basic_slope.c tesselate_bezier.c


M_FILE	=	$(addprefix $(SRC_DIR), $(SRC_FILE)) \
			$(addprefix $(UTILS_DIR), $(UTILS_FILE)) \
			$(addprefix $(DEBUG_DIR), $(DEBUG_FILE)) \
			$(addprefix $(EVENT_DIR), $(EVENT_FILE)) \
			$(addprefix $(RAY_TRACING_DIR), $(RAY_TRACING_FILE)) \
			$(addprefix $(RENDERING_DIR), $(RENDERING_FILE)) \
			$(addprefix $(MATH_DIR), $(MATH_FILE)) \
			$(addprefix $(UTILS_MLX_DIR), $(UTILS_MLX_FILE)) \
			$(addprefix $(CONTROLS_DIR), $(CONTROLS_FILE)) \
			$(addprefix $(PARSING_DIR), $(PARSING_FILE)) \
			$(addprefix $(FONT_PARS_DIR), $(FONT_PARS_FILE)) \
			$(addprefix $(FONT_REND_DIR), $(FONT_REND_FILE)) \
			$(addprefix $(ENV_DIR), $(ENV_FILE))

# Source files bonus
SRCS_BONUS = 

# Object files directory
OBJ_DIR   = .obj/
OBJ       = $(M_FILE:%.c=$(OBJ_DIR)%.o)
OBJ_BONUS = $(B_FILE:%.c=$(OBJ_DIR)%.o)

# Remake all if modified
REMAKE   = libft/includes/libft.h libft/includes/ft_printf.h \
		libft/includes/get_next_line.h libft/Makefile  Makefile \
		includes/structs.h includes/minirt.h

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
	@$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES)
	@printf "\n$(GREEN)[Compiling] $(NC)$(shell echo $< | sed 's|^srcs/||')";

all : make_libft make_mlx $(NAME) nothing_to_be_done

nothing_to_be_done:
	@if [ $(COMPILED_FILES) -eq 0 ]; then \
		echo "\n$(YELLOW)╔══════════════════════════════════════════════╗$(NC)";                          \
		echo "$(YELLOW)║        Nothing to be done for $(YELLOW2)$(NAME)$(YELLOW).        ║$(NC)";           \
		echo "$(YELLOW)╚══════════════════════════════════════════════╝$(NC)\n";                          \
	fi

end_message:
	@echo "$(WHITE)\n╔══════════════════════════════════════════════╗$(NC)"
	@echo "$(WHITE)║ $(GREEN)> miniRT                                     $(WHITE)║$(NC)"
	@echo "$(WHITE)╚══════════════════════════════════════════════╝$(NC)"
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