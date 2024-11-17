#include "glSetup.h"

#ifdef _WIN32
#define _USE_MATH_DEFINES
#endif

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

#include <iostream>
using namespace std;

void init();
void render(GLFWwindow* window);
void keyboard(GLFWwindow* window, int key, int code, int action, int mods);
void renderTorus(float currentAngle);

vec3 eye(4, 3, 4);
vec3 center(0, 0, 0);
vec3 up(0, 1, 0);

GLfloat bgColor[4] = { 1, 1, 1, 1 };

float AXIS_LENGTH = 1.75;
float AXIS_LINE_WIDTH = 2;

const int CURVE_POINTS = 18; // � �� ��
const int REVOLVING_STEPS = 36; // ȸ�� �ܰ� ��

vec3 vertex[REVOLVING_STEPS][CURVE_POINTS]; // �䷯�� ���� �迭
float sweepAngle = 0.0f; // ȸ�� ����

int selection = 1;

int main(int argc, char* argv[]) {
    vsync = 0;

    GLFWwindow* window = initializeOpenGL(argc, argv, bgColor);
    if (window == NULL) return -1;

    glfwSetKeyCallback(window, keyboard);

    glEnable(GL_NORMALIZE);
    glDisable(GL_CULL_FACE);
    reshape(window, windowW, windowH);

    init();

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        render(window);
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

void init() {
    float r = 0.25f; // Ʃ���� ������
    float R = 0.75f; // ���� ������
    for (int i = 0; i < REVOLVING_STEPS; i++) {
        float theta = (float)i / REVOLVING_STEPS * 2.0f * M_PI; // y�࿡ ���� ȸ�� ����
        for (int j = 0; j < CURVE_POINTS; j++) {
            float phi = (float)j / CURVE_POINTS * 2.0f * M_PI; // ���� �ܸ��� ����

            // y���� �������� ȸ���ϴ� �䷯���� ���� ���
            vertex[i][j] = vec3(
                (R + r * cos(phi)) * cos(-theta), // x ��ǥ
                r * sin(phi) + 0.5,               // y ��ǥ
                (R + r * cos(phi)) * sin(-theta)  // z ��ǥ
            );
        }
    }
}

void renderTorus(float currentAngle) {
    glEnable(GL_DEPTH_TEST);

    // ���̾�������
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    int stepsToRender = (int)(currentAngle / (2 * M_PI) * REVOLVING_STEPS);
    // ���� ������ ���� �������� �ܰ� �� ����

    for (int i = 0; i < stepsToRender; i++) {
        for (int j = 0; j < CURVE_POINTS; j++) {
            int i_next = (i + 1) % REVOLVING_STEPS;
            int j_next = (j + 1) % CURVE_POINTS;

            glVertex3fv(value_ptr(vertex[i][j]));
            glVertex3fv(value_ptr(vertex[i_next][j]));
            glVertex3fv(value_ptr(vertex[i_next][j_next]));
            glVertex3fv(value_ptr(vertex[i][j_next]));
        }
    }
    glEnd();

    if(selection == 2)
    {
        // ä���� ��
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glBegin(GL_QUADS);
        for (int i = 0; i < stepsToRender; i++) {
            for (int j = 0; j < CURVE_POINTS; j++) {
                int i_next = (i + 1) % REVOLVING_STEPS;
                int j_next = (j + 1) % CURVE_POINTS;

                vec3 p1 = vertex[i][j];
                vec3 p2 = vertex[i_next][j];
                vec3 p3 = vertex[i_next][j_next];
                vec3 p4 = vertex[i][j_next];

                // ���� ���� ���
                vec3 n = normalize(cross(p2 - p1, p3 - p1));

                // COP(ī�޶��� �߽�)������ ����
                vec3 centerToCOP = normalize(eye - (p1 + p2 + p3 + p4) / 4.0f);

                // ����/�ܺ� ���� ����
                if (dot(n, centerToCOP) > 0) {
                    glColor3f(0.0f, 0.0f, 1.0f); // �ٱ���: �Ķ���
                }
                else {
                    glColor3f(1.0f, 0.0f, 0.0f); // ����: ������
                }

                glVertex3fv(value_ptr(p1));
                glVertex3fv(value_ptr(p2));
                glVertex3fv(value_ptr(p3));
                glVertex3fv(value_ptr(p4));
            }
        }
        glEnd();
    }
}

void render(GLFWwindow* window) {
    glEnable(GL_DEPTH_TEST);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glClearColor(bgColor[0], bgColor[1], bgColor[2], bgColor[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], up[0], up[1], up[2]);

    glDisable(GL_LIGHTING);
    drawAxes(AXIS_LENGTH, AXIS_LINE_WIDTH * dpiScaling);

    renderTorus(sweepAngle); // ���� ������ �°� �䷯�� ������
}

void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        switch (key) {
        case GLFW_KEY_Q:
        case GLFW_KEY_ESCAPE:   glfwSetWindowShouldClose(window, GL_TRUE); break;
        case GLFW_KEY_RIGHT:
            if (sweepAngle < 2 * M_PI) { // �ִ� 360������ ����
                sweepAngle += radians(10.0f); // Increase angle
            }
            break;
        case GLFW_KEY_LEFT:
            if (sweepAngle > 0) { // �ּ� 0������ ����
                sweepAngle -= radians(10.0f); // Decrease angle
            }
            break;
        case GLFW_KEY_1: selection = 1;  break;
        case GLFW_KEY_2: selection = 2;  break;
        }
    }
}