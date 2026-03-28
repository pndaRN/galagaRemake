#include "weapon.h"
#include "shared_types.h"

static const WeaponDefinition WEAPON_DEFS[] = {
    {WEAPON_NEUTRAL, "assets/weapons/base_shot.png", 3, 3, 3, 8, 23, 300.0f},
    {
        WEAPON_PCN,
        "assets/weapons/pcn_shot.png",
        6,
        0,
        2,
        8,
        8,
        300.0f,
    },
    {
        WEAPON_PMX,
        "assets/weapons/pmx_shot.png",
        6,
        0,
        2,
        6,
        23,
        300.0f,
    }};

const WeaponDefinition *get_weapon_def(WeaponType type) {
  return &WEAPON_DEFS[type];
}

int calculate_damage(const WeaponDefinition *weapon, const BacteriaDefinition *bacteria) {
    if(weapon->type == WEAPON_NEUTRAL) {
        return weapon->damage_neutral;
    }
    if(weapon->type == bacteria->weakness) {
        return weapon->damage_effective;
    }
    return weapon->damage_ineffective;
}
