#ifndef GAME_H_
#define GAME_H_

typedef enum GameState
{
    PLAYING,
    END
} GameState;

void LoadGame(void);
void UnloadGame(void);
void GameStart(void);
void GameEnd(void);
void UpdateGame(void);
void DrawGame(void);
void UpdateAsteroids(void);
void UpdateStar(void);

#endif