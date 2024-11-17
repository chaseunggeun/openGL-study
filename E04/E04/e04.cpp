#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

#include <iostream>
using namespace std;

void
part1()
{
	cout << "Exercise 01" << endl;
	glm::vec<3, float>	v1(1, 2, 3);
	glm::vec<3, float>	v2(3, 2, 1);
	cout << "v1 = " << to_string(v1) << endl;
	cout << "v2 = " << to_string(v2) << endl;

	cout << "v1 + v2 = " << to_string(v1 + v2) << endl;
	cout << "v1 - v2 = " << to_string(v1 - v2) << endl;

	cout << "-v1 = " << to_string(-v1) << endl;

	cout << "v1 - 2 * v2 = " << to_string(v1 - 2.0f*v2) << endl;
	cout << "dot(v1, v2) = " << to_string(dot(v1, v2)) << endl;
	cout << "cross(v1, v2) = " << to_string(cross(v1, v2)) << endl;

}

void
part2()
{
	cout << "Exercise 02" << endl;
	glm::vec<3, float>	v1(1, 2, 3);
	glm::vec<3, float>	v2(3, 2, 1);

	glm::mat3	A1(1, 2, 1, 2, 3, 1, 3, 2, 2);
	glm::mat3	A2(2, 2, 1, 1, 2, 1, 2, 1, 1);

	cout << "A1 = " << to_string(A1) << endl;
	cout << "A2 = " << to_string(A2) << endl;

	cout << "A1 + A2 = " << to_string(A1 + A2) << endl;
	cout << "A1 - A2 = " << to_string(A1 - A2) << endl;

	cout << "-A1 = " << to_string(-A1) << endl;

	cout << "A1 - 2 * A2 = " << to_string(A1 - 2.0f * A2) << endl;

	cout << "A1 * A2 = " << to_string(A1 * A2) << endl;
	cout << "A2 * A1 = " << to_string(A2 * A1) << endl;
	cout << "A1 * v1 = " << to_string(A1 * v1) << endl;
	cout << "A2 * v2 = " << to_string(A2 * v2) << endl;
}

int
main(int argc, char* argv[])
{
	int e = (argc < 2) ? 1 : atoi(argv[1]);

	switch (e)
	{
	case 1: part1(); break;
	case 2: part2(); break;
	}

	return 0;
}
