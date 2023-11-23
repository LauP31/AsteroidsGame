#include "input.h"
#include "raymath.h"

Vector2 GetMouseDirection(Vector2 pos)
{
    Vector2 input;
    input = Vector2Subtract(GetMousePosition(), pos);
    input = Vector2Normalize(input);
    return input;
}

float GetMouseAngle(Vector2 pos)
{
    return atan2(GetMouseDirection(pos).y, GetMouseDirection(pos).x)*RAD2DEG;
}