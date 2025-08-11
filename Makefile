
# Program name
NAME			= cub3D

# Compiler and flags
CC				= cc
CFLAGS			= -Wall -Wextra -Werror -O3 -g3

# Directories
SRC_DIR			= src
OBJ_DIR			= obj
INCLUDES_DIR	= includes
LIBFT_DIR		= libft

# Platform detection
OS				= $(shell uname)

# Source files (updated to match your original structure)
MAIN_FILES		= main.c \
				  memory_management.c

PARSING_FILES	= parsing/parse_main.c \
				  parsing/parse_utils.c \
				  parsing/parse_rgb_texture.c \
				  parsing/map_statistics.c \
				  parsing/wall_validation.c

GAME_FILES		= game/game_init.c \
				  game/input_handling.c \
				  game/player_movement.c

RAYCAST_FILES	= raycasting/raycasting.c \
				  raycasting/raycasting_utils.c \
				  raycasting/rendering.c \
				  raycasting/drawing.c \
				  raycasting/minimap.c

# All source files
SRCS			= $(addprefix $(SRC_DIR)/, $(MAIN_FILES) $(PARSING_FILES) $(GAME_FILES) $(RAYCAST_FILES))

# Object files
OBJS			= $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Library files
LIBFT			= $(LIBFT_DIR)/libft.a

# Include flags
INCLUDES		= -I$(INCLUDES_DIR) -I$(LIBFT_DIR)

# Math library
MATH_LIB		= -lm

# Platform-specific MLX configuration
ifeq ($(OS), Linux)
	# Linux with system-installed MLX
	INCLUDES		+= -I/usr/include
	MLX_FLAGS		= -L/usr/lib -lmlx -lXext -lX11
	MLX_LIB			= # No local MLX library needed
else ifeq ($(OS), Darwin)
	# macOS - check for system MLX or use local
	MLX_DIR			= minilibx
	MLX_LIB			= $(MLX_DIR)/libmlx.a
	INCLUDES		+= -I$(MLX_DIR)
	MLX_FLAGS		= -L$(MLX_DIR) -lmlx -framework OpenGL -framework AppKit
endif

# Colors for pretty output
RED				= \033[0;31m
GREEN			= \033[0;32m
YELLOW			= \033[0;33m
BLUE			= \033[0;34m
MAGENTA			= \033[0;35m
CYAN			= \033[0;36m
WHITE			= \033[0;37m
RESET			= \033[0m

# Emojis
SUCCESS			= ✅
BUILD			= 🔨
CLEAN			= 🧹

# ================================== RULES =================================== #

# Default target
all: $(LIBFT) $(MLX_LIB) $(OBJ_DIR) $(NAME)

# Create object directory
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

# Build libft
$(LIBFT):
	@echo "$(BUILD) $(CYAN)Making libft...$(RESET)"
	@$(MAKE) -C $(LIBFT_DIR) --no-print-directory
	@echo "$(SUCCESS) $(YELLOW)Made libft!$(RESET)"

# Build MinilibX (macOS only)
ifeq ($(OS), Darwin)
$(MLX_LIB):
	@echo "$(BUILD) $(CYAN)Making MinilibX...$(RESET)"
	@$(MAKE) -C $(MLX_DIR) --no-print-directory
	@echo "$(SUCCESS) $(YELLOW)Made MinilibX!$(RESET)"
endif

# Create the main executable
$(NAME): $(OBJS) Makefile
	@echo "$(BUILD) $(GREEN)Compiling $(NAME)...$(RESET)"
	@$(CC) $(CFLAGS) $(OBJS) -L$(LIBFT_DIR) -lft $(MLX_FLAGS) $(MATH_LIB) -o $(NAME)
	@echo "$(SUCCESS) $(YELLOW)Compiling $(NAME) FINISHED$(RESET)"

# Compile object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo "$(YELLOW)Compiling $<...$(RESET)"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Clean object files
clean:
	@rm -rf $(OBJ_DIR)
	@$(MAKE) -C $(LIBFT_DIR) clean --no-print-directory
ifeq ($(OS), Darwin)
	@if [ -d "$(MLX_DIR)" ]; then $(MAKE) -C $(MLX_DIR) clean --no-print-directory; fi
endif
	@echo "$(CLEAN) $(RED)Cleaned!$(RESET)"

# Clean everything
fclean: clean
	@rm -f $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean --no-print-directory
	@echo "$(CLEAN) $(RED)Full Cleaned!$(RESET)"

# Rebuild everything
re: fclean all

# Bonus target (with additional features)
bonus: CFLAGS += -DBONUS
bonus: all
	@echo "$(MAGENTA)$(SUCCESS) Bonus features enabled!$(RESET)"

# Debug target with additional debugging flags
debug: CFLAGS += -fsanitize=address -DDEBUG
debug: all
	@echo "$(CYAN)$(SUCCESS) Debug version compiled with AddressSanitizer!$(RESET)"

# Help target
help:
	@echo "$(CYAN)Available targets:$(RESET)"
	@echo "  $(GREEN)all$(RESET)     - Build the project"
	@echo "  $(GREEN)clean$(RESET)   - Remove object files"
	@echo "  $(GREEN)fclean$(RESET)  - Remove object files and executable"
	@echo "  $(GREEN)re$(RESET)      - Rebuild the project"
	@echo "  $(GREEN)bonus$(RESET)   - Build with bonus features"
	@echo "  $(GREEN)debug$(RESET)   - Build debug version with AddressSanitizer"
	@echo "  $(GREEN)help$(RESET)    - Show this help message"

# Check for coding style (if norminette is available)
norm:
	@if command -v norminette >/dev/null 2>&1; then \
		echo "$(BLUE)Checking coding style...$(RESET)"; \
		norminette $(SRC_DIR) $(INCLUDES_DIR); \
	else \
		echo "$(YELLOW)⚠️  norminette not found, skipping style check$(RESET)"; \
	fi

# Test the program with a sample map
test: $(NAME)
	@echo "$(CYAN)Testing with sample map...$(RESET)"
	@if [ -f maps/sample.cub ]; then \
		./$(NAME) maps/sample.cub; \
	else \
		echo "$(RED)❌ No test map found at maps/sample.cub$(RESET)"; \
	fi

# Install dependencies
install-deps:
	@echo "$(BLUE)Installing dependencies...$(RESET)"
ifeq ($(OS), Linux)
	@if command -v apt >/dev/null 2>&1; then \
		sudo apt update && sudo apt install -y build-essential libx11-dev libxext-dev libmlx-dev; \
	elif command -v yum >/dev/null 2>&1; then \
		sudo yum groupinstall -y "Development Tools" && sudo yum install -y libX11-devel libXext-devel; \
	else \
		echo "$(YELLOW)⚠️  Package manager not detected. Please install X11 and MLX development libraries manually.$(RESET)"; \
	fi
else ifeq ($(OS), Darwin)
	@echo "$(GREEN)On macOS, using local MinilibX or system-installed version$(RESET)"
endif

# Show project info
info:
	@echo "$(CYAN)Project Information:$(RESET)"
	@echo "  Name: $(GREEN)$(NAME)$(RESET)"
	@echo "  Compiler: $(GREEN)$(CC)$(RESET)"
	@echo "  Flags: $(GREEN)$(CFLAGS)$(RESET)"
	@echo "  Platform: $(GREEN)$(OS)$(RESET)"
	@echo "  Source files: $(GREEN)$(words $(SRCS))$(RESET)"
	@echo "  Object files: $(GREEN)$(words $(OBJS))$(RESET)"
	@echo "  MLX: $(GREEN)System-installed (Linux) / Local (macOS)$(RESET)"

# Declare phony targets
.PHONY: all clean fclean re bonus debug help norm test install-deps info