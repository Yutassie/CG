#include <stdlib.h>
#include <glut.h>
#include <stdio.h>


char TitleString[] = "�͂��߂Ă�OpenGL";
//���_���Œ�ix,y,z(���s)�j
GLfloat eye[] = { 40.0, 10.0, 20.0 };

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
/*
void coordinateprint(){
	glBegin(GL_POINTS);
	for (double i = -100; i < 0; i += 0.1){
		glColor3f(1.0, 0.0, 0.0);
		glVertex3d(i, 0, 0);
	}
	for (double j = -100; j < 0; j += 0.1){
		glColor3f(0.0, 1.0, 0.0);
		glVertex3d(0, j, 0);
	}
	for (double k = -100; k < 0; k += 0.1){
		glColor3f(0.0, 0.0, 1.0);
		glVertex3d(0, 0, k);
	}
	glEnd();
	glBegin(GL_LINES);
	glColor3f(1.0, 0.0, 0.0);
	glVertex3d(0, 0, 0);
	glVertex3d(100, 0, 0);
	glColor3f(0.0, 1.0, 0.0);
	glVertex3d(0, 0, 0);
	glVertex3d(0, 100, 0);
	glColor3f(0.0, 0.0, 1.0);
	glVertex3d(0, 0, 0);
	glVertex3d(0, 0, 100);
	glEnd();
}*/

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
	GLfloat black[] = { 0.0, 0.0, 0.0, 1.0 };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);
	glBegin(GL_QUADS);
	for (double l = -40; l <= 40; l += 2){
		for (double k = -40; k <= 40; k += 1){
			glVertex3d(-1.0 + k - l, 0.0, 0.0 + k);
			glVertex3d(0.0 + k - l, 0.0, 0.0 + k);
			glVertex3d(0.0 + k - l, 0.0, -1.0 + k);
			glVertex3d(-1.0 + k - l, 0.0, -1.0 + k);
		}
	}
	GLfloat white[] = { 255.0, 255.0, 255.0, 1.0 };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	glBegin(GL_QUADS);
	for (double l = -40; l <= 40; l += 2){
		for (double k = -40; k <= 40; k += 1){
			glVertex3d(-1.0 + k - l, 0.0, 1.0 + k);
			glVertex3d(0.0 + k - l, 0.0, 1.0 + k);
			glVertex3d(0.0 + k - l, 0.0, 0.0 + k);
			glVertex3d(-1.0 + k - l, 0.0, 0.0 + k);
		}
	}
	glEnd();
}
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(eye[0], eye[1], eye[2], 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	
	disp_tile();
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