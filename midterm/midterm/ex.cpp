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
GLfloat bgColor[4] = { 1,1,1,1 }; // 배경 색상 (흰색)

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
    // 초기화 코드 (예: 오브젝트 생성)
}

void quit() {
    // 종료 코드 (예: 자원 해제)
}

void render(GLFWwindow* window) {
    glClearColor(1, 1, 1, 1); // 배경색 설정

    // 뷰 변환 설정
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], up[0], up[1], up[2]);

    // 예시: 디렉셔널 스케일링, 로케이션 및 트랜스포메이션
    // 변환 행렬 초기화
    mat4 transform = mat4(1.0f);

    // 디렉셔널 스케일링 적용
    transform = scale(transform, vec3(2.0f, 1.0f, 1.0f)); // X 방향으로 2배 스케일

    // 로케이션 적용
    transform = translate(transform, vec3(1.0f, 0.0f, 0.0f)); // X 방향으로 이동

    // 트랜스포메이션 적용
    glMultMatrixf(value_ptr(transform));

    // 여기에 오브젝트 그리기 함수 호출
    // 예: drawCube(); 또는 drawSphere();
}

// 키보드 입력 처리
void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_Q || key == GLFW_KEY_ESCAPE) {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }
        // 추가적인 키 입력 처리
    }
}
