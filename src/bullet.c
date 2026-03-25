#include "bullet.h"
#include "weapon.h"

Bullet bullet_init(Player *player) {
  const WeaponDefinition *def = get_weapon_def(player->current_ammo);
  Bullet b;

  b.width = def->width;
  b.height = def->height;

  b.x = player->x + (player->width / 2.0f) - (b.width / 2.0f);
  b.y = player->y;

  b.speed = def->speed;
  b.active = true;

  b.type = player->current_ammo;

  b.texture = player->bullet_texture[player->current_ammo];

  return b;
}

void bullet_update(Bullet *b, float deltaTime) {
  b->y -= b->speed * deltaTime;
  if (b->y + b->height < 0) {
    b->active = false;
  }
}
