/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellucas <ellucas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 16:00:00 by jynra             #+#    #+#             */
/*   Updated: 2025/05/25 12:45:39 by ellucas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"

static void	print_banner(void);
static void	print_usage(void);
static int	handle_arguments(int argc, char **argv);

int	main(int argc, char **argv)
{
	t_game	game;
	int		arg_result;

	print_banner();
	arg_result = handle_arguments(argc, argv);
	if (arg_result != 0)
		return (arg_result);
	if (!game_init(&game))
	{
		debug_error("Failed to initialize game");
		return (1);
	}
	debug_info("Game initialized successfully");
	game_run(&game);
	game_cleanup(&game);
	debug_info("Game shutdown complete");
	return (0);
}

static void	print_banner(void)
{
	printf("\n");
	printf("╔══════════════════════════════════════════════════════════════╗\n");
	printf("║                    VERSUS TD CLEAN                           ║\n");
	printf("║                Tower Defense - Clean Edition                ║\n");
	printf("║                                                              ║\n");
	printf("║  Version: 1.0.0                                             ║\n");
	printf("║  Built: %s %s                               ║\n", __DATE__, __TIME__);
	printf("║  Architecture: Clean & Modular                              ║\n");
	printf("╚══════════════════════════════════════════════════════════════╝\n");
	printf("\n");
}

static void	print_usage(void)
{
	printf("Usage: %s [options]\n", "versus_td_clean");
	printf("\n");
	printf("Options:\n");
	printf("  -h, --help     Show this help message\n");
	printf("  -v, --version  Show version information\n");
	printf("  -d, --debug    Enable debug mode\n");
	printf("  --windowed     Force windowed mode\n");
	printf("  --fullscreen   Force fullscreen mode\n");
	printf("\n");
}

static int	handle_arguments(int argc, char **argv)
{
	int	i;

	if (argc == 1)
		return (0);
	i = 1;
	while (i < argc)
	{
		if (string_compare(argv[i], "-h") == 0 
			|| string_compare(argv[i], "--help") == 0)
		{
			print_usage();
			return (0);
		}
		else if (string_compare(argv[i], "-v") == 0 
			|| string_compare(argv[i], "--version") == 0)
		{
			printf("Versus TD Clean v1.0.0\n");
			return (0);
		}
		else if (string_compare(argv[i], "-d") == 0 
			|| string_compare(argv[i], "--debug") == 0)
		{
			debug_info("Debug mode enabled");
		}
		else
		{
			printf("Unknown option: %s\n", argv[i]);
			print_usage();
			return (1);
		}
		i++;
	}
	return (0);
}