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

//�t�@�C���l�[���A����̊p�w��A�e�N�X�`���̃^�C�v���w��
void map_texture(char *texture, double size, double x, double y, double z, int texture_type);

//���W�̕\��
void coordinateprint();

//�^�C���̕\��(����̊p��(x,y,z)�Ƃ���)
void disp_tile(double size, double x, double y, double z, GLfloat col[4]);

//�����̂�`��i���サ����w��j
void putcube(float size, float x, float z, float y, double axis_x, double axis_y, double axis_z, double theta);

//�����̃L���[�u��`��dif �ɂ͒��S����̍��ق��w��
void putanimalcube(float size, float x, float z, float y, double axis_x, double axis_y, double axis_z, double theta, double dif_x, double dif_z, double dif_y);

//�����̂�`��i�傫���A�J�n�_�A�����A�����j
void putcubeline(float size, float x, float z, float y, int lng, char dir, double axis_x, double axis_y, double axis_z, double theta, double dif_x, double dif_z, double dif_y);

//�O�p����`��i��ʂ̑傫���A����̊p�A�������w��j
void putpyramid(double size, double height, double x, double y, double z);

void putcubecube(float x, float z, float y, int Rlng, int Llng, char FirstPos);

//�n�ʂ̕\��
void disp_ground(void);

//�؁A�����̒��S���w��
void disp_wood(double size, double x, double y, double z, int type, double axis_x, double axis_y, double axis_z, double theta);

//�����̒��S���w��A��]���̍��W�A��]�p�x���w��(z - (size/1.25) - (size / (3.5*2)) = z - size*(825/875) = z-0.942857142*size �̓_���ŉ��_)
void disp_sheep(double size, double x, double y, double z, double axis_x, double axis_y, double axis_z, double theta);

//�����r��`��
void disp_moving_sheep(double size, double x, double y, double z, double axis_x, double axis_y, double axis_z, double theta, double time);

//�����̒��S���w��A��]���̍��W�A��]�p�x���w��(z - (size/1.5) - (size / (5.5*2)) = z - size*(125/165) = z-0.757575757*size �̓_���ŉ��_)
void disp_alpaca(double size, double x, double y, double z, double axis_x, double axis_y, double axis_z, double theta);

//�j�[�u���b�I�I
void kneebra(double x, double y, double z, double axis_x, double axis_y, double axis_z, double theta, double win_posx, double win_posy, double win_posz);

//keyboard�C�x���g���擾
void keyboard(unsigned char key, int n1, int n2);

//���ʂȃL�[����
void specialkey(int key, int n1, int n2);

void specialkeyboardup(int key, int n1, int n2);

void display(void);

//�^�C�}�[�ɂ���đ���A���_��ύX
void view_timer(int value);

void myReshape(int w, int h);