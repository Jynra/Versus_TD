/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellucas <ellucas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 11:25:42 by ellucas           #+#    #+#             */
/*   Updated: 2025/04/20 12:51:07 by ellucas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/core/game.h"

int main(void)
{
    t_game          game;
    t_input_state   input;
    Uint32          current_time;
    Uint32          previous_time;
    float           delta_time;
    
    printf("Starting game initialization\n");
    
    /* Initialisation du jeu */
    if (!game_init(&game))
    {
        printf("Game initialization failed\n");
        return (1);
    }
    
    /* Initialisation de la gestion des entrées */
    input_init(&input);
    
    /* Initialisation des temps pour le calcul du delta time */
    previous_time = SDL_GetTicks();
    
    printf("Starting main game loop\n");
    
    /* Boucle principale du jeu */
    while (game.is_running)
    {
        /* Calcul du delta time pour des mouvements fluides */
        current_time = SDL_GetTicks();
        delta_time = (current_time - previous_time) / 1000.0f;
        previous_time = current_time;
        
        /* Limitation du delta time pour éviter les sauts lors des ralentissements */
        if (delta_time > 0.05f)
            delta_time = 0.05f;
            
        /* Gestion des événements */
        input_handle_events(&game, &input);
        
        /* IMPORTANT: Mettre à jour l'état des entrées */
        input_update(&game, &input);
        
        /* Vérification de la demande de fermeture */
        if (input.quit_requested)
        {
            printf("Quit requested\n");
            game.is_running = false;
        }
        
        /* Mise à jour du jeu */
        game_update(&game, delta_time);
        
        /* Rendu du jeu */
        game_render(&game);
        
        /* Limitation de la fréquence d'images */
        if (FRAME_DELAY > (SDL_GetTicks() - current_time))
            SDL_Delay(FRAME_DELAY - (SDL_GetTicks() - current_time));
    }
    
    printf("Exiting main loop\n");
    
    /* Nettoyage des ressources */
    game_cleanup(&game);
    
    printf("Game exit complete\n");
    return (0);
}