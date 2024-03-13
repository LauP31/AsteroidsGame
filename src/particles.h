#ifndef PARTICLES_H_
#define PARTICLES_H_

#include "raylib.h"
#include <stdlib.h>



typedef struct Particle {

    Vector2 position;
    float velocity;
    float size;
    float color;
    bool active; 

} Particle;

typedef struct ParticleSystem {

    int max_particles;
    Particle *particles;
    Vector2 origin;
    float lifetime;

    union shape
    {
        struct RectangleEmmiter
        {
            Rectangle rect;
            float angle;
        };

        struct SphereEmmiter
        {
            float radius;
        };

        struct ConeEmmiter
        {
            float angle;
            float apperture_angle;
        };
    };

    int min_size;
    int max_size;

    Color initial_color;
    Color final_color;

} ParticleSystem;

void InitializeParticles(ParticleSystem* ps);
void FreeParticles(ParticleSystem* ps);
void DrawParticleSystem(ParticleSystem* ps);
void EmmitRectangleShape(ParticleSystem* ps);
void EmmitSphereShape(ParticleSystem* ps);
void EmmitConeShape(ParticleSystem* ps);

#endif