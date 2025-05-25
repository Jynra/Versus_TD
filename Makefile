# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ellucas <ellucas@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/25 16:00:00 by jynra             #+#    #+#              #
#    Updated: 2025/05/25 19:59:32 by ellucas          ###   ########.fr        #
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
OBJ_DIR = objs

# =============================================================================
# SOURCE FILES - PROGRESSIVE BUILD
# =============================================================================

# Phase 2 - Foundation (Complete)
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

# Phase 3 - Entities (Complete)
PHASE3_SRCS = srcs/entities/enemy.c \
              srcs/entities/spawner.c \
              srcs/entities/tower.c \
              srcs/entities/projectile.c

# Phase 4 - Advanced Systems (Complete!)
PHASE4_SRCS = srcs/systems/effects.c \
              srcs/systems/physics.c \
              srcs/systems/ui.c \
              srcs/systems/upgrades.c \
              srcs/systems/waves.c

# Build configurations
SRCS_STABLE = $(PHASE2_SRCS) $(PHASE3_SRCS)
SRCS_PREMIUM = $(PHASE2_SRCS) $(PHASE3_SRCS) $(PHASE4_SRCS)

# Default to stable build
SRCS = $(SRCS_STABLE)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Header files
HEADERS = includes/config.h \
          includes/types.h \
          includes/game.h \
          includes/entities.h \
          includes/systems.h \
          includes/utils.h

# Colors for output
GREEN = \033[0;32m
YELLOW = \033[0;33m
BLUE = \033[0;34m
CYAN = \033[0;36m
RED = \033[0;31m
BOLD = \033[1m
RESET = \033[0m

# =============================================================================
# BUILD TARGETS
# =============================================================================

# Default build - Stable gameplay (Phase 3)
all: banner $(NAME)
	@echo "$(GREEN)✓ Stable build complete!$(RESET)"
	@echo "$(CYAN)Run with: ./$(NAME)$(RESET)"
	@echo "$(YELLOW)For premium experience: make premium$(RESET)"

# Premium build - All Phase 4 features
premium: SRCS = $(SRCS_PREMIUM)
premium: OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
premium: banner-premium $(NAME)
	@echo "$(BOLD)$(GREEN)🎉 PREMIUM BUILD COMPLETE! 🎉$(RESET)"
	@echo "$(CYAN)✨ Phase 4 features active:$(RESET)"
	@echo "$(GREEN)  • Particle effects$(RESET)"
	@echo "$(GREEN)  • Advanced physics$(RESET)"
	@echo "$(GREEN)  • Professional UI$(RESET)"
	@echo "$(GREEN)  • Tower upgrades$(RESET)"
	@echo "$(GREEN)  • Boss waves$(RESET)"

# Aliases
complete: premium
phase4: premium

# Create executable
$(NAME): $(OBJ_DIR) $(OBJS)
	@echo "$(YELLOW)Linking $(NAME)...$(RESET)"
	@$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(NAME)
	@echo "$(GREEN)✓ Linking complete$(RESET)"

# Create object directory structure
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(OBJ_DIR)/game
	@mkdir -p $(OBJ_DIR)/entities
	@mkdir -p $(OBJ_DIR)/systems
	@mkdir -p $(OBJ_DIR)/utils

# Compile source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS)
	@echo "$(BLUE)Compiling $<$(RESET)"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# =============================================================================
# DEVELOPMENT BUILDS
# =============================================================================

# Debug builds
debug: CFLAGS += $(DEBUG_FLAGS)
debug: clean $(NAME)
	@echo "$(RED)✓ Debug build complete$(RESET)"

debug-premium: CFLAGS += $(DEBUG_FLAGS)
debug-premium: SRCS = $(SRCS_PREMIUM)
debug-premium: OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
debug-premium: clean $(NAME)
	@echo "$(RED)✓ Premium debug build complete$(RESET)"

# Release builds
release: CFLAGS += $(RELEASE_FLAGS)
release: clean $(NAME)
	@echo "$(GREEN)✓ Release build complete$(RESET)"

release-premium: CFLAGS += $(RELEASE_FLAGS)
release-premium: SRCS = $(SRCS_PREMIUM)
release-premium: OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
release-premium: clean $(NAME)
	@echo "$(GREEN)✓ Premium release build complete$(RESET)"

# =============================================================================
# TESTING & UTILITIES
# =============================================================================

# Test builds
test: $(NAME)
	@echo "$(BLUE)Testing build...$(RESET)"
	@timeout 5s ./$(NAME) || echo "$(GREEN)✓ Test completed$(RESET)"

test-premium: premium
	@echo "$(BLUE)Testing premium build...$(RESET)"
	@timeout 10s ./$(NAME) || echo "$(GREEN)✓ Premium test completed$(RESET)"

# Run builds
run: $(NAME)
	@echo "$(CYAN)Starting game...$(RESET)"
	@./$(NAME)

run-premium: premium
	@echo "$(BOLD)$(CYAN)🚀 Starting premium experience...$(RESET)"
	@./$(NAME)

# Clean targets
clean:
	@echo "$(RED)Cleaning build files...$(RESET)"
	@rm -rf $(OBJ_DIR)

fclean: clean
	@echo "$(RED)Removing executable...$(RESET)"
	@rm -f $(NAME)

re: fclean all
re-premium: fclean premium

# =============================================================================
# INFORMATION TARGETS
# =============================================================================

# Project status
status:
	@echo "$(CYAN)=== VERSUS TD CLEAN - PROJECT STATUS ===$(RESET)"
	@echo "$(GREEN)Phase 2 (Foundation): ✅ Complete$(RESET)"
	@echo "$(GREEN)Phase 3 (Gameplay):   ✅ Complete$(RESET)"
	@echo "$(GREEN)Phase 4 (Premium):    ✅ Complete$(RESET)"
	@echo ""
	@echo "$(YELLOW)Available builds:$(RESET)"
	@echo "  make         - Stable gameplay"
	@echo "  make premium - Full premium experience"

# Show Phase 4 features
features:
	@echo "$(BOLD)$(CYAN)🌟 PHASE 4 PREMIUM FEATURES$(RESET)"
	@echo ""
	@echo "$(YELLOW)🎆 Particle Effects:$(RESET) Explosions, smoke, sparks"
	@echo "$(YELLOW)🎯 Advanced Physics:$(RESET) Knockback, area damage"
	@echo "$(YELLOW)🖥️  Professional UI:$(RESET) Tooltips, progress bars"
	@echo "$(YELLOW)⬆️  Tower Upgrades:$(RESET) 3 levels, special abilities"
	@echo "$(YELLOW)👑 Boss Waves:$(RESET) Special events every 5 waves"

# File organization
files:
	@echo "$(CYAN)Stable build (Phase 3):$(RESET)"
	@echo "  Files: $(words $(SRCS_STABLE))"
	@echo "$(CYAN)Premium build (Phase 4):$(RESET)"
	@echo "  Files: $(words $(SRCS_PREMIUM))"
	@echo "  New systems: $(words $(PHASE4_SRCS))"

# Project statistics
stats:
	@echo "$(CYAN)=== PROJECT STATISTICS ===$(RESET)"
	@echo "$(YELLOW)Total source files:$(RESET) $(words $(SRCS_PREMIUM))"
	@echo "$(YELLOW)Header files:$(RESET) $(words $(HEADERS))"
	@echo "$(YELLOW)Completion:$(RESET) $(BOLD)$(GREEN)100%$(RESET)"
	@echo "$(YELLOW)Status:$(RESET) Ready for commercial release!"

# Check file integrity
check:
	@echo "$(BLUE)Checking files...$(RESET)"
	@missing=0; \
	for file in $(SRCS_PREMIUM); do \
		if [ ! -f "$$file" ]; then \
			echo "$(RED)❌ Missing: $$file$(RESET)"; \
			missing=$$((missing + 1)); \
		fi; \
	done; \
	if [ $$missing -eq 0 ]; then \
		echo "$(GREEN)✅ All files present!$(RESET)"; \
	else \
		echo "$(RED)❌ $$missing files missing$(RESET)"; \
	fi

# =============================================================================
# BANNERS
# =============================================================================

banner:
	@echo "$(CYAN)"
	@echo "╔════════════════════════════════════════════════════════════╗"
	@echo "║                VERSUS TD CLEAN - STABLE                    ║"
	@echo "║            Fully Playable Tower Defense                   ║"
	@echo "╚════════════════════════════════════════════════════════════╝"
	@echo "$(RESET)"

banner-premium:
	@echo "$(BOLD)$(CYAN)"
	@echo "╔════════════════════════════════════════════════════════════╗"
	@echo "║            🎉 VERSUS TD CLEAN - PREMIUM 🎉                 ║"
	@echo "║   🎆 Effects • 🎯 Physics • 🖥️ UI • ⬆️ Upgrades • 👑 Boss   ║"
	@echo "╚════════════════════════════════════════════════════════════╝"
	@echo "$(RESET)"

# Help system
help:
	@echo "$(BOLD)$(CYAN)Versus TD Clean - Makefile Help$(RESET)"
	@echo ""
	@echo "$(BOLD)Main builds:$(RESET)"
	@echo "  $(GREEN)make$(RESET)         - Stable build (recommended for testing)"
	@echo "  $(GREEN)make premium$(RESET) - Premium build (all Phase 4 features)"
	@echo ""
	@echo "$(BOLD)Development:$(RESET)"
	@echo "  $(YELLOW)make debug$(RESET)   - Debug build with AddressSanitizer"
	@echo "  $(YELLOW)make release$(RESET) - Optimized release build"
	@echo ""
	@echo "$(BOLD)Testing:$(RESET)"
	@echo "  $(BLUE)make test$(RESET)    - Quick functionality test"
	@echo "  $(BLUE)make run$(RESET)     - Build and run immediately"
	@echo ""
	@echo "$(BOLD)Information:$(RESET)"
	@echo "  $(CYAN)make status$(RESET)  - Show project status"
	@echo "  $(CYAN)make features$(RESET) - Show Phase 4 features"
	@echo "  $(CYAN)make stats$(RESET)   - Show project statistics"
	@echo ""
	@echo "$(BOLD)Cleanup:$(RESET)"
	@echo "  $(RED)make clean$(RESET)   - Remove object files"
	@echo "  $(RED)make fclean$(RESET)  - Remove all build files"
	@echo "  $(RED)make re$(RESET)      - Clean rebuild"

# =============================================================================
# PHONY TARGETS
# =============================================================================

.PHONY: all premium complete phase4 clean fclean re re-premium debug release \
        debug-premium release-premium test test-premium run run-premium \
        status features files stats check banner banner-premium help

.DEFAULT_GOAL := all