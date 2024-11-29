#include "glSetup.h"

#ifdef _WIN32
#define _USE_MATH_DEFINES
#endif

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

#include <iostream>
#include <vector>
using namespace std;

void render(GLFWwindow* window);
void keyboard(GLFWwindow* window, int key, int code, int action, int mods);
void mouseButton(GLFWwindow* window, int button, int action, int mods);
void mouseMove(GLFWwindow* window, double x, double y);

const int REVOLVING_STEPS = 36;
float AXIS_LENGTH = 2.0f;
float AXIS_LINE_WIDTH = 2.0f;
float POINT_SIZE = 10.0f;
float POINT_SELECT_THRESHOLD = 0.05f;

GLfloat bgColor[4] = { 1, 1, 1, 1 };
vector<pair<float, float>> points;
vector<vector<vec3>> surfacePoints;
vector<vector<vec3>> surfaceNormals;
int selectedPoint = -1;
bool isDragging = false;
float rotationAngle = 0.0f;
bool showWireframe = true;
bool smoothShading = true;
float currentAngle = 0.0f;
vec3 eye(0, 0, 5);
vec3 center(0, 0, 0);
vec3 up(0, 1, 0);

void screen2world(float xs, float ys, float& xw, float& yw) {
	float aspect = (float)screenW / screenH;
	xw = 2.0f * (xs / (screenW - 1) - 0.5f) * aspect;
	yw = -2.0f * (ys / (screenH - 1) - 0.5f);
}

float distance(float x1, float y1, float x2, float y2) {
	return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

vec3 calculateVertexNormal(const vector<vector<vec3>>& surface, int i, int j) {
	vec3 normal(0.0f);
	int prev_i = (i - 1 + REVOLVING_STEPS) % REVOLVING_STEPS;
	int next_i = (i + 1) % REVOLVING_STEPS;

	if (j > 0) {
		vec3 n1 = normalize(cross(
			surface[i][j] - surface[prev_i][j],
			surface[i][j - 1] - surface[i][j]
		));
		vec3 n2 = normalize(cross(
			surface[next_i][j] - surface[i][j],
			surface[i][j] - surface[i][j - 1]
		));
		normal += n1 + n2;
	}

	if (j < surface[i].size() - 1) {
		vec3 n1 = normalize(cross(
			surface[i][j + 1] - surface[i][j],
			surface[prev_i][j] - surface[i][j]
		));
		vec3 n2 = normalize(cross(
			surface[i][j] - surface[next_i][j],
			surface[i][j + 1] - surface[i][j]
		));
		normal += n1 + n2;
	}

	return length(normal) > 0.0f ? normalize(normal) : vec3(0.0f, 1.0f, 0.0f);
}

void generateSurface() {
	if (points.size() < 2) return;

	surfacePoints.clear();
	surfaceNormals.clear();
	surfacePoints.resize(REVOLVING_STEPS);
	surfaceNormals.resize(REVOLVING_STEPS);

	for (int i = 0; i < REVOLVING_STEPS; i++) {
		float theta = (float)i / REVOLVING_STEPS * 2.0f * M_PI;
		vector<vec3> currentStep;
		vector<vec3> currentNormals;

		for (const auto& point : points) {
			float x = point.first * cos(theta);
			float y = point.second;
			float z = point.first * sin(theta);
			currentStep.push_back(vec3(x, y, z));
		}
		surfacePoints[i] = currentStep;
	}

	for (int i = 0; i < REVOLVING_STEPS; i++) {
		vector<vec3> normals;
		for (size_t j = 0; j < surfacePoints[i].size(); j++) {
			normals.push_back(calculateVertexNormal(surfacePoints, i, j));
		}
		surfaceNormals[i] = normals;
	}
}

void init() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	GLfloat light_position[] = { 1.0f, 1.0f, 1.0f, 0.0f };
	GLfloat white_light[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

	GLfloat mat_specular[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat mat_shininess[] = { 50.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
}

void renderSurface() {
	if (surfacePoints.empty()) return;

	glPushMatrix();
	glRotatef(rotationAngle, 1, 0, 0);

	int stepsToRender = (int)(currentAngle / (2 * M_PI) * REVOLVING_STEPS);
	stepsToRender = std::min(stepsToRender, REVOLVING_STEPS);

	// 와이어프레임 렌더링
	if (showWireframe) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDisable(GL_LIGHTING);
		glColor3f(0.0f, 0.0f, 0.0f);
		for (int i = 0; i < stepsToRender; i++) {
			int next_i = (i + 1) % REVOLVING_STEPS;
			glBegin(GL_QUADS);
			for (size_t j = 0; j < surfacePoints[i].size() - 1; j++) {
				glVertex3fv(value_ptr(surfacePoints[i][j]));
				glVertex3fv(value_ptr(surfacePoints[next_i][j]));
				glVertex3fv(value_ptr(surfacePoints[next_i][j + 1]));
				glVertex3fv(value_ptr(surfacePoints[i][j + 1]));
			}
			glEnd();
		}
	}

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_LIGHTING);
	glShadeModel(smoothShading ? GL_SMOOTH : GL_FLAT);

	GLfloat material_out[] = { 0.0f, 0.0f, 1.0f, 1.0f }; // 바깥쪽: 파란색
	GLfloat material_in[] = { 1.0f, 0.0f, 0.0f, 1.0f };  // 안쪽: 빨간색
	GLfloat shininess[] = { 60.0f };
	GLfloat specular[] = { 0.5f, 0.5f, 0.5f, 1.0f };

	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glDisable(GL_CULL_FACE);

	for (int i = 0; i < stepsToRender; i++) {
		int next_i = (i + 1) % REVOLVING_STEPS;
		for (size_t j = 0; j < surfacePoints[i].size() - 1; j++) {
			vec3 p1 = surfacePoints[i][j];
			vec3 p2 = surfacePoints[next_i][j];
			vec3 p3 = surfacePoints[next_i][j + 1];
			vec3 p4 = surfacePoints[i][j + 1];

			glMaterialfv(GL_FRONT, GL_DIFFUSE, material_out);
			glMaterialfv(GL_BACK, GL_DIFFUSE, material_in);

			glBegin(GL_QUADS);
			if (smoothShading) {
				glNormal3fv(value_ptr(surfaceNormals[i][j]));
				glVertex3fv(value_ptr(p1));
				glNormal3fv(value_ptr(surfaceNormals[next_i][j]));
				glVertex3fv(value_ptr(p2));
				glNormal3fv(value_ptr(surfaceNormals[next_i][j + 1]));
				glVertex3fv(value_ptr(p3));
				glNormal3fv(value_ptr(surfaceNormals[i][j + 1]));
				glVertex3fv(value_ptr(p4));
			}
			else {
				vec3 faceNormal = normalize(cross(p2 - p1, p4 - p1));
				glNormal3fv(value_ptr(faceNormal));
				glVertex3fv(value_ptr(p1));
				glVertex3fv(value_ptr(p2));
				glVertex3fv(value_ptr(p3));
				glVertex3fv(value_ptr(p4));
			}
			glEnd();
		}
	}

	glPopMatrix();
}


int main(int argc, char* argv[]) {
	perspectiveView = false;
	vsync = 0;

	GLFWwindow* window = initializeOpenGL(argc, argv, bgColor);
	if (window == NULL) return -1;

	glfwSetKeyCallback(window, keyboard);
	glfwSetMouseButtonCallback(window, mouseButton);
	glfwSetCursorPosCallback(window, mouseMove);

	init();
	reshape(window, windowW, windowH);

	cout << endl;
	cout << "Left mouse button: Add/Select/Move points" << endl;
	cout << "Delete/Backspace: Delete selected point" << endl;
	cout << "C key: Clear all points" << endl;
	cout << "W key: Toggle wireframe" << endl;
	cout << "S key: Toggle smooth/flat shading" << endl;
	cout << "Left/Right arrow: Rotate around X-axis" << endl;
	cout << "Up/Down arrow: Increase/Decrease revolution angle" << endl;

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		render(window);
		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

void render(GLFWwindow* window) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], up[0], up[1], up[2]);

	glDisable(GL_LIGHTING);
	drawAxes(AXIS_LENGTH, AXIS_LINE_WIDTH * dpiScaling);

	if (!points.empty()) {
		glColor3f(0, 0, 0);
		glLineWidth(2.0f * dpiScaling);
		glBegin(GL_LINE_STRIP);
		for (const auto& point : points) {
			glVertex2f(point.first, point.second);
		}
		glEnd();

		glPointSize(POINT_SIZE * dpiScaling);
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

	if (!points.empty()) {
		renderSurface();
	}
}

void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS || action == GLFW_REPEAT) {
		switch (key) {
		case GLFW_KEY_Q:
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, GL_TRUE);
			break;
		case GLFW_KEY_DELETE:
		case GLFW_KEY_BACKSPACE:
			if (selectedPoint >= 0 && selectedPoint < points.size()) {
				points.erase(points.begin() + selectedPoint);
				selectedPoint = -1;
				generateSurface();
			}
			break;
		case GLFW_KEY_C:
			points.clear();
			surfacePoints.clear();
			selectedPoint = -1;
			break;
		case GLFW_KEY_W:
			showWireframe = !showWireframe;
			break;
		case GLFW_KEY_S:
			smoothShading = !smoothShading;
			break;
		case GLFW_KEY_LEFT:
			rotationAngle -= 5.0f;
			break;
		case GLFW_KEY_RIGHT:
			rotationAngle += 5.0f;
			break;
		case GLFW_KEY_UP:
			currentAngle = std::min<float>(currentAngle + 0.2f, 2.0f * M_PI);
			break;
		case GLFW_KEY_DOWN:
			currentAngle = std::max<float>(currentAngle - 0.2f, 0.0f);
			break;
		}
	}
}

void mouseButton(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		double xs, ys;
		glfwGetCursorPos(window, &xs, &ys);
		float xw, yw;
		screen2world((float)xs, (float)ys, xw, yw);

		if (action == GLFW_PRESS) {
			selectedPoint = -1;
			for (size_t i = 0; i < points.size(); i++) {
				if (distance(xw, yw, points[i].first, points[i].second) < POINT_SELECT_THRESHOLD) {
					selectedPoint = i;
					isDragging = true;
					break;
				}
			}

			if (selectedPoint == -1) {
				points.push_back(make_pair(xw, yw));
				selectedPoint = points.size() - 1;
				isDragging = true;
				generateSurface();
			}
		}
		else if (action == GLFW_RELEASE) {
			isDragging = false;
		}
	}
}

void mouseMove(GLFWwindow* window, double x, double y) {
	if (isDragging && selectedPoint >= 0) {
		float xw, yw;
		screen2world((float)x, (float)y, xw, yw);
		points[selectedPoint].first = xw;
		points[selectedPoint].second = yw;
		generateSurface();
	}
}