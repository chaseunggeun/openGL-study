#include <iostream>

using namespace std;

double arr[4][6] = { {0.5000000, 0.6574145, 0.8292986, 1.0150706, 1.2140877, 1.4256394}, };
double f(double t, double w) {
	return w - t * t + 1;
}

// 기본 오일러 방법
void euler(double alpha, double h, double n) {
	double t = 0.0;
	double w = alpha;

	for (int t_i = 0; t_i <= n * 10; t_i++) {
		arr[1][t_i] = w;
		for (int i = 0; i < 4; i++) {
			w = w + h * f(t, w);
			t += h;
		}
	}
}

// 수정된 오일러 방법
void modifiedEuler(double alpha, double h, double n) {
	double t = 0.0;
	double w = alpha;

	for (int t_i = 0; t_i <= n * 10; t_i++) {
		arr[2][t_i] = w;
		for (int i = 0; i < 2; i++) {
			double w_temp = w + h * f(t, w);
			w = w + h / 2 * (f(t, w) + f(t + h, w_temp));
			t += h;
		}
	}
}

// 4차 룽게-쿠타 방법
void rungeKutta(double alpha, double h, double n) {
	double t = 0.0;
	double w = alpha;

	for (int t_i = 0; t_i <= n * 10; t_i++) {
		arr[3][t_i] = w;
		double k1 = h * f(t, w);
		double k2 = h * f(t + h / 2, w + k1 / 2);
		double k3 = h * f(t + h / 2, w + k2 / 2);
		double k4 = h * f(t + h, w + k3);

		w = w + (k1 + 2 * k2 + 2 * k3 + k4) / 6;
		t += h;
	}
}


int main() {
	euler(0.5, 0.025, 0.5);
	modifiedEuler(0.5, 0.05, 0.5);
	rungeKutta(0.5, 0.1, 0.5);

	// 표 형식으로 배열 출력
	cout << fixed;
	cout.precision(7);
	cout << "t_i\tExact\t\tEuler\t\tModified Euler\tRunge-Kutta" << endl;

	for (int t_i = 0; t_i < 6; t_i++) {
		cout << "0." << t_i << "\t";
		for (int method = 0; method < 4; method++) {
			cout << arr[method][t_i] << "\t";
		}
		cout << endl;
	}
}
