#include "vapp.h"
#include "glgraph.h"
#include "glmatrix.h"
#include <stdio.h>
#include <math.h>
#define TRA_NUM 8
GLubyte* colorBuffer = 0;
int vertexs[] = {-50,50,50,50,-50,-50};
int vertexstmp[6] = {0};

float vertexs3[] = {-50,50,50,50,50,50,-50,-50,50
	,-50,-50,50,50,50,50,50,-50,50//front
	,-50,50,50,50,50,-50,50,50,50
	,-50,50,50,50,50,-50,-50,50,-50//top
	,-50,50,-50,50,50,-50,-50,-50,-50
	,-50,-50,-50,50,50,-50,50,-50,-50//back
	,-50,-50,50,50,-50,-50,50,-50,50
	,-50,-50,50,50,-50,-50,-50,-50,-50//bottom
};
float vertexstmp3[TRA_NUM*9]={0};
int ivertexstmp3[TRA_NUM*6]={0};
float stamp = 0;
volatile int _status;
const float sqrtthird = .57735f;
BEGIN_APP_DECLARATION(OP2)
	virtual void Initialize(const char * title);
    virtual void Display(bool auto_redraw);
    virtual void Finalize(void);
    virtual void Reshape(int width, int height);
	virtual void MainLoop(void);
	private:
		static void Keyboard(unsigned char key, int x, int y);
		int width;
		int height;
		matrix m;
		matrix3 m3;
END_APP_DECLARATION()

DEFINE_APP(OP2, "opengl demo 2")

void OP2::Initialize(const char * title){
	base::Initialize();
	glutKeyboardFunc(Keyboard);
	_status = 1;
	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
	glTranslatef(0,0,-0.5);
	glDepthFunc(GL_LEQUAL);
	//colorBuffer = new GLubyte[width*height*3];
    //memset(colorBuffer, 0, width*height*3);
	glClearColor(0,0,0,1);
	glShadeModel(GL_FLAT);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	loadIdentityMatrix(&this->m);
}
void OP2::Keyboard(unsigned char key, int x, int y){
	// ����Q��q��ESC��ʱ����ֹ����
	if(key == 'Q' || key == 'q'|| key == '\27'|| key == '\x1b'){
		_status = 0;
	}
}

void OP2::Display(bool auto_redraw){
	//glReadBuffer(GL_FRONT);
	//glReadPixels(0,0,480,480,GL_RGB,GL_UNSIGNED_BYTE, colorBuffer);
	//glDrawBuffer(GL_BACK);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);//�����Ļ�ϵ���ɫ
	glRasterPos2i(0, 0);
	register GLubyte*tmp = colorBuffer;
	unsigned char c = 0;
	for (int i = 0; i < height; i++){
		for (int j = 0; j < width; j++){
			if (i==0||i==height-1||j==0||j==width-1){
				c = 255;
			}else{
				c = 0;
			}
			*tmp=c;
			tmp++;
			*tmp=c;
			tmp++;
			*tmp=c;
			tmp++;
		}
	}
	//2dƽ�滭ͼ
	stamp+=.02;
	loadIdentityMatrix(&this->m);
	rotate(&this->m,stamp);
	transition(&this->m,240,240);
	convertByMatrix(&this->m,vertexs,vertexstmp);
	convertByMatrix(&this->m,vertexs+2,vertexstmp+2);
	convertByMatrix(&this->m,vertexs+4,vertexstmp+4);
	//ptriangle(colorBuffer,width,height,vertexstmp);

	//3d���廭ͼ
	loadIdentityMatrix3(&this->m3);
	rotate3(&this->m3,sqrtthird,sqrtthird,sqrtthird,-stamp);
	transition3(&this->m3,0,0,450);
	
	perspective(&this->m3,400,500,-300,300,300,-300);
	convertByMatrix3(&this->m3,vertexs3,vertexstmp3,3*TRA_NUM);
	viewport(vertexstmp3,ivertexstmp3,width,height,3*TRA_NUM);
	ptriangles(colorBuffer,width,height,ivertexstmp3,TRA_NUM);

	glDrawPixels(width, height, GL_RGB, GL_UNSIGNED_BYTE, colorBuffer);
	base::Display(auto_redraw);
}

void OP2::MainLoop(void){
	for (;_status;)
        glutMainLoopEvent(); 
}

void OP2::Finalize(void){
	free(colorBuffer);
}

void OP2::Reshape(int width, int height){
	this->width = width-8;
	this->height = height-8;
    glViewport(0, 0 , width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, 0, height, -1, 1);
	if (colorBuffer){
		free(colorBuffer);
		colorBuffer = 0;
	}
	colorBuffer = new GLubyte[width*height*3];
}