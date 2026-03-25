#include "weapon.h"
#include "bacteria.h"

static const WeaponDefinition WEAPON_DEFS[] = {
    {WEAPON_NEUTRAL, "assets/weapons/base_shot.png", 3, 3, 3, 8, 23, 300.0f,
     GRAM_BOTH},
    {WEAPON_PCN, "assets/weapons/pcn_shot.png", 6, 0, 2, 8, 8, 300.0f,
     GRAM_POSITIVE},
    {WEAPON_PMX, "assets/weapons/pmx_shot.png", 6, 0, 2, 6, 23, 300.0f,
     GRAM_NEGATIVE}};

const WeaponDefinition *get_weapon_def(WeaponType type) {
  return &WEAPON_DEFS[type];
}
