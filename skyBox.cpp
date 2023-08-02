#include "Headers/skyBox.h"

SkyBox::SkyBox(GLuint front, GLuint back, GLuint left, GLuint right, GLuint up, GLuint down) {
    frontTexture = front;
    backTexture = back;
    leftTexture = left;
    rightTexture = right;
    upTexture = up;
    downTexture = down;
}

void SkyBox::draw_textured_quad()
{
    // calculate UV coordinates for each vertex
    const float uv_start_x = 0.0f;
    const float uv_start_y = 0.0f;
    const float uv_width = 1.0f;
    const float uv_height = 1.0f;

    glBegin(GL_POLYGON);
    glTexCoord2f(uv_start_x, uv_start_y);
    glVertex2f(-1, -1);

    glTexCoord2f(uv_start_x + uv_width, uv_start_y);
    glVertex2f(1, -1);

    glTexCoord2f(uv_start_x + uv_width, uv_start_y + uv_height);
    glVertex2f(1, 1);

    glTexCoord2f(uv_start_x, uv_start_y + uv_height);
    glVertex2f(-1, 1);
    glEnd();
}

void SkyBox::render(float cameraPos_x, float cameraPos_y, float cameraPos_z)
{
    // enable textures, disable depth testing
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    glDepthMask(false);
    glDisable(GL_LIGHTING);
    glDisable(GL_BLEND);
    glColor3f(1,1,1);

    const float scale = 280.0f;

    // front
    glBindTexture(GL_TEXTURE_2D, frontTexture);
    glPushMatrix();
        glTranslatef(cameraPos_x, -scale+cameraPos_y, cameraPos_z);
        glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
        glScalef(scale, scale, scale);
        draw_textured_quad();
    glPopMatrix();

    // back
    glBindTexture(GL_TEXTURE_2D, backTexture);
    glPushMatrix();
        glTranslatef(cameraPos_x, scale+cameraPos_y, cameraPos_z);
        glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
        glScalef(scale, scale, scale);
        draw_textured_quad();
    glPopMatrix();

    // left
    glBindTexture(GL_TEXTURE_2D, leftTexture);
    glPushMatrix();
        glTranslatef(-scale+cameraPos_x, cameraPos_y, cameraPos_z);
        glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
        glScalef(scale, scale, scale);
        draw_textured_quad();
    glPopMatrix();

    // right
    glBindTexture(GL_TEXTURE_2D, rightTexture);
    glPushMatrix();
        glTranslatef(scale+cameraPos_x, cameraPos_y, cameraPos_z);
        glRotatef(90.0f, 0.0f, -1.0f, 0.0f);
        glScalef(scale, scale, scale);
        draw_textured_quad();
    glPopMatrix();

    // up
    glBindTexture(GL_TEXTURE_2D, upTexture);
    glPushMatrix();
        glColor3f(1,1,1);
        glTranslatef(cameraPos_x, cameraPos_y, -scale+cameraPos_z);
        glScalef(scale, scale, scale);
        draw_textured_quad();
    glPopMatrix();

    // down
    glBindTexture(GL_TEXTURE_2D, downTexture);
    glPushMatrix();
        glTranslatef(cameraPos_x, cameraPos_y, scale+cameraPos_z);
        glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
        glScalef(scale, scale, scale);
        draw_textured_quad();
    glPopMatrix();

    glBindTexture(GL_TEXTURE_2D, 0);

    // restore stuff
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    // glEnable(GL_LIGHTING);
    glDepthMask(true);
    glClear(GL_DEPTH_BUFFER_BIT);
}