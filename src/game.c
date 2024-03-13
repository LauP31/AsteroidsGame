#include "game.h"
#include "defines.h"
#include "raylib.h"
#include "spaceship.h"
#include "asteroids.h"
#include "star.h"
#include <stdio.h>
#include "raymath.h"
#include "particles.h"

GameState _state;

// Time related vars
float _timeGameStarted;
float _timeGameEnded;
float _asteroidSpawnTimer;

// Textures 
Texture2D _ship_texture;
Texture2D _asteroidTexture;
Texture2D _starTexture;

// Audio
Music music;

// Game data
Spaceship _ship;
Asteroid asteroids[MAX_ASTEROIDS];
Star _star;


ParticleSystem starParticles;
ParticleSystem rocketParticles;

int lives = STARTING_LIVES;
int score = 0;
int new_score = 0;
float new_time = INITIAL_TIME_BETWEEN_ASTEROIDS;
int min_asteroid_speed;


void LoadGame(void)
{
    _ship_texture = LoadTexture("resources/playerShip1_blue.png");
    _asteroidTexture = LoadTexture("resources/meteorBrown_big4.png");
    _starTexture = LoadTexture("resources/star_gold.png");

    music = LoadMusicStream("resources/Venus.wav");

    starParticles = (ParticleSystem){
        .max_particles = 200,
        .lifetime = 12.5,
        .emission_rate = 0.5,
        .min_size = 1,
        .max_size = 3,
        .initial_color = WHITE,
        .min_speed = 80,
        .origin = (Vector2){0, 0},
        .emitterType = RECTANGLE_EMITTER,
        .emitter.rectangleEmitter.width = 1000,
        .emitter.rectangleEmitter.angle = 90     
    };

    rocketParticles = (ParticleSystem){
        .emitterType = RECTANGLE_EMITTER,
        .max_particles = 100,
        .lifetime = 0.15,
        .emission_rate = 0.05,
        .min_size = 2,
        .max_size = 4,
        .initial_color = THRUSTER_COLOR,
        .min_speed = 150,
        .origin = (Vector2){-50,-50},
        .emitter.rectangleEmitter.width = 15,
        .emitter.rectangleEmitter.angle = 0
    };

    InitializeParticles(&starParticles);
    InitializeParticles(&rocketParticles);

}

void UnloadGame(void)
{
    UnloadTexture(_ship_texture);
    UnloadTexture(_asteroidTexture);
    UnloadTexture(_starTexture);
    UnloadMusicStream(music);
    FreeParticles(&starParticles);
    FreeParticles(&rocketParticles);
}

void GameStart(void)
{
    PlayMusicStream(music);
    _state = PLAYING;
    _timeGameStarted = GetTime();

    score = 0;
    lives = STARTING_LIVES;
    new_score = 0;
    new_time = INITIAL_TIME_BETWEEN_ASTEROIDS;

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

    ResetParticleSystem(&rocketParticles);
    starParticles.emitting = true;

}

void GameEnd(void)
{
    _state = END;
    _timeGameEnded = GetTime();
    ClearAsteroids(asteroids);
    StopMusicStream(music);
}

void UpdateGame(void)
{
    UpdateMusicStream(music);
    UpdateParticleSystem(&starParticles);
    if (_state == END)
    {   
        if (IsKeyPressed(KEY_R))
        {
            GameStart();
        }
    }
    else
    {
        UpdateSpaceship(&_ship);
        UpdateStar();
        rocketParticles.emitting = IsMouseButtonDown(MOUSE_BUTTON_LEFT);
        UpdateParticleSystem(&rocketParticles);
        rocketParticles.origin = Vector2Add(_ship.position, Vector2Scale((Vector2){cos((_ship.rotation + 90) * DEG2RAD), sin((_ship.rotation + 90) * DEG2RAD)}, 40));
        rocketParticles.emitter.rectangleEmitter.angle = _ship.rotation + 90;
        // Spawn asteroids once the player has collected at least one star
        if (score > 0)
        {
            _asteroidSpawnTimer -= GetFrameTime();
            if (_asteroidSpawnTimer < 0)
            {
                if (new_time > MAX_TIME_TIME_BETWEEN_ASTEROIDS)
                {
                    new_time = new_score * (-0.0007) + INITIAL_TIME_BETWEEN_ASTEROIDS;
                }
                //new_time = fmax(new_time, MAX_TIME_TIME_BETWEEN_ASTEROIDS);
                _asteroidSpawnTimer = new_time;
                if (min_asteroid_speed < MAX_ASTEROID_SPEED)
                {
                    min_asteroid_speed = new_score * 0.35 + INITIAL_ASTEROID_SPEED; 
                }
                int max_asteroid_speed = min_asteroid_speed + ASTEROID_SPEED_RANGE; 
                int speed = GetRandomValue(min_asteroid_speed, max_asteroid_speed);
                //speed = fmin(speed, MAX_ASTEROID_SPEED);
                SpawnAsteroid(asteroids, speed);
            }
            UpdateAsteroids();
        }
       
       if (score >= new_score + 100)
       {
            new_score = score;
            printf("INCREASING DIFFICULTY\n");
            printf("NEW SPEED: %d\n", min_asteroid_speed);
            printf("NEW TIME: %f\n", new_time);
       }
        
    }
}

void DrawGame(void)
{   
    ClearBackground(BACKGROUND_COLOR);
    DrawFPS(0,0);
    DrawParticleSystem(&starParticles);
    if (_state == END)
    {
        const char* scoreText = TextFormat("Highscore: %d", score);
        DrawText(scoreText, SCREEN_WIDTH/2-MeasureText(scoreText, 48)*0.5, 400, 48, YELLOW);
        float opacity = sin(GetTime()*2);
        int alpha = 50+fabs(100*opacity); 
        DrawText("Press R to restart.", SCREEN_WIDTH/2-MeasureText("Press R to restart.", 48)*0.5, 450, 48, (Color){255, 255, 255, alpha});
    }
    else
    {

        DrawParticleSystem(&rocketParticles);        
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

        DrawText(livesText, SCREEN_WIDTH/2-100, 0, 48, SKYBLUE);
        DrawText(scoreText, SCREEN_WIDTH/2-100, 50, 48, SKYBLUE);

        // DEBUG STUFF:
        //DrawCircleLines(star.position.x, star.position.y, star.radius, WHITE);
        //DrawCircleV(_ship.position, 1, RED);
        //DrawLineV(_ship.position, rocketParticles.origin, GREEN);
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