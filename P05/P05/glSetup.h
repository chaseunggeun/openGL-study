#ifndef __GL_SETUP_H_
#define __GL_SETUP_H_

#if defined(__APPLE__) && defined(__MACH__)
#include <OpenGL/glu.h>
#else
#include <windows.h>
#include <gl/GLU.h>
#endif

#include <GLFW/glfw3.h>

extern float	scteenscale;		// 윈도우를 띄울 때 윈도우가 전체 어느정도로 만들어지게 할 지
extern int		screenW, screenH;	// 모니터의 해상도
extern int		windowW, windowH;	// 프레임버퍼 사이즈
extern float	aspect;				// windowW / windowH

extern float	dpiScaling;			// windowW와 screenW의 비율

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