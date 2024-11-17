//p02
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

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
	if (fill1)	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);  // 채우기 모드
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

//p03
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
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

//p04
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

#include <iostream>
using namespace std;

// Part 1: 벡터 생성 및 접근
void
part1()
{
	cout << "3x1 vectors" << endl;

	// 3차원 벡터 a를 0으로 초기화
	glm::vec3 a(0);
	cout << "a = " << to_string(a) << endl;

	// 3차원 벡터 b를 (3, 2, 1)로 초기화
	glm::vec<3, float> b(3, 2, 1);
	cout << "b = " << to_string(b) << endl;

	// a에 새로운 값 (1, 2, 3) 대입
	a = glm::vec3(1, 2, 3);
	cout << "a = " << to_string(a) << endl;

	// a의 z값 변경
	a.z = 3;
	cout << "a[0] = " << a[0] << endl;
	cout << "a.z = " << a.z << endl;
}

// Part 2: 벡터 연산
void
part2()
{
	cout << "Vector operations" << endl;

	// 두 3차원 벡터 a와 b를 정의
	glm::vec3 a(1, 2, 3);
	glm::vec3 b(3, 2, 1);
	cout << "a = " << to_string(a) << endl;
	cout << "b = " << to_string(b) << endl;

	// 벡터 덧셈
	cout << "a + b = " << to_string(a + b) << endl;
	// 벡터 뺄셈
	cout << "a - b = " << to_string(a - b) << endl;

	// 벡터 부호 반전
	cout << "-a = " << to_string(-a) << endl;

	// 벡터 스칼라 곱셈
	cout << "1.5*a = " << to_string(1.5f * a) << endl;

	// 내적 계산
	cout << "dot(a, b) = " << dot(a, b) << endl;

	// 외적 계산
	cout << "cross(a, b) = " << to_string(cross(a, b)) << endl;

	// 벡터 길이 계산
	cout << "length(a) = " << length(a) << endl;
	cout << endl;
}

// Part 3: 3x3 행렬 생성 및 접근
void
part3()
{
	cout << "3x3 matrices" << endl;

	// 3x3 행렬 A를 0으로 초기화
	glm::mat3 A(0);
	cout << "A = " << to_string(A) << endl;

	// A에 단위 행렬 할당
	A = glm::mat3(1.0);
	cout << "A = " << to_string(A) << endl;

	// 행렬 B를 직접 초기화
	glm::mat3 B(1.0, 0.0, 0.0, 2.0, 1.0, 0.0, 3.0, 0.0, 1.0);
	cout << "B = " << to_string(B) << endl;

	// 배열을 사용해 행렬 B를 초기화
	float B_data[9] = { 1.0, 0.0, 0.0, 2.0, 1.0, 0.0, 3.0, 0.0, 1.0 };
	B = glm::make_mat3(B_data);
	cout << "B = " << to_string(B) << endl;

	// 행렬 B의 3번째 열을 출력
	cout << "3rd col of B = " << to_string(B[2]) << endl;
	cout << "3rd col B = " << to_string(column(B, 2)) << endl;
	// 행렬 B의 3번째 행을 출력
	cout << "3rd row of B = " << to_string(row(B, 2)) << endl;
	// 1행 3열의 값 접근
	cout << "1st row 3rd col of B = " << B[2][0] << endl;
	cout << "1st row 3rd col of B = " << B[2].x << endl;
}

// Part 4: 행렬 연산
void
part4()
{
	cout << "Matrix operations" << endl;

	// 두 행렬 A와 B 정의
	glm::mat3 A(1.0);
	glm::mat3 B(1.0, 0.0, 0.0, 2.0, 1.0, 0.0, 3.0, 0.0, 1.0);
	cout << "A = " << to_string(A) << endl;
	cout << "B = " << to_string(B) << endl;

	// 행렬 덧셈
	cout << "A + B = " << to_string(A + B) << endl;
	// 행렬 뺄셈
	cout << "A - B = " << to_string(A - B) << endl;

	// 행렬 부호 반전
	cout << "-A = " << to_string(-A) << endl;

	// 행렬 스칼라 곱셈
	cout << "1.5*A = " << to_string(1.5f * A) << endl;

	// 행렬 곱셈
	cout << "A X B = " << to_string(A * B) << endl;

	// 행렬 전치
	cout << "transpose(B) = " << to_string(transpose(B)) << endl;

	// 행렬 역행렬
	cout << "inverse(B) = " << to_string(inverse(B)) << endl;
	cout << endl;

	// 역행렬 곱셈 확인
	cout << "inverse(B) * B = " << to_string(inverse(B) * B) << endl;
}

// Part 5: 행렬과 벡터 곱셈
void
part5()
{
	cout << "Matrix-vector multiplication and assembling" << endl;

	// 벡터 a 정의
	glm::vec3 a(1, 2, 3);
	cout << "a = " << to_string(a) << endl;

	// 행렬 B 정의
	glm::mat3 B(1.0, 0.0, 0.0, 2.0, 1.0, 0.0, 3.0, 0.0, 1.0);
	cout << "B = " << to_string(B) << endl;

	// 행렬과 벡터 곱셈
	cout << "B x a = " << to_string(B * a) << endl;
	cout << "a x B = " << to_string(a * B) << endl;

	// 벡터 4차원 확장
	cout << "(a, 1.0) = " << to_string(glm::vec4(a, 1.0)) << endl;
	cout << "(1.0, a) = " << to_string(glm::vec4(1.0, a)) << endl;

	// 3x3 행렬을 4x4 행렬로 확장
	glm::mat4 C = glm::mat4(B);
	cout << "C = " << to_string(C) << endl;
}

// Part 6: 연습 문제 1 - 벡터 연산
void
part6()
{
	cout << "Exercise 01" << endl;
	glm::vec<3, float> v1(1, 2, 3);
	glm::vec<3, float> v2(3, 2, 1);
	cout << "v1 = " << to_string(v1) << endl;
	cout << "v2 = " << to_string(v2) << endl;

	// 벡터 덧셈 및 뺄셈
	cout << "v1 + v2 = " << to_string(v1 + v2) << endl;
	cout << "v1 - v2 = " << to_string(v1 - v2) << endl;

	// 벡터 부호 반전
	cout << "-v1 = " << to_string(-v1) << endl;

	// 스칼라 곱셈과 벡터 뺄셈
	cout << "v1 - 2 * v2 = " << to_string(v1 - 2.0f * v2) << endl;

	// 내적 및 외적
	cout << "dot(v1, v2) = " << to_string(dot(v1, v2)) << endl;
	cout << "cross(v1, v2) = " << to_string(cross(v1, v2)) << endl;
}

// Part 7: 연습 문제 2 - 행렬 연산
void
part7()
{
	cout << "Exercise 02" << endl;
	glm::vec<3, float> v1(1, 2, 3);
	glm::vec<3, float> v2(3, 2, 1);

	// 행렬 A1과 A2 정의
	glm::mat3 A1(1, 2, 1, 2, 3, 1, 3, 2, 2);
	glm::mat3 A2(2, 2, 1, 1, 2, 1, 2, 1, 1);

	cout << "A1 = " << to_string(A1) << endl;
	cout << "A2 = " << to_string(A2) << endl;

	// 행렬 덧셈 및 뺄셈
	cout << "A1 + A2 = " << to_string(A1 + A2) << endl;
	cout << "A1 - A2 = " << to_string(A1 - A2) << endl;

	// 행렬 부호 반전
	cout << "-A1 = " << to_string(-A1) << endl;

	// 스칼라 곱셈과 행렬 뺄셈
	cout << "A1 - 2 * A2 = " << to_string(A1 - 2.0f * A2) << endl;

	// 행렬 곱셈 및 행렬과 벡터 곱셈
	cout << "A1 * A2 = " << to_string(A1 * A2) << endl;
	cout << "A2 * A1 = " << to_string(A2 * A1) << endl;
	cout << "A1 * v1 = " << to_string(A1 * v1) << endl;
	cout << "A2 * v2 = " << to_string(A2 * v2) << endl;
}

int
main(int argc, char* argv[])
{
	int a;
	cin >> a; // 실행할 파트 선택

	// 입력한 값에 따라 함수 실행
	switch (a)
	{
	case 1: part1(); break;
	case 2: part2(); break;
	case 3: part3(); break;
	case 4: part4(); break;
	case 5: part5(); break;
	case 6: part6(); break;
	case 7: part7(); break;
	}

	return 0;
}

//p05
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
#include "glSetup.h"

#ifdef _WIN32
#define _USE_MATH_DEFINES
#endif

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

#include <iostream>
#include <fstream>
#include <list>
using namespace std;

list<pair<int, int>> edgeList;
bool edgeExists(int v1, int v2) {
	if (v1 > v2) std::swap(v1, v2);
	for (const auto& edge : edgeList) {
		if (edge.first == v1 && edge.second == v2) {
			return true;
		}
	}
	return false;
}

void	init();
void	quit();
void	render(GLFWwindow* window);
void	keyboard(GLFWwindow* window, int key, int code, int action, int mods);

bool	readMesh(const char* filename);
void	deleteMesh();

vec3	eye(3, 3, 3);
vec3	center(0, 0, 0);
vec3	up(0, 0, 1);

vec4	light(5.0, 5.0, 0.0, 1);

float	AXIS_LENGTH = 1.75;
float	AXIS_LINE_WIDTH = 2;

GLfloat bgColor[4] = { 1,1,1,1 };

int selection = 1;

GLUquadricObj* sphere = NULL;
GLUquadricObj* cylinder = NULL;
GLUquadricObj* disk = NULL;

bool	pause = true;

float	timeStep = 1.0f / 120;
float	period = 8.0;
float	theta = 0;

bool	depthTest = true;
bool	polygonFill = true;

int
main(int argc, char* argv[])
{
	vsync = 0;

	GLFWwindow* window = initializeOpenGL(argc, argv, bgColor);
	if (window == NULL) return -1;

	glfwSetKeyCallback(window, keyboard);

	glEnable(GL_NORMALIZE);

	glDisable(GL_CULL_FACE);

	reshape(window, windowW, windowH);

	init();

	float	previous = (float)glfwGetTime();
	float	elapsed = 0;
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		float	now = (float)glfwGetTime();
		float	delta = now - previous;
		previous = now;

		elapsed += delta;

		if (elapsed > timeStep)
		{
			if (!pause)
			{
				theta += float(2.0 * M_PI) / period * elapsed;
			}
			elapsed = 0;
		}

		render(window);
		glfwSwapBuffers(window);
	}

	quit();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

void
init()
{
	sphere = gluNewQuadric();
	gluQuadricDrawStyle(sphere, GLU_FILL);
	gluQuadricNormals(sphere, GLU_SMOOTH);
	gluQuadricOrientation(sphere, GLU_OUTSIDE);
	gluQuadricTexture(sphere, GL_FALSE);

	cylinder = gluNewQuadric();
	gluQuadricDrawStyle(cylinder, GLU_FILL);
	gluQuadricNormals(cylinder, GLU_SMOOTH);
	gluQuadricOrientation(cylinder, GLU_OUTSIDE);
	gluQuadricTexture(cylinder, GL_FALSE);

	disk = gluNewQuadric();
	gluQuadricDrawStyle(disk, GLU_FILL);
	gluQuadricNormals(disk, GLU_SMOOTH);
	gluQuadricOrientation(disk, GLU_OUTSIDE);
	gluQuadricTexture(disk, GL_FALSE);

	readMesh(".\\m01_bunny.off");

	cout << endl;
	cout << "Keyboard Input: d for depth test on/off" << endl;
	cout << "Keyboard Input: f for polygonfill on/off" << endl;
	cout << endl;
	cout << "Keyboard Input: 1 for sphere" << endl;
	cout << "Keyboard Input: 2 for cylinder" << endl;
	cout << "Keyboard Input: 3 for disk" << endl;
	cout << "Keyboard Input: 4 for bunny" << endl;
}

void
quit()
{
	gluDeleteQuadric(sphere);
	gluDeleteQuadric(cylinder);
	gluDeleteQuadric(disk);

	deleteMesh();
}

void
setupLight()
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	GLfloat	ambient[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
	GLfloat	diffuse[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat	specular[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_POSITION, value_ptr(light));

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
}

void
setupMaterial()
{
	GLfloat	mat_ambient[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
	GLfloat mat_specular[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat mat_shininess = 128;

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
}

void
setDiffuseColor(const vec3& color)
{
	GLfloat	mat_diffuse[4] = { color[0], color[1], color[2], 1 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
}

void
drawSphere(float radius, const vec3& color)
{
	setDiffuseColor(color);

	gluSphere(sphere, radius, 36, 36);

}

void
drawCylinder(float radius, float height, const vec3& color)
{
	setDiffuseColor(color);

	gluCylinder(cylinder, radius, radius, height, 72, 5);
}

void
drawDisk(float inner_radius, float outer_radius, const vec3& color)
{
	setDiffuseColor(color); // 주어진 색상으로 확산 색상 설정

	gluDisk(disk, inner_radius, outer_radius, 72, 5); // 주어진 반지름으로 원반 그리기
}

int		nVertices = 0, nFaces = 0, nEdges = 0; // 정점, 면, 엣지의 수를 저장할 변수
vec3* vertex = NULL; // 정점 배열 포인터
int* face[3] = { NULL, NULL, NULL }; // 면 배열 포인터

int edgeCount = 0; // 엣지 개수 카운터
void addEdge(int v1, int v2) {
	if (v1 > v2) std::swap(v1, v2); // v1이 v2보다 크면 두 값을 교환

	if (!edgeExists(v1, v2)) { // 엣지가 존재하지 않으면
		edgeList.emplace_back(v1, v2); // 엣지를 엣지 리스트에 추가
		edgeCount++; // 엣지 개수 증가
	}
}

bool readMesh(const char* filename) {
	ifstream is(filename); // 파일 읽기 스트림 생성
	if (is.fail()) return false; // 파일 열기 실패 시 false 반환

	char magicNumber[256];
	is >> magicNumber; // 매직 넘버 읽기

	is >> nVertices >> nFaces >> nEdges; // 정점, 면, 엣지 개수 읽기
	cout << "# vertices = " << nVertices << endl; // 정점 수 출력
	cout << "# faces = " << nFaces << endl; // 면 수 출력

	vertex = new vec3[nVertices]; // 정점 배열 동적 할당
	for (int i = 0; i < nVertices; i++) {
		is >> vertex[i].x >> vertex[i].y >> vertex[i].z; // 정점 좌표 읽기
	}

	face[0] = new int[nFaces]; // 면 배열 동적 할당
	face[1] = new int[nFaces]; // 면 배열 동적 할당
	face[2] = new int[nFaces]; // 면 배열 동적 할당

	int n; // 면의 정점 수
	for (int i = 0; i < nFaces; i++) {
		int v1, v2, v3; // 면을 구성하는 정점
		is >> n >> v1 >> v2 >> v3; // 면 정보 읽기
		addEdge(v1, v2); // 엣지 추가
		addEdge(v2, v3); // 엣지 추가
		addEdge(v3, v1); // 엣지 추가
	}
	cout << "# edges = " << edgeCount << endl; // 엣지 수 출력
	return true; // 성공 시 true 반환
}

void drawEdges() {
	glBegin(GL_LINES); // 선 그리기 시작
	for (const auto& edge : edgeList) { // 엣지 리스트를 순회
		glVertex3f(vertex[edge.first].x, vertex[edge.first].y, vertex[edge.first].z); // 첫 번째 정점
		glVertex3f(vertex[edge.second].x, vertex[edge.second].y, vertex[edge.second].z); // 두 번째 정점
	}
	glEnd(); // 선 그리기 종료
}

void
deleteMesh()
{
	if (vertex) { delete[] vertex; vertex = NULL; } // 정점 배열 해제
	if (face[0]) { delete[] face[0]; face[0] = NULL; } // 면 배열 해제
	if (face[1]) { delete[] face[1]; face[1] = NULL; } // 면 배열 해제
	if (face[2]) { delete[] face[2]; face[2] = NULL; } // 면 배열 해제
}

void
drawMesh()
{
	setDiffuseColor(vec3(0, 0, 0)); // 색상 설정

	glRotatef(90, 0, 0, 1); // Z축을 중심으로 90도 회전
	glRotatef(90, 1, 0, 0); // X축을 중심으로 90도 회전

	glPointSize(3 * dpiScaling); // 포인트 크기 설정
	glBegin(GL_POINTS); // 점 그리기 시작
	for (int i = 0; i < nVertices; i++)
		glVertex3f(vertex[i].x, vertex[i].y, vertex[i].z); // 각 정점 그리기
	glEnd(); // 점 그리기 종료
}

void
render(GLFWwindow* window)
{
	if (depthTest)	glEnable(GL_DEPTH_TEST); // 깊이 테스트 활성화
	else			glDisable(GL_DEPTH_TEST); // 깊이 테스트 비활성화

	if (polygonFill)	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // 면 채우기
	else				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // 선 모드

	glClearColor(bgColor[0], bgColor[1], bgColor[2], bgColor[3]); // 배경색 설정
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 색상 및 깊이 버퍼 지우기

	glMatrixMode(GL_MODELVIEW); // 모델 뷰 행렬 모드 설정
	glLoadIdentity(); // 행렬 초기화

	// 카메라 위치 및 방향 설정
	gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], up[0], up[1], up[2]);

	glDisable(GL_LIGHTING); // 조명 비활성화
	drawAxes(AXIS_LENGTH, AXIS_LINE_WIDTH * dpiScaling); // 축 그리기

	setupLight(); // 조명 설정
	setupMaterial(); // 재질 설정

	glRotatef(degrees(theta), 0, 1, 0); // Y축을 기준으로 회전

	// 선택된 도형 그리기
	switch (selection)
	{
	case 1:	drawSphere(1.0, vec3(0.95, 0.95, 0.95));		break; // 구 그리기
	case 2: drawCylinder(1.0, 1.0, vec3(0.95, 0.95, 0.95));	break; // 원기둥 그리기
	case 3: drawDisk(0.5, 1.0, vec3(0.95, 0.95, 0.95));		break; // 원반 그리기
	case 4: drawMesh();										break; // 메시 그리기
	case 5: drawEdges();									break; // 엣지 그리기
	}
}

void
keyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS || action == GLFW_REPEAT) // 키 입력 처리
	{
		switch (key)
		{
		case GLFW_KEY_Q:
		case GLFW_KEY_ESCAPE:	glfwSetWindowShouldClose(window, GL_TRUE); break; // 종료 키 처리

		case GLFW_KEY_1:	selection = 1; break; // 구 선택
		case GLFW_KEY_2:	selection = 2; break; // 원기둥 선택
		case GLFW_KEY_3:	selection = 3; break; // 원반 선택
		case GLFW_KEY_4:	selection = 4; break; // 메시 선택
		case GLFW_KEY_5:	selection = 5; break; // 엣지 선택

		case GLFW_KEY_SPACE:	pause = !pause; break; // 일시정지/재개

		case GLFW_KEY_D:		depthTest = !depthTest; break; // 깊이 테스트 토글
		case GLFW_KEY_F:		polygonFill = !polygonFill; break; // 면 채우기 토글
		}
	}
}



//p06
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
#define GLM_ENABLE_EXPERIMENTAL
#include "glSetup.h"

#ifdef _WIN32
#define _USE_MATH_DEFINES
#endif

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

#include <iostream>
using namespace std;

// 초기화 및 종료 함수 선언
void	init();
void	quit();
void	render(GLFWwindow* window);
void	keyboard(GLFWwindow* window, int key, int code, int action, int mods);

// 카메라 위치 및 방향 설정
vec3	eye(3.5, 3, 3.5);
vec3	center(0, 0, 0);
vec3	up(0, 1, 0);

// 조명 위치 설정
vec4	light(5.0, 5.0, 5.0, 1);

// 축 길이 및 선 두께 설정
float	AXIS_LENGTH = 3;
float	AXIS_LINE_WIDTH = 2;

// 배경 색상 설정
GLfloat bgColor[4] = { 1,1,1,1 };

// 선택된 예시 번호
int selection = 1;

// 구, 원기둥, 디스크를 위한 쿼드릭 객체 포인터
GLUquadricObj* sphere = NULL;
GLUquadricObj* cylinder = NULL;
GLUquadricObj* disk = NULL;

// 일시 정지 상태 변수
bool	pause = true;

// 시간 관련 변수 설정
float	timeStep = 1.0f / 120;
float	period = 4.0;

// 프레임 카운터
int frame = 0;

int
main(int argc, char* argv[])
{
	vsync = 0; // 수직 동기화 설정

	// OpenGL 초기화
	GLFWwindow* window = initializeOpenGL(argc, argv, bgColor);
	if (window == NULL) return -1; // 창 초기화 실패 시 종료

	glfwSetKeyCallback(window, keyboard); // 키 입력 콜백 설정

	glEnable(GL_DEPTH_TEST); // 깊이 테스트 활성화

	glEnable(GL_NORMALIZE); // 노멀 벡터 정규화 활성화

	reshape(window, windowW, windowH); // 창 크기 조정

	init(); // 초기화 함수 호출

	float	previous = (float)glfwGetTime(); // 이전 시간 저장
	float	elapsed = 0; // 경과 시간 초기화
	while (!glfwWindowShouldClose(window)) // 창이 닫힐 때까지 반복
	{
		glfwPollEvents(); // 이벤트 처리

		float	now = (float)glfwGetTime(); // 현재 시간 가져오기
		float	delta = now - previous; // 시간 차 계산
		previous = now; // 이전 시간 업데이트

		elapsed += delta; // 경과 시간 누적

		if (elapsed > timeStep) // 일정 시간 이상 경과 시
		{
			if (!pause) frame += 1; // 일시 정지 상태가 아닐 경우 프레임 증가

			elapsed = 0; // 경과 시간 초기화
		}

		render(window); // 렌더링 함수 호출
		glfwSwapBuffers(window); // 버퍼 스왑
	}

	quit(); // 종료 함수 호출

	glfwDestroyWindow(window); // 창 파괴
	glfwTerminate(); // GLFW 종료

	return 0; // 프로그램 종료
}

void
init()
{
	// 구 객체 초기화
	sphere = gluNewQuadric();
	gluQuadricDrawStyle(sphere, GLU_FILL); // 채우기 스타일 설정
	gluQuadricNormals(sphere, GLU_SMOOTH); // 부드러운 노멀 설정
	gluQuadricOrientation(sphere, GLU_OUTSIDE); // 외부 방향 설정
	gluQuadricTexture(sphere, GL_FALSE); // 텍스처 사용 안 함

	// 원기둥 객체 초기화
	cylinder = gluNewQuadric();
	gluQuadricDrawStyle(cylinder, GLU_FILL);
	gluQuadricNormals(cylinder, GLU_SMOOTH);
	gluQuadricOrientation(cylinder, GLU_OUTSIDE);
	gluQuadricTexture(cylinder, GL_FALSE);

	// 디스크 객체 초기화
	disk = gluNewQuadric();
	gluQuadricDrawStyle(disk, GLU_FILL);
	gluQuadricNormals(disk, GLU_SMOOTH);
	gluQuadricOrientation(disk, GLU_OUTSIDE);
	gluQuadricTexture(disk, GL_FALSE);

	// 키보드 입력 안내 메시지 출력
	cout << endl;
	cout << "Keyboard Input: space for play/pause" << endl;
	cout << endl;

	cout << "Keyboard Input: 1 for rotation example" << endl;
	cout << "Keyboard Input: 2 for rotation wrt a pivot example" << endl;
	cout << "Keyboard Input: 3 for rotation wrt a pivot example using GLM" << endl;
	cout << "Keyboard Input: 4 for scaling example" << endl;
	cout << "Keyboard Input: 5 for scaling wrt a pivot example" << endl;
	cout << "Keyboard Input: 6 for scaling wrt a direction example" << endl;
	cout << "Keyboard Input: 7 for a solar system example" << endl;
	cout << "Keyboard Input: 8 scalingPivotExampleGLM" << endl;
	cout << "Keyboard Input: 9 scalingDirectionExampleGLM" << endl;
}
void
quit()
{
	// 쿼드릭 객체 메모리 해제
	gluDeleteQuadric(sphere);
	gluDeleteQuadric(cylinder);
	gluDeleteQuadric(disk);
}

void
setupLight()
{
	// 조명 활성화
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// 조명 속성 설정
	GLfloat	ambient[4] = { 0.1f, 0.1f, 0.1f, 1.0f }; // 주변광
	GLfloat	diffuse[4] = { 1.0f, 1.0f, 1.0f, 1.0f }; // 확산광
	GLfloat	specular[4] = { 0.1f, 0.1f, 0.1f, 1.0f }; // 반사광

	// 조명 속성 적용
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_POSITION, value_ptr(light)); // 조명 위치 설정
}

void
setupMaterial()
{
	// 재질 속성 설정
	GLfloat	mat_ambient[4] = { 0.1f, 0.1f, 0.1f, 1.0f }; // 주변광
	GLfloat mat_specular[4] = { 0.5f, 0.5f, 0.5f, 1.0f }; // 반사광
	GLfloat mat_shininess = 128; // 광택도

	// 재질 속성 적용
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
}

void
setDiffuseColor(const vec3& color)
{
	// 확산광 색상 설정
	GLfloat	mat_diffuse[4] = { color[0], color[1], color[2], 1 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
}

void
drawSphere(float radius, const vec3& color)
{
	// 구 그리기
	setDiffuseColor(color); // 색상 설정
	gluSphere(sphere, radius, 72, 72); // 구 생성
}

void
drawCylinder(float radius, float height, const vec3& color)
{
	// 원기둥 그리기
	setDiffuseColor(color); // 색상 설정
	gluCylinder(cylinder, radius, radius, height, 72, 5); // 원기둥 생성
}

void
drawDisk(float inner_radius, float outer_radius, const vec3& color)
{
	// 디스크 그리기
	setDiffuseColor(color); // 색상 설정
	gluDisk(disk, inner_radius, outer_radius, 72, 5); // 디스크 생성
}

void
drawCube()
{
	// 큐브 그리기
	glBegin(GL_QUADS); // 사각형 그리기 시작

	// 앞면
	glNormal3f(0, 0, 1); // 면의 법선
	glVertex3f(0, 0, 1);
	glVertex3f(1, 0, 1);
	glVertex3f(1, 1, 1);
	glVertex3f(0, 1, 1);

	// 뒷면
	glNormal3f(0, 0, -1);
	glVertex3f(0, 1, 0);
	glVertex3f(1, 1, 0);
	glVertex3f(1, 0, 0);
	glVertex3f(0, 0, 0);

	// 오른쪽 면
	glNormal3f(1, 0, 0);
	glVertex3f(1, 0, 1);
	glVertex3f(1, 0, 0);
	glVertex3f(1, 1, 0);
	glVertex3f(1, 1, 1);

	// 왼쪽 면
	glNormal3f(-1, 0, 0);
	glVertex3f(0, 1, 1);
	glVertex3f(0, 1, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 1);

	// 위쪽 면
	glNormal3f(0, 1, 0);
	glVertex3f(0, 1, 1);
	glVertex3f(1, 1, 1);
	glVertex3f(1, 1, 0);
	glVertex3f(0, 1, 0);

	// 아래쪽 면
	glNormal3f(0, -1, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(1, 0, 0);
	glVertex3f(1, 0, 1);
	glVertex3f(0, 0, 1);

	glEnd(); // 사각형 그리기 종료
}
void
rotationExample()
{
	float		theta1 = frame * 4 / period; // 회전 각도 계산
	vec3		axis(0, 0, 1); // 회전 축 설정

	glRotatef(theta1, axis.x, axis.y, axis.z); // 회전 적용

	setDiffuseColor(vec3(1, 1, 1)); // 색상 설정
	drawCube(); // 큐브 그리기
}

void
rotationPivotExample()
{
	float		theta1 = frame * 4 / period; // 회전 각도 계산
	vec3		axis(0, 0, 1); // 회전 축 설정
	vec3		pivot(0.5, 0.5, 0); // 피벗 포인트 설정

	glTranslatef(pivot.x, pivot.y, pivot.z); // 피벗 위치로 이동
	glRotatef(theta1, axis.x, axis.y, axis.z); // 회전 적용
	glTranslatef(-pivot.x, -pivot.y, -pivot.z); // 원래 위치로 이동

	setDiffuseColor(vec3(1, 1, 1)); // 색상 설정
	drawCube(); // 큐브 그리기
}

void
rotationPivotExampleGLM()
{
	float		theta1 = frame * 4 / period; // 회전 각도 계산
	vec3		axis(0, 0, 1); // 회전 축 설정
	vec3		pivot(0.5, 0.5, 0); // 피벗 포인트 설정

	mat4		M(1.0); // 단위 행렬 생성

	M = translate(M, pivot); // 피벗 위치로 변환 추가

	M = rotate(M, radians(theta1), axis); // 회전 추가

	M = translate(M, -pivot); // 원래 위치로 변환 추가

	glMultMatrixf(value_ptr(M)); // 변환 행렬 적용

	setDiffuseColor(vec3(1, 1, 1)); // 색상 설정
	drawCube(); // 큐브 그리기
}

void
scalingExample()
{
	float		s = 1.0f + 0.95f * sin(frame * 0.1f / period); // 스케일링 비율 계산

	glScalef(s, 1, 1); // 스케일링 적용

	setDiffuseColor(vec3(1, 1, 1)); // 색상 설정
	drawCube(); // 큐브 그리기
}

void
scalingPivotExample()
{
	float		s = 1.0f + 0.95f * sin(frame * 0.1f / period); // 스케일링 비율 계산
	vec3		pivot(0.5, 0.5, 0.5); // 피벗 포인트 설정

	glTranslatef(pivot.x, pivot.y, pivot.z); // 피벗 위치로 이동
	glScalef(s, 1, 1); // 스케일링 적용
	glTranslatef(-pivot.x, -pivot.y, -pivot.z); // 원래 위치로 이동

	setDiffuseColor(vec3(1, 1, 1)); // 색상 설정
	drawCube(); // 큐브 그리기
}

void
scalingDirectionExample()
{
	float		s = 1.0f + 0.95f * sin(frame * 0.1f / period); // 스케일링 비율 계산
	vec3		direction(1, 1, 0); // 방향 설정
	direction = normalize(direction); // 방향 벡터 정규화

	vec3		axis = cross(vec3(1, 0, 0), direction); // 회전 축 계산
	float		sinTheta = length(axis); // 축의 길이 (sin 값)
	float		cosTheta = dot(vec3(1, 0, 0), direction); // cos 값
	float		theta = (float)atan2(sinTheta, cosTheta) * float(180.0 / M_PI); // 각도 계산
	axis /= sinTheta; // 축 정규화

	glRotatef(theta, axis.x, axis.y, axis.z); // 회전 적용
	glScalef(s, 1, 1); // 스케일링 적용
	glRotatef(-theta, axis.x, axis.y, axis.z); // 회전 원복

	setDiffuseColor(vec3(1, 1, 1)); // 색상 설정
	drawCube(); // 큐브 그리기
}
void
solarSystemExample()
{
	// 행성의 회전 각도 및 궤도 반경 설정
	float	theta1 = frame * 4.0f / period + 10; // 첫 번째 행성 회전 각도
	float	orbit1 = 1.2f; // 첫 번째 행성 궤도 반경
	float	radius1 = 0.2f; // 첫 번째 행성 크기

	float	theta2 = frame * 3.0f / period + 120; // 두 번째 행성 회전 각도
	float	orbit2 = 2.0f; // 두 번째 행성 궤도 반경
	float	radius2 = 0.2f; // 두 번째 행성 크기

	float	theta21 = frame * 9.0f / period + 20; // 첫 번째 위성 회전 각도
	float	orbit21 = 0.5f; // 첫 번째 위성 궤도 반경
	float	radius21 = 0.1f; // 첫 번째 위성 크기

	drawSphere(0.5, vec3(0.95, 0.95, 0.15)); // 태양 그리기

	// 첫 번째 행성 그리기
	glPushMatrix();
	glRotatef(theta1, 0, 1, 0); // 행성 회전

	glPushMatrix();
	glTranslatef(0, 0, orbit1); // 궤도로 이동
	drawSphere(radius1, vec3(0.15, 0.95, 0.15)); // 행성 그리기
	glPopMatrix();

	drawCylinder(radius1 / 4, orbit1, vec3(0.95, 0.95, 0.95)); // 궤도 표시
	glPopMatrix();

	glPushMatrix();
	glRotatef(-90, 1, 0, 0); // 디스크 회전
	drawDisk(orbit1 - radius1 / 4, orbit1 + radius1 / 4, vec3(0, 1, 0)); // 궤도 디스크 그리기
	glPopMatrix();

	// 두 번째 행성 그리기
	glPushMatrix();
	glRotatef(theta2, 0, 1, 0); // 행성 회전

	glPushMatrix();
	glTranslatef(0, 0, orbit2); // 궤도로 이동
	drawSphere(radius2, vec3(0.15, 0.15, 0.95)); // 행성 그리기

	glPushMatrix();
	glRotatef(theta21, 0, 1, 0); // 위성 회전

	glPushMatrix();
	glTranslatef(0, 0, orbit21); // 궤도로 이동
	drawSphere(radius21, vec3(0.95, 0.15, 0.95)); // 위성 그리기
	glPopMatrix();

	drawCylinder(radius21 / 4, orbit21, vec3(0.95, 0.95, 0.95)); // 궤도 표시
	glPopMatrix();

	glPushMatrix();
	glRotatef(-90, 1, 0, 0); // 디스크 회전
	drawDisk(orbit21 - radius21 / 4, orbit21 + radius21 / 4, vec3(1, 0, 1)); // 위성 궤도 디스크 그리기
	glPopMatrix();
	glPopMatrix();

	drawCylinder(radius2 / 4, orbit2, vec3(0.95, 0.95, 0.95)); // 궤도 표시
	glPopMatrix();

	glPushMatrix();
	glRotatef(-90, 1, 0, 0); // 디스크 회전
	drawDisk(orbit2 - radius2 / 4, orbit2 + radius2 / 4, vec3(0, 0, 1)); // 행성 궤도 디스크 그리기
	glPopMatrix();
}

void scalingPivotExampleGLM()
{
	float s = 1.0f + 0.95f * sin(frame * 0.1f / period); // 스케일링 비율 계산
	vec3 pivot(0.5, 0.5, 0.5); // 피벗 포인트 설정

	// 초기화된 단위 행렬을 생성
	mat4 M(1.0);

	// 피벗 점으로 이동
	M = translate(M, pivot);

	// 스케일 적용
	M = scale(M, vec3(s, 1, 1));

	// 원래 위치로 되돌림
	M = translate(M, -pivot);

	// GL에 변환 행렬 적용
	glMultMatrixf(value_ptr(M));

	setDiffuseColor(vec3(1, 1, 1)); // 색상 설정
	drawCube(); // 큐브 그리기
}
void scalingDirectionExampleGLM()
{
	// 스케일링 비율 계산
	float s = 1.0f + 0.95f * sin(frame * 0.1f / period);
	vec3 direction(1, 1, 0); // 방향 벡터 설정
	direction = normalize(direction); // 방향 벡터 정규화

	// 회전 축 계산
	vec3 axis = cross(vec3(1, 0, 0), direction);
	float sinTheta = length(axis); // sin 값 계산
	float cosTheta = dot(vec3(1, 0, 0), direction); // cos 값 계산
	float theta = atan2(sinTheta, cosTheta); // 회전 각도 계산

	// GLM을 이용하여 변환 행렬 생성
	mat4 M(1.0); // 단위 행렬 생성

	// 축을 기준으로 회전
	M = rotate(M, theta, axis); // 회전 적용

	// 스케일 적용
	M = scale(M, vec3(s, 1, 1)); // 스케일링 적용

	// 원래 위치로 되돌리기 위해 역회전 적용
	M = rotate(M, -theta, axis); // 역회전 적용

	// GL에 변환 행렬 적용
	glMultMatrixf(value_ptr(M));

	setDiffuseColor(vec3(1, 1, 1)); // 색상 설정
	drawCube(); // 큐브 그리기
}

void
render(GLFWwindow* window)
{
	// 배경 색상 설정 및 화면 지우기
	glClearColor(bgColor[0], bgColor[1], bgColor[2], bgColor[3]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW); // 모델뷰 행렬 모드 설정
	glLoadIdentity(); // 행렬 초기화

	// 카메라 설정
	gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], up[0], up[1], up[2]);

	glDisable(GL_LIGHTING); // 조명 비활성화
	drawAxes(AXIS_LENGTH, AXIS_LINE_WIDTH * dpiScaling); // 축 그리기

	setupLight(); // 조명 설정
	setupMaterial(); // 재질 설정

	// 선택된 예시에 따라 다른 함수 호출
	switch (selection)
	{
	case 1:	rotationExample();				break;
	case 2: rotationPivotExample();			break;
	case 3: rotationPivotExampleGLM();		break;
	case 4: scalingExample();				break;
	case 5:	scalingPivotExample();			break;
	case 6:	scalingDirectionExample();		break;
	case 7:	solarSystemExample();			break;
	case 8: scalingPivotExampleGLM();		break;
	case 9: scalingDirectionExampleGLM();	break;
	}
}

void
keyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// 키 입력 처리
	if (action == GLFW_PRESS || action == GLFW_REPEAT)
	{
		switch (key)
		{
		case GLFW_KEY_Q:
		case GLFW_KEY_ESCAPE:	glfwSetWindowShouldClose(window, GL_TRUE); break; // 종료

		case GLFW_KEY_1:	selection = 1; break; // 선택 1
		case GLFW_KEY_2:	selection = 2; break; // 선택 2
		case GLFW_KEY_3:	selection = 3; break; // 선택 3
		case GLFW_KEY_4:	selection = 4; break; // 선택 4
		case GLFW_KEY_5:	selection = 5; break; // 선택 5
		case GLFW_KEY_6:	selection = 6; break; // 선택 6
		case GLFW_KEY_7:	selection = 7; break; // 선택 7
		case GLFW_KEY_8:	selection = 8; break; // 선택 8
		case GLFW_KEY_9:	selection = 9; break; // 선택 9
		case GLFW_KEY_SPACE:	pause = !pause; break; // 일시 정지/재개
		}
	}
}
