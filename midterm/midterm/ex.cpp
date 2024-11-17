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

// Function prototypes
void init();
void quit();
void render(GLFWwindow* window);
void keyboard(GLFWwindow* window, int key, int code, int action, int mods);
GLfloat bgColor[4] = { 1,1,1,1 }; // ��� ���� (���)

vec3 eye(3.5, 3, 3.5);
vec3 center(0, 0, 0);
vec3 up(0, 1, 0);

// Main function
int main(int argc, char* argv[])
{
    GLFWwindow* window = initializeOpenGL(argc, argv, bgColor);
    if (window == NULL) return -1;

    glfwSetKeyCallback(window, keyboard);
    glEnable(GL_DEPTH_TEST);
    reshape(window, windowW, windowH);
    init();

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        render(window);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    quit();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

void init() {
    // �ʱ�ȭ �ڵ� (��: ������Ʈ ����)
}

void quit() {
    // ���� �ڵ� (��: �ڿ� ����)
}

void render(GLFWwindow* window) {
    glClearColor(1, 1, 1, 1); // ���� ����

    // �� ��ȯ ����
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], up[0], up[1], up[2]);

    // ����: �𷺼ų� �����ϸ�, �����̼� �� Ʈ���������̼�
    // ��ȯ ��� �ʱ�ȭ
    mat4 transform = mat4(1.0f);

    // �𷺼ų� �����ϸ� ����
    transform = scale(transform, vec3(2.0f, 1.0f, 1.0f)); // X �������� 2�� ������

    // �����̼� ����
    transform = translate(transform, vec3(1.0f, 0.0f, 0.0f)); // X �������� �̵�

    // Ʈ���������̼� ����
    glMultMatrixf(value_ptr(transform));

    // ���⿡ ������Ʈ �׸��� �Լ� ȣ��
    // ��: drawCube(); �Ǵ� drawSphere();
}

// Ű���� �Է� ó��
void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_Q || key == GLFW_KEY_ESCAPE) {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }
        // �߰����� Ű �Է� ó��
    }
}
