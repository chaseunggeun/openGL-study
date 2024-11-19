#include <iostream>
#include <cmath>
#include <iomanip>

using namespace std;

double exactSolution(double t) {
    return t - exp(-5 * t);
}
double f(double t, double y) {
    return 5 * exp(5 * t) * pow(y - t, 2) + 1;
}
double df(double t, double y) {
    return 10 * exp(5 * t) * (y - t);
}

void trapezoidalMethod(double y0, double h, int steps, double tol, int maxIter) {
    double t = 0.0;
    double y = y0;

    cout << "Trapezoidal Method (h = " << h << "):" << endl;
    cout << "t\t\tApproximation\t\tError" << endl;
    cout << fixed << setprecision(7);

    for (int i = 0; i <= steps; i++) {
        double exact = exactSolution(t);
        double error = fabs(y - exact);

        cout << t << "\t" << y << "\t\t" << error << endl;

        double alpha = y;
        double w = alpha;

        // Newton's Method를 통해 w1 계산
        for (int j = 0; j < maxIter; j++) {
            double F = w - alpha - (h / 2) * (f(t, alpha) + f(t + h, w));
            double F_prime = 1 - (h / 2) * df(t + h, w);

            double w_new = w - F / F_prime;

            if (fabs(w_new - w) < tol) {
                w = w_new;
                break;
            }
            w = w_new;
        }

        y = w;
        t += h;
    }
    cout << endl;
}

// Runge-Kutta 4차 방법
void rungeKutta4(double y0, double h, int steps) {
    double t = 0.0;
    double y = y0;

    cout << "Runge-Kutta Method (h = " << h << "):" << endl;
    cout << "t\t\tApproximation\t\tError" << endl;
    cout << fixed << setprecision(7);

    for (int i = 0; i <= steps; i++) {
        double exact = exactSolution(t);
        double error = fabs(y - exact);

        cout << t << "\t" << y << "\t\t" << error << endl;

        double k1 = h * f(t, y);
        double k2 = h * f(t + h / 2, y + k1 / 2);
        double k3 = h * f(t + h / 2, y + k2 / 2);
        double k4 = h * f(t + h, y + k3);

        y = y + (k1 + 2 * k2 + 2 * k3 + k4) / 6;
        t += h;
    }
    cout << endl;
}

int main() {
    double y0 = -1.0; // 초기값 y(0)
    double tol = 0.000001; // TOL
    int maxIter = 10; // M = 10 반복 제한

    // h = 0.2, N = 5
    rungeKutta4(y0, 0.2, 5);
    trapezoidalMethod(y0, 0.2, 5, tol, maxIter);

    // h = 0.25, N = 4
    rungeKutta4(y0, 0.25, 4);
    trapezoidalMethod(y0, 0.25, 4, tol, maxIter);

    return 0;
}
