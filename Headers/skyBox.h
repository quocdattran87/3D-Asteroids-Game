#ifndef SKYBOX_CLASS_H
#define SKYBOX_CLASS_H

#define GL_SILENCE_DEPRECATION

#include <OpenGL/gl.h>


class SkyBox {
    public:
        SkyBox(GLuint front, GLuint back, GLuint left, GLuint right, GLuint up, GLuint down);
        void render(float cameraPos_x, float cameraPos_y, float cameraPos_z);

    private:
        GLuint frontTexture;
        GLuint backTexture;
        GLuint leftTexture;
        GLuint rightTexture;
        GLuint upTexture;
        GLuint downTexture;

        void draw_textured_quad();
};
#endif