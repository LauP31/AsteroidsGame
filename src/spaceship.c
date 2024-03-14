#include "spaceship.h"
#include "raylib.h"
#include "raymath.h"
#include "input.h"
#include "defines.h"

Vector2 input = (Vector2){0,0};

Spaceship spaceship;

void UpdateSpaceship(Spaceship* s)
{
    input = Vector2Subtract(GetMousePosition(), s->position);
    input = Vector2Normalize(input);
    
    s->rotation = GetMouseAngle(s->position)+90;
    s->acceleration = Vector2Scale(input, s->max_acc  * IsMouseButtonDown(MOUSE_BUTTON_LEFT));

    s->velocity = Vector2Add(s->velocity, Vector2Scale(s->acceleration, GetFrameTime()));
    s->velocity = Vector2ClampValue(s->velocity, s->min_speed, s->max_speed);    
    
    s->position = Vector2Add(s->position, Vector2Add(Vector2Scale(s->velocity,GetFrameTime()), Vector2Scale(s->acceleration,0.5*GetFrameTime()*GetFrameTime())));
    
    if (s->position.y <= s->radius || s->position.y >= SCREEN_HEIGHT - s->radius)
    {
        s->velocity.y = s->velocity.y * -1;
    }

    if (s->position.x <= s->radius || s->position.x >= SCREEN_WIDTH - s->radius)
    {
        s->velocity.x = s->velocity.x * -1;
    }

    s->position.x = Wrap(s->position.x, 0-SPACESHIP_RECT_WIDTH/2, SCREEN_WIDTH+SPACESHIP_RECT_WIDTH/2);
    s->position.y = Wrap(s->position.y, 0-SPACESHIP_RECT_HEIGHT/2, SCREEN_HEIGHT+SPACESHIP_RECT_HEIGHT/2);

}