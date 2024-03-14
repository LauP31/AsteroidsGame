#include <stdlib.h>
#include <time.h>
#include "raylib.h"
#include "raymath.h"
#include "defines.h"
#include "game.h"

int main(void)
{

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Asteroids!");
    InitAudioDevice();
    LoadGame();
    GameStart();
    SetTargetFPS(TARGET_FPS); 
    
    while (!WindowShouldClose())
    {
        UpdateGame();
        BeginDrawing();
        DrawGame();
        EndDrawing();
    }
    UnloadGame();
    
    CloseAudioDevice();
    CloseWindow();

    return 0;
}