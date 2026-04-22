#include "assets.h"
#include "texture_loader.h"
#include "weapon.h"

void assets_init(Assets *a, SDL_Renderer *renderer) {
  for (int i = 0; i < WEAPON_COUNT; i++) {
    const WeaponDefinition *def = get_weapon_def(i);
    a->ships[i] = texture_load(renderer, def->ship_texture_path);
    a->bullets[i] = texture_load(renderer, def->bullet_texture_path);
  }
  for (int i = 0; i < SPECIES_COUNT; i++) {
    const BacteriaDefinition *def = get_bacteria_def(i);
    a->bacteria[i] = texture_load(renderer, def->texture_path);
  }
}

void assets_destroy(Assets *assets) {
  for (int i = 0; i < WEAPON_COUNT; i++) {
    if (assets->ships[i])
      SDL_DestroyTexture(assets->ships[i]);
    assets->ships[i] = NULL;
    if (assets->bullets[i])
      SDL_DestroyTexture(assets->bullets[i]);
    assets->bullets[i] = NULL;
  }
  for (int i = 0; i < SPECIES_COUNT; i++) {
    if (assets->bacteria[i])
      SDL_DestroyTexture(assets->bacteria[i]);
    assets->bacteria[i] = NULL;
  }
}
