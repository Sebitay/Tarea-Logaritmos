#include <bits/stdc++.h>
using namespace std;


double generate_random_coordinate() {
    static random_device rd;
    static mt19937 gen(rd());
    static uniform_real_distribution<> dis(0.0, 1.0);
    return dis(gen);
}

// Función para generar el conjunto de puntos P
set<pair<double, double>> generate_points(int n) {
    set<pair<double, double>> points;
    for (int i = 0; i < n; ++i) {
        double x = generate_random_coordinate();
        double y = generate_random_coordinate();
        points.insert({x, y});
    }
    return points;
}