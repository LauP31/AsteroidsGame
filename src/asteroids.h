#ifndef ASTEROIDS_H_
#define ASTEROIDS_H_

#include "raylib.h"

typedef struct Asteroid
{
    Vector2 position;
    Vector2 velocity;
    int radius;
    int speed;
    bool active;

} Asteroid;


void SetAsteroidAt(Asteroid* asteroids, int i, Vector2 position, int speed, int radius);
void UnsetAsteroidAt(Asteroid* asteroids, int i);
void DrawAsteroids(Asteroid* asteroids, Texture2D texture);
void SpawnAsteroid(Asteroid* asteroids, int speed);
void ClearAsteroids(Asteroid* asteroids);

#endif