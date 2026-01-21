#include "math_utils.h"

SDL_FPoint bezier_point(SDL_FPoint p0, SDL_FPoint p1, SDL_FPoint p2,
                        SDL_FPoint p3, float t) {
  float u = 1.0f - t;
  float tt = t * t;
  float uu = u * u;
  float uuu = uu * u;
  float ttt = tt * t;

  SDL_FPoint result;

  result.x = uuu * p0.x + 3 * uu * t * p1.x + 3 * u * tt * p2.x + ttt * p3.x;

  result.y = uuu * p0.y + 3 * uu * t * p1.y + 3 * u * tt * p2.y + ttt * p3.y;

  return result;
}
