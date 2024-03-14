#include "game.h"
#include "defines.h"
#include "raylib.h"
#include "spaceship.h"
#include "asteroids.h"
#include "star.h"
#include "raymath.h"
#include "particles.h"

GameState state;

// Time related vars
float asteroidSpawnTimer;
float flashTimer = 0;

// Textures 
Texture2D shipTexture;
Texture2D asteroidTexture;
Texture2D starTexture;

// Audio
Music music;
Sound sfxExplosion;
Sound sfxStarPickup;
Sound sfxThurster;

// Game data
Spaceship starship;
Asteroid asteroids[MAX_ASTEROIDS];
Star star;


ParticleSystem starParticles;
ParticleSystem rocketParticles;

int lives = STARTING_LIVES;
int score = 0;
int newScore = 0;
float newTime = INITIAL_TIME_BETWEEN_ASTEROIDS;
int minAsteroidSpeed;

bool paused;
bool muted;
bool flashing;

void LoadGame(void)
{
    shipTexture = LoadTexture("resources/playerShip1_blue.png");
    asteroidTexture = LoadTexture("resources/meteorBrown_big4.png");
    starTexture = LoadTexture("resources/star_gold.png");

    music = LoadMusicStream("resources/Mercury.wav");
    sfxExplosion = LoadSound("resources/explosion.wav");
    sfxStarPickup = LoadSound("resources/star_pickup.wav");
    sfxThurster = LoadSound("resources/thruster.ogg");


    starParticles = (ParticleSystem){
        .maxParticles = 100,
        .lifetime = 12.5,
        .emissionRate = 0.5,
        .minSize = 1,
        .maxSize = 3,
        .color = WHITE,
        .speed = 80,
        .origin = (Vector2){0, 0},
        .width = 1000,
        .angle = 90,
        .emitting = true
    };

    rocketParticles = (ParticleSystem){
        .maxParticles = 100,
        .lifetime = 0.15,
        .emissionRate = 0.05,
        .minSize = 2,
        .maxSize = 4,
        .color = ORANGE,
        .speed = 150,
        .origin = (Vector2){-50,-50},
        .width = 15,
        .angle = 0,
        .emitting = true
    };

    InitializeParticles(&starParticles);
    InitializeParticles(&rocketParticles);
}

void UnloadGame(void)
{
    // Unload textures
    UnloadTexture(shipTexture);
    UnloadTexture(asteroidTexture);
    UnloadTexture(starTexture);

    // Unload audio
    UnloadMusicStream(music);
    UnloadSound(sfxExplosion);
    UnloadSound(sfxStarPickup);
    UnloadSound(sfxThurster);

    // Unload particles
    FreeParticles(&starParticles);
    FreeParticles(&rocketParticles);
}

void GameStart(void)
{   
    paused = false;
    PlayMusicStream(music);
    state = PLAYING;

    score = 0;
    lives = STARTING_LIVES;
    newScore = 0;
    newTime = INITIAL_TIME_BETWEEN_ASTEROIDS;
    flashTimer = 0;

    // Intialize spaceship
    starship.acceleration = (Vector2){0,0};
    starship.velocity = (Vector2){0,0};
    starship.position = (Vector2){SCREEN_WIDTH/2, SCREEN_HEIGHT/2};
    starship.rotation = 0;
    starship.radius = SPACESHIP_COL_RADIUS;
    starship.max_acc = SPACESHIP_ACCEL;
    starship.min_speed = SPACESHIP_MIN_VEL;
    starship.max_speed = SPACESHIP_MAX_VEL;

    // Place star
    Vector2 starStartPos = (Vector2){SCREEN_WIDTH/2, SCREEN_HEIGHT/2 - 200};
    SetStar(&star, starStartPos, 40);

    ResetParticleSystem(&rocketParticles);

}

void GameEnd(void)
{
    state = END;
    StopSound(sfxThurster);
    ClearAsteroids(asteroids);
    StopMusicStream(music);
}

void UpdateGame(void)
{
    if (IsKeyPressed(KEY_M)) muted = !muted;

    if (!IsWindowFocused()) paused = true;

    if(!paused)
    {
        if (!muted) UpdateMusicStream(music);
        UpdateParticleSystem(&starParticles);
    }
    
    if (state == END)
    {   
        if (IsKeyPressed(KEY_R))
        {
            GameStart();
        }
    }
    else
    {
        // Handle pause toggle
        if (paused)
        {   
            if (IsKeyPressed(KEY_SPACE))
            {
                paused = false;
            }
            return;
        }

        if (IsKeyPressed(KEY_SPACE))
        {
            paused = true;
        }

        // Game update
        UpdateSpaceship(&starship);
        UpdateStar();
        rocketParticles.emitting = IsMouseButtonDown(MOUSE_BUTTON_LEFT);
        UpdateParticleSystem(&rocketParticles);
        rocketParticles.origin = Vector2Add(starship.position, Vector2Scale((Vector2){cos(rocketParticles.angle * DEG2RAD), sin(rocketParticles.angle * DEG2RAD)}, 40));
        rocketParticles.angle = starship.rotation + 90;

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && !muted && !paused)
        {
            if (!IsSoundPlaying(sfxThurster))
                PlaySound(sfxThurster);
        }
        else
        {
            StopSound(sfxThurster);
        }

        // Spawn asteroids once the player has collected at least one star
        if (score > 0)
        {
            asteroidSpawnTimer -= GetFrameTime();
            if (asteroidSpawnTimer < 0)
            {
                if (newTime > MAX_TIME_TIME_BETWEEN_ASTEROIDS)
                {
                    newTime = newScore * (-0.0007) + INITIAL_TIME_BETWEEN_ASTEROIDS;
                }
                //new_time = fmax(new_time, MAX_TIME_TIME_BETWEEN_ASTEROIDS);
                asteroidSpawnTimer = newTime;
                if (minAsteroidSpeed < MAX_ASTEROID_SPEED)
                {
                    minAsteroidSpeed = newScore * 0.35 + INITIAL_ASTEROID_SPEED;  
                }
                int max_asteroid_speed = minAsteroidSpeed + ASTEROID_SPEED_RANGE; 
                int speed = GetRandomValue(minAsteroidSpeed, max_asteroid_speed);
                //speed = fmin(speed, MAX_ASTEROID_SPEED);
                SpawnAsteroid(asteroids, speed);
            }
            UpdateAsteroids();
        }

        HandlePlayerFlashing();

       if (score >= newScore + 100)
       {
            newScore = score;
       }
        
    }
}

void DrawGame(void)
{   
    ClearBackground(BACKGROUND_COLOR);
    DrawFPS(0,0);
    DrawText("Mute: M\nPause: Space", 0, 20, 18, LIGHTGRAY);
    DrawParticleSystem(&starParticles);
    if (state == END)
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
        if (!flashing)
        DrawTexturePro(shipTexture,
                        SPACESHIP_SOURCE_RECT,
                        (Rectangle){starship.position.x, starship.position.y, SPACESHIP_RECT_WIDTH, SPACESHIP_RECT_HEIGHT},
                        (Vector2){SPACESHIP_RECT_WIDTH/2, SPACESHIP_RECT_HEIGHT/2},
                        starship.rotation,
                        WHITE);

        // Draw star
        DrawTexture(starTexture, star.position.x - 15, star.position.y - 15, WHITE);

        // Draw asteroids
        DrawAsteroids(asteroids, asteroidTexture);

        // UI
        const char* livesText = TextFormat("Lives: %d", lives);
        const char* scoreText = TextFormat("Score: %d", score);

        DrawText(livesText, SCREEN_WIDTH/2-100, 0, 48, SKYBLUE);
        DrawText(scoreText, SCREEN_WIDTH/2-100, 50, 48, SKYBLUE);

        if (paused)
        {
            float opacity = sin(GetTime()*5);
            int alpha = fabs(255*opacity); 
            DrawText("PAUSED", SCREEN_WIDTH/2-MeasureText("PAUSED", 48)*0.5, 450, 48, (Color){255, 255, 255, alpha});
        }

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

        if (CheckCollisionCircles(starship.position, starship.radius, asteroids[i].position, asteroids[i].radius) && flashTimer <= 0)
        {
            if (!muted) PlaySound(sfxExplosion);

            flashTimer = FLASHING_TIME;
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
    if (CheckCollisionCircles(starship.position, starship.radius, star.position, star.radius))
    {
        if (!muted) PlaySound(sfxStarPickup);
        MoveStar(&star);
        score += SCORE_PER_STAR;
    }
}

void HandlePlayerFlashing()
{
    if (flashTimer > 0)
    {   
        float sine = sin(GetTime()*15);
        sine = fabs(sine);
        flashing = sine > 0.8;
        flashTimer -= GetFrameTime();
        return;
    }
    flashing = false;
}