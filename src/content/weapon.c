#include "weapon.h"
#include "shared_types.h"

static const WeaponDefinition WEAPON_DEFS[] = {
    {
        .type = WEAPON_NEUTRAL,
        .bullet_texture_path = "assets/weapons/base_shot.png",
        .ship_texture_path = "assets/ships/ship_neutral.png",
        .damage_effective = 3,
        .damage_neutral = 3,
        .damage_ineffective = 3,
        .width = 8,
        .height = 23,
        .speed = 300.0f,
    },
    {
        .type = WEAPON_PCN,
        .bullet_texture_path = "assets/weapons/pcn_shot.png",
        .ship_texture_path = "assets/ships/ship_pcn.png",
        .damage_effective = 6,
        .damage_neutral = 0,
        .damage_ineffective = 2,
        .width = 8,
        .height = 8,
        .speed = 300.0f,
    },
    {
        .type = WEAPON_PMX,
        .bullet_texture_path = "assets/weapons/pmx_shot.png",
        .ship_texture_path = "assets/ships/ship_pmx.png",
        .damage_effective = 6,
        .damage_neutral = 0,
        .damage_ineffective = 2,
        .width = 6,
        .height = 23,
        .speed = 300.0f,
    }};

const WeaponDefinition *get_weapon_def(WeaponType type) {
  return &WEAPON_DEFS[type];
}

int calculate_damage(const WeaponDefinition *weapon,
                     const BacteriaDefinition *bacteria) {
  if (weapon->type == WEAPON_NEUTRAL) {
    return weapon->damage_neutral;
  }
  if (weapon->type == bacteria->weakness) {
    return weapon->damage_effective;
  }
  return weapon->damage_ineffective;
}
