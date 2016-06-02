#include <stdlib.h>
#include <glut.h>
#include <stdio.h>
#include "ppmload2.h"

enum direction{
	LEFT,
	LEFT_REV,
	RIGHT,
	RIGHT_REV,
	UP,
	DOWN
};

enum treetype{
	Softwood,
	Broadleaf
};

enum texture{
	intend,
	extend,
	repeat_x,
	repeat_y
};

enum cubefirstpos{
	CENTER,
	LEFTCORNER,
};

char TitleString[] = "�͂��߂Ă�OpenGL";
//���_���Œ�ix,y,z(���s)�j
GLfloat eye[] = { 30.0, 40.0, 50.0 };
//GLfloat eye[] = { 10.0, 20.0, 50.0 };

void myinit(void)
{
	GLfloat light_position[] = { 100.0, 100.0, 100.0, 1.0 };

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	//�E�C���h��h��Ԃ�(�w�i)
	glClearColor(0.0, 100.0, 100.0, 1.0);


	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
}

//�t�@�C���l�[���A����̊p�w��A�e�N�X�`���̃^�C�v���w��
void map_texture(char *texture, double size, double x, double y, double z, int texture_type){
	struct ppmimg *image = NULL;
	if (image == NULL)
		image = (struct ppmimg *)malloc(sizeof(ppmimg));
	image->dat = NULL;
	loadppmimage(texture, image);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->iwidth, image->iheight, 0, GL_RGB, GL_UNSIGNED_BYTE, image->dat);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, image->iwidth, image->iheight, GL_RGB, GL_UNSIGNED_BYTE, image->dat);

	switch (texture_type){
	case texture::intend: {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		break;
	}
	case texture::extend: {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		break;
	}
	case texture::repeat_x: {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		break;
	}
	case texture::repeat_y: {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		break;
	}
	}
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glEnable(GL_TEXTURE_2D);
	GLdouble norm[3];
	glPushMatrix();
	glBegin(GL_POLYGON);
	norm[0] = 1;
	norm[1] = norm[2] = 0;
	glNormal3dv(norm);
	glTexCoord2f(0.0, 0.0); glVertex3d(x, z, y);
	glTexCoord2f(0.0, 1.0); glVertex3d(x, z, y + size);
	glTexCoord2f(1.0, 1.0); glVertex3d(x + size, z, y + size);
	glTexCoord2f(1.0, 0.0); glVertex3d(x + size, z, y);

	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

//���W�̕\��
void coordinateprint(){
	GLfloat red[] = { 10.0, 0, 0, 1.0 };
	GLfloat blue[] = { 0.0, 0, 10.0, 1.0 };
	GLfloat green[] = { 0.0, 10.0, 0, 1.0 };

	glBegin(GL_POINTS);
	//glPushMatrix();
	for (double i = -100; i < 0; i += 0.1){
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, red);
		glVertex3d(i, 0, 0);
	}
	//glPopMatrix();
	//glPushMatrix();
	for (double j = -100; j < 0; j += 0.1){
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, green);
		glVertex3d(0, j, 0);
	}
	//glPopMatrix();
	//glPushMatrix();
	for (double k = -100; k < 0; k += 0.1){
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blue);
		glVertex3d(0, 0, k);
	}
	//glPopMatrix();
	glEnd();
	glBegin(GL_LINES);
	//glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, red);
	glVertex3d(0, 0, 0);
	glVertex3d(100, 0, 0);
	//glPopMatrix();
	//glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, green);
	glVertex3d(0, 0, 0);
	glVertex3d(0, 100, 0);
	//glPopMatrix();
	//glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blue);
	glVertex3d(0, 0, 0);
	glVertex3d(0, 0, 100);
	//glPopMatrix();
	glEnd();
}

//�^�C���̕\��(����̊p��(x,y,z)�Ƃ���)
void disp_tile(double size, double x, double y, double z, GLfloat col[4]){
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, col);
	glPushMatrix();
	glBegin(GL_QUADS);
	glVertex3d(x, z, y);
	glVertex3d(x + size, z, y);
	glVertex3d(x + size, z, y + size);
	glVertex3d(x, z, y + size);
	glPopMatrix();
	glEnd();
}

//�����̂�`��i���サ����w��j
void putcube(float size, float x, float z, float y, double axis_x, double axis_y, double axis_z, double theta){
	y = -y;
	double axis_n = sqrt(pow(axis_x, 2) + pow(axis_y, 2) + pow(axis_z, 2));
	if (axis_n > 0.0){
		axis_x /= axis_n;
		axis_y /= axis_n;
		axis_z /= axis_n;
	}
	//Push��Pop���s�����ƂŐ�Έʒu�w��ɕύX
	glPushMatrix();
	if (theta != 0 && axis_n > 0.0){
		glTranslatef(x, z, y);
		glRotatef(theta, axis_x, axis_y, axis_z);
		glTranslatef(-x, -z, -y);
	}
	glTranslatef(x, z, y);
	//          �@ ���@��@�E
	glutSolidCube(size);
	glPopMatrix();
}

//�����̃L���[�u��`��dif �ɂ͒��S����̍��ق��w��
void putanimalcube(float size, float x, float z, float y, double axis_x, double axis_y, double axis_z, double theta, double dif_x, double dif_z, double dif_y){
	y = -y;
	double axis_n = sqrt(pow(axis_x, 2) + pow(axis_y, 2) + pow(axis_z, 2));
	if (axis_n > 0.0){
		axis_x /= axis_n;
		axis_y /= axis_n;
		axis_z /= axis_n;
	}
	//Push��Pop���s�����ƂŐ�Έʒu�w��ɕύX
	glPushMatrix();
	if (theta != 0 && axis_n > 0.0){
		glTranslatef(x, z, y);
		glRotatef(theta, axis_x, axis_y, axis_z);
		glTranslatef(-x, -z, -y);
	}
	glTranslatef(x + dif_x, z + dif_z, y+dif_y);
	//          �@ ���@��@�E
	glutSolidCube(size);
	glPopMatrix();
}


//�����̂�`��i�傫���A�J�n�_�A�����A�����j
void putcubeline(float size, float x, float z, float y, int lng, char dir, double axis_x, double axis_y, double axis_z, double theta, double dif_x, double dif_z, double dif_y){
	y = -y;
	double axis_n = sqrt(pow(axis_x, 2) + pow(axis_y, 2) + pow(axis_z, 2));
	if (axis_n > 0.0){
		axis_x /= axis_n;
		axis_y /= axis_n;
		axis_z /= axis_n;
	}

	glPushMatrix();
	if (theta != 0 && axis_n > 0.0){
		glTranslatef(x, z, y);
		glRotatef(theta, axis_x, axis_y, axis_z);
		glTranslatef(-x, -z, -y);
	}
	glTranslatef(x+dif_x, z+dif_z, y+dif_y);
	//          �@ ���@��@�E
	switch (dir){
	case LEFT:
		for (int i = 0; i < lng; i++){
			glutSolidCube(size);
			glTranslatef(0.0, 0.0, size);
		} break;
	case LEFT_REV:
		for (int i = 0; i < lng; i++){
			glutSolidCube(size);
			glTranslatef(0.0, 0.0, -size);
		} break;
	case RIGHT:
		for (int i = 0; i < lng; i++){
			glutSolidCube(size);
			glTranslatef(size, 0.0, 0.0);
		} break;
	case RIGHT_REV:
		for (int i = 0; i < lng; i++){
			glutSolidCube(size);
			glTranslatef(-size, 0.0, 0.0);
		} break;
	case UP:
		for (int i = 0; i < lng; i++){
			glutSolidCube(size);
			glTranslatef(0.0, size, 0.0);
		} break;
	case DOWN:
		for (int i = 0; i < lng; i++){
			glutSolidCube(size);
			glTranslatef(0.0, -size, 0.0);
		} break;
	}
	glPopMatrix();
}

//�O�p����`��i��ʂ̑傫���A����̊p�A�������w��j
void putpyramid(double size, double height, double x, double y, double z){
	GLfloat green[] = { 0.1, 1.0, 0.4 };

	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, green);
	glBegin(GL_QUADS);
	glVertex3d(x, z, y);
	glVertex3d(x + size, z, y);
	glVertex3d(x + size, z, y + size);
	glVertex3d(x, z, y + size);
	glPopMatrix();

	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, green);
	glBegin(GL_TRIANGLES);
	glVertex3d(x, z, y);
	glVertex3d(x + size, z, y);
	glVertex3d(x + size / 2, z + height, y + size / 2);
	glPopMatrix();

	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, green);
	glBegin(GL_TRIANGLES);
	glVertex3d(x + size, z, y);
	glVertex3d(x + size, z, y + size);
	glVertex3d(x + size / 2, z + height, y + size / 2);
	glPopMatrix();

	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, green);
	glBegin(GL_TRIANGLES);
	glVertex3d(x + size, z, y + size);
	glVertex3d(x, z, y + size);
	glVertex3d(x + size / 2, z + height, y + size / 2);
	glPopMatrix();

	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, green);
	glBegin(GL_TRIANGLES);
	glVertex3d(x, z, y + size);
	glVertex3d(x, z, y);
	glVertex3d(x + size / 2, z + height, y + size / 2);
	glPopMatrix();
}

void putcubecube(float x, float z, float y, int Rlng, int Llng, char FirstPos){
	y = -y;
	glPushMatrix();
	glTranslatef(x, z, y);
	glPopMatrix();
}

//�n�ʂ̕\��
void disp_ground(void){
	/*glBEGIN�̈����̉��
	GL_LINES          : 2�_�Ԓ���
	GL_POINTS         : �_
	GL_LINE_STRIP     : �܂��
	GL_LINE_LOOP      : �܂���i�n�_�ƏI�_�����ԁj
	GL_TRIANGLES      : �O�p�`�i3�_�w��j
	GL_QUADS          : �l�p�`�i4�_�w��j
	GL_TRIANGLE_STRIP : ��ӂ����L����O�p�`�̕`��i3�_�ȏ�̎w��j�X�l�[�N�p�Y���݂����Ȑ}�`
	GL_QUAD_STRIP     : ��ӂ����L����l�p�`�̕`��i4�_�ȏ�̎w��j�l�p�`�̘A��
	GL_TRIANGLE_FAN   : �я�ɎO�p�`�������i�Z���X�̃C���[�W�j
	GL_POLYGON        : �ʑ��p�`�̕`��
	*/
	glPushMatrix();
	GLfloat gray[] = { 0.8, 0.8, 0.8, 1.0 };
	GLfloat white[] = { 255, 255, 255, 1.0 };
	GLfloat green[] = { 0.1, 1.0, 0.4 };
	for (double i = 0; i < 10; i++){
		disp_tile(2, -1, -10 + (i * 2), 0, gray); //road
	}
	for (double i = 0; i < 8; i++){
		disp_tile(0.25, -0.0625, -9 + (i * 2), 0.0625, white); //white line
		disp_tile(0.25, -0.0625, -8 + (i * 2), 0.0625, white);
		disp_tile(0.25, -0.0625, -7 + (i * 2), 0.0625, white);
		disp_tile(0.25, -0.0625, -6 + (i * 2), 0.0625, white);
		disp_tile(0.25, -0.0625, -5 + (i * 2), 0.0625, white);
	}

	//disp_tile(20, -10, -10, -0.0625, green); //grass

	map_texture("grass.ppm", 20, -10, -10, -0.0625, texture::repeat_y);

}

//�؁A�����̒��S���w��
void disp_wood(double size, double x, double y, double z, int type, double axis_x, double axis_y, double axis_z, double theta){
	GLfloat wood[] = { 0.5, 0.3, 0.2 };
	GLfloat leaf[] = { 0.3, 0.55, 0.3 };
	z = z + size / 2;

	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, wood);
	putcubeline(size, x, z, y, 3.0, direction::UP, axis_x, axis_y, axis_z, theta, 0, 0, 0);

	switch (type){
	case treetype::Softwood:

		break;
	case treetype::Broadleaf:
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, leaf);
		putcube(size * 3.0, x - size*1.125, z + size * 3.0, y + size*1.25, axis_x, axis_y, axis_z, theta);
		putcube(size * 2.25, x + size*0.75, z + size * 2.75, y + size * 0.50, axis_x, axis_y, axis_z, theta);
		putcube(size * 1.5, x + size*0.50, z + size * 1.25, y + size*0.50, axis_x, axis_y, axis_z, theta);
		putcube(size * 2.125, x - size*0.0625, z + size * 1.50, y - size*0.75, axis_x, axis_y, axis_z, theta);
		putcube(size * 1.5, x - size*1.0, z + size * 2.25, y - size*0.5, axis_x, axis_y, axis_z, theta);
		break;
	}
}

//�����̒��S���w��A��]���̍��W�A��]�p�x���w��(z - (size/1.25) - (size / (3.5*2)) = z - size*(825/875) = z-0.942857142*size �̓_���ŉ��_)
void disp_sheep(double size, double x, double y, double z, double axis_x, double axis_y, double axis_z, double theta){
	GLfloat wool[] = { 1.0, 0.9, 0.7 };
	GLfloat skin[] = { 2.0, 1.8, 1.3 };
	GLfloat eye[] = { 0.0, 0.0, 0.0 };


	//�r��
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, wool);
	putanimalcube(size, x, z, y, axis_x, axis_y, axis_z, theta, 0, 0, 0);
	putanimalcube(size / 1.375, x, z, y, axis_x, axis_y, axis_z, theta, size / 2.0, size / 3.0, 0);
	//�K��
	putanimalcube(size / 1.5, x, z, y, axis_x, axis_y, axis_z, theta, -size / 1.75, 0, 0);
	//�r��
	putanimalcube(size / 3.0, x, z, y, axis_x, axis_y, axis_z, theta, size / 3.125, -size / 2.0, size / 3.125);
	putanimalcube(size / 3.0, x, z, y, axis_x, axis_y, axis_z, theta, size / 3.125, -size / 2.0, -size / 3.125);
	putanimalcube(size / 3.0, x, z, y, axis_x, axis_y, axis_z, theta, -size / 3.125, -size / 2.0, size / 3.125);
	putanimalcube(size / 3.0, x, z, y, axis_x, axis_y, axis_z, theta, -size / 3.125, -size / 2.0, -size / 3.125);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, skin);
	putanimalcube(size / 3.5, x, z, y, axis_x, axis_y, axis_z, theta, size / 3.125, -size / 1.25, size / 3.125);
	putanimalcube(size / 3.5, x, z, y, axis_x, axis_y, axis_z, theta, size / 3.125, -size / 1.25, -size / 3.125);
	putanimalcube(size / 3.5, x, z, y, axis_x, axis_y, axis_z, theta, -size / 3.125, -size / 1.25, size / 3.125);
	putanimalcube(size / 3.5, x, z, y, axis_x, axis_y, axis_z, theta, -size / 3.125, -size / 1.25, -size / 3.125);
	//��
	putanimalcube(size / 2.0, x, z, y, axis_x, axis_y, axis_z, theta, size / 1.375, size / 3, 0);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, eye);
	putcubeline(size / 13.0, x, z, y, 2.0, direction::LEFT_REV, axis_x, axis_y, axis_z, theta, size / 1.0625, size / 2.875, size / 5.0);
	putcubeline(size / 13.0, x, z, y, 2.0, direction::LEFT_REV, axis_x, axis_y, axis_z, theta, size / 1.0625, size / 2.875, -size / 9.0);
}

//�����̒��S���w��A��]���̍��W�A��]�p�x���w��(z - (size/1.5) - (size / (5.5*2)) = z - size*(125/165) = z-0.757575757*size �̓_���ŉ��_)
void disp_alpaca(double size, double x, double y, double z, double axis_x, double axis_y, double axis_z, double theta){
	GLfloat wool[] = { 1.5, 1.4, 1.2 };
	GLfloat skin[] = { 2.0, 2.0, 2.0 };
	GLfloat eye[] = { 0.0, 0.0, 0.0 };


	//body
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, wool);
	putanimalcube(size / 1.75, x, z, y, axis_x, axis_y, axis_z, theta, -size / 3.5, 0, 0);
	putanimalcube(size / 1.75, x, z, y, axis_x, axis_y, axis_z, theta, size / 3.5, 0, 0);
	//putanimalcube(size / 2.5, x, z, y, axis_x, axis_y, axis_z, theta, size / 2.5, 0, 0);
	putanimalcube(size / 1.875, x, z, y, axis_x, axis_y, axis_z, theta, -size / 2.0, 0, 0);
	//head
	putanimalcube(size / 1.75, x, z, y, axis_x, axis_y, axis_z, theta, size / 2.0, size, 0);
	putcubeline(size / 2.5, x, z, y, 2.0, direction::UP, axis_x, axis_y, axis_z, theta, size / 3.0, size/2.0, 0);
	//�r��
	putanimalcube(size / 4.0, x, z, y, axis_x, axis_y, axis_z, theta, size / 3.125, -size / 3.0, -size / 6.0);
	putanimalcube(size / 4.0, x, z, y, axis_x, axis_y, axis_z, theta, size / 3.125, -size / 3.0, size / 6.0);
	putanimalcube(size / 4.0, x, z, y, axis_x, axis_y, axis_z, theta, -size / 3.125, -size / 3.0, size / 6.0);
	putanimalcube(size / 4.0, x, z, y, axis_x, axis_y, axis_z, theta, -size / 3.125, -size / 3.0, -size / 6.0);
	putanimalcube(size / 4.75, x, z, y, axis_x, axis_y, axis_z, theta, size / 3.125, -size / 1.75, -size / 6.0);
	putanimalcube(size / 4.75, x, z, y, axis_x, axis_y, axis_z, theta, size / 3.125, -size / 1.75, size / 6.0);
	putanimalcube(size / 4.75, x, z, y, axis_x, axis_y, axis_z, theta, -size / 3.125, -size /1.750, size / 6.0);
	putanimalcube(size / 4.75, x, z, y, axis_x, axis_y, axis_z, theta, -size / 3.125, -size /1.750, -size / 6.0);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, skin);
	putanimalcube(size / 5.5, x, z, y, axis_x, axis_y, axis_z, theta, size / 3.125, -size / 1.5, size / 6.0);
	putanimalcube(size / 5.5, x, z, y, axis_x, axis_y, axis_z, theta, size / 3.125, -size / 1.5, -size / 6.0);
	putanimalcube(size / 5.5, x, z, y, axis_x, axis_y, axis_z, theta, -size / 3.125, -size / 1.5, size / 6.0);
	putanimalcube(size / 5.5, x, z, y, axis_x, axis_y, axis_z, theta, -size / 3.125, -size / 1.5, -size /6.0);
	//��
	putanimalcube(size / 2.5, x, z, y, axis_x, axis_y, axis_z, theta, size / 1.50, size, 0);
	putanimalcube(size / 5.5, x, z, y, axis_x, axis_y, axis_z, theta, size / 1.1875, size/1.09375, 0);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, eye);
	putcubeline(size / 30.0, x, z, y, 2.0, direction::DOWN, axis_x, axis_y, axis_z, theta, size / 1.15, size / 0.875, size / 15.0);
	putcubeline(size / 30.0, x, z, y, 2.0, direction::DOWN, axis_x, axis_y, axis_z, theta, size / 1.15, size / 0.875, -size / 15.0);
	putanimalcube(size / 18.0, x, z, y, axis_x, axis_y, axis_z, theta, size / 1.0625, size / 1.09375, 0);
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(eye[0], eye[1], eye[2], 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	//gluLookAt(eye[0], eye[1], eye[2], 0.0, 0.0, 0.0, 0.0, 0.0, -1.0);
	disp_ground();
	//���Ԑ����̖�
	disp_wood(0.5, -2.0, -9.0, -0.0625, treetype::Broadleaf, 0, 0, 0, 0);
	disp_wood(0.5, -2.0, -4.5, -0.0625, treetype::Broadleaf, 0, 0, 0, 0);
	disp_wood(0.5, -2.0, 0.0, -0.0625, treetype::Broadleaf, 0, 0, 0, 0);
	disp_wood(0.5, -2.0, 4.5, -0.0625, treetype::Broadleaf, 0, 0, 0, 0);
	disp_wood(0.5, -2.0, 9.0, -0.0625, treetype::Broadleaf, 0, 0, 0, 0);
	//�E�Ԑ����̖�
	disp_wood(0.5, 2.0, -9.0, -0.0625, treetype::Broadleaf, 0, 0, 0, 0);
	disp_wood(0.5, 2.0, -4.5, -0.0625, treetype::Broadleaf, 0, 0, 0, 0);
	disp_wood(0.5, 2.0, 0.0, -0.0625, treetype::Broadleaf, 0, 0, 0, 0);
	disp_wood(0.5, 2.0, 4.5, -0.0625, treetype::Broadleaf, 0, 0, 0, 0);
	disp_wood(0.5, 2.0, 9.0, -0.0625, treetype::Broadleaf, 0, 0, 0, 0);

	//(825.0/875.0)*size�{�i�n�ʂ�Z���W�j��about���n
	disp_sheep(0.5, 8.0, 5.0, (825.0 / 875.0)*0.5 - 0.0625, 0, 1, 0, -140);
	disp_sheep(0.3, 7.0, 3.0, (825.0 / 875.0)*0.3 - 0.0625, 0, 1, 0, 60);
	disp_sheep(0.3, 6.5, 6.0, (825.0 / 875.0)*0.3 - 0.0625, 0, 1, 0, 0);
	disp_sheep(0.2, 6.2, 5.0, (825.0 / 875.0)*0.2 - 0.0625, 0, 1, 0, -20);
	disp_sheep(0.8, -6.2, -8.0, (825.0 / 875.0)*0.8 - 0.0625, 0, 1, 0, 90);
	disp_sheep(2.0, 3.0, -2.0, (825.0 / 875.0)*2.0 - 0.0625, 0, 1, 0, -20);
	for (int i = -300; i <= 300; i++)
		disp_sheep(0.2, i*5%10/5, i*4%10/5, 6, 0, 1, 1, i*29%360);
	//�~��r
	for (int i = 1; i <= 6; i++){
		disp_sheep(0.10, -8.0, -i, 0.10*(825.0 / 875.0) - 0.0625, 0, 1, 0, -90);
		disp_sheep(0.15, -7.5, -i, 0.15*(825.0 / 875.0) - 0.0625, 0, 1, 0, -90);
		disp_sheep(0.10, -7.0, -i, 0.10*(825.0 / 875.0) - 0.0625, 0, 1, 0, -90);
		disp_sheep(0.10, -6.5, -i, 0.10*(825.0 / 875.0) - 0.0625, 0, 1, 0, -90);
		disp_sheep(0.15, -6.0, -i, 0.15*(825.0 / 875.0) - 0.0625, 0, 1, 0, -90);
		disp_sheep(0.15, -5.5, -i, 0.15*(825.0 / 875.0) - 0.0625, 0, 1, 0, -90);
	}


	//�V�j�p�J(size / 1.75�Œ��n)
	disp_alpaca(0.2, 8.0, -3.0, 0.2 / 1.75 - 0.0625, 1, 0, 0, 90);
	disp_alpaca(0.3, 7.0, -5.0, 0.3 / 1.75 - 0.0625, 1, 0, 0, -90);
	disp_alpaca(0.2, 4.0, -5.0, 0.2 / 1.75 - 0.0625, 1, 0, 0, -90);
	disp_alpaca(0.1, 5.0, -2.0, 0.1 / 1.75 - 0.0625, 1, 0, 0, 90);
	disp_alpaca(0.4, 7.0, -6.0, 0.4 / 1.75 - 0.0625, 1, 0, 0, 90);
	disp_alpaca(0.2, 8.0, -2.0, 0.2 / 1.75 - 0.0625, 1, 0, 0, -90);
	disp_alpaca(0.2, 7.0, -3.0, 0.2 / 1.75 - 0.0625, 1, 0, 0, 90);
	//�A���p�J
	disp_alpaca(0.8, -8.0, 3.0, 0.8*(125 / 165), 0, 1, 0, -35);
	disp_alpaca(0.7, -6.0, 3.0, 0.7*(125.0 / 165.0) - 0.0625, 0, 1, 0, -150);
	disp_alpaca(0.6, -4.0, 1.0, 0.7*(125.0 / 165.0) - 0.0625, 0, 1, 0, 140);
	disp_alpaca(0.4, -6.0, 0.5, 0.7*(125.0 / 165.0) - 0.0625, 0, 1, 0, 120);
	disp_alpaca(0.7, -5.0, 4.0, 0.7*(125.0 / 165.0) - 0.0625, 0, 1, 0, -140);
	disp_alpaca(0.9, -8.0, 6.0, 0.7*(125.0 / 165.0) - 0.0625, 0, 1, 0, -90);
	disp_alpaca(0.5, -7.5, 0.0, 0.7*(125.0 / 165.0) - 0.0625, 0, 1, 0, 100);
	
	//coordinateprint();

	//�����̂�`��i�j���̐����ő傫�����ω�
	//glutSolidCube(2.0);

	glFlush();
}


void myReshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	printf("%d %d\n", w, h);
	gluPerspective(10.0, (GLfloat)w / (GLfloat)h, 1.0, 100.0);

	glMatrixMode(GL_MODELVIEW);
}

void main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(1500, 800);
	glutCreateWindow(TitleString);
	myinit();
	glutReshapeFunc(myReshape);
	glutDisplayFunc(display);
	glutMainLoop();
}