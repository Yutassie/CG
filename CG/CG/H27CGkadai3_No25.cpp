#define _USE_MATH_DEFINES

#include <stdlib.h>
#include <glut.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "ppmload2.h"
#include "func.h"

char TitleString[] = "�͂��߂Ă�OpenGL";
//���_���Œ�ix,y,z(���s)�j
GLfloat eye[] = { 0, 2, 0 };
GLfloat look[] = { 10000, 0, 0 };
//GLfloat eye[] = { 30.0, 40.0, 50.0 };
int view_flag[] = { 0, 0, 0, 0 };
int move_flag[] = { 0, 0, 0, 0 };

double movement = 0.375; //�ړ���
double angle = 0.3125; //���_�̉�]�p

int kneebra_flag = 0;
double movetime = 0.0;
double kneebradian = 0.0;
double ani_pos[32][2];
double ani_ang[32];
int hitflag[32] = { 0 };

void animal_init(void){
	int i = 0;
	srand((unsigned int)time(NULL));
	while (i != 31){
		ani_pos[i][0] = rand() % 450 * pow(-1.0, rand() % 2);
		ani_pos[i][1] = rand() % 450 * pow(-1.0, rand() % 2);
		//ani_pos[i][2] = rand() % 450 * pow(-1.0, rand() % 2);
		ani_ang[i] = rand() % 360;
		++i;
	}
}

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

void idle(void){
	glutPostRedisplay();
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
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		break;
	}
	case texture::repeat_y: {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		break;
	}
	}
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

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
	glTranslatef(x + dif_x, z + dif_z, y + dif_y);
	//          �@ ���@��@�E
	glutSolidCube(size);
	glPopMatrix();
}

//�O�p����`��i��ʂ̑傫���A����̊p�A�������w��j
void putpyramid(double size, double height, double x, double y, double z, double axis_x, double axis_y, double axis_z, double theta, double dif_x, double dif_z, double dif_y, GLfloat color[3]){
	double axis_n = sqrt(pow(axis_x, 2) + pow(axis_y, 2) + pow(axis_z, 2));
	double axis_zx, axis_zy, axis_zz, axis_zn, eye_anglez;
	eye_anglez = atan2((look[1] - eye[1]), sqrt(pow((look[0] - eye[0]), 2) + pow((look[2] - eye[0]), 2)));
	axis_zx = sin(atan2((look[2] - eye[2]), (look[0] - eye[0])));
	axis_zz = -cos(atan2((look[2] - eye[2]), (look[0] - eye[0])));
	axis_zy = 0;
	axis_zn = sqrt(pow(axis_zx, 2) + pow(axis_zy, 2) + pow(axis_zz, 2));
	
	if (axis_n > 0.0){
		axis_x /= axis_n;
		axis_y /= axis_n;
		axis_z /= axis_n;
	}
	if (axis_zn > 0.0){
		axis_zx /= axis_zn;
		axis_zy /= axis_zn;
		axis_zz /= axis_zn;
	}

	glPushMatrix();

	if (eye_anglez != 0 && axis_zn > 0.0){
		glTranslatef(x, z, y);
		glRotatef(-eye_anglez * 180 / M_PI, axis_zx, axis_zy, axis_zz);
		glTranslatef(-x, -z, -y);
	}
	if (theta != 0 && axis_n > 0.0){
		glTranslatef(x, z, y);
		glRotatef(theta, axis_x, axis_y, axis_z);
		glTranslatef(-x, -z, -y);
	}
	glTranslatef(x + dif_x, z + dif_z, y + dif_y);
	glRotatef(60, 0, 0, 1);
	glTranslatef(-x - dif_x, -z - dif_z, -y - dif_y);

	glTranslatef(x + dif_x, z + dif_z, y + dif_y);
	glRotatef(-25, 1, 0, 0);
	glTranslatef(-x - dif_x, -z - dif_z, -y - dif_y);


	glTranslatef(x + dif_x, z + dif_z, y + dif_y);
	
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
	glBegin(GL_QUADS);
	glVertex3d(-size / 2, 0, -size / 2);
	glVertex3d(+size / 2, 0, -size / 2);
	glVertex3d(+size / 2, 0, +size / 2);
	glVertex3d(-size / 2, 0, +size / 2);
	glEnd();

	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
	glBegin(GL_TRIANGLES);
	glVertex3d(-size / 2, 0, -size / 2);
	glVertex3d(+size / 2, 0, -size / 2);
	glVertex3d(0, height, 0);
	glEnd();

	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
	glBegin(GL_TRIANGLES);
	glVertex3d(+size / 2, 0, -size / 2);
	glVertex3d(+size / 2, 0, +size / 2);
	glVertex3d(0, height, 0);
	glEnd();

	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
	glBegin(GL_TRIANGLES);
	glVertex3d(+size / 2, 0, +size / 2);
	glVertex3d(-size / 2, 0, +size / 2);
	glVertex3d(0, height, 0);
	glEnd();

	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
	glBegin(GL_TRIANGLES);
	glVertex3d(-size / 2, 0, +size / 2);
	glVertex3d(-size / 2, 0, -size / 2);
	glVertex3d(0, height, 0);
	glEnd();
	glPopMatrix();
}

void putspearhead(double size, double height, double x, double y, double z, double axis_x, double axis_y, double axis_z, double theta, double dif_x, double dif_z, double dif_y, GLfloat color[3]){
	double axis_n = sqrt(pow(axis_x, 2) + pow(axis_y, 2) + pow(axis_z, 2));
	double axis_zx, axis_zy, axis_zz, axis_zn, eye_anglez;
	eye_anglez = atan2((look[1] - eye[1]), sqrt(pow((look[0] - eye[0]), 2) + pow((look[2] - eye[0]), 2)));
	axis_zx = sin(atan2((look[2] - eye[2]), (look[0] - eye[0])));
	axis_zz = -cos(atan2((look[2] - eye[2]), (look[0] - eye[0])));
	axis_zy = 0;
	axis_zn = sqrt(pow(axis_zx, 2) + pow(axis_zy, 2) + pow(axis_zz, 2));
	
	if (axis_n > 0.0){
		axis_x /= axis_n;
		axis_y /= axis_n;
		axis_z /= axis_n;
	}
	if (axis_zn > 0.0){
		axis_zx /= axis_zn;
		axis_zy /= axis_zn;
		axis_zz /= axis_zn;
	}

	glPushMatrix();

	if (eye_anglez != 0 && axis_zn > 0.0){
		glTranslatef(x, z, y);
		glRotatef(-eye_anglez * 180 / M_PI, axis_zx, axis_zy, axis_zz);
		glTranslatef(-x, -z, -y);
	}
	if (theta != 0 && axis_n > 0.0){
		glTranslatef(x, z, y);
		glRotatef(theta, axis_x, axis_y, axis_z);
		glTranslatef(-x, -z, -y);
	}
	glTranslatef(x + dif_x, z + dif_z, y + dif_y);
	glRotatef(60, 0, 0, 1);
	glTranslatef(-x - dif_x, -z - dif_z, -y - dif_y);
	
	glTranslatef(x + dif_x, z + dif_z, y + dif_y);
	glRotatef(-25, 1, 0, 0);
	glTranslatef(-x - dif_x, -z - dif_z, -y - dif_y);


	glTranslatef(x + dif_x, z + dif_z, y + dif_y);
	
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
	glBegin(GL_QUADS);
	glVertex3d(-size / 2, 0, -size / 2);
	glVertex3d(+size / 2, 0, -size / 2);
	glVertex3d(+size / 2, 0, +size / 2);
	glVertex3d(-size / 2, 0, +size / 2);
	glEnd();

	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
	glBegin(GL_TRIANGLES);
	glVertex3d(-size / 2, 0, -size / 2);
	glVertex3d(+size / 2, 0, -size / 2);
	glVertex3d(0, height, 0);
	glEnd();

	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
	glBegin(GL_TRIANGLES);
	glVertex3d(+size / 2, 0, -size / 2);
	glVertex3d(+size / 2, 0, +size / 2);
	glVertex3d(0, height, 0);
	glEnd();

	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
	glBegin(GL_TRIANGLES);
	glVertex3d( + size / 2, 0, + size / 2);
	glVertex3d( - size / 2, 0, + size / 2);
	glVertex3d(0, height, 0);
	glEnd();

	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
	glBegin(GL_TRIANGLES);
	glVertex3d(- size / 2, 0, + size / 2);
	glVertex3d(- size / 2, 0, - size / 2);
	glVertex3d(0, height, 0);
	glEnd();
	glPopMatrix();
}

//����`��i�傫���A�J�n�_�A�����A�����j
void putspear(float size, int lng, float x, float z, float y, double axis_x, double axis_y, double axis_z, double theta, double dif_x, double dif_z, double dif_y){
	y = -y;
	GLfloat stick[] = { 1.0, 0.9, 0.7 };
	GLfloat iron[] = { 2.0, 1.9, 2.0 };
	putspearhead(0.125, 0.375, eye[0], eye[2], eye[1], 0, 1, 0, theta, 3.0, -0.15625, 0.15625, iron);

	double axis_n = sqrt(pow(axis_x, 2) + pow(axis_y, 2) + pow(axis_z, 2));

	double axis_zx, axis_zy, axis_zz, axis_zn, eye_anglez;
	eye_anglez = atan2((look[1] - eye[1]), sqrt(pow((look[0] - eye[0]), 2) + pow((look[2] - eye[0]), 2)));
	axis_zx = sin(atan2((look[2] - eye[2]), (look[0] - eye[0])));
	axis_zz = -cos(atan2((look[2] - eye[2]), (look[0] - eye[0])));
	axis_zy = 0;
	axis_zn = sqrt(pow(axis_zx, 2) + pow(axis_zy, 2) + pow(axis_zz, 2));

	if (axis_n > 0.0){
		axis_x /= axis_n;
		axis_y /= axis_n;
		axis_z /= axis_n;
	}
	if (axis_zn > 0.0){
		axis_zx /= axis_zn;
		axis_zy /= axis_zn;
		axis_zz /= axis_zn;
	}

	glPushMatrix();
	
	if (eye_anglez != 0 && axis_zn > 0.0){
		glTranslatef(x, z, y);
		glRotatef(-eye_anglez * 180 / M_PI, axis_zx, axis_zy, axis_zz);
		glTranslatef(-x, -z, -y);
	}
	if (theta != 0 && axis_n > 0.0){
		glTranslatef(x, z, y);
		glRotatef(theta, axis_x, axis_y, axis_z);
		glTranslatef(-x, -z, -y);
	}
	glTranslatef(x + dif_x, z + dif_z, y + dif_y);
	glRotatef(-85.0, 0, 9, -1);
	glTranslatef(-x - dif_x, -z - dif_z, -y - dif_y);


	glTranslatef(x + dif_x, z + dif_z, y + dif_y);
	//          �@ ���@��@�E
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, stick);

	for (int i = 0; i < lng; i++){
		glutSolidCube(size);
		glTranslatef(0.0, 0.0, -size);
	}

	glPopMatrix();
	
}

//���E�̒��S�ɔ���`��i�傫���A�J�n�_�A�����A�����j
void putcentercube(float size, float x, float z, float y, double axis_x, double axis_y, double axis_z, double theta, double dif_x, double dif_z, double dif_y){
	y = -y;
	double axis_n = sqrt(pow(axis_x, 2) + pow(axis_y, 2) + pow(axis_z, 2));
	
	double axis_zx, axis_zy, axis_zz, axis_zn, eye_anglez;
	eye_anglez = atan2((look[1] - eye[1]), sqrt(pow((look[0] - eye[0]), 2) + pow((look[2] - eye[0]), 2) ));
	axis_zx = sin(atan2(( look[2] - eye[2]), (look[0] - eye[0])));
	axis_zz = -cos(atan2((look[2] - eye[2]), (look[0] - eye[0])));
	axis_zy = 0;
	axis_zn = sqrt(pow(axis_zx, 2) + pow(axis_zy, 2) + pow(axis_zz, 2));

	if (axis_n > 0.0){
		axis_x /= axis_n;
		axis_y /= axis_n;
		axis_z /= axis_n;
	}
	if (axis_zn > 0.0){
		axis_zx /= axis_zn;
		axis_zy /= axis_zn;
		axis_zz /= axis_zn;
	}

	glPushMatrix();
	if (eye_anglez != 0 && axis_zn > 0.0){
		glTranslatef(x, z, y);
		glRotatef(-eye_anglez*180/M_PI, axis_zx, axis_zy, axis_zz);
		glTranslatef(-x, -z, -y);
	}
	if (theta != 0 && axis_n > 0.0){
		glTranslatef(x, z, y);
		glRotatef(theta, axis_x, axis_y, axis_z);
		glTranslatef(-x, -z, -y);
	}
	glTranslatef(x + dif_x, z + dif_z, y + dif_y);
	glRotatef(0.0, 0, 9, -1);
	glTranslatef(-x - dif_x, -z - dif_z, -y - dif_y);


	glTranslatef(x + dif_x, z + dif_z, y + dif_y);
	//          �@ ���@��@�E

	for (int i = 0; i < 1.0; i++){
		glutSolidCube(size);
		glTranslatef(0.0, 0.0, -size);
	}

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
	glTranslatef(x + dif_x, z + dif_z, y + dif_y);
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

	//disp_tile(1000, -500, -500, 0, green); //grass

	map_texture("grass.ppm", 1000, -500, -500, 0, texture::repeat_x);

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
void disp_sheep(double size, double x, double y, double z, double axis_x, double axis_y, double axis_z, double theta, int *hit){
	double angle = atan2((look[2] - eye[2]), (look[0] - eye[0]));
	if (x + size >= eye[0] + 4.0*cos(angle) && eye[0] + 4.0*cos(angle) >= x - size && -y - size <= eye[2] + 4.0*sin(angle) && eye[2] + 4.0*sin(angle) <= -y + size)
		*hit = 1;
	//printf("sheep_X : %f > %f\n", x + size / 2.0, x - size / 2.0);
	//printf("sheep_Y : %f > %f\n", -y + size / 2.0, -y - size / 2.0);
	if (*hit == 1);
	else{
		GLfloat wool[] = { 1.0, 0.9, 0.7 };
		GLfloat skin[] = { 2.0, 1.8, 1.3 };
		GLfloat sheepeye[] = { 0.0, 0.0, 0.0 };

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
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, sheepeye);
		putcubeline(size / 13.0, x, z, y, 2.0, direction::LEFT_REV, axis_x, axis_y, axis_z, theta, size / 1.0625, size / 2.875, size / 5.0);
		putcubeline(size / 13.0, x, z, y, 2.0, direction::LEFT_REV, axis_x, axis_y, axis_z, theta, size / 1.0625, size / 2.875, -size / 9.0);
	}
}

void disp_moving_sheep(double size, double x, double y, double z, double axis_x, double axis_y, double axis_z, double *theta, double time, int *hit){
	double angle = atan2((look[2] - eye[2]), (look[0] - eye[0]));
	if (x + time*cos(*theta * M_PI / 180) + size >= eye[0] + 4.0*cos(angle) && eye[0] + 4.0*cos(angle) >= x + time*cos(*theta * M_PI / 180) - size && -y - time*sin(*theta * M_PI / 180) - size <= eye[2] + 4.0*sin(angle) && eye[2] + 4.0*sin(angle) <= -y - time*sin(*theta * M_PI / 180) + size)
		*hit = 1;
	
	if (x + time*cos(*theta * M_PI / 180) >= 500 || x + time*cos(*theta * M_PI / 180) <= -500 || -y - time*sin(*theta * M_PI / 180) >= 500 || -y - time*sin(*theta * M_PI / 180) <= -500)
		*theta += 180;
	
	if (*hit == 1);
	else{
		GLfloat wool[] = { 1.0, 0.9, 0.7 };
		GLfloat skin[] = { 2.0, 1.8, 1.3 };
		GLfloat sheepeye[] = { 0.0, 0.0, 0.0 };


		//�r��
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, wool);
		putanimalcube(size, x + time*cos(*theta * M_PI / 180), z, y + time*sin(*theta * M_PI / 180), axis_x, axis_y, axis_z, *theta, 0, 0, 0);
		putanimalcube(size / 1.375, x + time*cos(*theta * M_PI / 180), z, y + time*sin(*theta * M_PI / 180), axis_x, axis_y, axis_z, *theta, size / 2.0, size / 3.0, 0);
		//�K��
		putanimalcube(size / 1.5, x + time*cos(*theta * M_PI / 180), z, y + time*sin(*theta * M_PI / 180), axis_x, axis_y, axis_z, *theta, -size / 1.75, 0, 0);
		//�r��
		putanimalcube(size / 3.0, x + time*cos(*theta * M_PI / 180), z, y + time*sin(*theta * M_PI / 180), axis_x, axis_y, axis_z, *theta, size / 3.125, -size / 2.0, size / 3.125);
		putanimalcube(size / 3.0, x + time*cos(*theta * M_PI / 180), z, y + time*sin(*theta * M_PI / 180), axis_x, axis_y, axis_z, *theta, size / 3.125, -size / 2.0, -size / 3.125);
		putanimalcube(size / 3.0, x + time*cos(*theta * M_PI / 180), z, y + time*sin(*theta * M_PI / 180), axis_x, axis_y, axis_z, *theta, -size / 3.125, -size / 2.0, size / 3.125);
		putanimalcube(size / 3.0, x + time*cos(*theta * M_PI / 180), z, y + time*sin(*theta * M_PI / 180), axis_x, axis_y, axis_z, *theta, -size / 3.125, -size / 2.0, -size / 3.125);
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, skin);
		putanimalcube(size / 3.5, x + time*cos(*theta * M_PI / 180), z, y + time*sin(*theta * M_PI / 180), axis_x, axis_y, axis_z, *theta, size / 3.125, -size / 1.25, size / 3.125);
		putanimalcube(size / 3.5, x + time*cos(*theta * M_PI / 180), z, y + time*sin(*theta * M_PI / 180), axis_x, axis_y, axis_z, *theta, size / 3.125, -size / 1.25, -size / 3.125);
		putanimalcube(size / 3.5, x + time*cos(*theta * M_PI / 180), z, y + time*sin(*theta * M_PI / 180), axis_x, axis_y, axis_z, *theta, -size / 3.125, -size / 1.25, size / 3.125);
		putanimalcube(size / 3.5, x + time*cos(*theta * M_PI / 180), z, y + time*sin(*theta * M_PI / 180), axis_x, axis_y, axis_z, *theta, -size / 3.125, -size / 1.25, -size / 3.125);
		//��
		putanimalcube(size / 2.0, x + time*cos(*theta * M_PI / 180), z, y + time*sin(*theta * M_PI / 180), axis_x, axis_y, axis_z, *theta, size / 1.375, size / 3, 0);
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, sheepeye);
		putcubeline(size / 13.0, x + time*cos(*theta * M_PI / 180), z, y + time*sin(*theta * M_PI / 180), 2.0, direction::LEFT_REV, axis_x, axis_y, axis_z, *theta, size / 1.0625, size / 2.875, size / 5.0);
		putcubeline(size / 13.0, x + time*cos(*theta * M_PI / 180), z, y + time*sin(*theta * M_PI / 180), 2.0, direction::LEFT_REV, axis_x, axis_y, axis_z, *theta, size / 1.0625, size / 2.875, -size / 9.0);
	}
}

//�����̒��S���w��A��]���̍��W�A��]�p�x���w��(z - (size/1.5) - (size / (5.5*2)) = z - size*(125/165) = z-0.757575757*size �̓_���ŉ��_)
void disp_alpaca(double size, double x, double y, double z, double axis_x, double axis_y, double axis_z, double theta, int *hit){
	double angle = atan2((look[2] - eye[2]), (look[0] - eye[0]));
	if (x + size / 1.5 >= eye[0] + 4.0*cos(angle) && eye[0] + 4.0*cos(angle) >= x - size / 2.0 && -y - size / 2.0 <= eye[2] + 4.0*sin(angle) && eye[2] + 4.0*sin(angle) <= -y + size / 1.5)
		*hit = 1;
	
	//printf("alpaca_X : %f > %f\n", x + size / 2.0, x - size / 2.0);
	//printf("alpaca_Y : %f > %f\n", -y + size / 2.0, -y - size / 2.0);
	if (*hit == 1);
	else{
		GLfloat wool[] = { 1.5, 1.4, 1.2 };
		GLfloat skin[] = { 2.0, 2.0, 2.0 };
		GLfloat sheepeye[] = { 0.0, 0.0, 0.0 };


		//body
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, wool);
		putanimalcube(size / 1.75, x, z, y, axis_x, axis_y, axis_z, theta, -size / 3.5, 0, 0);
		putanimalcube(size / 1.75, x, z, y, axis_x, axis_y, axis_z, theta, size / 3.5, 0, 0);
		//putanimalcube(size / 2.5, x, z, y, axis_x, axis_y, axis_z, theta, size / 2.5, 0, 0);
		putanimalcube(size / 1.875, x, z, y, axis_x, axis_y, axis_z, theta, -size / 2.0, 0, 0);
		//head
		putanimalcube(size / 1.75, x, z, y, axis_x, axis_y, axis_z, theta, size / 2.0, size, 0);
		putcubeline(size / 2.5, x, z, y, 2.0, direction::UP, axis_x, axis_y, axis_z, theta, size / 3.0, size / 2.5, 0);
		//�r��
		putanimalcube(size / 4.0, x, z, y, axis_x, axis_y, axis_z, theta, size / 3.125, -size / 3.0, -size / 6.0);
		putanimalcube(size / 4.0, x, z, y, axis_x, axis_y, axis_z, theta, size / 3.125, -size / 3.0, size / 6.0);
		putanimalcube(size / 4.0, x, z, y, axis_x, axis_y, axis_z, theta, -size / 3.125, -size / 3.0, size / 6.0);
		putanimalcube(size / 4.0, x, z, y, axis_x, axis_y, axis_z, theta, -size / 3.125, -size / 3.0, -size / 6.0);
		putanimalcube(size / 4.75, x, z, y, axis_x, axis_y, axis_z, theta, size / 3.125, -size / 1.75, -size / 6.0);
		putanimalcube(size / 4.75, x, z, y, axis_x, axis_y, axis_z, theta, size / 3.125, -size / 1.75, size / 6.0);
		putanimalcube(size / 4.75, x, z, y, axis_x, axis_y, axis_z, theta, -size / 3.125, -size / 1.750, size / 6.0);
		putanimalcube(size / 4.75, x, z, y, axis_x, axis_y, axis_z, theta, -size / 3.125, -size / 1.750, -size / 6.0);
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, skin);
		putanimalcube(size / 5.5, x, z, y, axis_x, axis_y, axis_z, theta, size / 3.125, -size / 1.5, size / 6.0);
		putanimalcube(size / 5.5, x, z, y, axis_x, axis_y, axis_z, theta, size / 3.125, -size / 1.5, -size / 6.0);
		putanimalcube(size / 5.5, x, z, y, axis_x, axis_y, axis_z, theta, -size / 3.125, -size / 1.5, size / 6.0);
		putanimalcube(size / 5.5, x, z, y, axis_x, axis_y, axis_z, theta, -size / 3.125, -size / 1.5, -size / 6.0);
		//��
		putanimalcube(size / 2.5, x, z, y, axis_x, axis_y, axis_z, theta, size / 1.50, size, 0);
		putanimalcube(size / 5.5, x, z, y, axis_x, axis_y, axis_z, theta, size / 1.1875, size / 1.09375, 0);
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, sheepeye);
		putcubeline(size / 30.0, x, z, y, 2.0, direction::DOWN, axis_x, axis_y, axis_z, theta, size / 1.15, size / 0.875, size / 15.0);
		putcubeline(size / 30.0, x, z, y, 2.0, direction::DOWN, axis_x, axis_y, axis_z, theta, size / 1.15, size / 0.875, -size / 15.0);
		putanimalcube(size / 18.0, x, z, y, axis_x, axis_y, axis_z, theta, size / 1.0625, size / 1.09375, 0);
	}
}

//���ʂȃL�[����
void specialkey(int key, int n1, int n2){
	switch (key){
	case GLUT_KEY_UP: {
		view_flag[0] = 1;
		break;
	}
	case GLUT_KEY_DOWN: {
		view_flag[1] = 1;
		break;
	}
	case GLUT_KEY_RIGHT: {
		view_flag[2] = 1;
		break;
	}
	case GLUT_KEY_LEFT: {
		view_flag[3] = 1;
		break;
	}
	}
}

void specialkeyboardup(int key, int n1, int n2){
	switch (key){
	case GLUT_KEY_UP: {
		view_flag[0] = 0;
		break;
	}
	case GLUT_KEY_DOWN: {
		view_flag[1] = 0;
		break;
	}
	case GLUT_KEY_RIGHT: {
		view_flag[2] = 0;
		break;
	}
	case GLUT_KEY_LEFT: {
		view_flag[3] = 0;
		break;
	}
	}
}

//keyboard�C�x���g���擾
void keyboard(unsigned char key, int n1, int n2){
	switch (key){
	case 'w':{
		move_flag[0] = 1;
		break;
	}
	case 's':{
		move_flag[1] = 1;
		break;
	}
	case 'd':{
		move_flag[2] = 1;
		break;
	}
	case 'a':{
		move_flag[3] = 1;
		break;
	}
	case 32: {   //space key
		kneebra_flag = 1;
		break;
	}
	case '\033': exit(0); //esc ->�I��
	default: break;
	}
}

void keyboardup(unsigned char key, int n1, int n2){
	switch (key){
	case 'w':{
		move_flag[0] = 0;
		break;
	}
	case 's':{
		move_flag[1] = 0;
		break;
	}
	case 'd':{
		move_flag[2] = 0;
		break;
	}
	case 'a':{
		move_flag[3] = 0;
		break;
	}
	default: break;
	}
}

void move_beside(double movement, double dir){
	GLfloat temp[3];
	temp[0] = look[0];
	temp[1] = look[1];
	temp[2] = look[2];
	GLfloat eyetemp[3];
	eyetemp[0] = eye[0];
	eyetemp[1] = eye[1];
	eyetemp[2] = eye[2];
	double tx, ty;
	double exeptzero = 0.01;
	tx = cos(atan2((temp[2] - eye[2]), (temp[0] - eye[0])));
	ty = sin(atan2((temp[2] - eye[2]), (temp[0] - eye[0])));

	if (eye[0] == 0 && eye[2] == 0);
	else if (tx >= -exeptzero && tx <= exeptzero){
		if (eye[2] > 0 && look[2] < 0){//�����ɖ�肠��A���_�̊p�x���g���ׂ�
			look[0] = temp[0] + movement * dir;
			look[2] = temp[2];
			eye[0] = eye[0] + movement * dir;
			eye[2] = eye[2];
		}
		if (eye[2] > 0 && look[2] > 0){
			look[0] = temp[0] + movement * -dir;
			look[2] = temp[2];
			eye[0] = eye[0] + movement * -dir;
			eye[2] = eye[2];
		}
		if (eye[2] <= 0 && look[2] < 0){
			look[0] = temp[0] + 0.1 * dir;
			look[2] = temp[2];
			eye[0] = eye[0] + movement * dir;
			eye[2] = eye[2];
		}
		if (eye[2] <= 0 && look[2] > 0){
			look[0] = temp[0] + movement * -dir;
			look[2] = temp[2];
			eye[0] = eye[0] + movement * -dir;
			eye[2] = eye[2];
		}
	}
	else if (ty >= -exeptzero && ty <= exeptzero){
		if (eye[0] > 0 && look[0] < 0){//�����ɖ�肠��A���_�̊p�x���g���ׂ�
			look[0] = temp[0];
			look[2] = temp[2] + movement * dir * -eye[0] / abs(eye[0]);
			eye[0] = eye[0];
			eye[2] = eye[2] + movement * dir * -eye[0] / abs(eye[0]);
		}
		if (eye[0] > 0 && look[0] > 0){
			look[0] = temp[0];
			look[2] = temp[2] + movement * dir * eye[0] / abs(eye[0]);
			eye[0] = eye[0];
			eye[2] = eye[2] + movement * dir * eye[0] / abs(eye[0]);
		}
		if (eye[0] <= 0 && look[0] < 0){
			look[0] = temp[0];
			look[2] = temp[2] + movement * dir * eye[0] / abs(eye[0]);
			eye[0] = eye[0];
			eye[2] = eye[2] + movement * dir * eye[0] / abs(eye[0]);
		}
		if (eye[0] <= 0 && look[0] > 0){
			look[0] = temp[0];
			look[2] = temp[2] + movement * dir * -eye[0] / abs(eye[0]);
			eye[0] = eye[0];
			eye[2] = eye[2] + movement * dir * -eye[0] / abs(eye[0]);
		}
	}
	
	if ((tx < -exeptzero || tx > exeptzero) && (ty < -exeptzero || ty > exeptzero) && eye[2] <= 0.0){
		if (eye[2] > 0 && look[2] < 0){//right
			look[0] = dir * -movement / sqrt(1.0 + pow(tx / ty, 2)) + look[0];
			look[2] = (tx*temp[0] + ty*temp[2] - tx*look[0]) / ty;
			eye[0] = dir * -movement / sqrt(1.0 + pow(tx / ty, 2)) + eye[0];
			eye[2] = (tx*eyetemp[0] + ty*eyetemp[2] - tx*eye[0]) / ty;
		}
		if (eye[2] > 0 && look[2] > 0){//left
			look[0] = dir * movement / sqrt(1.0 + pow(tx / ty, 2)) + look[0];
			look[2] = (tx*temp[0] + ty*temp[2] - tx*look[0]) / ty;
			eye[0] = dir * movement / sqrt(1.0 + pow(tx / ty, 2)) + eye[0];
			eye[2] = (tx*eyetemp[0] + ty*eyetemp[2] - tx*eye[0]) / ty;
		}
		if (eye[2] <= 0 && look[2] < 0){
			look[0] = dir * movement / sqrt(1.0 + pow(tx / ty, 2)) + look[0];
			look[2] = (tx*temp[0] + ty*temp[2] - tx*look[0]) / ty;
			eye[0] = dir * movement / sqrt(1.0 + pow(tx / ty, 2)) + eye[0];
			eye[2] = (tx*eyetemp[0] + ty*eyetemp[2] - tx*eye[0]) / ty;
		}
		if (eye[2] <= 0 && look[2] > 0){
			look[0] = dir * -movement / sqrt(1.0 + pow(tx / ty, 2)) + look[0];
			look[2] = (tx*temp[0] + ty*temp[2] - tx*look[0]) / ty;
			eye[0] = dir * -movement / sqrt(1.0 + pow(tx / ty, 2)) + eye[0];
			eye[2] = (tx*eyetemp[0] + ty*eyetemp[2] - tx*eye[0]) / ty;
		}
	}
	else if ((tx < -exeptzero || tx > exeptzero) && (ty < -exeptzero || ty > exeptzero) && eye[2] >= 0.0){
		if (eyetemp[2] > 0 && look[2] < 0){
			look[0] = dir * movement / sqrt(1.0 + pow(tx / ty, 2)) + look[0];
			look[2] = (tx*temp[0] + ty*temp[2] - tx*look[0]) / ty;
			eye[0] = dir * movement / sqrt(1.0 + pow(tx / ty, 2)) + eye[0];
			eye[2] = (tx*eyetemp[0] + ty*eyetemp[2] - tx*eye[0]) / ty;
		}
		if (eyetemp[2] > 0 && look[2] > 0){
			look[0] = dir * -movement / sqrt(1.0 + pow(tx / ty, 2)) + look[0];
			look[2] = (tx*temp[0] + ty*temp[2] - tx*look[0]) / ty;
			eye[0] = dir * -movement / sqrt(1.0 + pow(tx / ty, 2)) + eye[0];
			eye[2] = (tx*eyetemp[0] + ty*eyetemp[2] - tx*eye[0]) / ty;
		}
		if (eyetemp[2] <= 0 && look[2] < 0){
			look[0] = dir * -movement / sqrt(1.0 + pow(tx / ty, 2)) + look[0];
			look[2] = (tx*temp[0] + ty*temp[2] - tx*look[0]) / ty;
			eye[0] = dir * -movement / sqrt(1.0 + pow(tx / ty, 2)) + eye[0];
			eye[2] = (tx*eyetemp[0] + ty*eyetemp[2] - tx*eye[0]) / ty;
		}
		if (eyetemp[2] <= 0 && look[2] > 0){
			look[0] = dir * movement / sqrt(1.0 + pow(tx / ty, 2)) + look[0];
			look[2] = (tx*temp[0] + ty*temp[2] - tx*look[0]) / ty;
			eye[0] = dir * movement / sqrt(1.0 + pow(tx / ty, 2)) + eye[0];
			eye[2] = (tx*eyetemp[0] + ty*eyetemp[2] - tx*eye[0]) / ty;
		}
	}
	
	
}

void look_up_down(double theta){
	GLfloat temp[3];
	temp[0] = look[0];
	temp[1] = look[1];
	temp[2] = look[2];
	GLfloat eyetemp[3];
	eyetemp[0] = eye[0];
	eyetemp[1] = eye[1];
	eyetemp[2] = eye[2];
	double nx, ny;

	nx = sin(atan2((temp[2] - eye[2]), (temp[0] - eye[0])));
	ny = -cos(atan2((temp[2] - eye[2]), (temp[0] - eye[0])));
	printf("nx : %f \n", nx);
	printf("ny : %f \n\n", ny);
	look[0] = temp[0] * (pow(nx, 2) * (1 - cos(theta * M_PI / 180.0)) + cos(theta * M_PI / 180.0)) + temp[2] * nx * ny *(1 - cos(theta * M_PI / 180.0)) + temp[1] * ny * sin(theta * M_PI / 180.0) + (eye[0] * (1 - pow(nx, 2) * (1 - cos(theta * M_PI / 180.0)) - cos(theta * M_PI / 180.0)) - eye[2] * nx * ny * (1 - cos(theta * M_PI / 180.0)) - eye[1] * ny * sin(-1.0 * M_PI / 180.0));
	look[2] = temp[0] * (nx * ny * (1 - cos(theta * M_PI / 180.0))) + temp[2] * (pow(ny, 2) * (1 - cos(theta * M_PI / 180.0)) + cos(theta * M_PI / 180.0)) - temp[1] * nx * sin(theta * M_PI / 180.0) + (-eye[0] * nx * ny * (1 - cos(theta * M_PI / 180.0)) + eye[2] * (1 - pow(ny, 2) * (1 - cos(theta * M_PI / 180.0)) - cos(theta * M_PI / 180.0)) + eye[1] * nx * sin(theta * M_PI / 180.0));
	look[1] = -temp[0] * ny * sin(theta * M_PI / 180.0) + temp[2] * nx * sin(theta * M_PI / 180.0) + temp[1] * cos(theta * M_PI / 180.0) + (eye[0] * ny * sin(theta * M_PI / 180.0) - eye[2] * nx * sin(theta * M_PI / 180.0) + eye[1] * (1 - cos(theta * M_PI / 180.0)));

}

//�ڂ̑O�ɔ���\���ł���֐��i�����Ŋp�x�ύX�j
void centercube(double theta){
	GLfloat wool[] = { 1.0, 0.9, 0.7 };
	double x, y, z;
	double eye_angle = atan2((look[2] - eye[2]), (look[0] - eye[0]));
	double tx = cos(-atan2((look[2] - eye[2]), (look[0] - eye[0])));
	double ty = sin(-atan2((look[2] - eye[2]), (look[0] - eye[0])));
	double pos_x = 10*(tx * cos(10 * M_PI / 180) + ty * sin(10 * M_PI / 180));
	double pos_y = 10*(tx * sin(10 * M_PI / 180) - ty * cos(10 * M_PI / 180));

	x = eye[0];
	y = -eye[2];
	z = eye[1];

	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, wool);
	putcentercube(0.1, x, z, y, 0, 1, 0, -180 / M_PI*eye_angle, +sqrt(pow(tx, 2) + pow(ty, 2)) * 2, -0.125, 0.125);
}

//�������i�����ő��̊p�x�ύX�j
void spear(double theta){
	GLfloat wool[] = { 1.0, 0.9, 0.7 };
	double x, y, z;
	double eye_angle = atan2((look[2] - eye[2]), (look[0] - eye[0]));
	double tx = cos(-atan2((look[2] - eye[2]), (look[0] - eye[0])));
	double ty = sin(-atan2((look[2] - eye[2]), (look[0] - eye[0])));
	double pos_x = (tx * cos(10 * M_PI / 180) - ty * sin(10 * M_PI / 180));
	double pos_y = (tx * sin(10 * M_PI / 180) - ty * cos(10 * M_PI / 180));
	
	x = eye[0];
	y = -eye[2];
	z = eye[1];
	
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, wool);
	putspear(0.03125, 40, x, z, y, 0, 1, 0, -180 / M_PI*eye_angle, +sqrt(pow(tx, 2) + pow(ty, 2)) * 2, -0.25, 0.25);
}

void kneebra(double theta){
	GLfloat wool[] = { 1.0, 0.9, 0.7 };
	double x, y, z;
	double eye_angle = -atan2((look[2] - eye[2]), (look[0] - eye[0]));

	printf("eyeangle : %f\n\n", eye_angle * 180 / M_PI);
	x = (eye[0] + 1.5) * cos(eye_angle) - (-eye[2] - 0.25) * sin(eye_angle) + (eye[0] * (1 - cos(eye_angle)) + eye[2] * sin(eye_angle));
	y = (eye[0] + 1.5) * sin(eye_angle) + (-eye[2] - 0.25) * cos(eye_angle) + (-eye[2] * (1 - cos(eye_angle)) - eye[0] * sin(eye_angle));
	z = eye[1];
	printf(" x : %f \n y : %f \n z : %f \n", x, -y, z);

	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, wool);
	putcubeline(0.1, x, z, y, 3.0, direction::LEFT_REV, 0, 10, -1, theta - 120.0, 0, 0, 0);
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(eye[0], eye[1], eye[2], look[0], look[1], look[2], 0.0, 1.0, 0.0);
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

	//�����̖�
	disp_wood(0.5, 80.0, 0.0, -0.0625, treetype::Broadleaf, 0, 0, 0, 0);
	disp_wood(0.5, -80.0, 0.0, -0.0625, treetype::Broadleaf, 0, 0, 0, 0);
	disp_wood(0.5, 0.0, 80.0, -0.0625, treetype::Broadleaf, 0, 0, 0, 0);
	disp_wood(0.5, 0.0, -80.0, -0.0625, treetype::Broadleaf, 0, 0, 0, 0);
	disp_wood(0.5, 60.0, 60.0, -0.0625, treetype::Broadleaf, 0, 0, 0, 0);
	disp_wood(0.5, 60.0, -60.0, -0.0625, treetype::Broadleaf, 0, 0, 0, 0);
	disp_wood(0.5, -60.0, 60.0, -0.0625, treetype::Broadleaf, 0, 0, 0, 0);
	disp_wood(0.5, -60.0, -60.0, -0.0625, treetype::Broadleaf, 0, 0, 0, 0);
	disp_wood(0.5, 60.0, 70.0, -0.0625, treetype::Broadleaf, 0, 0, 0, 0);
	disp_wood(0.5, 70.0, -60.0, -0.0625, treetype::Broadleaf, 0, 0, 0, 0);
	disp_wood(0.5, -60.0, 70.0, -0.0625, treetype::Broadleaf, 0, 0, 0, 0);
	disp_wood(0.5, -70.0, -60.0, -0.0625, treetype::Broadleaf, 0, 0, 0, 0);

	//(825.0/875.0)*size�{�i�n�ʂ�Z���W�j��about���n
	for (int i = 0; i <= 31; i+=2){
		disp_moving_sheep(1.0, ani_pos[i][0], ani_pos[i][1], (825.0 / 875.0)*1.0, 0, 1, 0, &ani_ang[i], movetime, &hitflag[i]);
		disp_alpaca(1.0, ani_pos[i + 1][0], ani_pos[i + 1][1], 1.0*(125.0 / 165.0), 0, 1, 0, ani_ang[i + 1], &hitflag[i + 1]);
	}
	//double angle = atan2((look[2] - eye[2]), (look[0] - eye[0]));
	//printf("angle   : %f\nspear_Y : %f\nspear_X : %f\n", angle * 180 / M_PI, eye[2] + 4.0*sin(angle), eye[0] + 4.0*cos(angle));


	//disp_sheep(2.0, 15.0, -10.0, (825.0 / 875.0)*2.0, 0, 1, 0, -120, &hitflag[0]);
	//disp_moving_sheep(2.0, 3.0, -2.0, (825.0 / 875.0)*2.0, 0, 1, 0, -20, movetime, &hitflag[1]);
	//disp_alpaca(1.0, -7.5, 0.0, 1.0*(125.0 / 165.0), 0, 1, 0, 100, &hitflag[2]);

	if (kneebra_flag){
		kneebra(kneebradian);
	}
	else{
		spear(0.0);
		//centercube(0.0);
	}

	coordinateprint();

	//�����̂�`��i�j���̐����ő傫�����ω�
	//glutSolidCube(2.0);
	glutSwapBuffers();
	//glFlush();
}

//�^�C�}�[�ɂ���đ���A���_��ύX�A�Ɂ[�Ԃ���J�n
void view_timer(int value){
	GLfloat temp[3];
	temp[0] = look[0];
	temp[1] = look[1];
	temp[2] = look[2];
	double nx, ny;//�@���x�N�g���̐�������

	//printf("%f %f %f\n", temp[0] - eye[0], -temp[2] + eye[2], atan2((temp[2] - eye[2]), (temp[0] - eye[0]))* 180 / M_PI);
	printf("%f %f %f \n", eye[0], eye[1], eye[2]);
	printf("%f %f %f \n", look[0], look[1], look[2]);

	if (view_flag[0]){
		look_up_down(angle);
		glutIdleFunc(idle);
	}
	if (view_flag[1]){
		look_up_down(-angle);
		glutIdleFunc(idle); 
	}
	if (view_flag[2]){
		//��]�s��𗘗p���Ď��_����]
		look[0] = temp[0] * cos(angle * M_PI / 180.0) - temp[2] * sin(angle * M_PI / 180.0) + (eye[0] * (1 - cos(angle * M_PI / 180.0)) + eye[2] * sin(angle * M_PI / 180.0));
		look[2] = temp[0] * sin(angle * M_PI / 180.0) + temp[2] * cos(angle * M_PI / 180.0) + (-eye[2] * (1 - cos(angle * M_PI / 180.0)) + eye[0] * sin(angle * M_PI / 180.0));
		glutIdleFunc(idle);
		temp[0] = look[0]; temp[2] = look[2];
	}
	if (view_flag[3]){
		look[0] = temp[0] * cos(-angle * M_PI / 180.0) - temp[2] * sin(-angle * M_PI / 180.0) + (eye[0] * (1 - cos(-angle * M_PI / 180.0)) + eye[2] * sin(-angle * M_PI / 180.0));
		look[2] = temp[0] * sin(-angle * M_PI / 180.0) + temp[2] * cos(-angle * M_PI / 180.0) + (-eye[2] * (1 - cos(-angle * M_PI / 180.0)) - eye[0] * sin(-angle * M_PI / 180.0));
		glutIdleFunc(idle);
		temp[0] = look[0]; temp[2] = look[2];
	}

	if (move_flag[0]){
		look[0] = temp[0] + movement * cos(atan2((temp[2] - eye[2]), (temp[0] - eye[0])));
		look[2] = temp[2] + movement * sin(atan2((temp[2] - eye[2]), (temp[0] - eye[0])));
		eye[0] = eye[0] + movement * cos(atan2((temp[2] - eye[2]), (temp[0] - eye[0])));
		eye[2] = eye[2] + movement * sin(atan2((temp[2] - eye[2]), (temp[0] - eye[0])));
		glutIdleFunc(idle);
	}
	if (move_flag[1]){
		look[0] = temp[0] - movement * cos(atan2((temp[2] - eye[2]), (temp[0] - eye[0])));
		look[2] = temp[2] - movement * sin(atan2((temp[2] - eye[2]), (temp[0] - eye[0])));
		eye[0] = eye[0] - movement * cos(atan2((temp[2] - eye[2]), (temp[0] - eye[0])));
		eye[2] = eye[2] - movement * sin(atan2((temp[2] - eye[2]), (temp[0] - eye[0])));
		glutIdleFunc(idle);
	}
	if (move_flag[2]){//�E//�@���x�N�g���ƈ�_��蒼��������o���A���̒����ɉ����Đ��`�ϊ�
		move_beside(movement, 1.0);
		glutIdleFunc(idle);
	}
	if (move_flag[3]){
		move_beside(movement, -1.0);//��
		glutIdleFunc(idle);
	}

	if (kneebra_flag){
		kneebradian += 2.0;
		if (kneebradian >= 60.0){
			kneebra_flag = 0;
			kneebradian = 0.0;
		}
	}

	movetime+=0.05;
	glutTimerFunc(1, view_timer, 0);
}


void myReshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	printf("%d %d\n", w, h);
	gluPerspective(10.0, (GLfloat)w / (GLfloat)h, 1.0, 500.0);

	glMatrixMode(GL_MODELVIEW);
}

void main(int argc, char **argv)
{
	animal_init();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(1000, 700);
	glutCreateWindow(TitleString);
	myinit();
	glutReshapeFunc(myReshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyboardup);
	glutSpecialFunc(specialkey);
	glutSpecialUpFunc(specialkeyboardup);
	glutTimerFunc(1, view_timer, 0);
	glutMainLoop();
}