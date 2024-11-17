#define GLM_ENABLE_EXPERIMENTAL
#include "glSetup.h" // OpenGL ���� ��� ���� ����

#ifdef _WIN32
#define _USE_MATH_DEFINES // �����쿡�� ���� ��� ���
#endif

#include <glm/glm.hpp> // GLM ���̺귯�� ����
#include <glm/gtx/string_cast.hpp> // GLM ���ڿ� ��ȯ ����
#include <glm/gtc/type_ptr.hpp> // GLM Ÿ�� ������ ����
using namespace glm; // glm ���ӽ����̽� ���

#include <iostream> // ����� ��Ʈ�� ����
using namespace std; // std ���ӽ����̽� ���

// �Լ� ����
void	init();
void	quit();
void	render(GLFWwindow* window);
void	keyboard(GLFWwindow* window, int key, int code, int action, int mods);

// ī�޶� �� ���� �ʱ�ȭ
vec3	eye(3.5, 3, 3.5); // ī�޶� ��ġ
vec3	center(0, 0, 0); // ī�޶� �ٶ󺸴� �߽�
vec3	up(0, 1, 0); // ī�޶��� �� ����

vec4	light(5.0, 5.0, 5.0, 1); // ���� ��ġ

float	AXIS_LENGTH = 3; // �� ����
float	AXIS_LINE_WIDTH = 2; // �� �� �ʺ�

GLfloat bgColor[4] = { 1,1,1,1 }; // ��� ���� (���)

int selection = 1; // ���õ� ���� �ε���

// GLUquadricObj ��ü ������ ����
GLUquadricObj* sphere = NULL;
GLUquadricObj* cylinder = NULL;
GLUquadricObj* disk = NULL;

bool	pause = true; // �Ͻ����� ����

float	timeStep = 1.0f / 120; // �ð� ����
float	period = 4.0; // �ֱ�

int frame = 0; // ������ ī����

int
main(int argc, char* argv[])
{
	vsync = 0; // ���� ����ȭ ��Ȱ��ȭ

	// OpenGL �ʱ�ȭ
	GLFWwindow* window = initializeOpenGL(argc, argv, bgColor);
	if (window == NULL) return -1; // �ʱ�ȭ ���� �� ����

	glfwSetKeyCallback(window, keyboard); // Ű �Է� �ݹ� ����

	glEnable(GL_DEPTH_TEST); // ���� �׽�Ʈ Ȱ��ȭ
	glEnable(GL_NORMALIZE); // ���� ����ȭ Ȱ��ȭ

	reshape(window, windowW, windowH); // â ũ�� ����

	init(); // �ʱ�ȭ �Լ� ȣ��

	float	previous = (float)glfwGetTime(); // ���� �ð� ����
	float	elapsed = 0; // ��� �ð� �ʱ�ȭ
	while (!glfwWindowShouldClose(window)) // â�� ���� ������ �ݺ�
	{
		glfwPollEvents(); // �̺�Ʈ ó��

		float	now = (float)glfwGetTime(); // ���� �ð� ��������
		float	delta = now - previous; // ��� �ð� ���
		previous = now; // ���� �ð��� ���� �ð����� ������Ʈ

		elapsed += delta; // ��� �ð� ����

		if (elapsed > timeStep) // ������ �ð� ������ ����� ���
		{
			if (!pause) frame += 1; // �Ͻ����� ���� �ƴϸ� ������ ����

			elapsed = 0; // ��� �ð� �ʱ�ȭ
		}

		render(window); // ������ �Լ� ȣ��
		glfwSwapBuffers(window); // ���� ��ü
	}

	quit(); // ���� �Լ� ȣ��

	glfwDestroyWindow(window); // â ����
	glfwTerminate(); // GLFW ����

	return 0; // ���α׷� ����
}

void
init()
{
	sphere = gluNewQuadric(); // ���ο� ��ü ����
	gluQuadricDrawStyle(sphere, GLU_FILL); // �׸��� ��Ÿ�� ����
	gluQuadricNormals(sphere, GLU_SMOOTH); // ���� �ε巴�� ����
	gluQuadricOrientation(sphere, GLU_OUTSIDE); // �ܺ� ���� ����
	gluQuadricTexture(sphere, GL_FALSE); // �ؽ�ó ��� �� ��

	cylinder = gluNewQuadric(); // ���ο� ����� ����
	gluQuadricDrawStyle(cylinder, GLU_FILL); // �׸��� ��Ÿ�� ����
	gluQuadricNormals(cylinder, GLU_SMOOTH); // ���� �ε巴�� ����
	gluQuadricOrientation(cylinder, GLU_OUTSIDE); // �ܺ� ���� ����
	gluQuadricTexture(cylinder, GL_FALSE); // �ؽ�ó ��� �� ��

	disk = gluNewQuadric(); // ���ο� ���� ����
	gluQuadricDrawStyle(disk, GLU_FILL); // �׸��� ��Ÿ�� ����
	gluQuadricNormals(disk, GLU_SMOOTH); // ���� �ε巴�� ����
	gluQuadricOrientation(disk, GLU_OUTSIDE); // �ܺ� ���� ����
	gluQuadricTexture(disk, GL_FALSE); // �ؽ�ó ��� �� ��

	cout << endl;
	cout << "Keyboard Input: space for play/pause" << endl; // Ű �Է� �ȳ�

	cout << "Keyboard Input: 1 for rotation example" << endl; // Ű �Է� �ȳ�
	cout << "Keyboard Input: 2 for rotation wrt a pivot example" << endl; // Ű �Է� �ȳ�
	cout << "Keyboard Input: 3 for rotation wrt a pivot example using GLM" << endl; // Ű �Է� �ȳ�
	cout << "Keyboard Input: 4 for scaling example" << endl; // Ű �Է� �ȳ�
	cout << "Keyboard Input: 5 for scaling wrt a pivot example" << endl; // Ű �Է� �ȳ�
	cout << "Keyboard Input: 6 for scaling wrt a direction example" << endl; // Ű �Է� �ȳ�
	cout << "Keyboard Input: 7 for a solar system example" << endl; // Ű �Է� �ȳ�
	cout << "Keyboard Input: 8 scalingPivotExampleGLM" << endl; // Ű �Է� �ȳ�
	cout << "Keyboard Input: 9 scalingDirectionExampleGLM" << endl; // Ű �Է� �ȳ�
}

void
quit()
{
	gluDeleteQuadric(sphere); // ��ü ��ü ����
	gluDeleteQuadric(cylinder); // ����� ��ü ����
	gluDeleteQuadric(disk); // ���� ��ü ����
}

void
setupLight()
{
	glEnable(GL_LIGHTING); // ���� Ȱ��ȭ
	glEnable(GL_LIGHT0); // ù ��° ���� Ȱ��ȭ

	// ���� �Ӽ� ����
	GLfloat	ambient[4] = { 0.1f, 0.1f, 0.1f, 1.0f }; // �ֺ���
	GLfloat	diffuse[4] = { 1.0f, 1.0f, 1.0f, 1.0f }; // Ȯ�걤
	GLfloat	specular[4] = { 0.1f, 0.1f, 0.1f, 1.0f }; // �ݻ籤

	// ���� �Ӽ� ����
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_POSITION, value_ptr(light)); // ���� ��ġ
}

void
setupMaterial()
{
	// ���� �Ӽ� ����
	GLfloat	mat_ambient[4] = { 0.1f, 0.1f, 0.1f, 1.0f }; // �ֺ���
	GLfloat mat_specular[4] = { 0.5f, 0.5f, 0.5f, 1.0f }; // �ݻ籤
	GLfloat mat_shininess = 128; // ����

	// ���� �Ӽ� ����
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
}

void
setDiffuseColor(const vec3& color)
{
	GLfloat	mat_diffuse[4] = { color[0], color[1], color[2], 1 }; // Ȯ�� ���� ����
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse); // ���� �Ӽ� ����
}

void
drawSphere(float radius, const vec3& color)
{
	setDiffuseColor(color); // ���� ����

	gluSphere(sphere, radius, 72, 72); // ��ü �׸���
}

void
drawCylinder(float radius, float height, const vec3& color)
{
	setDiffuseColor(color); // ���� ����

	gluCylinder(cylinder, radius, radius, height, 72, 5); // ����� �׸���
}

void
drawDisk(float inner_radius, float outer_radius, const vec3& color)
{
	setDiffuseColor(color); // ���� ����

	gluDisk(disk, inner_radius, outer_radius, 72, 5); // ���� �׸���
}
