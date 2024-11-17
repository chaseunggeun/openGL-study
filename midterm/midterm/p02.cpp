#include "glSetup.h"  // OpenGL 설정을 위한 헤더 파일 포함
#include <iostream>   // 표준 입출력 사용
using namespace std;

void	render(GLFWwindow* window);  // 화면 렌더링 함수
void	keyboard(GLFWwindow* window, int key, int code, int action, int mods);  // 키보드 입력 처리 함수

GLfloat bgColor[4] = { 1,1,1,1 };  // 배경색을 흰색으로 설정 (RGBA)

GLenum	mode = GL_POINTS;  // 그리기 모드를 GL_POINTS로 초기화
bool	stipple = false;  // 선 패턴(점선) 사용 여부
bool	fill1 = true;  // 폴리곤을 채울지 여부

int
main(int argc, char* argv[])
{
	perspectiveView = false;  // 투시 뷰 사용하지 않음

	GLFWwindow* window = initializeOpenGL(argc, argv, bgColor);  // OpenGL 윈도우 초기화
	if (window == NULL) return -1;  // 윈도우 생성 실패 시 종료

	glfwSetKeyCallback(window, keyboard);  // 키보드 입력 콜백 함수 설정

	reshape(window, windowW, windowH);  // 윈도우 크기 조정

	// 키보드 입력 안내 메시지 출력
	std::cout << std::endl;
	std::cout << "Keyboard Input: 1 GL_POINTS" << std::endl;
	std::cout << "Keyboard Input: 2 GL_LINES" << std::endl;
	std::cout << "Keyboard Input: 3 GL_LINE_STRIP" << std::endl;
	std::cout << "Keyboard Input: 4 GL_LINE LOOP" << std::endl;
	std::cout << "Keyboard Input: 5 GL_TRIANGLES" << std::endl;
	std::cout << "Keyboard Input: 6 GL_TRIANGLE_STRIP" << std::endl;
	std::cout << "Keyboard Input: 7 GL_TRIANGLE_FAN" << std::endl;
	std::cout << "Keyboard Input: 8 GL_POLYGON" << std::endl;
	std::cout << std::endl;
	std::cout << "Keyboard Input: s for stippling on/off" << std::endl;
	std::cout << "Keyboard Input: f for polygon filling on/off" << std::endl;

	// 메인 루프: 창이 닫히기 전까지 계속 실행
	while (!glfwWindowShouldClose(window))
	{
		render(window);  // 화면을 렌더링
		glfwSwapBuffers(window);  // 더블 버퍼링을 통해 화면 표시
		glfwPollEvents();  // 이벤트 처리
	}
	glfwDestroyWindow(window);  // 창을 파괴
	glfwTerminate();  // OpenGL 종료
	return 0;
}

void
render(GLFWwindow* window)
{
	// 화면 클리어 (색상 및 깊이 버퍼 초기화)
	glClearColor(bgColor[0], bgColor[1], bgColor[2], bgColor[3]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);  // 모델뷰 행렬 설정
	glLoadIdentity();  // 모델뷰 행렬 초기화

	// 점선 모드 설정
	if (stipple) {
		glEnable(GL_LINE_STIPPLE);
		glLineStipple(int(3 * dpiScaling), 0xcccc);  // 점선 패턴 설정
	}
	else	glDisable(GL_LINE_STIPPLE);

	// 폴리곤 채우기 여부 설정
	if(fill1)	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);  // 채우기 모드
	else	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  // 선 모드

	glPointSize(5 * dpiScaling);  // 점 크기 설정

	glLineWidth(5 * dpiScaling);  // 선 너비 설정

	// 지정된 모드에 따라 도형 그리기
	glBegin(mode);
	{
		// 각각의 정점에 색상을 설정하여 사각형 그리기
		if (mode == GL_QUADS) {
			glColor3f(1, 0, 0); glVertex2f(-1.5, -0.5);
			glColor3f(0, 1, 0); glVertex2f(-0.5, -0.5);
			glColor3f(0, 0, 1); glVertex2f(-0.5, 0.5);
			glColor3f(0, 0, 0); glVertex2f(-1.5, 0.5);

			glColor3f(1, 0, 0); glVertex2f(0.5, -0.5);
			glColor3f(0, 1, 0); glVertex2f(1.5, -0.5);
			glColor3f(0, 0, 1); glVertex2f(1.5, 0.5);
			glColor3f(0, 0, 0); glVertex2f(0.5, 0.5);
		}
		else if (mode == GL_QUAD_STRIP) {
			glColor3f(1, 0, 0); glVertex2f(-1.5, -0.5);
			glColor3f(0, 1, 0); glVertex2f(-1.5, 0.5);
			glColor3f(0, 0, 1); glVertex2f(-0.5, -0.5);
			glColor3f(0, 0, 0); glVertex2f(-0.5, 0.5);
			glColor3f(1, 0, 0); glVertex2f(0.5, -0.5);
			glColor3f(0, 1, 0); glVertex2f(0.5, 0.5);
			glColor3f(0, 0, 1); glVertex2f(1.5, -0.5);
			glColor3f(0, 0, 0); glVertex2f(1.5, 0.5);
		}
		else {
			glColor3f(1, 0, 0); glVertex2f(-0.5, -0.5);
			glColor3f(0, 1, 0); glVertex2f(0.5, -0.5);
			glColor3f(0, 0, 1); glVertex2f(0.5, 0.5);
			glColor3f(0, 0, 0); glVertex2f(-0.5, 0.5);
		}

	}
	glEnd();
}

void
keyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// 키가 눌리거나 반복될 때 실행
	if (action == GLFW_PRESS || action == GLFW_REPEAT)
	{
		switch (key)
		{
		case GLFW_KEY_Q:
		case GLFW_KEY_ESCAPE: glfwSetWindowShouldClose(window, GL_TRUE); break;  // 창 닫기

		// s 키를 눌러 점선 on/off
		case GLFW_KEY_S:	stipple = !stipple;			break;
		
		// f 키를 눌러 폴리곤 채우기 on/off
		case GLFW_KEY_F:	fill1 = !fill1;				break;

		// 숫자 키를 눌러 그리기 모드 변경
		case GLFW_KEY_1:	mode = GL_POINTS;			break;
		case GLFW_KEY_2:	mode = GL_LINES;			break;
		case GLFW_KEY_3:	mode = GL_LINE_STRIP;		break;
		case GLFW_KEY_4:	mode = GL_LINE_LOOP;		break;
		case GLFW_KEY_5:	mode = GL_TRIANGLES;		break;
		case GLFW_KEY_6:	mode = GL_TRIANGLE_STRIP;	break;
		case GLFW_KEY_7:	mode = GL_TRIANGLE_FAN;		break;
		case GLFW_KEY_8:	mode = GL_POLYGON;			break;
		case GLFW_KEY_9:	mode = GL_QUADS;			break;
		case GLFW_KEY_0:	mode = GL_QUAD_STRIP;			break;

		// 나머지 키에 대한 처리 (아직 구현되지 않음)
		case GLFW_KEY_SPACE:			break;
		case GLFW_KEY_PERIOD:			break;
		case GLFW_KEY_LEFT:				break;
		case GLFW_KEY_RIGHT:			break;	
		case GLFW_KEY_LEFT_BRACKET:		break;
		case GLFW_KEY_RIGHT_BRACKET:	break;
		case GLFW_KEY_UP:				break;
		case GLFW_KEY_DOWN:				break;
		}
	}
}
