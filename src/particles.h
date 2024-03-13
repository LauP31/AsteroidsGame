#ifndef PARTICLES_H_
#define PARTICLES_H_

#include "raylib.h"
#include <stdlib.h>


typedef struct Particle {

    Vector2 position;
    Vector2 velocity;
    float lifetime;
    float size;
    bool active; 

} Particle;


typedef struct RectangleEmitter
{
    float width;
    float height;
    float angle;
} RectangleEmitter;

typedef struct CircleEmitter
{
    float radius;
} CircleEmitter;

typedef struct ConeEmitter
{
    float angle;
    float apperture_angle;
} ConeEmitter;

typedef struct ParticleSystem {

    int max_particles;
    Vector2 origin;
    float lifetime;
    float emission_rate;
    bool emitting;

    enum {
        RECTANGLE_EMITTER,
        CIRCLE_EMITTER,
        CONE_EMITTER
    } emitterType;

    union emitter
    {
        RectangleEmitter rectangleEmitter;
        CircleEmitter circleEmitter;
        ConeEmitter ConeEmitter;
    } emitter;

    int min_size;
    int max_size;

    int min_speed;
    int max_speed;

    Color initial_color;
    Color final_color;

    float timer;
    Particle *particles;

} ParticleSystem;

void InitializeParticles(ParticleSystem* ps);
void FreeParticles(ParticleSystem* ps);
void UpdateParticleSystem(ParticleSystem* ps);
void DrawParticleSystem(ParticleSystem* ps);
void ResetParticleSystem(ParticleSystem* ps);

#endif