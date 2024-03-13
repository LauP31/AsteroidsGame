#include "particles.h"
#include "defines.h"
#include <stdio.h>
#include "raymath.h"

// Create the particles pool
void InitializeParticles(ParticleSystem* ps)
{
    ps->particles = (Particle *)malloc(ps->max_particles * sizeof(Particle));
    if (ps->particles == NULL)
    {
        exit(-1);
    }

    ps->timer = PARTICLE_SPAWN_RATE;

    for (int i = 0; i < ps->max_particles; i++) 
    {
        ps->particles[i].position = ps->origin;
        //ps->particles[i].velocity = ps->min_speed;
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
    switch (ps->emitterType)
    {
    case RECTANGLE_EMITTER:

        ps->timer -= GetFrameTime();

        if (ps->timer <= 0)
        {   
            for (int i = 0; i < ps->max_particles; i++)
            {
                if (!ps->particles[i].active)
                {   
                    // Initialize particle
                    ps->particles[i].position.y = ps->origin.y;
                    ps->particles[i].position.x = GetRandomValue(ps->origin.x, ps->origin.x + ps->emitter.rectangleEmitter.width);
                    ps->particles[i].active = true;
                    break;
                }
            }
            
            ps->timer = PARTICLE_SPAWN_RATE;
            
        }

        for (int i = 0; i < ps->max_particles; i++) 
        {   
            if (!ps->particles[i].active) continue;

            ps->particles[i].position.y += ps->min_speed * GetFrameTime();
            ps->particles[i].lifetime -= GetFrameTime();

            if (ps->particles[i].lifetime <= 0)
            {
                ps->particles[i].active = false;
                ps->particles[i].lifetime = ps->lifetime;
            }
            
        }        
        break;
    case CIRCLE_EMITTER:
        break;
    case CONE_EMITTER:
        break;
    default:
        break;
    }
}

void DrawParticleSystem(ParticleSystem* ps)
{
    for (int i = 0; i < ps->max_particles; i++)
    {
        if (!ps->particles[i].active) continue;
        DrawCircleV(ps->particles[i].position, ps->min_size, ps->initial_color);
    }
}
