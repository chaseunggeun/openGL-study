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
