#define GL_SILENCE_DEPRECATION

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>

#include "Headers/arena.h"

#include <iostream>

Arena::Arena() {
	meshSpace = (float)2*BORDER_SIZE/NUMBER_OF_GRIDS;
}

void Arena::drawTopPlane(float ship_z_position) {
	if (ship_z_position < 0.7 * -BORDER_SIZE) {
 		glColor3f(0.8,0,0);
		glLineWidth(3);
	} else {
		glColor3f(0.35, 0.35, 0.35);
		glLineWidth(1);
	}
	glBegin(GL_LINE_LOOP);
	glVertex3f(-BORDER_SIZE, -BORDER_SIZE, -BORDER_SIZE);
	glVertex3f( BORDER_SIZE, -BORDER_SIZE, -BORDER_SIZE);
	glVertex3f( BORDER_SIZE,  BORDER_SIZE, -BORDER_SIZE);
	glVertex3f(-BORDER_SIZE,  BORDER_SIZE, -BORDER_SIZE);
	glEnd();

	float last_mesh = BORDER_SIZE;
	for (int i = 0; i < NUMBER_OF_GRIDS-1; i++) {
		last_mesh -= meshSpace;		
		glBegin(GL_LINES);
		glVertex3f(last_mesh, -BORDER_SIZE, -BORDER_SIZE); // Vertical grid
		glVertex3f(last_mesh,  BORDER_SIZE, -BORDER_SIZE);
		glVertex3f(-BORDER_SIZE, last_mesh, -BORDER_SIZE); // Horizontal grid
		glVertex3f( BORDER_SIZE, last_mesh, -BORDER_SIZE);
		glEnd();
	}
}

void Arena::drawLeftPlane(float ship_x_position) {
	if (ship_x_position < 0.7 * -BORDER_SIZE) {
 		glColor3f(1.0,0,0);
		glLineWidth(3);
	} else {
		glColor3f(0.35, 0.35, 0.35);
		glLineWidth(1);
	}
	glBegin(GL_LINE_LOOP);
	glVertex3f(-BORDER_SIZE, -BORDER_SIZE, -BORDER_SIZE);
	glVertex3f(-BORDER_SIZE,  BORDER_SIZE, -BORDER_SIZE);
	glVertex3f(-BORDER_SIZE,  BORDER_SIZE,  BORDER_SIZE);
	glVertex3f(-BORDER_SIZE, -BORDER_SIZE,  BORDER_SIZE);
	glEnd();

	float last_mesh = BORDER_SIZE;
	for (int i = 0; i < NUMBER_OF_GRIDS-1; i++) {
		last_mesh -= meshSpace;	
		glBegin(GL_LINES);
		glVertex3f(-BORDER_SIZE, -BORDER_SIZE, last_mesh); // Vertical grid
		glVertex3f(-BORDER_SIZE,  BORDER_SIZE, last_mesh);
		glVertex3f(-BORDER_SIZE, last_mesh, -BORDER_SIZE); // Horizontal grid
		glVertex3f(-BORDER_SIZE, last_mesh,  BORDER_SIZE);
		glEnd();
	}
}

void Arena::drawRightPlane(float ship_x_position) {
	if (ship_x_position > 0.7 * BORDER_SIZE) {
 		glColor3f(1.0,0,0);
		glLineWidth(3);
	} else {
		glColor3f(0.35, 0.35, 0.35);
		glLineWidth(1);
	}
	glBegin(GL_LINE_LOOP);
	glVertex3f(BORDER_SIZE, -BORDER_SIZE,  BORDER_SIZE);
	glVertex3f(BORDER_SIZE, -BORDER_SIZE, -BORDER_SIZE);
	glVertex3f(BORDER_SIZE,  BORDER_SIZE, -BORDER_SIZE);
	glVertex3f(BORDER_SIZE,  BORDER_SIZE,  BORDER_SIZE);
	glEnd();

	float last_mesh = BORDER_SIZE;
	for (int i = 0; i < NUMBER_OF_GRIDS-1; i++) {
		last_mesh -= meshSpace;	
		glBegin(GL_LINES);
		glVertex3f(BORDER_SIZE, -BORDER_SIZE, last_mesh); // Vertical grid
		glVertex3f(BORDER_SIZE,  BORDER_SIZE, last_mesh);
		glVertex3f(BORDER_SIZE, last_mesh, -BORDER_SIZE); // Horizontal grid
		glVertex3f(BORDER_SIZE, last_mesh,  BORDER_SIZE);
		glEnd();
	}
}

void Arena::drawBottomPlane(float ship_z_position) {
	if (ship_z_position > 0.7 * BORDER_SIZE) {
 		glColor3f(1.0,0,0);
		glLineWidth(3);
	} else {
		glColor3f(0.35, 0.35, 0.35);
		glLineWidth(1);
	}
	glBegin(GL_LINE_LOOP);
	glVertex3f( BORDER_SIZE, -BORDER_SIZE, BORDER_SIZE);
	glVertex3f( BORDER_SIZE,  BORDER_SIZE, BORDER_SIZE);
	glVertex3f(-BORDER_SIZE,  BORDER_SIZE, BORDER_SIZE);
	glVertex3f(-BORDER_SIZE, -BORDER_SIZE, BORDER_SIZE);
	glEnd();

	float last_mesh = BORDER_SIZE;
	for (int i = 0; i < NUMBER_OF_GRIDS-1; i++) {
		last_mesh -= meshSpace;	
		glBegin(GL_LINES);
		glVertex3f(last_mesh, -BORDER_SIZE, BORDER_SIZE); // Vertical grid
		glVertex3f(last_mesh,  BORDER_SIZE, BORDER_SIZE);
		glVertex3f(-BORDER_SIZE, last_mesh, BORDER_SIZE); // Horizontal grid
		glVertex3f( BORDER_SIZE, last_mesh, BORDER_SIZE);
		glEnd();
	}
}

void Arena::drawFrontPlane(float ship_y_position) {
	if (ship_y_position < 0.7 * -BORDER_SIZE) {
 		glColor3f(1.0,0,0);
		glLineWidth(3);
	} else {
		glColor3f(0.35, 0.35, 0.35);
		glLineWidth(1);
	}
	glBegin(GL_LINE_LOOP);
	glVertex3f(-BORDER_SIZE, -BORDER_SIZE, -BORDER_SIZE);
	glVertex3f( BORDER_SIZE, -BORDER_SIZE, -BORDER_SIZE);
	glVertex3f( BORDER_SIZE, -BORDER_SIZE,  BORDER_SIZE);
	glVertex3f(-BORDER_SIZE, -BORDER_SIZE,  BORDER_SIZE);
	glEnd();

	float last_mesh = BORDER_SIZE;
	for (int i = 0; i < NUMBER_OF_GRIDS-1; i++) {
		last_mesh -= meshSpace;		
		glBegin(GL_LINES);
		glVertex3f(last_mesh, -BORDER_SIZE, -BORDER_SIZE); // Horizontal grid
		glVertex3f(last_mesh, -BORDER_SIZE,  BORDER_SIZE);
		glVertex3f(-BORDER_SIZE, -BORDER_SIZE, last_mesh); // Vertical grid
		glVertex3f( BORDER_SIZE, -BORDER_SIZE, last_mesh);
		glEnd();
	}
}

void Arena::drawBackPlane(float ship_y_position) {
	if (ship_y_position > 0.7 * BORDER_SIZE) {
 		glColor3f(1.0,0,0);
		glLineWidth(3);
	} else {
		glColor3f(0.35, 0.35, 0.35);
		glLineWidth(1);
	}
	glBegin(GL_LINE_LOOP);
	glVertex3f( BORDER_SIZE, BORDER_SIZE,  BORDER_SIZE);
	glVertex3f( BORDER_SIZE, BORDER_SIZE, -BORDER_SIZE);
	glVertex3f(-BORDER_SIZE, BORDER_SIZE, -BORDER_SIZE);
	glVertex3f(-BORDER_SIZE, BORDER_SIZE,  BORDER_SIZE);
	glEnd();

	float last_mesh = BORDER_SIZE;
	for (int i = 0; i < NUMBER_OF_GRIDS-1; i++) {
		last_mesh -= meshSpace;		
		glBegin(GL_LINES);
		glVertex3f(last_mesh, BORDER_SIZE, -BORDER_SIZE); // Horizontal grid
		glVertex3f(last_mesh, BORDER_SIZE,  BORDER_SIZE);
		glVertex3f(-BORDER_SIZE, BORDER_SIZE, last_mesh); // Vertical grid
		glVertex3f( BORDER_SIZE, BORDER_SIZE, last_mesh);
		glEnd();
	}
}

void Arena::render(float ship_x_position, float ship_y_position, float ship_z_position) {
    drawFrontPlane(ship_y_position);
    drawLeftPlane(ship_x_position);
    drawRightPlane(ship_x_position);
    drawBackPlane(ship_y_position);
    drawBottomPlane(ship_z_position);
    drawTopPlane(ship_z_position);
}