#include "enemy.h"
#include <stdlib.h>

Enemy enemy_init(int screen_width, int screen_height) {
    Enemy e;
    
    e.width = 50;
    e.height = 50;
    
    e.x = rand() % (screen_width - e.width + 1);
    e.y = -e.height;

    e.speed = 200;
    e.active = true;

    return e;
}

void enemy_update(Enemy *e, float deltaTime, int screen_height) {
    e->y += e->speed * deltaTime;
    if (e->y > screen_height) {
        e->active = false;
    }
}
