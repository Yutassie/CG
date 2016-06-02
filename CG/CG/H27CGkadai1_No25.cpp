#include <stdlib.h>
#include <glut.h>
#include <stdio.h>

enum direction{
	LEFT,
	RIGHT,
	UP,
	DOWN
};

enum cubefirstpos{
	CENTER,
	LEFTCORNER,

};


char TitleString[] = "�͂��߂Ă�OpenGL";
//���_���Œ�ix,y,z(���s)�j
//GLfloat eye[] = { 40.0, 10.0, 20.0 };
GLfloat eye[] = { 40.0, 50.0,50.0 };

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

//�^�C���̕\��
void disp_tile(void){
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
	GLfloat black[] = { 1.0, 0.2, 0.2, 1.0 };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);
	glBegin(GL_QUADS);
	for (double l = -100; l <= 100; l += 0.5){
		for (double k = -100; k <= 100; k += 0.25){
			glVertex3d(-0.25/2 + k - l, -0.5/4, 0.25/2 + k);
			glVertex3d(0.25/2 + k - l, -0.5/4, 0.25/2 + k);
			glVertex3d(0.25/2 + k - l, -0.5/4, -0.25/2 + k);
			glVertex3d(-0.25/2 + k - l, -0.5/4, -0.25/2 + k);
		}
	}
	glPopMatrix();
	glPushMatrix();
	GLfloat white[] = { 3.0, 3.0, 3.0, 1.0 };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	glBegin(GL_QUADS);
	for (double l = -100; l <= 100; l += 0.5){
		for (double k = -100; k <= 100; k += 0.25){
			glVertex3d(0.5/4 + k - l, -0.5/4, 0.5/4 + k);
			glVertex3d(1.5/4 + k - l, -0.5/4, 0.5/4 + k);
			glVertex3d(1.5/4 + k - l, -0.5/4, -0.5/4 + k);
			glVertex3d(0.5/4 + k - l, -0.5/4, -0.5/4 + k);
		}
	}
	glPopMatrix();
	glEnd();
}

void putcube(float x, float z, float y){
	y = -y;
	//Push��Pop���s�����ƂŐ�Έʒu�w��ɕύX
	glPushMatrix();
	glTranslatef(x, z, y);
	//          �@ ���@��@�E
	glutSolidCube(0.23);
	glPopMatrix();
}

void putcubeline(float x, float z, float y, int lng, char dir){
	y = -y;
	glPushMatrix();
	glTranslatef(x, z, y);
	//          �@ ���@��@�E
	switch (dir){
	case LEFT:
		for (int i = 0; i < lng; i++){
			glutSolidCube(1.0);
			glTranslatef(-1.0, 0.0, 0.0);
		} break;
	case RIGHT:
		for (int i = 0; i < lng; i++){
			glutSolidCube(1.0);
			glTranslatef(1.0, 0.0, 0.0);
		} break;
	case UP:
		for (int i = 0; i < lng; i++){
			glutSolidCube(1.0);
			glTranslatef(0.0, 0.0, -1.0);
		} break;
	case DOWN:
		for (int i = 0; i < lng; i++){
			glutSolidCube(1.0);
			glTranslatef(0.0, 0.0, 1.0);
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

void disp_arima(void){
	FILE *fp = NULL;
	if ((fp = fopen("pixmap.txt", "r")) == NULL)
		exit(0);
	double pixdate[5];

	while (fscanf(fp, "%lf,%lf,%lf,%lf,%lf", &pixdate[0], &pixdate[1], &pixdate[2], &pixdate[3], &pixdate[4]) != EOF){
		GLfloat color[] = { pixdate[2]/255, pixdate[3]/255, pixdate[4]/255, 1.0 };

		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
		putcube((pixdate[0])/4, 0.0, (pixdate[1])/4);
	}
	/*�֊s
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blk);
	putcubeline(-3.0, 0.0, 2.0, 7, RIGHT);
	putcube(-2.0, 0.0, 0.0);
	putcube(-2.0, 0.0, -1.0);
	putcube(-2.0, 0.0, -2.0);
	putcube(-3.0, 0.0, -3.0);
	putcube(-2.0, 0.0, -3.0);
	putcube(-2.0, 0.0, -4.0);
	putcube(-2.0, 0.0, -5.0);*/
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	//gluLookAt(eye[0], eye[1], eye[2], -20.0, -20.0, 0.0, 0.6, 1.0, 1.0);
	gluLookAt(eye[0], eye[1], eye[2], 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	disp_tile();
	disp_arima();
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
	glutInitWindowSize(500, 500);
	glutCreateWindow(TitleString);
	myinit();
	glutReshapeFunc(myReshape);
	glutDisplayFunc(display);
	glutMainLoop();
}