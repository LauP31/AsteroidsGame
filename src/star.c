#include "star.h"
#include "defines.h"

void SetStar(Star* star, Vector2 position, int radius)
{
    star->position = position;
    star->radius = radius;
}

void MoveStar(Star* star)
{
     Vector2 randomPosition;
    randomPosition.x = GetRandomValue(100, SCREEN_WIDTH-100);
    randomPosition.y = SCREEN_HEIGHT / 2 + GetRandomValue(-200, 300);
    star->position = randomPosition;
}