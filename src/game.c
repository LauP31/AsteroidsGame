#include "game.h"
#include "defines.h"
#include "raylib.h"
#include "spaceship.h"
#include "asteroids.h"
#include "star.h"
#include <stdio.h>

GameState _state;

// Time related vars
float _timeGameStarted;
float _timeGameEnded;
float _asteroidSpawnTimer;

// Textures 
Texture2D _ship_texture;
Texture2D _asteroidTexture;
Texture2D _starTexture;

// Game data
Spaceship _ship;
Asteroid asteroids[MAX_ASTEROIDS];
Star _star;

int lives = STARTING_LIVES;
int score = 0;
int new_score = 0;

// Load resources
void LoadGame(void)
{
    _ship_texture = LoadTexture("resources/playerShip1_blue.png");
    _asteroidTexture = LoadTexture("resources/meteorBrown_big4.png");
    _starTexture = LoadTexture("resources/star_gold.png");
}

// Unload resources
void UnloadGame(void)
{
    UnloadTexture(_ship_texture);
    UnloadTexture(_asteroidTexture);
    UnloadTexture(_starTexture);
}

// Called when game starts
void GameStart(void)
{
    _state = PLAYING;
    _timeGameStarted = GetTime();

    score = 0;
    lives = STARTING_LIVES;

    // Intialize spaceship
    _ship.acceleration = (Vector2){0,0};
    _ship.velocity = (Vector2){0,0};
    _ship.position = (Vector2){SCREEN_WIDTH/2, SCREEN_HEIGHT/2};
    _ship.rotation = 0;
    _ship.radius = SPACESHIP_COL_RADIUS;
    _ship.max_acc = SPACESHIP_ACCEL;
    _ship.min_speed = SPACESHIP_MIN_VEL;
    _ship.max_speed = SPACESHIP_MAX_VEL;

    // Place star
    Vector2 starStartPos = (Vector2){SCREEN_WIDTH/2, SCREEN_HEIGHT/2 - 200};
    SetStar(&_star, starStartPos, 40);

}

// Called when game ends
void GameEnd(void)
{
    _state = END;
    _timeGameEnded = GetTime();
    ClearAsteroids(asteroids);
}

// Called every frame
void UpdateGame(void)
{
    if (_state == END)
    {   
        // Restart game check
        if (IsKeyPressed(KEY_R))
        {
            GameStart();
        }
    }
    else
    {
        UpdateSpaceship(&_ship);
        UpdateStar();
        // Spawn asteroids once the player has collected at least one star
        if (score > 0)
        {
            _asteroidSpawnTimer -= GetFrameTime();
            if (_asteroidSpawnTimer < 0)
            {
                _asteroidSpawnTimer = TIME_BETWEEN_ASTEROIDS;
                int min_asteroid_speed = new_score * 0.5 + INITIAL_ASTEROID_SPEED; 
                int max_asteroid_speed = min_asteroid_speed + ASTEROID_SPEED_RANGE; 
                int speed = GetRandomValue(min_asteroid_speed, max_asteroid_speed);
                SpawnAsteroid(asteroids, speed);
            }
            UpdateAsteroids();
        }
       
       if (score > new_score + 200)
       {
            new_score = score;
            //printf("");
       }
        
    }
}

// Called every draw frame
void DrawGame(void)
{   
    ClearBackground(BACKGROUND_COLOR);
    DrawFPS(0,0);
    // Draw different screens depending on game state
    if (_state == END)
    {
        const char* scoreText = TextFormat("Highscore: %d", score);
        DrawText(scoreText, SCREEN_WIDTH/2-100, 0, 48, WHITE);
        DrawText("Press R to restart.", SCREEN_WIDTH/2-100, 50, 48, WHITE);
    }
    else
    {
        //const char* timerText = TextFormat("Asteroid timer: %f", _asteroidSpawnTimer);
        //const char* asteroidSpeed = TextFormat("Asteroid timer: %f", _asteroidSpawnTimer);
        //DrawText(timerText, 0, 20, 20, GREEN);

        // Draw spaceship 
        DrawTexturePro(_ship_texture,
                        SPACESHIP_SOURCE_RECT,
                        (Rectangle){_ship.position.x, _ship.position.y, SPACESHIP_RECT_WIDTH, SPACESHIP_RECT_HEIGHT},
                        (Vector2){SPACESHIP_RECT_WIDTH/2, SPACESHIP_RECT_HEIGHT/2},
                        _ship.rotation,
                        WHITE);

        // Draw star
        DrawTexture(_starTexture, _star.position.x - 15, _star.position.y - 15, WHITE);

        // Draw asteroids
        DrawAsteroids(asteroids, _asteroidTexture);

        // UI
        const char* livesText = TextFormat("Lives: %d", lives);
        const char* scoreText = TextFormat("Score: %d", score);

        DrawText(livesText, SCREEN_WIDTH/2-100, 0, 48, WHITE);
        DrawText(scoreText, SCREEN_WIDTH/2-100, 50, 48, WHITE);

        // DEBUG STUFF:
        //DrawCircleLines(star.position.x, star.position.y, star.radius, WHITE);
        //DrawLineV(_ship.position, Vector2Add(_ship.position, _ship.velocity), GREEN);
        //DrawLineV(_ship.position, Vector2Add(_ship.position, _ship.acceleration), RED);
        //DrawCircleLinesV(_ship.position, _ship.radius, YELLOW);

    }
}

void UpdateAsteroids()
{
    for (int i = 0; i < MAX_ASTEROIDS; i++)
    {
        if (!asteroids[i].active) continue;
        
        if (asteroids[i].position.y > SCREEN_HEIGHT + ASTEROID_RECT_HEIGHT / 2)
        {
            UnsetAsteroidAt(asteroids, i);
            continue;
        }

        if (CheckCollisionCircles(_ship.position, _ship.radius, asteroids[i].position, asteroids[i].radius))
        {
            lives -= 1;
            if (lives <= 0)
            {
                GameEnd();
                break;
            }
            UnsetAsteroidAt(asteroids, i);
            continue;
        }

        asteroids[i].position.y += asteroids[i].speed * GetFrameTime();

    }    
}

void UpdateStar()
{
    if (CheckCollisionCircles(_ship.position, _ship.radius, _star.position, _star.radius))
    {
        MoveStar(&_star);
        score += SCORE_PER_STAR;
    }
}