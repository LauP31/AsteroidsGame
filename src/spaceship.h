#ifndef SPACESHIP_H_
#define SPACESHIP_H_

#include "raylib.h"

typedef struct Spaceship
{
    Vector2 position;    
    Vector2 velocity;
    Vector2 acceleration;
    float rotation;
    int min_speed;
    int max_speed;
    int max_acc;
    int radius;
    
} Spaceship;

void UpdateSpaceship(Spaceship* s);

#endif