#include "glSetup.h"
#include <iostream>
#include <vector>
using namespace std;

void render(GLFWwindow* window);
void keyboard(GLFWwindow* window, int key, int code, int action, int mods);
void mouseButton(GLFWwindow* window, int button, int action, int mods);
void mouseMove(GLFWwindow* window, double x, double y);

float AXIS_LENGTH = 0.5;
float AXIS_LINE_WIDTH = 2.0;
GLfloat bgColor[4] = { 1,1,1,1 };

// ���������� ���� ������ ����
vector<pair<float, float>> points;  // ��� ������ ��ǥ
int selectedPoint = -1;             // ���� ���õ� ���� �ε���
bool isDragging = false;            // �巡�� ������ ����
float POINT_SELECT_THRESHOLD = 0.05f; // �� ���� �Ӱ谪
float LINE_SELECT_THRESHOLD = 0.03f;  // ���� ���� �Ӱ谪

// ���� ���콺 ��ġ ����
float currentMouseX = 0.0f;
float currentMouseY = 0.0f;

void screen2world(float xs, float ys, float& xw, float& yw)
{
    float aspect = (float)screenW / screenH;
    xw = 2.0f * (xs / (screenW - 1) - 0.5f) * aspect;
    yw = -2.0f * (ys / (screenH - 1) - 0.5f);
}

float distance(float x1, float y1, float x2, float y2) {
    return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

float pointToLineDistance(float px, float py, float x1, float y1, float x2, float y2) {
    float A = px - x1;
    float B = py - y1;
    float C = x2 - x1;
    float D = y2 - y1;

    float dot = A * C + B * D;
    float len_sq = C * C + D * D;

    float param = -1;
    if (len_sq != 0)
        param = dot / len_sq;

    float xx, yy;

    if (param < 0) {
        xx = x1;
        yy = y1;
    }
    else if (param > 1) {
        xx = x2;
        yy = y2;
    }
    else {
        xx = x1 + param * C;
        yy = y1 + param * D;
    }

    return distance(px, py, xx, yy);
}

void render(GLFWwindow* window)
{
    glClearColor(bgColor[0], bgColor[1], bgColor[2], bgColor[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // �� �׸���
    glDisable(GL_LINE_STIPPLE);
    drawAxes(AXIS_LENGTH, AXIS_LINE_WIDTH * dpiScaling);

    // �������� �׸���
    if (!points.empty()) {
        glColor3f(0, 0, 1);
        glLineWidth(2 * dpiScaling);

        // �Ǽ����� ���� �������� �׸���
        glDisable(GL_LINE_STIPPLE);
        glBegin(GL_LINE_STRIP);
        for (const auto& p : points) {
            glVertex2f(p.first, p.second);
        }
        glEnd();

        // ������ ���� ���� ���콺 Ŀ���� �մ� ���� �׸���
        glEnable(GL_LINE_STIPPLE);
        glLineStipple(int(3 * dpiScaling), 0xAAAA);
        glBegin(GL_LINES);
        glVertex2f(points.back().first, points.back().second);
        glVertex2f(currentMouseX, currentMouseY);
        glEnd();
        glDisable(GL_LINE_STIPPLE);

        // ���� �׸���
        glPointSize(10.0f * dpiScaling);
        glBegin(GL_POINTS);
        for (const auto& p : points) {
            glVertex2f(p.first, p.second);
        }
        glEnd();
    }
}

void mouseButton(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        double xs, ys;
        glfwGetCursorPos(window, &xs, &ys);
        float xw, yw;
        screen2world((float)xs, (float)ys, xw, yw);

        if (action == GLFW_PRESS) {
            // �� �� �߰�
            points.push_back(make_pair(xw, yw));
            currentMouseX = xw;
            currentMouseY = yw;
        }
    }
}

void mouseMove(GLFWwindow* window, double x, double y)
{
    // ���� ���콺 ��ġ ������Ʈ
    float xw, yw;
    screen2world((float)x, (float)y, xw, yw);
    currentMouseX = xw;
    currentMouseY = yw;
}

void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        switch (key)
        {
        case GLFW_KEY_Q:
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, GL_TRUE);
            break;

        case GLFW_KEY_C:
            points.clear();
            break;
        }
    }
}

int main(int argc, char* argv[])
{
    perspectiveView = false;
    vsync = 0;

    GLFWwindow* window = initializeOpenGL(argc, argv, bgColor);
    if (window == NULL) return -1;

    glfwSetKeyCallback(window, keyboard);
    glfwSetMouseButtonCallback(window, mouseButton);
    glfwSetCursorPosCallback(window, mouseMove);

    reshape(window, windowW, windowH);

    cout << endl;
    cout << "Left mouse button: Add points" << endl;
    cout << "C key: Clear all points" << endl;

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        render(window);
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}