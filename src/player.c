#include "player.h"

Player player_create(int screen_width, int screen_height) {
  Player p;

  p.width = 50;
  p.height = 50;
  p.x = (screen_width / 2) - (p.width / 2);
  p.y = screen_height - p.height - 20;
  p.speed = 300;

  return p;
}
