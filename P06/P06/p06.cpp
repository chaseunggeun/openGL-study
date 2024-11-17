#define GLM_ENABLE_EXPERIMENTAL
#include "glSetup.h" // OpenGL 설정 헤더 파일 포함

#ifdef _WIN32
#define _USE_MATH_DEFINES // 윈도우에서 수학 상수 사용
#endif

#include <glm/glm.hpp> // GLM 라이브러리 포함
#include <glm/gtx/string_cast.hpp> // GLM 문자열 변환 포함
#include <glm/gtc/type_ptr.hpp> // GLM 타입 포인터 포함
using namespace glm; // glm 네임스페이스 사용

#include <iostream> // 입출력 스트림 포함
using namespace std; // std 네임스페이스 사용

// 함수 선언
void	init();
void	quit();
void	render(GLFWwindow* window);
void	keyboard(GLFWwindow* window, int key, int code, int action, int mods);

// 카메라 및 조명 초기화
vec3	eye(3.5, 3, 3.5); // 카메라 위치
vec3	center(0, 0, 0); // 카메라가 바라보는 중심
vec3	up(0, 1, 0); // 카메라의 업 벡터

vec4	light(5.0, 5.0, 5.0, 1); // 조명 위치

float	AXIS_LENGTH = 3; // 축 길이
float	AXIS_LINE_WIDTH = 2; // 축 선 너비

GLfloat bgColor[4] = { 1,1,1,1 }; // 배경 색상 (흰색)

int selection = 1; // 선택된 도형 인덱스

// GLUquadricObj 객체 포인터 선언
GLUquadricObj* sphere = NULL;
GLUquadricObj* cylinder = NULL;
GLUquadricObj* disk = NULL;

bool	pause = true; // 일시정지 상태

float	timeStep = 1.0f / 120; // 시간 간격
float	period = 4.0; // 주기

int frame = 0; // 프레임 카운터

int
main(int argc, char* argv[])
{
	vsync = 0; // 수직 동기화 비활성화

	// OpenGL 초기화
	GLFWwindow* window = initializeOpenGL(argc, argv, bgColor);
	if (window == NULL) return -1; // 초기화 실패 시 종료

	glfwSetKeyCallback(window, keyboard); // 키 입력 콜백 설정

	glEnable(GL_DEPTH_TEST); // 깊이 테스트 활성화
	glEnable(GL_NORMALIZE); // 법선 정규화 활성화

	reshape(window, windowW, windowH); // 창 크기 조정

	init(); // 초기화 함수 호출

	float	previous = (float)glfwGetTime(); // 이전 시간 저장
	float	elapsed = 0; // 경과 시간 초기화
	while (!glfwWindowShouldClose(window)) // 창이 닫힐 때까지 반복
	{
		glfwPollEvents(); // 이벤트 처리

		float	now = (float)glfwGetTime(); // 현재 시간 가져오기
		float	delta = now - previous; // 경과 시간 계산
		previous = now; // 이전 시간을 현재 시간으로 업데이트

		elapsed += delta; // 경과 시간 누적

		if (elapsed > timeStep) // 지정한 시간 간격이 경과한 경우
		{
			if (!pause) frame += 1; // 일시정지 중이 아니면 프레임 증가

			elapsed = 0; // 경과 시간 초기화
		}

		render(window); // 렌더링 함수 호출
		glfwSwapBuffers(window); // 버퍼 교체
	}

	quit(); // 종료 함수 호출

	glfwDestroyWindow(window); // 창 삭제
	glfwTerminate(); // GLFW 종료

	return 0; // 프로그램 종료
}

void
init()
{
	sphere = gluNewQuadric(); // 새로운 구체 생성
	gluQuadricDrawStyle(sphere, GLU_FILL); // 그리기 스타일 설정
	gluQuadricNormals(sphere, GLU_SMOOTH); // 법선 부드럽게 설정
	gluQuadricOrientation(sphere, GLU_OUTSIDE); // 외부 방향 설정
	gluQuadricTexture(sphere, GL_FALSE); // 텍스처 사용 안 함

	cylinder = gluNewQuadric(); // 새로운 원기둥 생성
	gluQuadricDrawStyle(cylinder, GLU_FILL); // 그리기 스타일 설정
	gluQuadricNormals(cylinder, GLU_SMOOTH); // 법선 부드럽게 설정
	gluQuadricOrientation(cylinder, GLU_OUTSIDE); // 외부 방향 설정
	gluQuadricTexture(cylinder, GL_FALSE); // 텍스처 사용 안 함

	disk = gluNewQuadric(); // 새로운 원반 생성
	gluQuadricDrawStyle(disk, GLU_FILL); // 그리기 스타일 설정
	gluQuadricNormals(disk, GLU_SMOOTH); // 법선 부드럽게 설정
	gluQuadricOrientation(disk, GLU_OUTSIDE); // 외부 방향 설정
	gluQuadricTexture(disk, GL_FALSE); // 텍스처 사용 안 함

	cout << endl;
	cout << "Keyboard Input: space for play/pause" << endl; // 키 입력 안내

	cout << "Keyboard Input: 1 for rotation example" << endl; // 키 입력 안내
	cout << "Keyboard Input: 2 for rotation wrt a pivot example" << endl; // 키 입력 안내
	cout << "Keyboard Input: 3 for rotation wrt a pivot example using GLM" << endl; // 키 입력 안내
	cout << "Keyboard Input: 4 for scaling example" << endl; // 키 입력 안내
	cout << "Keyboard Input: 5 for scaling wrt a pivot example" << endl; // 키 입력 안내
	cout << "Keyboard Input: 6 for scaling wrt a direction example" << endl; // 키 입력 안내
	cout << "Keyboard Input: 7 for a solar system example" << endl; // 키 입력 안내
	cout << "Keyboard Input: 8 scalingPivotExampleGLM" << endl; // 키 입력 안내
	cout << "Keyboard Input: 9 scalingDirectionExampleGLM" << endl; // 키 입력 안내
}

void
quit()
{
	gluDeleteQuadric(sphere); // 구체 객체 삭제
	gluDeleteQuadric(cylinder); // 원기둥 객체 삭제
	gluDeleteQuadric(disk); // 원반 객체 삭제
}

void
setupLight()
{
	glEnable(GL_LIGHTING); // 조명 활성화
	glEnable(GL_LIGHT0); // 첫 번째 조명 활성화

	// 조명 속성 설정
	GLfloat	ambient[4] = { 0.1f, 0.1f, 0.1f, 1.0f }; // 주변광
	GLfloat	diffuse[4] = { 1.0f, 1.0f, 1.0f, 1.0f }; // 확산광
	GLfloat	specular[4] = { 0.1f, 0.1f, 0.1f, 1.0f }; // 반사광

	// 조명 속성 적용
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_POSITION, value_ptr(light)); // 조명 위치
}

void
setupMaterial()
{
	// 재질 속성 설정
	GLfloat	mat_ambient[4] = { 0.1f, 0.1f, 0.1f, 1.0f }; // 주변광
	GLfloat mat_specular[4] = { 0.5f, 0.5f, 0.5f, 1.0f }; // 반사광
	GLfloat mat_shininess = 128; // 광택

	// 재질 속성 적용
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
}

void
setDiffuseColor(const vec3& color)
{
	GLfloat	mat_diffuse[4] = { color[0], color[1], color[2], 1 }; // 확산 색상 설정
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse); // 재질 속성 적용
}

void
drawSphere(float radius, const vec3& color)
{
	setDiffuseColor(color); // 색상 설정

	gluSphere(sphere, radius, 72, 72); // 구체 그리기
}

void
drawCylinder(float radius, float height, const vec3& color)
{
	setDiffuseColor(color); // 색상 설정

	gluCylinder(cylinder, radius, radius, height, 72, 5); // 원기둥 그리기
}

void
drawDisk(float inner_radius, float outer_radius, const vec3& color)
{
	setDiffuseColor(color); // 색상 설정

	gluDisk(disk, inner_radius, outer_radius, 72, 5); // 원반 그리기
}
