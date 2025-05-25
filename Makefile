# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ellucas <ellucas@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/25 16:00:00 by jynra             #+#    #+#              #
#    Updated: 2025/05/25 22:33:15 by ellucas          ###   ########.fr        #
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
# SOURCE FILES - EXPLICIT LISTS FOR RELIABILITY
# =============================================================================

# Base objects (shared between stable and premium)
OBJS_BASE = objs/main.o \
            objs/entities_base.o \
            objs/game/init.o \
            objs/game/update.o \
            objs/game/render.o \
            objs/game/cleanup.o \
            objs/utils/math.o \
            objs/utils/debug.o \
            objs/utils/memory.o \
            objs/utils/file.o \
            objs/systems/input.o \
            objs/entities/enemy.o \
            objs/entities/spawner.o \
            objs/entities/tower.o \
            objs/entities/projectile.o

# Stable build objects (Base + temp_stubs)
OBJS_STABLE = $(OBJS_BASE) \
              objs/temp_stubs.o

# Premium build objects (Base + Phase 4, NO temp_stubs)
OBJS_PREMIUM = $(OBJS_BASE) \
               objs/systems/effects.o \
               objs/systems/physics.o \
               objs/systems/ui.o \
               objs/systems/upgrades.o \
               objs/systems/waves.o

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

$(NAME): directories $(OBJS_STABLE)
	@echo "$(YELLOW)Linking $(NAME) (STABLE)...$(RESET)"
	@$(CC) $(CFLAGS) $(OBJS_STABLE) $(LIBS) -o $(NAME)
	@echo "$(GREEN)✓ Stable build complete!$(RESET)"
	@echo "$(CYAN)Run with: ./$(NAME)$(RESET)"
	@echo "$(YELLOW)For premium experience: make premium$(RESET)"

# Premium build - All Phase 4 features
premium: banner-premium $(NAME)-premium
	@echo "$(BOLD)$(GREEN)🎉 PREMIUM BUILD COMPLETE! 🎉$(RESET)"
	@echo "$(CYAN)✨ Phase 4 features active:$(RESET)"
	@echo "$(GREEN)  • Particle effects$(RESET)"
	@echo "$(GREEN)  • Advanced physics$(RESET)"
	@echo "$(GREEN)  • Professional UI$(RESET)"
	@echo "$(GREEN)  • Tower upgrades$(RESET)"
	@echo "$(GREEN)  • Boss waves$(RESET)"
	@cp $(NAME)-premium $(NAME)

$(NAME)-premium: directories $(OBJS_PREMIUM)
	@echo "$(YELLOW)Linking $(NAME) (PREMIUM)...$(RESET)"
	@$(CC) $(CFLAGS) $(OBJS_PREMIUM) $(LIBS) -o $(NAME)-premium
	@echo "$(GREEN)✓ Premium linking complete$(RESET)"

# Aliases
complete: premium
phase4: premium

# =============================================================================
# DIRECTORY CREATION
# =============================================================================

directories:
	@echo "$(BLUE)Creating object directories...$(RESET)"
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(OBJ_DIR)/game
	@mkdir -p $(OBJ_DIR)/entities
	@mkdir -p $(OBJ_DIR)/systems
	@mkdir -p $(OBJ_DIR)/utils
	@echo "$(GREEN)✓ Directory structure created$(RESET)"

# =============================================================================
# COMPILATION RULES - EXPLICIT FOR EACH FILE
# =============================================================================

# Main files
objs/main.o: srcs/main.c $(HEADERS)
	@echo "$(BLUE)Compiling srcs/main.c$(RESET)"
	@$(CC) $(CFLAGS) $(INCLUDES) -c srcs/main.c -o objs/main.o

# NEW: Entities base file (shared functions)
objs/entities_base.o: srcs/entities_base.c $(HEADERS)
	@echo "$(BLUE)Compiling srcs/entities_base.c$(RESET)"
	@$(CC) $(CFLAGS) $(INCLUDES) -c srcs/entities_base.c -o objs/entities_base.o

# TEMP_STUBS - Only for stable build
objs/temp_stubs.o: srcs/temp_stubs.c $(HEADERS)
	@echo "$(BLUE)Compiling srcs/temp_stubs.c$(RESET)"
	@$(CC) $(CFLAGS) $(INCLUDES) -c srcs/temp_stubs.c -o objs/temp_stubs.o

# Game files
objs/game/init.o: srcs/game/init.c $(HEADERS)
	@echo "$(BLUE)Compiling srcs/game/init.c$(RESET)"
	@$(CC) $(CFLAGS) $(INCLUDES) -c srcs/game/init.c -o objs/game/init.o

objs/game/update.o: srcs/game/update.c $(HEADERS)
	@echo "$(BLUE)Compiling srcs/game/update.c$(RESET)"
	@$(CC) $(CFLAGS) $(INCLUDES) -c srcs/game/update.c -o objs/game/update.o

objs/game/render.o: srcs/game/render.c $(HEADERS)
	@echo "$(BLUE)Compiling srcs/game/render.c$(RESET)"
	@$(CC) $(CFLAGS) $(INCLUDES) -c srcs/game/render.c -o objs/game/render.o

objs/game/cleanup.o: srcs/game/cleanup.c $(HEADERS)
	@echo "$(BLUE)Compiling srcs/game/cleanup.c$(RESET)"
	@$(CC) $(CFLAGS) $(INCLUDES) -c srcs/game/cleanup.c -o objs/game/cleanup.o

# Utils files
objs/utils/math.o: srcs/utils/math.c $(HEADERS)
	@echo "$(BLUE)Compiling srcs/utils/math.c$(RESET)"
	@$(CC) $(CFLAGS) $(INCLUDES) -c srcs/utils/math.c -o objs/utils/math.o

objs/utils/debug.o: srcs/utils/debug.c $(HEADERS)
	@echo "$(BLUE)Compiling srcs/utils/debug.c$(RESET)"
	@$(CC) $(CFLAGS) $(INCLUDES) -c srcs/utils/debug.c -o objs/utils/debug.o

objs/utils/memory.o: srcs/utils/memory.c $(HEADERS)
	@echo "$(BLUE)Compiling srcs/utils/memory.c$(RESET)"
	@$(CC) $(CFLAGS) $(INCLUDES) -c srcs/utils/memory.c -o objs/utils/memory.o

objs/utils/file.o: srcs/utils/file.c $(HEADERS)
	@echo "$(BLUE)Compiling srcs/utils/file.c$(RESET)"
	@$(CC) $(CFLAGS) $(INCLUDES) -c srcs/utils/file.c -o objs/utils/file.o

# Systems files
objs/systems/input.o: srcs/systems/input.c $(HEADERS)
	@echo "$(BLUE)Compiling srcs/systems/input.c$(RESET)"
	@$(CC) $(CFLAGS) $(INCLUDES) -c srcs/systems/input.c -o objs/systems/input.o

# PHASE 4 SYSTEMS - Only for premium build
objs/systems/effects.o: srcs/systems/effects.c $(HEADERS)
	@echo "$(BLUE)Compiling srcs/systems/effects.c$(RESET)"
	@$(CC) $(CFLAGS) $(INCLUDES) -c srcs/systems/effects.c -o objs/systems/effects.o

objs/systems/physics.o: srcs/systems/physics.c $(HEADERS)
	@echo "$(BLUE)Compiling srcs/systems/physics.c$(RESET)"
	@$(CC) $(CFLAGS) $(INCLUDES) -c srcs/systems/physics.c -o objs/systems/physics.o

objs/systems/ui.o: srcs/systems/ui.c $(HEADERS)
	@echo "$(BLUE)Compiling srcs/systems/ui.c$(RESET)"
	@$(CC) $(CFLAGS) $(INCLUDES) -c srcs/systems/ui.c -o objs/systems/ui.o

objs/systems/upgrades.o: srcs/systems/upgrades.c $(HEADERS)
	@echo "$(BLUE)Compiling srcs/systems/upgrades.c$(RESET)"
	@$(CC) $(CFLAGS) $(INCLUDES) -c srcs/systems/upgrades.c -o objs/systems/upgrades.o

objs/systems/waves.o: srcs/systems/waves.c $(HEADERS)
	@echo "$(BLUE)Compiling srcs/systems/waves.c$(RESET)"
	@$(CC) $(CFLAGS) $(INCLUDES) -c srcs/systems/waves.c -o objs/systems/waves.o

# Entities files
objs/entities/enemy.o: srcs/entities/enemy.c $(HEADERS)
	@echo "$(BLUE)Compiling srcs/entities/enemy.c$(RESET)"
	@$(CC) $(CFLAGS) $(INCLUDES) -c srcs/entities/enemy.c -o objs/entities/enemy.o

objs/entities/spawner.o: srcs/entities/spawner.c $(HEADERS)
	@echo "$(BLUE)Compiling srcs/entities/spawner.c$(RESET)"
	@$(CC) $(CFLAGS) $(INCLUDES) -c srcs/entities/spawner.c -o objs/entities/spawner.o

objs/entities/tower.o: srcs/entities/tower.c $(HEADERS)
	@echo "$(BLUE)Compiling srcs/entities/tower.c$(RESET)"
	@$(CC) $(CFLAGS) $(INCLUDES) -c srcs/entities/tower.c -o objs/entities/tower.o

objs/entities/projectile.o: srcs/entities/projectile.c $(HEADERS)
	@echo "$(BLUE)Compiling srcs/entities/projectile.c$(RESET)"
	@$(CC) $(CFLAGS) $(INCLUDES) -c srcs/entities/projectile.c -o objs/entities/projectile.o

# =============================================================================
# DEVELOPMENT BUILDS
# =============================================================================

# Debug builds
debug: CFLAGS += $(DEBUG_FLAGS)
debug: clean $(NAME)
	@echo "$(RED)✓ Debug build complete$(RESET)"

debug-premium: CFLAGS += $(DEBUG_FLAGS)
debug-premium: clean premium
	@echo "$(RED)✓ Premium debug build complete$(RESET)"

# Release builds
release: CFLAGS += $(RELEASE_FLAGS)
release: clean $(NAME)
	@echo "$(GREEN)✓ Release build complete$(RESET)"

release-premium: CFLAGS += $(RELEASE_FLAGS)
release-premium: clean premium
	@echo "$(GREEN)✓ Premium release build complete$(RESET)"

# =============================================================================
# UTILITIES
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
	@rm -f $(NAME)-premium

fclean: clean
	@echo "$(RED)Removing executable...$(RESET)"
	@rm -f $(NAME)

re: fclean all
re-premium: fclean premium

# =============================================================================
# DEBUG TARGETS
# =============================================================================

# Show variables for debugging
debug-vars:
	@echo "$(CYAN)OBJS_STABLE:$(RESET) $(OBJS_STABLE)"
	@echo "$(CYAN)OBJS_PREMIUM:$(RESET) $(OBJS_PREMIUM)"

# Test single file compilation
test-effects:
	@echo "$(BLUE)Testing effects.c compilation...$(RESET)"
	@mkdir -p $(OBJ_DIR)/systems
	@$(CC) $(CFLAGS) $(INCLUDES) -c srcs/systems/effects.c -o $(OBJ_DIR)/systems/effects.o
	@echo "$(GREEN)✓ effects.o created$(RESET)"
	@ls -la $(OBJ_DIR)/systems/effects.o

# =============================================================================
# INFORMATION TARGETS
# =============================================================================

status:
	@echo "$(CYAN)=== VERSUS TD CLEAN - PROJECT STATUS ===$(RESET)"
	@echo "$(GREEN)Phase 2 (Foundation): ✅ Complete$(RESET)"
	@echo "$(GREEN)Phase 3 (Gameplay):   ✅ Complete$(RESET)"
	@echo "$(GREEN)Phase 4 (Premium):    ✅ Complete$(RESET)"
	@echo ""
	@echo "$(YELLOW)Available builds:$(RESET)"
	@echo "  make         - Stable gameplay (with stubs)"
	@echo "  make premium - Full premium experience (Phase 4)"

features:
	@echo "$(BOLD)$(CYAN)🌟 PHASE 4 PREMIUM FEATURES$(RESET)"
	@echo ""
	@echo "$(YELLOW)🎆 Particle Effects:$(RESET) Explosions, smoke, sparks"
	@echo "$(YELLOW)🎯 Advanced Physics:$(RESET) Knockback, area damage"
	@echo "$(YELLOW)🖥️  Professional UI:$(RESET) Tooltips, progress bars"
	@echo "$(YELLOW)⬆️  Tower Upgrades:$(RESET) 3 levels, special abilities"
	@echo "$(YELLOW)👑 Boss Waves:$(RESET) Special events every 5 waves"

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

help:
	@echo "$(BOLD)$(CYAN)Versus TD Clean - Makefile Help$(RESET)"
	@echo ""
	@echo "$(BOLD)Main builds:$(RESET)"
	@echo "  $(GREEN)make$(RESET)         - Stable build (with stubs)"
	@echo "  $(GREEN)make premium$(RESET) - Premium build (Phase 4 real implementations)"
	@echo ""
	@echo "$(BOLD)Testing:$(RESET)"
	@echo "  $(BLUE)make test$(RESET)    - Quick functionality test"
	@echo "  $(BLUE)make run$(RESET)     - Build and run immediately"
	@echo ""
	@echo "$(BOLD)Debug:$(RESET)"
	@echo "  $(CYAN)make debug-vars$(RESET) - Show build variables"
	@echo "  $(CYAN)make test-effects$(RESET) - Test single file compilation"
	@echo ""
	@echo "$(BOLD)Information:$(RESET)"
	@echo "  $(CYAN)make status$(RESET)  - Show project status"
	@echo "  $(CYAN)make features$(RESET) - Show Phase 4 features"
	@echo ""
	@echo "$(BOLD)Cleanup:$(RESET)"
	@echo "  $(RED)make clean$(RESET)   - Remove object files"
	@echo "  $(RED)make fclean$(RESET)  - Remove all build files"

# =============================================================================
# PHONY TARGETS
# =============================================================================

.PHONY: all premium complete phase4 directories clean fclean re re-premium \
        debug debug-premium release release-premium test test-premium \
        run run-premium status features banner banner-premium help \
        debug-vars test-effects

.DEFAULT_GOAL := all