# Changelog

All notable changes to Galaga Remake will be documented in this file.

## 2025-12-26

### Added
- Player struct with position (x, y), dimensions, and speed properties
- `player_create()` function that spawns player at bottom-center of screen
- Player dimensions: 50x50 pixels, speed: 300 units
- Player not yet rendered to screen (next step)

## 2025-12-09

### Added
- Initial project setup
- README with project description
- Basic SDL2 game window (1280x720) with hardware-accelerated rendering
- Game loop with event handling
  - Quit on window close
  - Quit on Escape key press
- Black screen render (foundation for game rendering)
- Makefile build system with gcc
