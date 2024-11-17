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

const int CURVE_POINTS = 18; // 곡선 점 수
const int REVOLVING_STEPS = 36; // 회전 단계 수

vec3 vertex[REVOLVING_STEPS][CURVE_POINTS]; // 토러스 정점 배열
float sweepAngle = 0.0f; // 회전 각도

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
    float r = 0.25f; // 튜브의 반지름
    float R = 0.75f; // 원의 반지름
    for (int i = 0; i < REVOLVING_STEPS; i++) {
        float theta = (float)i / REVOLVING_STEPS * 2.0f * M_PI; // y축에 대한 회전 각도
        for (int j = 0; j < CURVE_POINTS; j++) {
            float phi = (float)j / CURVE_POINTS * 2.0f * M_PI; // 원형 단면의 각도

            // y축을 기준으로 회전하는 토러스의 정점 계산
            vertex[i][j] = vec3(
                (R + r * cos(phi)) * cos(-theta), // x 좌표
                r * sin(phi) + 0.5,               // y 좌표
                (R + r * cos(phi)) * sin(-theta)  // z 좌표
            );
        }
    }
}

void renderTorus(float currentAngle) {
    glEnable(GL_DEPTH_TEST);

    // 와이어프레임
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    int stepsToRender = (int)(currentAngle / (2 * M_PI) * REVOLVING_STEPS);
    // 현재 각도에 따라 렌더링할 단계 수 결정

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
        // 채워진 면
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

                // 법선 벡터 계산
                vec3 n = normalize(cross(p2 - p1, p3 - p1));

                // COP(카메라의 중심)까지의 벡터
                vec3 centerToCOP = normalize(eye - (p1 + p2 + p3 + p4) / 4.0f);

                // 내부/외부 색상 결정
                if (dot(n, centerToCOP) > 0) {
                    glColor3f(0.0f, 0.0f, 1.0f); // 바깥쪽: 파란색
                }
                else {
                    glColor3f(1.0f, 0.0f, 0.0f); // 안쪽: 빨간색
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

    renderTorus(sweepAngle); // 현재 각도에 맞게 토러스 렌더링
}

void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        switch (key) {
        case GLFW_KEY_Q:
        case GLFW_KEY_ESCAPE:   glfwSetWindowShouldClose(window, GL_TRUE); break;
        case GLFW_KEY_RIGHT:
            if (sweepAngle < 2 * M_PI) { // 최대 360도까지 형성
                sweepAngle += radians(10.0f); // Increase angle
            }
            break;
        case GLFW_KEY_LEFT:
            if (sweepAngle > 0) { // 최소 0도까지 형성
                sweepAngle -= radians(10.0f); // Decrease angle
            }
            break;
        case GLFW_KEY_1: selection = 1;  break;
        case GLFW_KEY_2: selection = 2;  break;
        }
    }
}