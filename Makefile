# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ellucas <ellucas@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/19 15:56:12 by student           #+#    #+#              #
#    Updated: 2025/04/19 02:17:09 by ellucas          ###   ########.fr        #
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

# Fichiers source et objets
SRCS = srcs/entities.c \
       srcs/init.c \
       srcs/input.c \
       srcs/main.c \
       srcs/render.c \
       srcs/update.c \
       srcs/utils.c

OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Règle par défaut
all: $(NAME)

# Création du répertoire des objets
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Règle pour générer l'exécutable
$(NAME): $(OBJ_DIR) $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

# Règle pour compiler chaque fichier source en objet
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyage
clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re