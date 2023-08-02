#ifndef _PARTICLE_EFFECT_H_
#define _PARTICLE_EFFECT_H_

#define GL_SILENCE_DEPRECATION
// #include <stdlib.h>
#include "struct.h"

#define NUMBER_OF_PARTICLES 2000


class ParticleEffect {
    public:
        ParticleEffect();
        void createRandomEffect();
        void updateEffect(float dt);
        void render(vec3f enginePosition, vec3f rotation, bool W_IsPressed, bool A_IsPressed, bool D_IsPressed, float deltaTime);
        void render2(vec3f enginePosition, vec3f rotation, float deltaTime);

    private:
        bool isRendering;
        particle particles[NUMBER_OF_PARTICLES]; // the number of particles under this effects control
        
        float min_speed;          // the minimum animation speed for particles
        float max_speed;          // the maximum animation speed for particles
        float min_size;           // the smallest sided particle
        float max_size;           // the largest sized particle
        float spawn_radius_x;
        float spawn_radius_z;     // the radius to use for randomly spawning a particle
        float min_live_time;      // the minimum time the particle can be alive until destroyed
        float max_live_time;      // the maximum time the particle can be alive until destroyed

        void updateParticle(particle *p, float dt);
        void spawnParticle(particle *p);
        void renderParticle(particle *p, float deltaTime);
};
#endif