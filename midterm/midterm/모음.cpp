//p02
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

#include "glSetup.h"  // OpenGL ������ ���� ��� ���� ����
#include <iostream>   // ǥ�� ����� ���
using namespace std;

void	render(GLFWwindow* window);  // ȭ�� ������ �Լ�
void	keyboard(GLFWwindow* window, int key, int code, int action, int mods);  // Ű���� �Է� ó�� �Լ�

GLfloat bgColor[4] = { 1,1,1,1 };  // ������ ������� ���� (RGBA)

GLenum	mode = GL_POINTS;  // �׸��� ��带 GL_POINTS�� �ʱ�ȭ
bool	stipple = false;  // �� ����(����) ��� ����
bool	fill1 = true;  // �������� ä���� ����

int
main(int argc, char* argv[])
{
	perspectiveView = false;  // ���� �� ������� ����

	GLFWwindow* window = initializeOpenGL(argc, argv, bgColor);  // OpenGL ������ �ʱ�ȭ
	if (window == NULL) return -1;  // ������ ���� ���� �� ����

	glfwSetKeyCallback(window, keyboard);  // Ű���� �Է� �ݹ� �Լ� ����

	reshape(window, windowW, windowH);  // ������ ũ�� ����

	// Ű���� �Է� �ȳ� �޽��� ���
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

	// ���� ����: â�� ������ ������ ��� ����
	while (!glfwWindowShouldClose(window))
	{
		render(window);  // ȭ���� ������
		glfwSwapBuffers(window);  // ���� ���۸��� ���� ȭ�� ǥ��
		glfwPollEvents();  // �̺�Ʈ ó��
	}
	glfwDestroyWindow(window);  // â�� �ı�
	glfwTerminate();  // OpenGL ����
	return 0;
}

void
render(GLFWwindow* window)
{
	// ȭ�� Ŭ���� (���� �� ���� ���� �ʱ�ȭ)
	glClearColor(bgColor[0], bgColor[1], bgColor[2], bgColor[3]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);  // �𵨺� ��� ����
	glLoadIdentity();  // �𵨺� ��� �ʱ�ȭ

	// ���� ��� ����
	if (stipple) {
		glEnable(GL_LINE_STIPPLE);
		glLineStipple(int(3 * dpiScaling), 0xcccc);  // ���� ���� ����
	}
	else	glDisable(GL_LINE_STIPPLE);

	// ������ ä��� ���� ����
	if (fill1)	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);  // ä��� ���
	else	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  // �� ���

	glPointSize(5 * dpiScaling);  // �� ũ�� ����

	glLineWidth(5 * dpiScaling);  // �� �ʺ� ����

	// ������ ��忡 ���� ���� �׸���
	glBegin(mode);
	{
		// ������ ������ ������ �����Ͽ� �簢�� �׸���
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
	// Ű�� �����ų� �ݺ��� �� ����
	if (action == GLFW_PRESS || action == GLFW_REPEAT)
	{
		switch (key)
		{
		case GLFW_KEY_Q:
		case GLFW_KEY_ESCAPE: glfwSetWindowShouldClose(window, GL_TRUE); break;  // â �ݱ�

			// s Ű�� ���� ���� on/off
		case GLFW_KEY_S:	stipple = !stipple;			break;

			// f Ű�� ���� ������ ä��� on/off
		case GLFW_KEY_F:	fill1 = !fill1;				break;

			// ���� Ű�� ���� �׸��� ��� ����
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

			// ������ Ű�� ���� ó�� (���� �������� ����)
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
#include "glSetup.h"  // OpenGL ������ ���� ��� ���� ����
#include <iostream>   // ǥ�� ����� ���
using namespace std;

#ifdef _WIN32
#define _USE_MATH_DEFINES  // Windows ȯ�濡�� ���� ��� ����� ���� ��ũ��
#endif

#include <math.h>  // ���� �Լ� ���

// �Լ� ����
void	render(GLFWwindow* window);
void	keyboard(GLFWwindow* window, int key, int code, int action, int mods);
void	mouseButton(GLFWwindow* windodw, int button, int action, int mods);
void	mouseMove(GLFWwindow* window, double x, double y);

float	AXIS_LENGTH = 0.5;  // ���� ����
float	AXIS_LINE_WIDTH = 2.0;  // ���� �� ����

GLfloat bgColor[4] = { 1,1,1,1 };  // ������ ������� ���� (RGBA)

float	point[3][2] = { {0,0}, {0,0}, {0,0} };  // 2D ��ǥ�迡�� �� 3�� (�ﰢ���� �׸��� ���� ��)

enum class InputMode
{
	NONE = 0,  // �ƹ� �Էµ� ���� ����
	FIRST_POINT = 1,  // ù ��° ���� �Է��ϴ� ��
	SECOND_POINT = 2,  // �� ��° ���� �Է��ϴ� ��
	COMPLETE = 3,  // �ﰢ���� �ϼ��� ����
};

InputMode	inputMode = InputMode::NONE;  // ���� �Է� ���� �ʱ�ȭ

bool	pause = true;  // �ִϸ��̼� ���� ����

float	timeStep = 1.0f / 120;  // �ð� ���� (120fps)
float	period = 8.0;  // ȸ�� �ֱ�

// ���� ȸ����Ű�� �Լ�
void
rotate(float p[2], float delta_theta)
{
	float	x = p[0];
	float	y = p[1];

	float	cos_delta_theta = cos(delta_theta);
	float	sin_delta_theta = sin(delta_theta);

	// ȸ�� ��ȯ�� ���� ���ο� ��ǥ ���
	p[0] = cos_delta_theta * x - sin_delta_theta * y;
	p[1] = sin_delta_theta * x + cos_delta_theta * y;
}

int
main(int argc, char* argv[])
{
	perspectiveView = false;  // ���� �� ������� ����

	vsync = 0;  // ���� ����ȭ ��

	GLFWwindow* window = initializeOpenGL(argc, argv, bgColor);  // OpenGL ������ �ʱ�ȭ
	if (window == NULL) return -1;  // ������ ���� ���� �� ����

	// Ű���� �� ���콺 �ݹ� �Լ� ����
	glfwSetKeyCallback(window, keyboard);
	glfwSetMouseButtonCallback(window, mouseButton);
	glfwSetCursorPosCallback(window, mouseMove);

	reshape(window, windowW, windowH);  // ������ ũ�� ����

	// �Է� �ȳ� �޽��� ���
	cout << endl;
	cout << "Mouse button down : start point of the line segment" << endl;
	cout << "Mouse dragging :     change" << endl;
	cout << "Mouse button up :    end point of the line segment" << endl;
	cout << "Keyboard input :     space for play/pause" << endl;
	cout << "Keyboard input :     up for increasing period" << endl;
	cout << "Keyboard input :     down for decreasing period" << endl;

	float	previous = (float)glfwGetTime();  // ���� �ð� ����
	float	elapsed = 0;  // ��� �ð� �ʱ�ȭ
	while (!glfwWindowShouldClose(window))  // ���� ����
	{
		glfwPollEvents();  // �̺�Ʈ ó��

		float	now = (float)glfwGetTime();  // ���� �ð�
		float	delta_t = now - previous;  // �ð� ���� ���
		previous = now;  // ���� �ð� ������Ʈ

		elapsed += delta_t;  // ��� �ð� ����

		if (elapsed > timeStep) {
			// �ִϸ��̼��� ���� ���̰� �Է��� �Ϸ�Ǹ� ȸ��
			if (!pause && inputMode == InputMode::COMPLETE) {
				float	delta_theta = float(2.0 * M_PI) / period * elapsed;

				// �ﰢ���� �� ���� ȸ��
				rotate(point[0], delta_theta);
				rotate(point[1], delta_theta);
				rotate(point[2], delta_theta);
			}

			elapsed = 0;
		}
		render(window);  // ȭ�� ������
		glfwSwapBuffers(window);  // ���� ���۸�
	}

	glfwDestroyWindow(window);  // â ����
	glfwTerminate();  // OpenGL ����
	return 0;
}

void
render(GLFWwindow* window)
{
	glClearColor(bgColor[0], bgColor[1], bgColor[2], bgColor[3]);  // ���� ����
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // ȭ�� �ʱ�ȭ

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glDisable(GL_LINE_STIPPLE);  // ���� ��� ��Ȱ��ȭ
	drawAxes(AXIS_LENGTH, AXIS_LINE_WIDTH * dpiScaling);  // ��ǥ�� �׸���

	glColor3f(0, 0, 1);  // �� ������ �Ķ������� ����
	glLineWidth(5 * dpiScaling);  // �� �ʺ� ����

	// ù ��° �� �Է� ���� ��
	if (inputMode == InputMode::FIRST_POINT) {
		glEnable(GL_LINE_STIPPLE);
		glLineStipple(int(3 * dpiScaling), 0xcccc);  // ���� ����

		glBegin(GL_LINES);
		glVertex2f(point[0][0], point[0][1]);  // ù ��° ��
		glVertex2f(point[1][0], point[1][1]);  // �� ��° �� (���콺 �̵� ��)
		glEnd();
	}

	// �� ��° �� �Է� ��
	if (inputMode >= InputMode::SECOND_POINT) {
		glBegin(GL_LINES);
		glVertex2f(point[0][0], point[0][1]);  // ù ��° ��
		glVertex2f(point[1][0], point[1][1]);  // �� ��° ��
		glEnd();
	}

	// �� ��° �� �Է� ���� ��
	if (inputMode == InputMode::SECOND_POINT) {
		glEnable(GL_LINE_STIPPLE);
		glLineStipple(int(3 * dpiScaling), 0xcccc);  // ���� ����

		glBegin(GL_LINES);
		glVertex2f(point[1][0], point[1][1]);  // �� ��° ��
		glVertex2f(point[2][0], point[2][1]);  // �� ��° �� (���콺 �̵� ��)
		glEnd();
	}

	// �ﰢ�� �ϼ� ����
	if (inputMode == InputMode::COMPLETE) {
		glBegin(GL_TRIANGLES);
		glVertex2f(point[0][0], point[0][1]);  // ù ��° ��
		glVertex2f(point[1][0], point[1][1]);  // �� ��° ��
		glVertex2f(point[2][0], point[2][1]);  // �� ��° ��
		glEnd();
	}
}

// Ű���� �Է� ó��
void
keyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS || action == GLFW_REPEAT)
	{
		switch (key)
		{
		case GLFW_KEY_Q:
		case GLFW_KEY_ESCAPE:	glfwSetWindowShouldClose(window, GL_TRUE); break;  // â �ݱ�

		case GLFW_KEY_SPACE:	pause = !pause; break;  // �ִϸ��̼� �Ͻ�����/�簳
		case GLFW_KEY_UP:		period += 0.1f; break;  // �ֱ� ����
		case GLFW_KEY_DOWN:		period = (float)max(period - 0.1, 0.1); break;  // �ֱ� ����
		}
	}
}

// ȭ�� ��ǥ�� ���� ��ǥ�� ��ȯ�ϴ� �Լ�
void
screen2world(float xs, float ys, float& xw, float& yw)
{
	float	aspect = (float)screenW / screenH;  // ȭ�� ���� ���
	xw = 2.0f * (xs / (screenW - 1) - 0.5f) * aspect;
	yw = -2.0f * (ys / (screenH - 1) - 0.5f);
}

// ���콺 ��ư �Է� ó��
void
mouseButton(GLFWwindow* window, int button, int action, int mods)
{
	double	xs, ys;
	glfwGetCursorPos(window, &xs, &ys);  // ���콺 Ŀ�� ��ġ ��������

	float	xw, yw;
	screen2world((float)xs, (float)ys, xw, yw);  // ȭ�� ��ǥ�� ���� ��ǥ�� ��ȯ
	if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_LEFT)
	{
		// ù ��° �� ����
		if (inputMode == InputMode::NONE) {
			inputMode = InputMode::FIRST_POINT;
			point[0][0] = xw;	point[0][1] = yw;
		}
		// �� ��° �� ����
		else if (inputMode == InputMode::FIRST_POINT) {
			inputMode = InputMode::SECOND_POINT;
			point[1][0] = xw;	point[1][1] = yw;
		}
		// �� ��° �� ���� �� �ﰢ�� �ϼ�
		else if (inputMode == InputMode::SECOND_POINT) {
			inputMode = InputMode::COMPLETE;
			point[2][0] = xw;	point[2][1] = yw;
		}
		// �ٽ� ù ��° ������ ����
		else if (inputMode == InputMode::COMPLETE) {
			inputMode = InputMode::FIRST_POINT;
			point[0][0] = xw;	point[0][1] = yw;
		}
	}
}

// ���콺 �̵� ó��
void
mouseMove(GLFWwindow* window, double x, double y) {
	// ù ��° ���� ������ �� ���콺 �̵� ���� �� �� ��° ���� ��ġ�� ������Ʈ
	if (inputMode == InputMode::FIRST_POINT) {
		screen2world((float)x, (float)y, point[1][0], point[1][1]);
	}
	// �� ��° ���� ������ �� ���콺 �̵� ���� �� �� ��° ���� ��ġ�� ������Ʈ
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

// Part 1: ���� ���� �� ����
void
part1()
{
	cout << "3x1 vectors" << endl;

	// 3���� ���� a�� 0���� �ʱ�ȭ
	glm::vec3 a(0);
	cout << "a = " << to_string(a) << endl;

	// 3���� ���� b�� (3, 2, 1)�� �ʱ�ȭ
	glm::vec<3, float> b(3, 2, 1);
	cout << "b = " << to_string(b) << endl;

	// a�� ���ο� �� (1, 2, 3) ����
	a = glm::vec3(1, 2, 3);
	cout << "a = " << to_string(a) << endl;

	// a�� z�� ����
	a.z = 3;
	cout << "a[0] = " << a[0] << endl;
	cout << "a.z = " << a.z << endl;
}

// Part 2: ���� ����
void
part2()
{
	cout << "Vector operations" << endl;

	// �� 3���� ���� a�� b�� ����
	glm::vec3 a(1, 2, 3);
	glm::vec3 b(3, 2, 1);
	cout << "a = " << to_string(a) << endl;
	cout << "b = " << to_string(b) << endl;

	// ���� ����
	cout << "a + b = " << to_string(a + b) << endl;
	// ���� ����
	cout << "a - b = " << to_string(a - b) << endl;

	// ���� ��ȣ ����
	cout << "-a = " << to_string(-a) << endl;

	// ���� ��Į�� ����
	cout << "1.5*a = " << to_string(1.5f * a) << endl;

	// ���� ���
	cout << "dot(a, b) = " << dot(a, b) << endl;

	// ���� ���
	cout << "cross(a, b) = " << to_string(cross(a, b)) << endl;

	// ���� ���� ���
	cout << "length(a) = " << length(a) << endl;
	cout << endl;
}

// Part 3: 3x3 ��� ���� �� ����
void
part3()
{
	cout << "3x3 matrices" << endl;

	// 3x3 ��� A�� 0���� �ʱ�ȭ
	glm::mat3 A(0);
	cout << "A = " << to_string(A) << endl;

	// A�� ���� ��� �Ҵ�
	A = glm::mat3(1.0);
	cout << "A = " << to_string(A) << endl;

	// ��� B�� ���� �ʱ�ȭ
	glm::mat3 B(1.0, 0.0, 0.0, 2.0, 1.0, 0.0, 3.0, 0.0, 1.0);
	cout << "B = " << to_string(B) << endl;

	// �迭�� ����� ��� B�� �ʱ�ȭ
	float B_data[9] = { 1.0, 0.0, 0.0, 2.0, 1.0, 0.0, 3.0, 0.0, 1.0 };
	B = glm::make_mat3(B_data);
	cout << "B = " << to_string(B) << endl;

	// ��� B�� 3��° ���� ���
	cout << "3rd col of B = " << to_string(B[2]) << endl;
	cout << "3rd col B = " << to_string(column(B, 2)) << endl;
	// ��� B�� 3��° ���� ���
	cout << "3rd row of B = " << to_string(row(B, 2)) << endl;
	// 1�� 3���� �� ����
	cout << "1st row 3rd col of B = " << B[2][0] << endl;
	cout << "1st row 3rd col of B = " << B[2].x << endl;
}

// Part 4: ��� ����
void
part4()
{
	cout << "Matrix operations" << endl;

	// �� ��� A�� B ����
	glm::mat3 A(1.0);
	glm::mat3 B(1.0, 0.0, 0.0, 2.0, 1.0, 0.0, 3.0, 0.0, 1.0);
	cout << "A = " << to_string(A) << endl;
	cout << "B = " << to_string(B) << endl;

	// ��� ����
	cout << "A + B = " << to_string(A + B) << endl;
	// ��� ����
	cout << "A - B = " << to_string(A - B) << endl;

	// ��� ��ȣ ����
	cout << "-A = " << to_string(-A) << endl;

	// ��� ��Į�� ����
	cout << "1.5*A = " << to_string(1.5f * A) << endl;

	// ��� ����
	cout << "A X B = " << to_string(A * B) << endl;

	// ��� ��ġ
	cout << "transpose(B) = " << to_string(transpose(B)) << endl;

	// ��� �����
	cout << "inverse(B) = " << to_string(inverse(B)) << endl;
	cout << endl;

	// ����� ���� Ȯ��
	cout << "inverse(B) * B = " << to_string(inverse(B) * B) << endl;
}

// Part 5: ��İ� ���� ����
void
part5()
{
	cout << "Matrix-vector multiplication and assembling" << endl;

	// ���� a ����
	glm::vec3 a(1, 2, 3);
	cout << "a = " << to_string(a) << endl;

	// ��� B ����
	glm::mat3 B(1.0, 0.0, 0.0, 2.0, 1.0, 0.0, 3.0, 0.0, 1.0);
	cout << "B = " << to_string(B) << endl;

	// ��İ� ���� ����
	cout << "B x a = " << to_string(B * a) << endl;
	cout << "a x B = " << to_string(a * B) << endl;

	// ���� 4���� Ȯ��
	cout << "(a, 1.0) = " << to_string(glm::vec4(a, 1.0)) << endl;
	cout << "(1.0, a) = " << to_string(glm::vec4(1.0, a)) << endl;

	// 3x3 ����� 4x4 ��ķ� Ȯ��
	glm::mat4 C = glm::mat4(B);
	cout << "C = " << to_string(C) << endl;
}

// Part 6: ���� ���� 1 - ���� ����
void
part6()
{
	cout << "Exercise 01" << endl;
	glm::vec<3, float> v1(1, 2, 3);
	glm::vec<3, float> v2(3, 2, 1);
	cout << "v1 = " << to_string(v1) << endl;
	cout << "v2 = " << to_string(v2) << endl;

	// ���� ���� �� ����
	cout << "v1 + v2 = " << to_string(v1 + v2) << endl;
	cout << "v1 - v2 = " << to_string(v1 - v2) << endl;

	// ���� ��ȣ ����
	cout << "-v1 = " << to_string(-v1) << endl;

	// ��Į�� ������ ���� ����
	cout << "v1 - 2 * v2 = " << to_string(v1 - 2.0f * v2) << endl;

	// ���� �� ����
	cout << "dot(v1, v2) = " << to_string(dot(v1, v2)) << endl;
	cout << "cross(v1, v2) = " << to_string(cross(v1, v2)) << endl;
}

// Part 7: ���� ���� 2 - ��� ����
void
part7()
{
	cout << "Exercise 02" << endl;
	glm::vec<3, float> v1(1, 2, 3);
	glm::vec<3, float> v2(3, 2, 1);

	// ��� A1�� A2 ����
	glm::mat3 A1(1, 2, 1, 2, 3, 1, 3, 2, 2);
	glm::mat3 A2(2, 2, 1, 1, 2, 1, 2, 1, 1);

	cout << "A1 = " << to_string(A1) << endl;
	cout << "A2 = " << to_string(A2) << endl;

	// ��� ���� �� ����
	cout << "A1 + A2 = " << to_string(A1 + A2) << endl;
	cout << "A1 - A2 = " << to_string(A1 - A2) << endl;

	// ��� ��ȣ ����
	cout << "-A1 = " << to_string(-A1) << endl;

	// ��Į�� ������ ��� ����
	cout << "A1 - 2 * A2 = " << to_string(A1 - 2.0f * A2) << endl;

	// ��� ���� �� ��İ� ���� ����
	cout << "A1 * A2 = " << to_string(A1 * A2) << endl;
	cout << "A2 * A1 = " << to_string(A2 * A1) << endl;
	cout << "A1 * v1 = " << to_string(A1 * v1) << endl;
	cout << "A2 * v2 = " << to_string(A2 * v2) << endl;
}

int
main(int argc, char* argv[])
{
	int a;
	cin >> a; // ������ ��Ʈ ����

	// �Է��� ���� ���� �Լ� ����
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

// �ʱ�ȭ �� ���� �Լ� ����
void	init();
void	quit();
void	render(GLFWwindow* window);
void	keyboard(GLFWwindow* window, int key, int code, int action, int mods);

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
			if (!pause) frame += 1; // �Ͻ� ���� ���°� �ƴ� ��� ������ ����

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
		case GLFW_KEY_SPACE:	pause = !pause; break; // �Ͻ� ����/�簳
		}
	}
}
