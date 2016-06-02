#define _USE_MATH_DEFINES

#include <stdlib.h>
#include <glut.h>
#include <stdio.h>
#include <math.h>

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


void myinit(void);

void idle(void);

//ファイルネーム、左上の角指定、テクスチャのタイプを指定
void map_texture(char *texture, double size, double x, double y, double z, int texture_type);

//座標の表示
void coordinateprint();

//タイルの表示(左上の角を(x,y,z)とする)
void disp_tile(double size, double x, double y, double z, GLfloat col[4]);

//立方体を描画（ちゅしんを指定）
void putcube(float size, float x, float z, float y, double axis_x, double axis_y, double axis_z, double theta);

//動物のキューブを描画dif には中心からの差異を指定
void putanimalcube(float size, float x, float z, float y, double axis_x, double axis_y, double axis_z, double theta, double dif_x, double dif_z, double dif_y);

//直方体を描画（大きさ、開始点、長さ、方向）
void putcubeline(float size, float x, float z, float y, int lng, char dir, double axis_x, double axis_y, double axis_z, double theta, double dif_x, double dif_z, double dif_y);

//三角錐を描画（底面の大きさ、左上の角、高さを指定）
void putpyramid(double size, double height, double x, double y, double z);

void putcubecube(float x, float z, float y, int Rlng, int Llng, char FirstPos);

//地面の表示
void disp_ground(void);

//木、根元の中心を指定
void disp_wood(double size, double x, double y, double z, int type, double axis_x, double axis_y, double axis_z, double theta);

//動物の中心を指定、回転軸の座標、回転角度を指定(z - (size/1.25) - (size / (3.5*2)) = z - size*(825/875) = z-0.942857142*size の点が最下点)
void disp_sheep(double size, double x, double y, double z, double axis_x, double axis_y, double axis_z, double theta);

//動く羊を描画
void disp_moving_sheep(double size, double x, double y, double z, double axis_x, double axis_y, double axis_z, double theta, double time);

//動物の中心を指定、回転軸の座標、回転角度を指定(z - (size/1.5) - (size / (5.5*2)) = z - size*(125/165) = z-0.757575757*size の点が最下点)
void disp_alpaca(double size, double x, double y, double z, double axis_x, double axis_y, double axis_z, double theta);

//ニーブラッ！！
void kneebra(double x, double y, double z, double axis_x, double axis_y, double axis_z, double theta, double win_posx, double win_posy, double win_posz);

//keyboardイベントを取得
void keyboard(unsigned char key, int n1, int n2);

//特別なキー入力
void specialkey(int key, int n1, int n2);

void specialkeyboardup(int key, int n1, int n2);

void display(void);

//タイマーによって操作、視点を変更
void view_timer(int value);

void myReshape(int w, int h);