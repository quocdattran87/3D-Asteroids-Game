#ifndef BULLET_CLASS_H
#define BULLET_CLASS_H

#define GL_SILENCE_DEPRECATION
#define MAX_BULLET_DISTANCE 1500 //250 Same as arena size

#include "struct.h"


class Bullet {
    public:
        Bullet(vec3f shipPosition, vec3f shipForward, vec3f shipLeft, vec3f shipRight, animation anim, int gun);
        void move(float deltaTime);
        vec3f getPosition();
        bool maxDistanceTravelled();
        void render(float deltaTime);

    private:
        float speed;
    	vec3f position; // x,y,z position
        vec3f forward; // x,y,z forward direction
        vec3f scale;	// x,y,z scale
        animation anim; // our animation

        void billboardCheatSphericalBegin();
        void billboardEnd();
};
#endif