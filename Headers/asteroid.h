#ifndef ASTEROID_CLASS_H
#define ASTEROID_CLASS_H

#define STACKS 16
#define SLICES 16

#include <vector>
#include "struct.h"

class Asteroid {
    public:
        Asteroid(GLuint texture, int health, vec3f shipPosition);
        bool crash(vec3f otherPosition);
        void reduceHealth();
        int getHealth();
        void setHealth(int n);
        vec3f getPosition();
        void rotate(float deltaTime);
        void move(float deltaTime, vec3f shipPosition);
        void render();

    private:
        GLuint texture;
        int health;
        float radius;
        float maxSpeed;
        // float rotationSpeed_x; 
        // float rotationSpeed_y; 
        // float rotationSpeed_z; 

        vec3f position;
        vec3f direction;

        std::vector<float> randomEdges;
        vec3f vertices[STACKS][SLICES];
        vec2f texes[STACKS][SLICES];
 
        bool inArena;
        int side;
        int rotationX;
        int rotationY;
        int rotationZ;
        vec3f rotation;

        void aimAtPlayer(vec3f shipPosition);
};

#endif