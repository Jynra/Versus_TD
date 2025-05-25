# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ellucas <ellucas@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/25 16:00:00 by jynra             #+#    #+#              #
#    Updated: 2025/05/25 13:31:28 by ellucas          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Program name
NAME = versus_td_clean

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99
INCLUDES = -Iincludes
LIBS = -lSDL2 -lSDL2_ttf -lm

# Debug flags
DEBUG_FLAGS = -g3 -fsanitize=address -fsanitize=undefined -DDEBUG
RELEASE_FLAGS = -O3 -DNDEBUG

# Platform detection
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
    INCLUDES += -I/usr/local/include -I/opt/homebrew/include
    LIBS += -L/usr/local/lib -L/opt/homebrew/lib
endif

# Directories
SRC_DIR = srcs
INC_DIR = includes
OBJ_DIR = objs

# =============================================================================
# PROGRESSIVE BUILD - ONLY IMPLEMENTED FILES
# =============================================================================

# Phase 2 - Current Implementation (Foundation + Utils)
PHASE2_SRCS = srcs/main.c \
              srcs/game/init.c \
              srcs/game/update.c \
              srcs/game/render.c \
              srcs/game/cleanup.c \
              srcs/utils/math.c \
              srcs/utils/debug.c \
              srcs/utils/memory.c \
              srcs/utils/file.c \
              srcs/systems/input.c \
              srcs/temp_stubs.c

# Phase 3 - Entities (To be implemented)
PHASE3_SRCS = srcs/entities/enemy.c \
              srcs/entities/tower.c \
              srcs/entities/projectile.c \
              srcs/entities/spawner.c

# Phase 4 - Advanced Systems (To be implemented)
PHASE4_SRCS = srcs/systems/physics.c \
              srcs/systems/effects.c \
              srcs/systems/waves.c \
              srcs/systems/ui.c \
              srcs/systems/upgrades.c

# =============================================================================
# CURRENT BUILD CONFIGURATION
# =============================================================================

# Use only Phase 2 files for now
SRCS = $(PHASE2_SRCS)

# Phase 3 build (when entities are ready)
SRCS_PHASE3 = $(PHASE2_SRCS) $(PHASE3_SRCS)

# Complete build (all phases)
SRCS_COMPLETE = $(PHASE2_SRCS) $(PHASE3_SRCS) $(PHASE4_SRCS)

# Object files
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Header files for dependency tracking
HEADERS = includes/config.h \
         includes/types.h \
         includes/game.h \
         includes/entities.h \
         includes/systems.h \
         includes/utils.h

# Colors for pretty output
RED = \033[0;31m
GREEN = \033[0;32m
YELLOW = \033[0;33m
BLUE = \033[0;34m
PURPLE = \033[0;35m
CYAN = \033[0;36m
WHITE = \033[0;37m
RESET = \033[0m

# Progress tracking
TOTAL_FILES = $(words $(SRCS))
COMPILED_FILES = 0

# =============================================================================
# BUILD TARGETS
# =============================================================================

# Default target (Phase 2)
all: banner $(NAME)
	@echo "$(GREEN)✓ Phase 2 build completed successfully!$(RESET)"
	@echo "$(CYAN)Run with: ./$(NAME)$(RESET)"

# Phase 3 build (when ready)
phase3: SRCS = $(SRCS_PHASE3)
phase3: OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
phase3: banner $(NAME)
	@echo "$(GREEN)✓ Phase 3 build completed successfully!$(RESET)"

# Complete build (all phases)
complete: SRCS = $(SRCS_COMPLETE)
complete: OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
complete: banner $(NAME)
	@echo "$(GREEN)✓ Complete build finished!$(RESET)"

# Create executable
$(NAME): $(OBJ_DIR) $(OBJS)
	@echo "$(YELLOW)Linking $(NAME)...$(RESET)"
	@$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(NAME)
	@echo "$(GREEN)✓ Linking complete$(RESET)"

# Create object directory structure
$(OBJ_DIR):
	@echo "$(BLUE)Creating build directories...$(RESET)"
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(OBJ_DIR)/game
	@mkdir -p $(OBJ_DIR)/entities
	@mkdir -p $(OBJ_DIR)/systems
	@mkdir -p $(OBJ_DIR)/utils

# Compile source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS)
	@$(eval COMPILED_FILES := $(shell echo $$(($(COMPILED_FILES) + 1))))
	@printf "$(YELLOW)[%2d/%2d]$(RESET) Compiling %-30s" $(COMPILED_FILES) $(TOTAL_FILES) "$<"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
	@echo " $(GREEN)✓$(RESET)"

# =============================================================================
# DEVELOPMENT TARGETS
# =============================================================================

# Debug build
debug: CFLAGS += $(DEBUG_FLAGS)
debug: clean banner $(NAME)
	@echo "$(RED)✓ Debug build complete$(RESET)"

# Release build
release: CFLAGS += $(RELEASE_FLAGS)
release: clean banner $(NAME)
	@echo "$(GREEN)✓ Release build complete$(RESET)"

# Test the current phase
test: $(NAME)
	@echo "$(BLUE)Testing current build...$(RESET)"
	@timeout 3s ./$(NAME) || echo "$(GREEN)✓ Build test passed$(RESET)"

# Show what files are included in current build
show-files:
	@echo "$(CYAN)Current build includes:$(RESET)"
	@for file in $(SRCS); do echo "  ✓ $$file"; done
	@echo ""
	@echo "$(YELLOW)Phase 3 will add:$(RESET)"
	@for file in $(PHASE3_SRCS); do echo "  ⏳ $$file"; done
	@echo ""
	@echo "$(BLUE)Phase 4 will add:$(RESET)"
	@for file in $(PHASE4_SRCS); do echo "  ⏳ $$file"; done

# Check what files are missing
check-missing:
	@echo "$(BLUE)Checking for missing source files...$(RESET)"
	@missing=0; \
	for file in $(SRCS); do \
		if [ ! -f "$$file" ]; then \
			echo "$(RED)❌ Missing: $$file$(RESET)"; \
			missing=$$((missing + 1)); \
		fi; \
	done; \
	if [ $$missing -eq 0 ]; then \
		echo "$(GREEN)✓ All required files present for current phase$(RESET)"; \
	else \
		echo "$(RED)❌ $$missing files missing$(RESET)"; \
	fi

# =============================================================================
# UTILITY TARGETS
# =============================================================================

# Clean object files
clean:
	@echo "$(RED)Cleaning build files...$(RESET)"
	@rm -rf $(OBJ_DIR)
	@echo "$(GREEN)✓ Clean complete$(RESET)"

# Full clean
fclean: clean
	@echo "$(RED)Removing executable...$(RESET)"
	@rm -f $(NAME)
	@echo "$(GREEN)✓ Full clean complete$(RESET)"

# Rebuild
re: fclean all

# Run the game
run: $(NAME)
	@echo "$(CYAN)Starting $(NAME)...$(RESET)"
	@./$(NAME)

# Count lines of code (current files only)
count:
	@echo "$(BLUE)Counting lines of code (current phase)...$(RESET)"
	@echo "$(CYAN)Source files:$(RESET)"
	@wc -l $(SRCS) 2>/dev/null | tail -1 || echo "Some files missing"
	@echo "$(CYAN)Header files:$(RESET)"
	@find $(INC_DIR) -name "*.h" | xargs wc -l | tail -1

# Show project statistics
stats:
	@echo "$(CYAN)=== Versus TD Clean - Project Statistics ===$(RESET)"
	@echo "$(WHITE)Current Phase:$(RESET) 2 (Foundation + Utils)"
	@echo "$(WHITE)Implemented files:$(RESET) $(words $(SRCS))"
	@echo "$(WHITE)Phase 3 files:$(RESET) $(words $(PHASE3_SRCS))"
	@echo "$(WHITE)Phase 4 files:$(RESET) $(words $(PHASE4_SRCS))"
	@echo "$(WHITE)Total planned:$(RESET) $(words $(SRCS_COMPLETE))"
	@echo "$(WHITE)Headers:$(RESET) $(words $(HEADERS))"
	@echo "$(WHITE)Progress:$(RESET) $(shell echo "scale=1; $(words $(SRCS)) * 100 / $(words $(SRCS_COMPLETE))" | bc)%"

# Show help
help:
	@echo "$(CYAN)Versus TD Clean - Progressive Makefile Help$(RESET)"
	@echo ""
	@echo "$(WHITE)Build Targets:$(RESET)"
	@echo "  $(GREEN)all$(RESET)              - Build current phase (Phase 2)"
	@echo "  $(GREEN)phase3$(RESET)           - Build with Phase 3 files (when ready)"
	@echo "  $(GREEN)complete$(RESET)         - Build complete project (when ready)"
	@echo "  $(GREEN)debug$(RESET)            - Build with debug symbols"
	@echo "  $(GREEN)release$(RESET)          - Build optimized version"
	@echo "  $(GREEN)clean$(RESET)            - Remove object files"
	@echo "  $(GREEN)fclean$(RESET)           - Remove all generated files"
	@echo "  $(GREEN)re$(RESET)               - Rebuild everything"
	@echo ""
	@echo "$(WHITE)Development:$(RESET)"
	@echo "  $(YELLOW)run$(RESET)              - Build and run the game"
	@echo "  $(YELLOW)test$(RESET)             - Quick build verification"
	@echo "  $(YELLOW)show-files$(RESET)       - Show current and planned files"
	@echo "  $(YELLOW)check-missing$(RESET)    - Check for missing files"
	@echo ""
	@echo "$(WHITE)Information:$(RESET)"
	@echo "  $(BLUE)count$(RESET)            - Count lines of code"
	@echo "  $(BLUE)stats$(RESET)            - Show project statistics"
	@echo "  $(BLUE)help$(RESET)             - Show this help"

# Project banner
banner:
	@echo "$(CYAN)"
	@echo "╔══════════════════════════════════════════════════════════════╗"
	@echo "║                 VERSUS TD CLEAN - PHASE 2                    ║"
	@echo "║              Foundation + Utilities Ready                    ║"
	@echo "║                                                              ║"
	@echo "║  Current: $(words $(SRCS)) files | Next: Phase 3 Entities             ║"
	@echo "╚══════════════════════════════════════════════════════════════╝"
	@echo "$(RESET)"

# Phony targets
.PHONY: all phase3 complete clean fclean re debug release run test \
        show-files check-missing count stats help banner

# Disable built-in rules
.SUFFIXES:

# Default goal
.DEFAULT_GOAL := all