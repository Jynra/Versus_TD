/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellucas <ellucas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 11:25:42 by ellucas           #+#    #+#             */
/*   Updated: 2025/04/20 01:49:21 by ellucas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/core/game.h"

/**
 * @brief Point d'entrée du programme
 * 
 * @return int Code de retour (0 en cas de succès)
 */
int	main(void)
{
	t_game			game;
	t_input_state	input;
	Uint32			current_time;
	Uint32			previous_time;
	float			delta_time;
	
	/* Initialisation du jeu */
	if (!game_init(&game))
		return (1);
	
	/* Initialisation de la gestion des entrées */
	input_init(&input);
	
	/* Initialisation des temps pour le calcul du delta time */
	previous_time = SDL_GetTicks();
	
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
		
		/* Mise à jour du jeu */
		game_update(&game, delta_time);
		
		/* Rendu du jeu */
		game_render(&game);
		
		/* Limitation de la fréquence d'images */
		if (FRAME_DELAY > (SDL_GetTicks() - current_time))
			SDL_Delay(FRAME_DELAY - (SDL_GetTicks() - current_time));
	}
	
	/* Nettoyage des ressources */
	game_cleanup(&game);
	
	return (0);
}