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

typedef struct ParticleSystem {

    int max_particles;
    Vector2 origin;
    float lifetime;
    float emission_rate;
    bool emitting;
    float width;
    float angle;

    int min_size;
    int max_size;

    int speed;

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