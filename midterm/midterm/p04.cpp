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
