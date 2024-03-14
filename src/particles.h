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

    int maxParticles;
    Vector2 origin;
    float lifetime;
    float emissionRate;
    bool emitting;
    float width;
    float angle;

    int minSize;
    int maxSize;

    int speed;

    Color color;

    float timer;
    Particle *particles;

} ParticleSystem;

void InitializeParticles(ParticleSystem* ps);
void FreeParticles(ParticleSystem* ps);
void UpdateParticleSystem(ParticleSystem* ps);
void DrawParticleSystem(ParticleSystem* ps);
void ResetParticleSystem(ParticleSystem* ps);

#endif