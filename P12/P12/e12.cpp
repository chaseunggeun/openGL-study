#define GLM_ENABLE_EXPERIMENTAL
#include "glSetup.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

void    init();
void    quit();
void    render(GLFWwindow* window);
void    keyboard(GLFWwindow* window, int key, int code, int action, int mods);
void    mouseButton(GLFWwindow* window, int button, int action, int mods);
void    cursorPos(GLFWwindow* window, double x, double y);

vec3    eyeTopView(0, 10, 0);
vec3    upTopView(0, 0, -1);

vec3    eyeFrontView(0, 0, 10);
vec3    upFrontView(0, 1, 0);

vec3    eyeBirdView(0, -10, 4);
vec3    upBirdView(0, 1, 0);
vec3    center(0, 0, 0);

vec4    light(0.0, 0.0, 0.8, 1);

bool    axes = true;
float   AXIS_LENGTH = 1.25;
float   AXIS_LINE_WIDTH = 2;
GLfloat bgColor[4] = { 1,1,1,1 };

int     view = 1;

// 드래그 관련 변수들
bool    isDragging = false;
double  dragStartX, dragStartY;
double  dragEndX, dragEndY;
vector<int> selectedSpheres; // 선택된 구체들의 인덱스를 저장

int
main(int argc, char* argv[])
{
    fovy = 16.1f;

    GLFWwindow* window = initializeOpenGL(argc, argv, bgColor);
    if (window == NULL) return -1;

    glfwSetKeyCallback(window, keyboard);
    glfwSetMouseButtonCallback(window, mouseButton);
    glfwSetCursorPosCallback(window, cursorPos); // 마우스 커서 콜백 추가

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);

    reshape(window, windowW, windowH);

    init();

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        render(window);
        glfwSwapBuffers(window);
    }

    quit();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

GLUquadricObj* sphere = NULL;

void
init()
{
    sphere = gluNewQuadric();
    gluQuadricDrawStyle(sphere, GLU_FILL);
    gluQuadricNormals(sphere, GLU_SMOOTH);
    gluQuadricOrientation(sphere, GLU_OUTSIDE);
    gluQuadricTexture(sphere, GL_FALSE);

    cout << endl;
    cout << "Keyboard input: x for axes on/off" << endl;
    cout << "Keyboard input: v for the top, front, bird-eye view" << endl;
    cout << "Mouse click: Select a single object" << endl;
    cout << "Mouse drag: Select multiple objects" << endl;
}

void
quit()
{
    gluDeleteQuadric(sphere);
}

// 드래그 사각형 그리기 함수
void drawDragRect()
{
    if (!isDragging) return;

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, windowW, 0, windowH);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_LIGHTING);
    glColor3f(0, 0, 0);

    // 점선 패턴 설정
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, 0xF0F0);

    glBegin(GL_LINE_LOOP);
    glVertex2f(dragStartX, windowH - dragStartY);
    glVertex2f(dragEndX, windowH - dragStartY);
    glVertex2f(dragEndX, windowH - dragEndY);
    glVertex2f(dragStartX, windowH - dragEndY);
    glEnd();

    glDisable(GL_LINE_STIPPLE);

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void
setupColoredMaterial(const vec3& color)
{
    GLfloat mat_ambient[4] = { 0.1f,0.1f,0.1f,1.0f };
    GLfloat mat_diffuse[4] = { color[0], color[1], color[2], 1.0f };
    GLfloat mat_specular[4] = { 0.5f,0.5f,0.5f,1.0f };
    GLfloat mat_shininess = 100.0f;

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
}

void drawSphere(float radius, int slices, int stacks)
{
    gluSphere(sphere, radius, slices, stacks);
}

void setupLight(const vec4& p)
{
    GLfloat ambient[4] = { 0.1f,0.1f,0.1f,1.0f };
    GLfloat diffuse[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat specular[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    glLightfv(GL_LIGHT0, GL_POSITION, value_ptr(p));
}

void
render(GLFWwindow* window)
{
    glInitNames();
    glPushName(-1);

    glClearColor(bgColor[0], bgColor[1], bgColor[2], bgColor[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    vec3    eye(0), up(0);
    switch (view)
    {
    case    0:    eye = eyeTopView;    up = upTopView;        break;
    case    1:    eye = eyeFrontView;    up = upFrontView;    break;
    case    2:    eye = eyeBirdView;    up = upBirdView;    break;
    }
    gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], up[0], up[1], up[2]);

    if (axes)
    {
        glDisable(GL_LIGHTING);
        drawAxes(AXIS_LENGTH, AXIS_LINE_WIDTH * dpiScaling);
    }

    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    setupLight(light);
    glScalef(0.4f, 0.4f, 0.4f);

    vec3    u[3];
    vec3    v[3];
    u[0] = vec3(0, 1, 0) * 2.0f;    u[1] = vec3(0, 0, 0);    u[2] = -u[0];
    v[0] = -vec3(1, 0, 0) * 4.0f;    v[1] = vec3(0, 0, 0);    v[2] = -v[0];

    for (int i = 0; i < 3; i++)
    {
        glPushMatrix();
        glTranslatef(u[i].x, u[i].y, u[i].z);
        for (int j = 0; j < 3; j++) {
            glPushMatrix();
            glTranslatef(v[j].x, v[j].y, v[j].z);

            int sphereIndex = 3 * i + j;
            bool isSelected = (find(selectedSpheres.begin(), selectedSpheres.end(), sphereIndex) != selectedSpheres.end());

            if (isSelected) setupColoredMaterial(vec3(0, 0, 1));
            else           setupColoredMaterial(vec3(1, 1, 1));

            glLoadName(sphereIndex);
            drawSphere(0.7f, 64, 64);

            glPopMatrix();
        }
        glPopMatrix();
    }

    // 드래그 사각형 그리기
    drawDragRect();
}

void
keyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        switch (key)
        {
        case GLFW_KEY_Q:
        case GLFW_KEY_ESCAPE: glfwSetWindowShouldClose(window, GL_TRUE); break;
        case GLFW_KEY_X:        axes = !axes; break;
        case GLFW_KEY_V:        view = (view + 1) % 3; break;
        }
    }
}

vector<int>
selectObjects(GLFWwindow* window, double x1, double y1, double x2, double y2)
{
    vector<int> selected;
    GLuint selectBuffer[256];
    glSelectBuffer(256, selectBuffer);

    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    // 드래그 영역에 맞춰 pick matrix 설정
    double centerX = (x1 + x2) / 2;
    double centerY = (y1 + y2) / 2;
    double width = fabs(x2 - x1);
    double height = fabs(y2 - y1);

    gluPickMatrix(centerX, viewport[3] - centerY, width, height, viewport);
    setupProjectionMatrix();

    glRenderMode(GL_SELECT);
    render(window);
    GLint hits = glRenderMode(GL_RENDER);

    int index = 0;
    for (int i = 0; i < hits; i++)
    {
        int n = selectBuffer[index + 0];
        for (int j = 0; j < n; j++)
        {
            int name = selectBuffer[index + 3 + j];
            if (find(selected.begin(), selected.end(), name) == selected.end())
            {
                selected.push_back(name);
            }
        }
        index += (3 + n);
    }

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    return selected;
}

void
mouseButton(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        if (action == GLFW_PRESS)
        {
            double x, y;
            glfwGetCursorPos(window, &x, &y);
            dragStartX = x * dpiScaling;
            dragStartY = y * dpiScaling;
            dragEndX = dragStartX;
            dragEndY = dragStartY;
            isDragging = true;
        }
        else if (action == GLFW_RELEASE)
        {
            isDragging = false;

            // 클릭인 경우 (시작점과 끝점이 거의 같은 경우)
            if (abs(dragStartX - dragEndX) < 2 && abs(dragStartY - dragEndY) < 2)
            {
                vector<int> clicked = selectObjects(window, dragStartX - 1, dragStartY - 1,
                    dragStartX + 1, dragStartY + 1);
                selectedSpheres.clear();
                if (!clicked.empty()) selectedSpheres.push_back(clicked[0]);
            }
            // 드래그 선택인 경우
            else
            {
                selectedSpheres = selectObjects(window, dragStartX, dragStartY,
                    dragEndX, dragEndY);
            }
        }
    }
}

void
cursorPos(GLFWwindow* window, double x, double y)
{
    if (isDragging)
    {
        dragEndX = x * dpiScaling;
        dragEndY = y * dpiScaling;
    }
}