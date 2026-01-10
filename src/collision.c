#include "collision.h"
#include <stdlib.h>

#include <stdbool.h>

bool check_collision(float x1, float y1, int w1, int h1,
                     float x2, float y2, int w2, int h2) {
    if (x1 + w1 <= x2 || x2 + w2 <= x1) {
        return false;
    }

    if (y1 + h1 <= y2 || y2 + h2 <= y1) {
        return false;
    }

    return true;
}