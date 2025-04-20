# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ellucas <ellucas@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/19 15:56:12 by student           #+#    #+#              #
#    Updated: 2025/04/20 01:29:49 by ellucas          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Nom du programme
NAME = versus_td

# Compilateur et options
CC = gcc
CFLAGS = -Wall -Wextra -Werror -Iincludes
LDFLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -lm

# Répertoires
SRC_DIR = srcs
INC_DIR = includes
OBJ_DIR = objs

# Structure des répertoires source
CORE_DIR = core
ENTITIES_DIR = entities
SYSTEMS_DIR = systems
UTILS_DIR = utils

# Création de la structure des répertoires d'objets
CORE_OBJ_DIR = $(OBJ_DIR)/$(CORE_DIR)
ENTITIES_OBJ_DIR = $(OBJ_DIR)/$(ENTITIES_DIR)
SYSTEMS_OBJ_DIR = $(OBJ_DIR)/$(SYSTEMS_DIR)
UTILS_OBJ_DIR = $(OBJ_DIR)/$(UTILS_DIR)

# Fichiers source par catégorie
CORE_SRCS = $(wildcard $(SRC_DIR)/$(CORE_DIR)/*.c)
ENTITIES_SRCS = $(wildcard $(SRC_DIR)/$(ENTITIES_DIR)/*.c)
SYSTEMS_SRCS = $(wildcard $(SRC_DIR)/$(SYSTEMS_DIR)/*.c)
UTILS_SRCS = $(wildcard $(SRC_DIR)/$(UTILS_DIR)/*.c)

# Tous les fichiers source
SRCS = $(CORE_SRCS) $(ENTITIES_SRCS) $(SYSTEMS_SRCS) $(UTILS_SRCS)

# Conversion des chemins source en chemins d'objets
CORE_OBJS = $(CORE_SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
ENTITIES_OBJS = $(ENTITIES_SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
SYSTEMS_OBJS = $(SYSTEMS_SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
UTILS_OBJS = $(UTILS_SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Tous les fichiers objets
OBJS = $(CORE_OBJS) $(ENTITIES_OBJS) $(SYSTEMS_OBJS) $(UTILS_OBJS)

# Règle par défaut
all: $(NAME)

# Création des répertoires d'objets
$(OBJ_DIR):
	mkdir -p $(CORE_OBJ_DIR) $(ENTITIES_OBJ_DIR) $(SYSTEMS_OBJ_DIR) $(UTILS_OBJ_DIR)

# Règle pour générer l'exécutable
$(NAME): $(OBJ_DIR) $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)
	@echo "Compilation terminée: $(NAME)"

# Règle pour compiler chaque fichier source en objet
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyage
clean:
	rm -rf $(OBJ_DIR)
	@echo "Fichiers objets supprimés"

fclean: clean
	rm -f $(NAME)
	@echo "Exécutable supprimé"

re: fclean all

# Règle pour vérifier les fuites mémoire avec valgrind
valgrind: $(NAME)
	valgrind --leak-check=full --show-leak-kinds=all ./$(NAME)

# Pour faciliter le debug
debug: CFLAGS += -g
debug: re

.PHONY: all clean fclean re valgrind debug