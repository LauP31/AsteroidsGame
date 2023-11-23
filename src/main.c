#include <stdlib.h>
#include <time.h>
#include "raylib.h"
#include "raymath.h"
#include "defines.h"
#include "game.h"
//#define PLATFORM_WEB
#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

void UpdateDrawFrame(void);

int main(void)
{

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Asteroids!");
    LoadGame();
    GameStart();
    #if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
    #else
    SetTargetFPS(TARGET_FPS); 
    while (!WindowShouldClose())
    {
        UpdateDrawFrame();
    }
    #endif
    UnloadGame();
    
    CloseWindow();

    return 0;
}

void UpdateDrawFrame(void)
{
    UpdateGame();
    BeginDrawing();
    DrawGame();
    EndDrawing();
}