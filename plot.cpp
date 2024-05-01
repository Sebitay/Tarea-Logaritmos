#include <bits/stdc++.h>
using namespace std;


double generate_random_coordinate() {
    static random_device rd;
    static mt19937 gen(rd());
    static uniform_real_distribution<> dis(0.0, 1.0);
    return dis(gen);
}

// Función para generar el conjunto de puntos P
vector<pair<double, double>> generate_points(int n) {
    vector<pair<double, double>> points;
    points.reserve(n);
    for (int i = 0; i < n; ++i) {
        double x = generate_random_coordinate();
        double y = generate_random_coordinate();
        points.emplace_back(x, y);
    }
    return points;
}

int main(){
    int P = 100;
    vector<pair<double, double>> points = generate_points(P);
    for(int i =0; i < P; i++){
        cout<<points[i].first<<'-'<<points[i].second<<endl;
    }
    return 0;
}