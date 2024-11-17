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

vector<pair<float, float>> points;
int selectedPoint = -1;
bool isDragging = false;
float POINT_SELECT_THRESHOLD = 0.05f;
float LINE_SELECT_THRESHOLD = 0.03f;

void screen2world(float xs, float ys, float& xw, float& yw)
{
    float aspect = (float)screenW/screenH;
    xw = 2.0f * (xs / (screenW - 1) - 0.5f) * aspect;
    yw = -2.0f * (ys / (screenH - 1) - 0.5f);
}

// 두 점 사이의 거리 계산
float distance(float x1, float y1, float x2, float y2) {
    return sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
}

// 점과 선분 사이의 거리 계산
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
    cout << "Left mouse button: Add/Select/Move points" << endl;
    cout << "Delete/Backspace: Delete selected point" << endl;
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

void render(GLFWwindow* window)
{
    glClearColor(bgColor[0], bgColor[1], bgColor[2], bgColor[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // 폴리라인 그리기
    if (!points.empty()) {
        glColor3f(0, 0, 0);
        glLineWidth(3 * dpiScaling);

        for (size_t i = 0; i < points.size() - 1; i++) {
            if (static_cast<int>(i) == selectedPoint - 1 || static_cast<int>(i) == selectedPoint) {
                glLineStipple(1, 0xFF00); // 점선 설정
                glEnable(GL_LINE_STIPPLE);
            }
            else {
                glDisable(GL_LINE_STIPPLE);
            }

            glBegin(GL_LINES);
            glVertex2f(points[i].first, points[i].second);
            glVertex2f(points[i + 1].first, points[i + 1].second);
            glEnd();
        }

        glDisable(GL_LINE_STIPPLE); // 점선 비활성화

        // 점들 그리기
        glPointSize(10.0f * dpiScaling);
        glBegin(GL_POINTS);
        for (size_t i = 0; i < points.size(); i++) {
            if (i == selectedPoint)
                glColor3f(1, 0, 0);
            else
                glColor3f(0, 0, 1);
            glVertex2f(points[i].first, points[i].second);
        }
        glEnd();
    }
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

            case GLFW_KEY_DELETE:
            case GLFW_KEY_BACKSPACE:
                if (selectedPoint >= 0 && selectedPoint < points.size()) {
                    points.erase(points.begin() + selectedPoint);
                    selectedPoint = -1;
                }
                break;

            case GLFW_KEY_C:
                points.clear();
                selectedPoint = -1;
                break;
        }
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
            // 점 선택 검사
            selectedPoint = -1;
            for (size_t i = 0; i < points.size(); i++) {
                if (distance(xw, yw, points[i].first, points[i].second) < POINT_SELECT_THRESHOLD) {
                    selectedPoint = i;
                    isDragging = true;
                    break;
                }
            }

            // 선분 위 점 삽입 검사
            if (selectedPoint == -1 && points.size() >= 2) {
                for (size_t i = 0; i < points.size() - 1; i++) {
                    float dist = pointToLineDistance(xw, yw,
                        points[i].first, points[i].second,
                        points[i + 1].first, points[i + 1].second);
                    if (dist < LINE_SELECT_THRESHOLD) {
                        points.insert(points.begin() + i + 1, make_pair(xw, yw));
                        selectedPoint = i + 1;
                        isDragging = true;
                        break;
                    }
                }
            }

            // 새 점 추가
            if (selectedPoint == -1) {
                points.push_back(make_pair(xw, yw));
                selectedPoint = points.size() - 1;
                isDragging = true;
            }
        }
        else if (action == GLFW_RELEASE) {
            isDragging = false;
        }
    }
}

void mouseMove(GLFWwindow* window, double x, double y)
{
    if (isDragging && selectedPoint >= 0) {
        float xw, yw;
        screen2world((float)x, (float)y, xw, yw);
        points[selectedPoint].first = xw;
        points[selectedPoint].second = yw;
    }
}