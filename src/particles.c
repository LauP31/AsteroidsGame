#include "particles.h"
#include "defines.h"
#include <stdio.h>
#include "raymath.h"

// Create the particles pool and set some default parameters
void InitializeParticles(ParticleSystem* ps)
{
    ps->particles = malloc(ps->max_particles * sizeof(Particle));
    if (ps->particles == NULL)
    {
        exit(-1);
    }

    ps->timer = ps->emission_rate;

    for (int i = 0; i < ps->max_particles; i++) 
    {
        ps->particles[i].position = ps->origin;
        //ps->particles[i].velocity = ps->speed;
        ps->particles[i].lifetime = ps->lifetime;
        ps->particles[i].active = false;
    }

    //printf("PARTICLE EMITTER ENUM INDEX: %lu \n", ps->emitterType);

}

void FreeParticles(ParticleSystem* ps)
{
    free(ps->particles);
}

void UpdateParticleSystem(ParticleSystem* ps)
{
    if (ps->emitting)
    {
        ps->timer -= GetFrameTime();
    }

    if (ps->timer <= 0)
    {   
        for (int i = 0; i < ps->max_particles; i++)
        {
            if (!ps->particles[i].active)
            {   
                // Emit particle
                ps->particles[i].position.y = ps->origin.y;
                ps->particles[i].position.x = GetRandomValue(ps->origin.x, ps->origin.x + ps->width);
                Vector2 direction = (Vector2){
                    cos(ps->angle * DEG2RAD), 
                    sin(ps->angle * DEG2RAD)
                };
                ps->particles[i].velocity = Vector2Scale(direction, ps->speed);
                ps->particles[i].size = GetRandomValue(ps->min_size, ps->max_size);
                ps->particles[i].active = true;
                break;
            }
        }
        
        ps->timer = ps->emission_rate;
        
    }

    for (int i = 0; i < ps->max_particles; i++) 
    {   
        if (!ps->particles[i].active) continue;

        ps->particles[i].position = Vector2Add(ps->particles[i].position, Vector2Scale(ps->particles[i].velocity, GetFrameTime()));
        ps->particles[i].lifetime -= GetFrameTime();

        if (ps->particles[i].lifetime <= 0)
        {
            ps->particles[i].active = false;
            ps->particles[i].lifetime = ps->lifetime;
        }
        
    }        
}


void DrawParticleSystem(ParticleSystem* ps)
{
    for (int i = 0; i < ps->max_particles; i++)
    {
        if (!ps->particles[i].active) continue;
        DrawCircleV(ps->particles[i].position, ps->particles[i].size, ps->initial_color);
    }
}

void ResetParticleSystem(ParticleSystem* ps)
{
    for (int i = 0; i < ps->max_particles; i++)
    {
        if (!ps->particles[i].active) continue;
        ps->particles[i].active = false;
    }
}
