#ifndef PLAYER_H
#define PLAYER_H

typedef struct {
  float x, y;
  int width;
  int height;
  float speed;
} Player;

Player player_create(int screen_width, int screen_height);

#endif
