#include "glSetup.h"  // OpenGL 설정을 위한 헤더 파일 포함
#include <iostream>   // 표준 입출력 사용
using namespace std;

#ifdef _WIN32
#define _USE_MATH_DEFINES  // Windows 환경에서 수학 상수 사용을 위한 매크로
#endif

#include <math.h>  // 수학 함수 사용

// 함수 선언
void	render(GLFWwindow* window);
void	keyboard(GLFWwindow* window, int key, int code, int action, int mods);
void	mouseButton(GLFWwindow* windodw, int button, int action, int mods);
void	mouseMove(GLFWwindow* window, double x, double y);

float	AXIS_LENGTH = 0.5;  // 축의 길이
float	AXIS_LINE_WIDTH = 2.0;  // 축의 선 굵기

GLfloat bgColor[4] = { 1,1,1,1 };  // 배경색을 흰색으로 설정 (RGBA)

float	point[3][2] = { {0,0}, {0,0}, {0,0} };  // 2D 좌표계에서 점 3개 (삼각형을 그리기 위한 점)

enum class InputMode
{
	NONE = 0,  // 아무 입력도 없는 상태
	FIRST_POINT = 1,  // 첫 번째 점을 입력하는 중
	SECOND_POINT = 2,  // 두 번째 점을 입력하는 중
	COMPLETE = 3,  // 삼각형이 완성된 상태
};

InputMode	inputMode = InputMode::NONE;  // 현재 입력 상태 초기화

bool	pause = true;  // 애니메이션 멈춤 여부

float	timeStep = 1.0f / 120;  // 시간 간격 (120fps)
float	period = 8.0;  // 회전 주기

// 점을 회전시키는 함수
void
rotate(float p[2], float delta_theta)
{
	float	x = p[0];
	float	y = p[1];

	float	cos_delta_theta = cos(delta_theta);
	float	sin_delta_theta = sin(delta_theta);

	// 회전 변환을 통해 새로운 좌표 계산
	p[0] = cos_delta_theta * x - sin_delta_theta * y;
	p[1] = sin_delta_theta * x + cos_delta_theta * y;
}

int
main(int argc, char* argv[])
{
	perspectiveView = false;  // 투시 뷰 사용하지 않음

	vsync = 0;  // 수직 동기화 끔

	GLFWwindow* window = initializeOpenGL(argc, argv, bgColor);  // OpenGL 윈도우 초기화
	if (window == NULL) return -1;  // 윈도우 생성 실패 시 종료

	// 키보드 및 마우스 콜백 함수 설정
	glfwSetKeyCallback(window, keyboard);
	glfwSetMouseButtonCallback(window, mouseButton);
	glfwSetCursorPosCallback(window, mouseMove);

	reshape(window, windowW, windowH);  // 윈도우 크기 설정

	// 입력 안내 메시지 출력
	cout << endl;
	cout << "Mouse button down : start point of the line segment" << endl;
	cout << "Mouse dragging :     change" << endl;
	cout << "Mouse button up :    end point of the line segment" << endl;
	cout << "Keyboard input :     space for play/pause" << endl;
	cout << "Keyboard input :     up for increasing period" << endl;
	cout << "Keyboard input :     down for decreasing period" << endl;

	float	previous = (float)glfwGetTime();  // 이전 시간 저장
	float	elapsed = 0;  // 경과 시간 초기화
	while (!glfwWindowShouldClose(window))  // 메인 루프
	{
		glfwPollEvents();  // 이벤트 처리

		float	now = (float)glfwGetTime();  // 현재 시간
		float	delta_t = now - previous;  // 시간 차이 계산
		previous = now;  // 이전 시간 업데이트

		elapsed += delta_t;  // 경과 시간 누적

		if (elapsed > timeStep) {
			// 애니메이션이 실행 중이고 입력이 완료되면 회전
			if (!pause && inputMode == InputMode::COMPLETE) {
				float	delta_theta = float(2.0 * M_PI) / period * elapsed;

				// 삼각형의 각 점을 회전
				rotate(point[0], delta_theta);
				rotate(point[1], delta_theta);
				rotate(point[2], delta_theta);
			}

			elapsed = 0;
		}
		render(window);  // 화면 렌더링
		glfwSwapBuffers(window);  // 더블 버퍼링
	}

	glfwDestroyWindow(window);  // 창 제거
	glfwTerminate();  // OpenGL 종료
	return 0;
}

void
render(GLFWwindow* window)
{
	glClearColor(bgColor[0], bgColor[1], bgColor[2], bgColor[3]);  // 배경색 설정
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // 화면 초기화

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glDisable(GL_LINE_STIPPLE);  // 점선 모드 비활성화
	drawAxes(AXIS_LENGTH, AXIS_LINE_WIDTH * dpiScaling);  // 좌표축 그리기

	glColor3f(0, 0, 1);  // 선 색상을 파란색으로 설정
	glLineWidth(5 * dpiScaling);  // 선 너비 설정

	// 첫 번째 점 입력 중일 때
	if (inputMode == InputMode::FIRST_POINT) {
		glEnable(GL_LINE_STIPPLE);
		glLineStipple(int(3 * dpiScaling), 0xcccc);  // 점선 설정

		glBegin(GL_LINES);
		glVertex2f(point[0][0], point[0][1]);  // 첫 번째 점
		glVertex2f(point[1][0], point[1][1]);  // 두 번째 점 (마우스 이동 중)
		glEnd();
	}

	// 두 번째 점 입력 후
	if (inputMode >= InputMode::SECOND_POINT) {
		glBegin(GL_LINES);
		glVertex2f(point[0][0], point[0][1]);  // 첫 번째 점
		glVertex2f(point[1][0], point[1][1]);  // 두 번째 점
		glEnd();
	}

	// 두 번째 점 입력 중일 때
	if (inputMode == InputMode::SECOND_POINT) {
		glEnable(GL_LINE_STIPPLE);
		glLineStipple(int(3 * dpiScaling), 0xcccc);  // 점선 설정

		glBegin(GL_LINES);
		glVertex2f(point[1][0], point[1][1]);  // 두 번째 점
		glVertex2f(point[2][0], point[2][1]);  // 세 번째 점 (마우스 이동 중)
		glEnd();
	}

	// 삼각형 완성 상태
	if (inputMode == InputMode::COMPLETE) {
		glBegin(GL_TRIANGLES);
		glVertex2f(point[0][0], point[0][1]);  // 첫 번째 점
		glVertex2f(point[1][0], point[1][1]);  // 두 번째 점
		glVertex2f(point[2][0], point[2][1]);  // 세 번째 점
		glEnd();
	}
}

// 키보드 입력 처리
void
keyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS || action == GLFW_REPEAT)
	{
		switch (key)
		{
		case GLFW_KEY_Q:
		case GLFW_KEY_ESCAPE:	glfwSetWindowShouldClose(window, GL_TRUE); break;  // 창 닫기

		case GLFW_KEY_SPACE:	pause = !pause; break;  // 애니메이션 일시정지/재개
		case GLFW_KEY_UP:		period += 0.1f; break;  // 주기 증가
		case GLFW_KEY_DOWN:		period = (float)max(period - 0.1, 0.1); break;  // 주기 감소
		}
	}
}

// 화면 좌표를 월드 좌표로 변환하는 함수
void
screen2world(float xs, float ys, float& xw, float& yw)
{
	float	aspect = (float)screenW / screenH;  // 화면 비율 계산
	xw = 2.0f * (xs / (screenW - 1) - 0.5f) * aspect;
	yw = -2.0f * (ys / (screenH - 1) - 0.5f);
}

// 마우스 버튼 입력 처리
void
mouseButton(GLFWwindow* window, int button, int action, int mods)
{
	double	xs, ys;
	glfwGetCursorPos(window, &xs, &ys);  // 마우스 커서 위치 가져오기

	float	xw, yw;
	screen2world((float)xs, (float)ys, xw, yw);  // 화면 좌표를 월드 좌표로 변환
	if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_LEFT)
	{
		// 첫 번째 점 설정
		if (inputMode == InputMode::NONE) {
			inputMode = InputMode::FIRST_POINT;
			point[0][0] = xw;	point[0][1] = yw;
		}
		// 두 번째 점 설정
		else if (inputMode == InputMode::FIRST_POINT) {
			inputMode = InputMode::SECOND_POINT;
			point[1][0] = xw;	point[1][1] = yw;
		}
		// 세 번째 점 설정 및 삼각형 완성
		else if (inputMode == InputMode::SECOND_POINT) {
			inputMode = InputMode::COMPLETE;
			point[2][0] = xw;	point[2][1] = yw;
		}
		// 다시 첫 번째 점으로 설정
		else if (inputMode == InputMode::COMPLETE) {
			inputMode = InputMode::FIRST_POINT;
			point[0][0] = xw;	point[0][1] = yw;
		}
	}
}

// 마우스 이동 처리
void
mouseMove(GLFWwindow* window, double x, double y) {
	// 첫 번째 점을 설정한 후 마우스 이동 중일 때 두 번째 점의 위치를 업데이트
	if (inputMode == InputMode::FIRST_POINT) {
		screen2world((float)x, (float)y, point[1][0], point[1][1]);
	}
	// 두 번째 점을 설정한 후 마우스 이동 중일 때 세 번째 점의 위치를 업데이트
	else if (inputMode == InputMode::SECOND_POINT) {
		screen2world((float)x, (float)y, point[2][0], point[2][1]);
	}
}
