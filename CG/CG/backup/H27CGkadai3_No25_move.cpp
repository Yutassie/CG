#define _USE_MATH_DEFINES

#include <stdlib.h>
#include <glut.h>
#include <stdio.h>
#include <math.h>
#include "ppmload2.h"
#include "func.h"

void disp_moving_sheep(double size, double x, double y, double z, double axis_x, double axis_y, double axis_z, double theta, double time){
	GLfloat wool[] = { 1.0, 0.9, 0.7 };
	GLfloat skin[] = { 2.0, 1.8, 1.3 };
	GLfloat eye[] = { 0.0, 0.0, 0.0 };


	//órñ—
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, wool);
	putanimalcube(size, x + time*cos(theta * M_PI / 180), z, y + time*sin(theta * M_PI / 180), axis_x, axis_y, axis_z, theta, 0, 0, 0);
	putanimalcube(size / 1.375, x + time*cos(theta * M_PI / 180), z, y + time*sin(theta * M_PI / 180), axis_x, axis_y, axis_z, theta, size / 2.0, size / 3.0, 0);
	//êKîˆ
	putanimalcube(size / 1.5, x + time*cos(theta * M_PI / 180), z, y + time*sin(theta * M_PI / 180), axis_x, axis_y, axis_z, theta, -size / 1.75, 0, 0);
	//ãrïî
	putanimalcube(size / 3.0, x + time*cos(theta * M_PI / 180), z, y + time*sin(theta * M_PI / 180), axis_x, axis_y, axis_z, theta, size / 3.125, -size / 2.0, size / 3.125);
	putanimalcube(size / 3.0, x + time*cos(theta * M_PI / 180), z, y + time*sin(theta * M_PI / 180), axis_x, axis_y, axis_z, theta, size / 3.125, -size / 2.0, -size / 3.125);
	putanimalcube(size / 3.0, x + time*cos(theta * M_PI / 180), z, y + time*sin(theta * M_PI / 180), axis_x, axis_y, axis_z, theta, -size / 3.125, -size / 2.0, size / 3.125);
	putanimalcube(size / 3.0, x + time*cos(theta * M_PI / 180), z, y + time*sin(theta * M_PI / 180), axis_x, axis_y, axis_z, theta, -size / 3.125, -size / 2.0, -size / 3.125);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, skin);
	putanimalcube(size / 3.5, x + time*cos(theta * M_PI / 180), z, y + time*sin(theta * M_PI / 180), axis_x, axis_y, axis_z, theta, size / 3.125, -size / 1.25, size / 3.125);
	putanimalcube(size / 3.5, x + time*cos(theta * M_PI / 180), z, y + time*sin(theta * M_PI / 180), axis_x, axis_y, axis_z, theta, size / 3.125, -size / 1.25, -size / 3.125);
	putanimalcube(size / 3.5, x + time*cos(theta * M_PI / 180), z, y + time*sin(theta * M_PI / 180), axis_x, axis_y, axis_z, theta, -size / 3.125, -size / 1.25, size / 3.125);
	putanimalcube(size / 3.5, x + time*cos(theta * M_PI / 180), z, y + time*sin(theta * M_PI / 180), axis_x, axis_y, axis_z, theta, -size / 3.125, -size / 1.25, -size / 3.125);
	//äÁ
	putanimalcube(size / 2.0, x + time*cos(theta * M_PI / 180), z, y + time*sin(theta * M_PI / 180), axis_x, axis_y, axis_z, theta, size / 1.375, size / 3, 0);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, eye);
	putcubeline(size / 13.0, x + time*cos(theta * M_PI / 180), z, y + time*sin(theta * M_PI / 180), 2.0, direction::LEFT_REV, axis_x, axis_y, axis_z, theta, size / 1.0625, size / 2.875, size / 5.0);
	putcubeline(size / 13.0, x + time*cos(theta * M_PI / 180), z, y + time*sin(theta * M_PI / 180), 2.0, direction::LEFT_REV, axis_x, axis_y, axis_z, theta, size / 1.0625, size / 2.875, -size / 9.0);
}

void kneebra(double x, double y, double z, double axis_x, double axis_y, double axis_z, double theta, double win_posx, double win_posy, double win_posz){
	GLfloat wool[] = { 1.0, 0.9, 0.7 };

	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, wool);
	putcubeline(0.1, x + 1.5, z-0.125 , -y-0.25 , 3.0, direction::LEFT_REV, 0, 10, -1, theta-120.0, 0, 0, -0.125);
}