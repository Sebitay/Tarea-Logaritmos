#include <bits/stdc++.h>
using namespace std;


double generate_random_number(double min, double max) {
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> dis(min, max);
    return dis(gen);
}

// Función para generar el conjunto de puntos P
set<pair<double, double>> generate_points(int n) {
    set<pair<double, double>> points;
    for (int i = 0; i < n; ++i) {
        double x = generate_random_number(0,1);
        double y = generate_random_number(0,1);
        points.insert({x, y});
    }
    return points;
}