#ifndef EXPLOSION_CLASS_H
#define EXPLOSION_CLASS_H

#include "struct.h"

class Explosion {
    public:
        Explosion(vec3f position, animation anim, float scale);
        bool animationIsFinished();
        void render(float deltaTime);

    private:
        animation anim;
        vec3f position;
        vec3f scale;

        bool animationFinished;

        void billboardCheatSphericalBegin();
        void billboardEnd();
};
#endif