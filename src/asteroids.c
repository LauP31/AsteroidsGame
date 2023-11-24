#include "asteroids.h"
#include "defines.h"

void SetAsteroidAt(Asteroid* asteroids, int i, Vector2 position, int speed, int radius)
{
    if (i < 0 || i >= MAX_ASTEROIDS) return;

    asteroids[i].position = position;
    asteroids[i].speed = speed;
    asteroids[i].active = true;
    asteroids[i].radius = radius;
}

void UnsetAsteroidAt(Asteroid * asteroids, int i)
{
    if (i < 0 || i >= MAX_ASTEROIDS) return;

    asteroids[i].active = false;

}

void DrawAsteroids(Asteroid* asteroids, Texture2D texture)
{
    for (int i = 0; i < MAX_ASTEROIDS; i++)
    {
        if (!asteroids[i].active) continue;
        
        Vector2 position = asteroids[i].position;
        position.x -= ASTEROID_RECT_WIDTH / 2;
        position.y -= ASTEROID_RECT_HEIGHT / 2;
        DrawTexture(texture, position.x, position.y, WHITE);
        //DrawCircleLines(asteroids[i].position.x, asteroids[i].position.y, asteroids[i].radius, WHITE);
    }
}

void SpawnAsteroid(Asteroid* asteroids, int speed)
{
    int avaliableIndex = -1;
    for (int i = 0; i < MAX_ASTEROIDS; i++)
    {
        if (!asteroids[i].active)
        {
            avaliableIndex = i;
            break;
        }
    }
    
    if (avaliableIndex == -1) return;

    int posX = GetRandomValue(ASTEROID_RECT_WIDTH / 2, SCREEN_WIDTH - ASTEROID_RECT_WIDTH / 2);
    
    SetAsteroidAt(asteroids, avaliableIndex, (Vector2){posX, -200}, speed, 35);
}

void ClearAsteroids(Asteroid* asteroids)
{
    for (int i = 0; i < MAX_ASTEROIDS; i++)
    {
        if (asteroids[i].active)
        {
            asteroids[i].active = false;
        }
    }
}