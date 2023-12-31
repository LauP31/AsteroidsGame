#ifndef DEFINES_H_
#define DEFINES_H_

// Game parameters and settings

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 1000
#define TARGET_FPS 75
#define BACKGROUND_COLOR CLITERAL(Color){23, 3, 33, 255} 

#define STARTING_LIVES 5

#define SPACESHIP_RECT_WIDTH 99
#define SPACESHIP_RECT_HEIGHT 75
#define SPACESHIP_SOURCE_RECT CLITERAL(Rectangle){0, 0, SPACESHIP_RECT_WIDTH, SPACESHIP_RECT_HEIGHT}
#define SPACESHIP_COL_RADIUS 35
#define SPACESHIP_ACCEL 2000
#define SPACESHIP_MIN_VEL 0
#define SPACESHIP_MAX_VEL 700

#define MAX_ASTEROIDS 10
#define ASTEROID_RECT_WIDTH 98
#define ASTEROID_RECT_HEIGHT 96
#define ASTEROID_SOURCE_RECT CLITERAL(Rectangle){0, 0, ASTEROID_RECT_WIDTH, ASTEROID_RECT_HEIGHT}
#define TIME_BETWEEN_ASTEROIDS 0.8


#endif