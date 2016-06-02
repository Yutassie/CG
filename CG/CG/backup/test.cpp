#include <stdlib.h>
#include <glut.h>
#include <stdio.h>


char TitleString[] = "はじめてのOpenGL";
//視点を固定（x,y,z(奥行)）
GLfloat eye[] = { 40.0, 10.0, 20.0 };

void myinit(void)
{
	GLfloat light_position[] = { 100.0, 100.0, 100.0, 1.0 };

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	//ウインドを塗りつぶす(背景)
	glClearColor(0.0, 100.0, 100.0, 1.0);


	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
}

//座標の表示
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

//タイルの表示
void disp_tile(void){
	/*glBEGINの引数の解説
	GL_LINES          : 2点間直線
	GL_POINTS         : 点
	GL_LINE_STRIP     : 折れ線
	GL_LINE_LOOP      : 折れ線（始点と終点も結ぶ）
	GL_TRIANGLES      : 三角形（3点指定）
	GL_QUADS          : 四角形（4点指定）
	GL_TRIANGLE_STRIP : 一辺を共有する三角形の描画（3点以上の指定）スネークパズルみたいな図形
	GL_QUAD_STRIP     : 一辺を共有する四角形の描画（4点以上の指定）四角形の連続
	GL_TRIANGLE_FAN   : 帯状に三角形を結合（センスのイメージ）
	GL_POLYGON        : 凸多角形の描画
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

	//立方体を描画（）内の数字で大きさが変化
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