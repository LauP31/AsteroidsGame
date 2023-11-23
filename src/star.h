#ifndef STAR_H_
#define STAR_H_

#include "raylib.h"

typedef struct Star
{
    Vector2 position;
    int radius;
}Star;

void SetStar(Star* star, Vector2 position, int radius);
void MoveStar(Star* star);

#endif