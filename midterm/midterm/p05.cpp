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
	setDiffuseColor(color); // �־��� �������� Ȯ�� ���� ����

	gluDisk(disk, inner_radius, outer_radius, 72, 5); // �־��� ���������� ���� �׸���
}

int		nVertices = 0, nFaces = 0, nEdges = 0; // ����, ��, ������ ���� ������ ����
vec3* vertex = NULL; // ���� �迭 ������
int* face[3] = { NULL, NULL, NULL }; // �� �迭 ������

int edgeCount = 0; // ���� ���� ī����
void addEdge(int v1, int v2) {
	if (v1 > v2) std::swap(v1, v2); // v1�� v2���� ũ�� �� ���� ��ȯ

	if (!edgeExists(v1, v2)) { // ������ �������� ������
		edgeList.emplace_back(v1, v2); // ������ ���� ����Ʈ�� �߰�
		edgeCount++; // ���� ���� ����
	}
}

bool readMesh(const char* filename) {
	ifstream is(filename); // ���� �б� ��Ʈ�� ����
	if (is.fail()) return false; // ���� ���� ���� �� false ��ȯ

	char magicNumber[256];
	is >> magicNumber; // ���� �ѹ� �б�

	is >> nVertices >> nFaces >> nEdges; // ����, ��, ���� ���� �б�
	cout << "# vertices = " << nVertices << endl; // ���� �� ���
	cout << "# faces = " << nFaces << endl; // �� �� ���

	vertex = new vec3[nVertices]; // ���� �迭 ���� �Ҵ�
	for (int i = 0; i < nVertices; i++) {
		is >> vertex[i].x >> vertex[i].y >> vertex[i].z; // ���� ��ǥ �б�
	}

	face[0] = new int[nFaces]; // �� �迭 ���� �Ҵ�
	face[1] = new int[nFaces]; // �� �迭 ���� �Ҵ�
	face[2] = new int[nFaces]; // �� �迭 ���� �Ҵ�

	int n; // ���� ���� ��
	for (int i = 0; i < nFaces; i++) {
		int v1, v2, v3; // ���� �����ϴ� ����
		is >> n >> v1 >> v2 >> v3; // �� ���� �б�
		addEdge(v1, v2); // ���� �߰�
		addEdge(v2, v3); // ���� �߰�
		addEdge(v3, v1); // ���� �߰�
	}
	cout << "# edges = " << edgeCount << endl; // ���� �� ���
	return true; // ���� �� true ��ȯ
}

void drawEdges() {
	glBegin(GL_LINES); // �� �׸��� ����
	for (const auto& edge : edgeList) { // ���� ����Ʈ�� ��ȸ
		glVertex3f(vertex[edge.first].x, vertex[edge.first].y, vertex[edge.first].z); // ù ��° ����
		glVertex3f(vertex[edge.second].x, vertex[edge.second].y, vertex[edge.second].z); // �� ��° ����
	}
	glEnd(); // �� �׸��� ����
}

void
deleteMesh()
{
	if (vertex) { delete[] vertex; vertex = NULL; } // ���� �迭 ����
	if (face[0]) { delete[] face[0]; face[0] = NULL; } // �� �迭 ����
	if (face[1]) { delete[] face[1]; face[1] = NULL; } // �� �迭 ����
	if (face[2]) { delete[] face[2]; face[2] = NULL; } // �� �迭 ����
}

void
drawMesh()
{
	setDiffuseColor(vec3(0, 0, 0)); // ���� ����

	glRotatef(90, 0, 0, 1); // Z���� �߽����� 90�� ȸ��
	glRotatef(90, 1, 0, 0); // X���� �߽����� 90�� ȸ��

	glPointSize(3 * dpiScaling); // ����Ʈ ũ�� ����
	glBegin(GL_POINTS); // �� �׸��� ����
	for (int i = 0; i < nVertices; i++)
		glVertex3f(vertex[i].x, vertex[i].y, vertex[i].z); // �� ���� �׸���
	glEnd(); // �� �׸��� ����
}

void
render(GLFWwindow* window)
{
	if (depthTest)	glEnable(GL_DEPTH_TEST); // ���� �׽�Ʈ Ȱ��ȭ
	else			glDisable(GL_DEPTH_TEST); // ���� �׽�Ʈ ��Ȱ��ȭ

	if (polygonFill)	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // �� ä���
	else				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // �� ���

	glClearColor(bgColor[0], bgColor[1], bgColor[2], bgColor[3]); // ���� ����
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // ���� �� ���� ���� �����

	glMatrixMode(GL_MODELVIEW); // �� �� ��� ��� ����
	glLoadIdentity(); // ��� �ʱ�ȭ

	// ī�޶� ��ġ �� ���� ����
	gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], up[0], up[1], up[2]);

	glDisable(GL_LIGHTING); // ���� ��Ȱ��ȭ
	drawAxes(AXIS_LENGTH, AXIS_LINE_WIDTH * dpiScaling); // �� �׸���

	setupLight(); // ���� ����
	setupMaterial(); // ���� ����

	glRotatef(degrees(theta), 0, 1, 0); // Y���� �������� ȸ��

	// ���õ� ���� �׸���
	switch (selection)
	{
	case 1:	drawSphere(1.0, vec3(0.95, 0.95, 0.95));		break; // �� �׸���
	case 2: drawCylinder(1.0, 1.0, vec3(0.95, 0.95, 0.95));	break; // ����� �׸���
	case 3: drawDisk(0.5, 1.0, vec3(0.95, 0.95, 0.95));		break; // ���� �׸���
	case 4: drawMesh();										break; // �޽� �׸���
	case 5: drawEdges();									break; // ���� �׸���
	}
}

void
keyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS || action == GLFW_REPEAT) // Ű �Է� ó��
	{
		switch (key)
		{
		case GLFW_KEY_Q:
		case GLFW_KEY_ESCAPE:	glfwSetWindowShouldClose(window, GL_TRUE); break; // ���� Ű ó��

		case GLFW_KEY_1:	selection = 1; break; // �� ����
		case GLFW_KEY_2:	selection = 2; break; // ����� ����
		case GLFW_KEY_3:	selection = 3; break; // ���� ����
		case GLFW_KEY_4:	selection = 4; break; // �޽� ����
		case GLFW_KEY_5:	selection = 5; break; // ���� ����

		case GLFW_KEY_SPACE:	pause = !pause; break; // �Ͻ�����/�簳

		case GLFW_KEY_D:		depthTest = !depthTest; break; // ���� �׽�Ʈ ���
		case GLFW_KEY_F:		polygonFill = !polygonFill; break; // �� ä��� ���
		}
	}
}
