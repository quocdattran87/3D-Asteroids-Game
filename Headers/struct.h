#ifndef STRUCT_CLASS_H
#define STRUCT_CLASS_H

#include <math.h>
#include <sstream>
#include <vector>
#include <iostream>
#include <fstream>
#include <OpenGL/gl.h>


#define PI 3.141592653589793238
#define DEG_2_RAD PI / 180.0
#define RAD_2_DEG 180.0 / PI




typedef struct {
    float x, y, z;
} vec3f;

typedef struct {
    float x, y;
} vec2f;

typedef struct {
    int x, y, z;
} vec3int;

typedef struct {
   vec3int x, y, z;
} faces;

typedef struct {
   int width, height, x_pos, y_pos;
   float fov;
   float m_near;
   float m_far;
   float last_time;
   bool fullscreen;
   bool wireframe;
} g_screen;

typedef struct {
	vec2f position; // UV x/y start position
	vec2f size;		 // UV width/height
} animation_frame;

typedef struct {
	animation_frame frames[200]; // our animation frames
	GLuint texture;				 // texture location in GPU as loaded by texture loader
	float speed;				 // speed of our animation
	float timeAccum;			 // accumulated timer to trigger animation
	int cframe;					 // our current animation frame
	int maxFrame;				 // maximum frames to render before looping
} animation;

typedef struct g_particle
{
    vec3f position;     // position of this particle in 3D space
    vec3f velocity;     // the speed and direction of this particle
    float size;         // size of this particle
    float death_time;   // when will this particle die?
    float elapsed_time; // the elapsed time in ms since this particle spawned
    float colour;
    bool isRendering;
} particle;

#endif