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
