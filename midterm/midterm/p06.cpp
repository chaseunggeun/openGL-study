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
