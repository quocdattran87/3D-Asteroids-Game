#include "Headers/particleEffect.h"

ParticleEffect::ParticleEffect() {
    isRendering = false;
}

float get_rand_number(float min, float max)
{
    float scale = rand() / (float)RAND_MAX;
    return min + scale * (max - min);
}

float map(float x, float in_min, float in_max, float out_min, float out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

vec3f vec3fNormalise(vec3f v)
{
    vec3f r = {0.0,0.0,0.0}; //Default

    // NOTE: This could cause a math error of length is 0
    // and could happen if a 0,0,0 is normalised
    // for students to solve! (if needed)
    const float length = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    r.x = v.x / length;
    r.y = v.y / length;
    r.z = v.z / length;

    return r;
}

/**
 * Spawns a new particle using the effect variables
 */
void ParticleEffect::spawnParticle(particle *p)
{
    // reset particle timer and set random live time
    p->elapsed_time = 0.0f;
    p->death_time = get_rand_number(min_live_time, max_live_time);

    // calculate particle position given radius
    p->position.x = get_rand_number(-spawn_radius_x, spawn_radius_x);
    p->position.y = 0.0f;
    p->position.z = get_rand_number(-spawn_radius_z, spawn_radius_z);

    // set direction to be a few units from spawn location in y axis at center
    vec3f dir = vec3fNormalise((vec3f){.x = -p->position.x, .y = 3.0f, .z = -p->position.z});
    float speed = get_rand_number(min_speed, max_speed);

    // set random speed
    p->velocity.x = dir.x * speed;
    p->velocity.y = dir.y * speed;
    p->velocity.z = dir.z * speed;

    // set the size of this particle
    p->size = get_rand_number(min_size, max_size);

    p->isRendering = false;

    p->colour = 0.0;
}

void ParticleEffect::createRandomEffect()
{
    min_speed = 1.0f;
    max_speed = 3.0f;
    spawn_radius_x = 0.21f;
    spawn_radius_z = 0.34f;
    min_live_time = 0.3f;
    max_live_time = 0.7f;
    min_size = 5.0f;
    max_size = 8.0f;

    for (int i = 0; i < NUMBER_OF_PARTICLES; i++)
    {
        spawnParticle(&particles[i]);
    }
}

void ParticleEffect::updateParticle(particle *p, float dt)
{
    p->elapsed_time += dt;

    // reset particle - aka kill it
    if (p->elapsed_time > p->death_time)
    {
        // this is honestly a poor chain-of-responsibility
        spawnParticle(p);
    }
    else
    {
        p->position.x += p->velocity.x * dt;
        p->position.y += p->velocity.y * dt;
        p->position.z += p->velocity.z * dt;
    }
}

void ParticleEffect::updateEffect(float dt)
{
    for (int i = 0; i < NUMBER_OF_PARTICLES; i++)
    {
        if (particles[i].isRendering) {
            updateParticle(&particles[i], dt);
        }
    }
}

void ParticleEffect::renderParticle(particle *p, float deltaTime)
{   
    if (p->elapsed_time < p->death_time) {
        p->isRendering = true;
    } else {
        p->isRendering = false;
    }

    p->size -= 4.9 * deltaTime;
    if (p->size <= 0.0) {
        p->size = 0.000001;
    }
    p->colour += 1.1 * deltaTime;
    if (p->colour >= 1.0) {
        p->colour = 1.0;
    }
    glColor3f(1.0,p->colour,0.0);
    glPushMatrix();
        glPointSize(p->size);
        glBegin(GL_POINTS);
            glVertex3f(p->position.x, p->position.y, p->position.z);
        glEnd();
    glPopMatrix();
}

void ParticleEffect::render(vec3f enginePosition, vec3f rotation, bool W_IsPressed, bool A_IsPressed, bool D_IsPressed, float deltaTime)
{
    glPushMatrix();
        glTranslatef(enginePosition.x, enginePosition.y, enginePosition.z);
        glRotatef(rotation.x, 1, 0, 0);
        glRotatef(rotation.y, 0, 1, 0);
        glRotatef(rotation.z, 0, 0, 1);

        if (W_IsPressed || A_IsPressed || D_IsPressed) {
            for (int i = 0; i < NUMBER_OF_PARTICLES; i++)
            {
                renderParticle(&particles[i], deltaTime);
            }
        }
    glPopMatrix();
}

void ParticleEffect::render2(vec3f enginePosition, vec3f rotation, float deltaTime) {
    glPushMatrix();
        glTranslatef(enginePosition.x, enginePosition.y, enginePosition.z);
        glRotatef(rotation.x, 1, 0, 0);
        glRotatef(rotation.y, 0, 1, 0);
        glRotatef(rotation.z, 0, 0, 1);
        for (int i = 0; i < NUMBER_OF_PARTICLES; i++) {
            if (particles[i].isRendering) {
                renderParticle(&particles[i], deltaTime);
            }
        }
    glPopMatrix();
}