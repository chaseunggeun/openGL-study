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

#include <fstream>
#include <iostream>
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

// �ʱ�ȭ �� ���� �Լ� ����
void	init();
void	quit();
void	render(GLFWwindow* window);
void	keyboard(GLFWwindow* window, int key, int code, int action, int mods);

// ���� �ҷ�����
bool	readMesh(const char* filename);
void	deleteMesh();

// ī�޶� ��ġ �� ���� ����
vec3	eye(3.5, 3, 3.5);
vec3	center(0, 0, 0);
vec3	up(0, 1, 0);

// ���� ��ġ ����
vec4	light(5.0, 5.0, 5.0, 1);

// �� ���� �� �� �β� ����
float	AXIS_LENGTH = 3;
float	AXIS_LINE_WIDTH = 2;

// ��� ���� ����
GLfloat bgColor[4] = { 1,1,1,1 };

// ���õ� ���� ��ȣ
int selection = 1;

// ��, �����, ��ũ�� ���� ���帯 ��ü ������
GLUquadricObj* sphere = NULL;
GLUquadricObj* cylinder = NULL;
GLUquadricObj* disk = NULL;

// �Ͻ� ���� ���� ����
bool	pause = true;

// �ð� ���� ���� ����
float	timeStep = 1.0f / 120;
float	theta = 0;

// ȸ�� ���� ����
float	period = 4.0;

// ������ ī����
int frame = 0;

int
main(int argc, char* argv[])
{
	vsync = 0; // ���� ����ȭ ����

	// OpenGL �ʱ�ȭ
	GLFWwindow* window = initializeOpenGL(argc, argv, bgColor);
	if (window == NULL) return -1; // â �ʱ�ȭ ���� �� ����

	glfwSetKeyCallback(window, keyboard); // Ű �Է� �ݹ� ����

	glEnable(GL_DEPTH_TEST); // ���� �׽�Ʈ Ȱ��ȭ

	glEnable(GL_NORMALIZE); // ��� ���� ����ȭ Ȱ��ȭ

	reshape(window, windowW, windowH); // â ũ�� ����

	init(); // �ʱ�ȭ �Լ� ȣ��

	float	previous = (float)glfwGetTime(); // ���� �ð� ����
	float	elapsed = 0; // ��� �ð� �ʱ�ȭ
	while (!glfwWindowShouldClose(window)) // â�� ���� ������ �ݺ�
	{
		glfwPollEvents(); // �̺�Ʈ ó��

		float	now = (float)glfwGetTime(); // ���� �ð� ��������
		float	delta = now - previous; // �ð� �� ���
		previous = now; // ���� �ð� ������Ʈ

		elapsed += delta; // ��� �ð� ����

		if (elapsed > timeStep) // ���� �ð� �̻� ��� ��
		{
			if (!pause) {
				frame += 1; // �Ͻ� ���� ���°� �ƴ� ��� ������ ����
				theta += float(2.0 * M_PI) / period * elapsed;
			}

			elapsed = 0; // ��� �ð� �ʱ�ȭ
		}

		render(window); // ������ �Լ� ȣ��
		glfwSwapBuffers(window); // ���� ����
	}

	quit(); // ���� �Լ� ȣ��

	glfwDestroyWindow(window); // â �ı�
	glfwTerminate(); // GLFW ����

	return 0; // ���α׷� ����
}

void
init()
{
	// �� ��ü �ʱ�ȭ
	sphere = gluNewQuadric();
	gluQuadricDrawStyle(sphere, GLU_FILL); // ä��� ��Ÿ�� ����
	gluQuadricNormals(sphere, GLU_SMOOTH); // �ε巯�� ��� ����
	gluQuadricOrientation(sphere, GLU_OUTSIDE); // �ܺ� ���� ����
	gluQuadricTexture(sphere, GL_FALSE); // �ؽ�ó ��� �� ��

	// ����� ��ü �ʱ�ȭ
	cylinder = gluNewQuadric();
	gluQuadricDrawStyle(cylinder, GLU_FILL);
	gluQuadricNormals(cylinder, GLU_SMOOTH);
	gluQuadricOrientation(cylinder, GLU_OUTSIDE);
	gluQuadricTexture(cylinder, GL_FALSE);

	// ��ũ ��ü �ʱ�ȭ
	disk = gluNewQuadric();
	gluQuadricDrawStyle(disk, GLU_FILL);
	gluQuadricNormals(disk, GLU_SMOOTH);
	gluQuadricOrientation(disk, GLU_OUTSIDE);
	gluQuadricTexture(disk, GL_FALSE);

	readMesh(".\\m01_bunny.off");

	// Ű���� �Է� �ȳ� �޽��� ���
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
	// ���帯 ��ü �޸� ����
	gluDeleteQuadric(sphere);
	gluDeleteQuadric(cylinder);
	gluDeleteQuadric(disk);
}

void
setupLight()
{
	// ���� Ȱ��ȭ
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// ���� �Ӽ� ����
	GLfloat	ambient[4] = { 0.1f, 0.1f, 0.1f, 1.0f }; // �ֺ���
	GLfloat	diffuse[4] = { 1.0f, 1.0f, 1.0f, 1.0f }; // Ȯ�걤
	GLfloat	specular[4] = { 0.1f, 0.1f, 0.1f, 1.0f }; // �ݻ籤

	// ���� �Ӽ� ����
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_POSITION, value_ptr(light)); // ���� ��ġ ����
}

void
setupMaterial()
{
	// ���� �Ӽ� ����
	GLfloat	mat_ambient[4] = { 0.1f, 0.1f, 0.1f, 1.0f }; // �ֺ���
	GLfloat mat_specular[4] = { 0.5f, 0.5f, 0.5f, 1.0f }; // �ݻ籤
	GLfloat mat_shininess = 128; // ���õ�

	// ���� �Ӽ� ����
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
}

void
setDiffuseColor(const vec3& color)
{
	// Ȯ�걤 ���� ����
	GLfloat	mat_diffuse[4] = { color[0], color[1], color[2], 1 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
}

void
drawSphere(float radius, const vec3& color)
{
	// �� �׸���
	setDiffuseColor(color); // ���� ����
	gluSphere(sphere, radius, 72, 72); // �� ����
}

void
drawCylinder(float radius, float height, const vec3& color)
{
	// ����� �׸���
	setDiffuseColor(color); // ���� ����
	gluCylinder(cylinder, radius, radius, height, 72, 5); // ����� ����
}

void
drawDisk(float inner_radius, float outer_radius, const vec3& color)
{
	// ��ũ �׸���
	setDiffuseColor(color); // ���� ����
	gluDisk(disk, inner_radius, outer_radius, 72, 5); // ��ũ ����
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
drawCube()
{
	// ť�� �׸���
	glBegin(GL_QUADS); // �簢�� �׸��� ����

	// �ո�
	glNormal3f(0, 0, 1); // ���� ����
	glVertex3f(0, 0, 1);
	glVertex3f(1, 0, 1);
	glVertex3f(1, 1, 1);
	glVertex3f(0, 1, 1);

	// �޸�
	glNormal3f(0, 0, -1);
	glVertex3f(0, 1, 0);
	glVertex3f(1, 1, 0);
	glVertex3f(1, 0, 0);
	glVertex3f(0, 0, 0);

	// ������ ��
	glNormal3f(1, 0, 0);
	glVertex3f(1, 0, 1);
	glVertex3f(1, 0, 0);
	glVertex3f(1, 1, 0);
	glVertex3f(1, 1, 1);

	// ���� ��
	glNormal3f(-1, 0, 0);
	glVertex3f(0, 1, 1);
	glVertex3f(0, 1, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 1);

	// ���� ��
	glNormal3f(0, 1, 0);
	glVertex3f(0, 1, 1);
	glVertex3f(1, 1, 1);
	glVertex3f(1, 1, 0);
	glVertex3f(0, 1, 0);

	// �Ʒ��� ��
	glNormal3f(0, -1, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(1, 0, 0);
	glVertex3f(1, 0, 1);
	glVertex3f(0, 0, 1);

	glEnd(); // �簢�� �׸��� ����
}
void
rotationExample()
{
	float		theta1 = frame * 4 / period; // ȸ�� ���� ���
	vec3		axis(0, 0, 1); // ȸ�� �� ����

	glRotatef(theta1, axis.x, axis.y, axis.z); // ȸ�� ����

	setDiffuseColor(vec3(1, 1, 1)); // ���� ����
	drawCube(); // ť�� �׸���
}

void
rotationPivotExample()
{
	float		theta1 = frame * 4 / period; // ȸ�� ���� ���
	vec3		axis(0, 0, 1); // ȸ�� �� ����
	vec3		pivot(0.5, 0.5, 0); // �ǹ� ����Ʈ ����

	glTranslatef(pivot.x, pivot.y, pivot.z); // �ǹ� ��ġ�� �̵�
	glRotatef(theta1, axis.x, axis.y, axis.z); // ȸ�� ����
	glTranslatef(-pivot.x, -pivot.y, -pivot.z); // ���� ��ġ�� �̵�

	setDiffuseColor(vec3(1, 1, 1)); // ���� ����
	drawCube(); // ť�� �׸���
}

void
rotationPivotExampleGLM()
{
	float		theta1 = frame * 4 / period; // ȸ�� ���� ���
	vec3		axis(0, 0, 1); // ȸ�� �� ����
	vec3		pivot(0.5, 0.5, 0); // �ǹ� ����Ʈ ����

	mat4		M(1.0); // ���� ��� ����

	M = translate(M, pivot); // �ǹ� ��ġ�� ��ȯ �߰�

	M = rotate(M, radians(theta1), axis); // ȸ�� �߰�

	M = translate(M, -pivot); // ���� ��ġ�� ��ȯ �߰�

	glMultMatrixf(value_ptr(M)); // ��ȯ ��� ����

	setDiffuseColor(vec3(1, 1, 1)); // ���� ����
	drawCube(); // ť�� �׸���
}


void
scalingExample()
{
	float		s = 1.0f + 0.95f * sin(frame * 0.1f / period); // �����ϸ� ���� ���

	glScalef(s, 1, 1); // �����ϸ� ����

	setDiffuseColor(vec3(1, 1, 1)); // ���� ����
	drawCube(); // ť�� �׸���
}

void
scalingPivotExample()
{
	float		s = 1.0f + 0.95f * sin(frame * 0.1f / period); // �����ϸ� ���� ���
	vec3		pivot(0.5, 0.5, 0.5); // �ǹ� ����Ʈ ����

	glTranslatef(pivot.x, pivot.y, pivot.z); // �ǹ� ��ġ�� �̵�
	glScalef(s, 1, 1); // �����ϸ� ����
	glTranslatef(-pivot.x, -pivot.y, -pivot.z); // ���� ��ġ�� �̵�

	setDiffuseColor(vec3(1, 1, 1)); // ���� ����
	drawCube(); // ť�� �׸���
}

void
scalingDirectionExample()
{
	float		s = 1.0f + 0.95f * sin(frame * 0.1f / period); // �����ϸ� ���� ���
	vec3		direction(1, 1, 0); // ���� ����
	direction = normalize(direction); // ���� ���� ����ȭ

	vec3		axis = cross(vec3(1, 0, 0), direction); // ȸ�� �� ���
	float		sinTheta = length(axis); // ���� ���� (sin ��)
	float		cosTheta = dot(vec3(1, 0, 0), direction); // cos ��
	float		theta = (float)atan2(sinTheta, cosTheta) * float(180.0 / M_PI); // ���� ���
	axis /= sinTheta; // �� ����ȭ

	glRotatef(theta, axis.x, axis.y, axis.z); // ȸ�� ����
	glScalef(s, 1, 1); // �����ϸ� ����
	glRotatef(-theta, axis.x, axis.y, axis.z); // ȸ�� ����

	setDiffuseColor(vec3(1, 1, 1)); // ���� ����
	drawCube(); // ť�� �׸���
}


void
solarSystemExample()
{
	// �༺�� ȸ�� ���� �� �˵� �ݰ� ����
	float	theta1 = frame * 4.0f / period + 10; // ù ��° �༺ ȸ�� ����
	float	orbit1 = 1.2f; // ù ��° �༺ �˵� �ݰ�
	float	radius1 = 0.2f; // ù ��° �༺ ũ��

	float	theta2 = frame * 3.0f / period + 120; // �� ��° �༺ ȸ�� ����
	float	orbit2 = 2.0f; // �� ��° �༺ �˵� �ݰ�
	float	radius2 = 0.2f; // �� ��° �༺ ũ��

	float	theta21 = frame * 9.0f / period + 20; // ù ��° ���� ȸ�� ����
	float	orbit21 = 0.5f; // ù ��° ���� �˵� �ݰ�
	float	radius21 = 0.1f; // ù ��° ���� ũ��

	drawSphere(0.5, vec3(0.95, 0.95, 0.15)); // �¾� �׸���

	// ù ��° �༺ �׸���
	glPushMatrix();
	glRotatef(theta1, 0, 1, 0); // �༺ ȸ��

	glPushMatrix();
	glTranslatef(0, 0, orbit1); // �˵��� �̵�
	drawSphere(radius1, vec3(0.15, 0.95, 0.15)); // �༺ �׸���
	glPopMatrix();

	drawCylinder(radius1 / 4, orbit1, vec3(0.95, 0.95, 0.95)); // �˵� ǥ��
	glPopMatrix();

	glPushMatrix();
	glRotatef(-90, 1, 0, 0); // ��ũ ȸ��
	drawDisk(orbit1 - radius1 / 4, orbit1 + radius1 / 4, vec3(0, 1, 0)); // �˵� ��ũ �׸���
	glPopMatrix();

	// �� ��° �༺ �׸���
	glPushMatrix();
	glRotatef(theta2, 0, 1, 0); // �༺ ȸ��

	glPushMatrix();
	glTranslatef(0, 0, orbit2); // �˵��� �̵�
	drawSphere(radius2, vec3(0.15, 0.15, 0.95)); // �༺ �׸���

	glPushMatrix();
	glRotatef(theta21, 0, 1, 0); // ���� ȸ��

	glPushMatrix();
	glTranslatef(0, 0, orbit21); // �˵��� �̵�
	drawSphere(radius21, vec3(0.95, 0.15, 0.95)); // ���� �׸���
	glPopMatrix();

	drawCylinder(radius21 / 4, orbit21, vec3(0.95, 0.95, 0.95)); // �˵� ǥ��
	glPopMatrix();

	glPushMatrix();
	glRotatef(-90, 1, 0, 0); // ��ũ ȸ��
	drawDisk(orbit21 - radius21 / 4, orbit21 + radius21 / 4, vec3(1, 0, 1)); // ���� �˵� ��ũ �׸���
	glPopMatrix();
	glPopMatrix();

	drawCylinder(radius2 / 4, orbit2, vec3(0.95, 0.95, 0.95)); // �˵� ǥ��
	glPopMatrix();

	glPushMatrix();
	glRotatef(-90, 1, 0, 0); // ��ũ ȸ��
	drawDisk(orbit2 - radius2 / 4, orbit2 + radius2 / 4, vec3(0, 0, 1)); // �༺ �˵� ��ũ �׸���
	glPopMatrix();
}

void scalingPivotExampleGLM()
{
	float s = 1.0f + 0.95f * sin(frame * 0.1f / period); // �����ϸ� ���� ���
	vec3 pivot(0.5, 0.5, 0.5); // �ǹ� ����Ʈ ����

	// �ʱ�ȭ�� ���� ����� ����
	mat4 M(1.0);

	// �ǹ� ������ �̵�
	M = translate(M, pivot);

	// ������ ����
	M = scale(M, vec3(s, 1, 1));

	// ���� ��ġ�� �ǵ���
	M = translate(M, -pivot);

	// GL�� ��ȯ ��� ����
	glMultMatrixf(value_ptr(M));

	setDiffuseColor(vec3(1, 1, 1)); // ���� ����
	drawCube(); // ť�� �׸���
}
void scalingDirectionExampleGLM()
{
	// �����ϸ� ���� ���
	float s = 1.0f + 0.95f * sin(frame * 0.1f / period);
	vec3 direction(1, 1, 0); // ���� ���� ����
	direction = normalize(direction); // ���� ���� ����ȭ

	// ȸ�� �� ���
	vec3 axis = cross(vec3(1, 0, 0), direction);
	float sinTheta = length(axis); // sin �� ���
	float cosTheta = dot(vec3(1, 0, 0), direction); // cos �� ���
	float theta = atan2(sinTheta, cosTheta); // ȸ�� ���� ���

	// GLM�� �̿��Ͽ� ��ȯ ��� ����
	mat4 M(1.0); // ���� ��� ����

	// ���� �������� ȸ��
	M = rotate(M, theta, axis); // ȸ�� ����

	// ������ ����
	M = scale(M, vec3(s, 1, 1)); // �����ϸ� ����

	// ���� ��ġ�� �ǵ����� ���� ��ȸ�� ����
	M = rotate(M, -theta, axis); // ��ȸ�� ����

	// GL�� ��ȯ ��� ����
	glMultMatrixf(value_ptr(M));

	setDiffuseColor(vec3(1, 1, 1)); // ���� ����
	drawCube(); // ť�� �׸���
}

void scalingDirectionExampleGLM2()
{
	// �����ϸ� ���� ���
	float s = 1.0f + 0.95f * sin(frame * 0.1f / period);
	vec3 direction(1, 1, 0); // ���� ���� ����
	direction = normalize(direction); // ���� ���� ����ȭ

	// ȸ�� �� ���
	vec3 axis = cross(vec3(1, 0, 0), direction);
	float sinTheta = length(axis); // sin �� ���
	float cosTheta = dot(vec3(1, 0, 0), direction); // cos �� ���
	float theta = atan2(sinTheta, cosTheta); // ȸ�� ���� ���

	// GLM�� �̿��Ͽ� ��ȯ ��� ����
	mat4 M(1.0); // ���� ��� ����

	// ���� �������� ȸ��
	M = rotate(M, theta, axis); // ȸ�� ����

	// ������ ����
	M = scale(M, vec3(s, 1, 1)); // �����ϸ� ����

	// ���� ��ġ�� �ǵ����� ���� ��ȸ�� ����
	M = rotate(M, -theta, axis); // ��ȸ�� ����

	// GL�� ��ȯ ��� ����
	glMultMatrixf(value_ptr(M));

	setDiffuseColor(vec3(1, 1, 1)); // ���� ����
	drawEdges(); // ť�� �׸���
}

void
render(GLFWwindow* window)
{
	// ��� ���� ���� �� ȭ�� �����
	glClearColor(bgColor[0], bgColor[1], bgColor[2], bgColor[3]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW); // �𵨺� ��� ��� ����
	glLoadIdentity(); // ��� �ʱ�ȭ

	// ī�޶� ����
	gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], up[0], up[1], up[2]);

	glDisable(GL_LIGHTING); // ���� ��Ȱ��ȭ
	drawAxes(AXIS_LENGTH, AXIS_LINE_WIDTH * dpiScaling); // �� �׸���

	setupLight(); // ���� ����
	setupMaterial(); // ���� ����

	// ���õ� ���ÿ� ���� �ٸ� �Լ� ȣ��
	glRotatef(degrees(theta), 0, 1, 0); // Y���� �������� ȸ��

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
	case 10: drawMesh();	break;
	case 11: scalingDirectionExampleGLM2();	break;
	}
}

void
keyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Ű �Է� ó��
	if (action == GLFW_PRESS || action == GLFW_REPEAT)
	{
		switch (key)
		{
		case GLFW_KEY_Q:
		case GLFW_KEY_ESCAPE:	glfwSetWindowShouldClose(window, GL_TRUE); break; // ����

		case GLFW_KEY_1:	selection = 1; break; // ���� 1
		case GLFW_KEY_2:	selection = 2; break; // ���� 2
		case GLFW_KEY_3:	selection = 3; break; // ���� 3
		case GLFW_KEY_4:	selection = 4; break; // ���� 4
		case GLFW_KEY_5:	selection = 5; break; // ���� 5
		case GLFW_KEY_6:	selection = 6; break; // ���� 6
		case GLFW_KEY_7:	selection = 7; break; // ���� 7
		case GLFW_KEY_8:	selection = 8; break; // ���� 8
		case GLFW_KEY_9:	selection = 9; break; // ���� 9
		case GLFW_KEY_Z:	selection = 10; break; // ���� 9
		case GLFW_KEY_X:	selection = 11; break; // ���� 9
		case GLFW_KEY_SPACE:	pause = !pause; break; // �Ͻ� ����/�簳
		}
	}
}