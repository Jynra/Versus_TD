# Versus_TD

A simple Tower Defense game written in C using SDL2.

## Overview

Versus_TD is a grid-based tower defense game where players strategically place towers to defend against waves of enemies moving along a path. The game features multiple tower types with different abilities, resource management, and progressively challenging gameplay.

## Features

- **Multiple Tower Types**:
  - Basic Tower: Balanced damage and range
  - Slow Tower: Reduces enemy movement speed
  - Multi-Target Tower: Can attack multiple enemies at once

- **Resource Management**: Players earn money by defeating enemies and spend it to build towers

- **Simple Interface**: Grid-based gameplay with intuitive controls

- **Visual Feedback**: Tower range indicators, enemy health bars, and projectile animations

## Requirements

- SDL2
- SDL2_image
- SDL2_ttf
- C compiler (GCC recommended)
- Make

## Installation

1. Install the required dependencies:

```bash
# On Debian/Ubuntu
sudo apt-get install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev

# On macOS with Homebrew
brew install sdl2 sdl2_image sdl2_ttf

# On Fedora
sudo dnf install SDL2-devel SDL2_image-devel SDL2_ttf-devel
```

2. Clone the repository:

```bash
git clone https://github.com/yourusername/Versus_TD.git
cd Versus_TD
```

3. Compile the game:

```bash
make
```

## How to Play

1. Launch the game:

```bash
./versus_td
```

2. Game Controls:
   - Click on tower buttons in the toolbar to select a tower
   - Click on the grid to place the selected tower
   - Press `ESC` to cancel tower placement
   - Press `SPACE` to spawn enemies (for testing)
   - Number keys `1`, `2`, `3` to quickly select different tower types

3. Game Objective:
   - Prevent enemies from reaching the end of the path
   - Build towers strategically to maximize damage and efficiency
   - Manage your resources to build the most effective defense

## Code Structure

The project follows the 42 School Norm and is organized as follows:

- `includes/`: Header files
  - `config.h`: Game constants and configuration
  - `game.h`: Main game functions and structures
  - `types.h`: Game object type definitions
  - `utils.h`: Utility functions

- `srcs/`: Source files
  - `entities.c`: Enemy, tower, and projectile logic
  - `init.c`: Game initialization functions
  - `input.c`: User input handling
  - `main.c`: Main game loop
  - `render.c`: Rendering functions
  - `update.c`: Game state update logic
  - `utils.c`: Utility functions implementation

## Development Notes

This game follows the 42 School Norm, which includes specific coding standards:
- Function length limited to 25 lines
- Maximum 80 columns per line
- Specific naming conventions
- Restricted use of certain language features

## Future Improvements

- More enemy types with different behaviors
- Additional tower types with unique abilities
- Level design with varied path layouts
- Sound effects and music
- Player profiles and high scores
- Wave system with increasing difficulty

## License

This project is available under the MIT License - see the LICENSE file for details.

## Credits

Developed by [Your Name] as a project for 42 School.