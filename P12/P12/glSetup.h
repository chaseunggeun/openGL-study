#ifndef __GL_SETUP_H_
#define __GL_SETUP_H_

#if defined(__APPLE__) && defined(__MACH__)
#include <OpenGL/glu.h>
#else
#include <windows.h>
#include <gl/GLU.h>
#endif

#include <GLFW/glfw3.h>

extern float	scteenscale;		// �����츦 ��� �� �����찡 ��ü ��������� ��������� �� ��
extern int		screenW, screenH;	// ������� �ػ�
extern int		windowW, windowH;	// �����ӹ��� ������
extern float	aspect;				// windowW / windowH

extern float	dpiScaling;			// windowW�� screenW�� ����

extern int		vsync;				// vertical sync on/off

extern bool		perspectiveView;
extern float	fovy;				// field of view
extern float	nearDist;
extern float	farDist;

GLFWwindow* initializeOpenGL(int argc, char* argv[], GLfloat bg[4]);
void		reshape(GLFWwindow* window, int w, int h);
void		setupProjectionMatrix();
void		drawAxes(float l, float w);

#endif