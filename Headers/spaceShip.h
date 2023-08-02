#ifndef SPACE_SHIP_CLASS_H
#define SPACE_SHIP_CLASS_H

#define GL_SILENCE_DEPRECATION
#define MAX_DISTANCE 200 // 200 Same as arena size

#define OBJECT_PATH "Objects/ship.obj"

#include <OpenGL/gl.h>

#include "struct.h"
#include "particleEffect.h"


class SpaceShip {
    public:
        SpaceShip(GLuint texture);
        void moveForward(float deltaTime);
        void moveBack(float deltaTime);
        void strafeLeft(float deltaTime);
        void strafeRight(float deltaTime);
        void rotateUp(float deltaTime);
        void rotateDown(float deltaTime);
        void rotateLeft(float deltaTime);
        void rotateRight(float deltaTime);
        void rotateClockWise(float deltaTime);
        void rotateCounterClockWise(float deltaTime);

        vec3f getForward();
        vec3f getBack();
        vec3f getCameraPosition();
        vec3f getRight();
        vec3f getLeft();
        vec3f getUp();
        vec3f getDown();

        vec3f getPosition();
        vec3f getRotation();

        void switchDoubleGun();
        bool isDoubleGun();

        bool maxDistanceTravelled();
        int getLives();
        void setLives(int lives);
   
        void reset();
        void render(bool moveForwardIsPressed, bool leftStrafeIsPressed, bool rightStrafeIsPressed, bool shootIsPressed, float deltaTime);

    private:
        ParticleEffect* leftEngineParticleEffect;
        ParticleEffect* rightEngineParticleEffect;
        GLuint texture;

        vec3f position;
        vec3f forward;
        vec3f cameraPosition;
        vec3f up;
        vec3f right;
        vec3f rotation;

        vec3f leftEnginePosition;
        vec3f rightEnginePosition;

        vec3f vertices[142];
        vec2f textureCoords[261];
        vec3f normals[103];
        faces faces[280];

        int lives;
        float radius;
        float maxSpeed;
        float turnLeftAndRightSpeed;
        float turnUpAndDownSpeed;
        float horizontalTiltAnimation;

        //float previousTime;
        bool isAlive;
        bool doubleGun;
        bool isRecoiling;
        bool isReturning;
        float recoil;
        float previousTime;

        void parseObjectFile();

        vec3f getLeftEngine(vec3f back, vec3f left, vec3f down);
        vec3f getRightEngine(vec3f back, vec3f right, vec3f down);
        vec3f addEngineFlames(float deltaTime, int n);
};
#endif