#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

#include <iostream>
using namespace std;

// Part 1: 벡터 생성 및 접근
void
part1()
{
	cout << "3x1 vectors" << endl;

	// 3차원 벡터 a를 0으로 초기화
	glm::vec3 a(0);
	cout << "a = " << to_string(a) << endl;

	// 3차원 벡터 b를 (3, 2, 1)로 초기화
	glm::vec<3, float> b(3, 2, 1);
	cout << "b = " << to_string(b) << endl;

	// a에 새로운 값 (1, 2, 3) 대입
	a = glm::vec3(1, 2, 3);
	cout << "a = " << to_string(a) << endl;

	// a의 z값 변경
	a.z = 3;
	cout << "a[0] = " << a[0] << endl;
	cout << "a.z = " << a.z << endl;
}

// Part 2: 벡터 연산
void
part2()
{
	cout << "Vector operations" << endl;

	// 두 3차원 벡터 a와 b를 정의
	glm::vec3 a(1, 2, 3);
	glm::vec3 b(3, 2, 1);
	cout << "a = " << to_string(a) << endl;
	cout << "b = " << to_string(b) << endl;

	// 벡터 덧셈
	cout << "a + b = " << to_string(a + b) << endl;
	// 벡터 뺄셈
	cout << "a - b = " << to_string(a - b) << endl;

	// 벡터 부호 반전
	cout << "-a = " << to_string(-a) << endl;

	// 벡터 스칼라 곱셈
	cout << "1.5*a = " << to_string(1.5f * a) << endl;

	// 내적 계산
	cout << "dot(a, b) = " << dot(a, b) << endl;

	// 외적 계산
	cout << "cross(a, b) = " << to_string(cross(a, b)) << endl;

	// 벡터 길이 계산
	cout << "length(a) = " << length(a) << endl;
	cout << endl;
}

// Part 3: 3x3 행렬 생성 및 접근
void
part3()
{
	cout << "3x3 matrices" << endl;

	// 3x3 행렬 A를 0으로 초기화
	glm::mat3 A(0);
	cout << "A = " << to_string(A) << endl;

	// A에 단위 행렬 할당
	A = glm::mat3(1.0);
	cout << "A = " << to_string(A) << endl;

	// 행렬 B를 직접 초기화
	glm::mat3 B(1.0, 0.0, 0.0, 2.0, 1.0, 0.0, 3.0, 0.0, 1.0);
	cout << "B = " << to_string(B) << endl;

	// 배열을 사용해 행렬 B를 초기화
	float B_data[9] = { 1.0, 0.0, 0.0, 2.0, 1.0, 0.0, 3.0, 0.0, 1.0 };
	B = glm::make_mat3(B_data);
	cout << "B = " << to_string(B) << endl;

	// 행렬 B의 3번째 열을 출력
	cout << "3rd col of B = " << to_string(B[2]) << endl;
	cout << "3rd col B = " << to_string(column(B, 2)) << endl;
	// 행렬 B의 3번째 행을 출력
	cout << "3rd row of B = " << to_string(row(B, 2)) << endl;
	// 1행 3열의 값 접근
	cout << "1st row 3rd col of B = " << B[2][0] << endl;
	cout << "1st row 3rd col of B = " << B[2].x << endl;
}

// Part 4: 행렬 연산
void
part4()
{
	cout << "Matrix operations" << endl;

	// 두 행렬 A와 B 정의
	glm::mat3 A(1.0);
	glm::mat3 B(1.0, 0.0, 0.0, 2.0, 1.0, 0.0, 3.0, 0.0, 1.0);
	cout << "A = " << to_string(A) << endl;
	cout << "B = " << to_string(B) << endl;

	// 행렬 덧셈
	cout << "A + B = " << to_string(A + B) << endl;
	// 행렬 뺄셈
	cout << "A - B = " << to_string(A - B) << endl;

	// 행렬 부호 반전
	cout << "-A = " << to_string(-A) << endl;

	// 행렬 스칼라 곱셈
	cout << "1.5*A = " << to_string(1.5f * A) << endl;

	// 행렬 곱셈
	cout << "A X B = " << to_string(A * B) << endl;

	// 행렬 전치
	cout << "transpose(B) = " << to_string(transpose(B)) << endl;

	// 행렬 역행렬
	cout << "inverse(B) = " << to_string(inverse(B)) << endl;
	cout << endl;

	// 역행렬 곱셈 확인
	cout << "inverse(B) * B = " << to_string(inverse(B) * B) << endl;
}

// Part 5: 행렬과 벡터 곱셈
void
part5()
{
	cout << "Matrix-vector multiplication and assembling" << endl;

	// 벡터 a 정의
	glm::vec3 a(1, 2, 3);
	cout << "a = " << to_string(a) << endl;

	// 행렬 B 정의
	glm::mat3 B(1.0, 0.0, 0.0, 2.0, 1.0, 0.0, 3.0, 0.0, 1.0);
	cout << "B = " << to_string(B) << endl;

	// 행렬과 벡터 곱셈
	cout << "B x a = " << to_string(B * a) << endl;
	cout << "a x B = " << to_string(a * B) << endl;

	// 벡터 4차원 확장
	cout << "(a, 1.0) = " << to_string(glm::vec4(a, 1.0)) << endl;
	cout << "(1.0, a) = " << to_string(glm::vec4(1.0, a)) << endl;

	// 3x3 행렬을 4x4 행렬로 확장
	glm::mat4 C = glm::mat4(B);
	cout << "C = " << to_string(C) << endl;
}

// Part 6: 연습 문제 1 - 벡터 연산
void
part6()
{
	cout << "Exercise 01" << endl;
	glm::vec<3, float> v1(1, 2, 3);
	glm::vec<3, float> v2(3, 2, 1);
	cout << "v1 = " << to_string(v1) << endl;
	cout << "v2 = " << to_string(v2) << endl;

	// 벡터 덧셈 및 뺄셈
	cout << "v1 + v2 = " << to_string(v1 + v2) << endl;
	cout << "v1 - v2 = " << to_string(v1 - v2) << endl;

	// 벡터 부호 반전
	cout << "-v1 = " << to_string(-v1) << endl;

	// 스칼라 곱셈과 벡터 뺄셈
	cout << "v1 - 2 * v2 = " << to_string(v1 - 2.0f * v2) << endl;

	// 내적 및 외적
	cout << "dot(v1, v2) = " << to_string(dot(v1, v2)) << endl;
	cout << "cross(v1, v2) = " << to_string(cross(v1, v2)) << endl;
}

// Part 7: 연습 문제 2 - 행렬 연산
void
part7()
{
	cout << "Exercise 02" << endl;
	glm::vec<3, float> v1(1, 2, 3);
	glm::vec<3, float> v2(3, 2, 1);

	// 행렬 A1과 A2 정의
	glm::mat3 A1(1, 2, 1, 2, 3, 1, 3, 2, 2);
	glm::mat3 A2(2, 2, 1, 1, 2, 1, 2, 1, 1);

	cout << "A1 = " << to_string(A1) << endl;
	cout << "A2 = " << to_string(A2) << endl;

	// 행렬 덧셈 및 뺄셈
	cout << "A1 + A2 = " << to_string(A1 + A2) << endl;
	cout << "A1 - A2 = " << to_string(A1 - A2) << endl;

	// 행렬 부호 반전
	cout << "-A1 = " << to_string(-A1) << endl;

	// 스칼라 곱셈과 행렬 뺄셈
	cout << "A1 - 2 * A2 = " << to_string(A1 - 2.0f * A2) << endl;

	// 행렬 곱셈 및 행렬과 벡터 곱셈
	cout << "A1 * A2 = " << to_string(A1 * A2) << endl;
	cout << "A2 * A1 = " << to_string(A2 * A1) << endl;
	cout << "A1 * v1 = " << to_string(A1 * v1) << endl;
	cout << "A2 * v2 = " << to_string(A2 * v2) << endl;
}

int
main(int argc, char* argv[])
{
	int a;
	cin >> a; // 실행할 파트 선택

	// 입력한 값에 따라 함수 실행
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
