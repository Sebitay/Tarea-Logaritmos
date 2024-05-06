#include <bits/stdc++.h>
#include <iostream>
#include <string>
#include "construccion/cp.cpp"
#include "construccion/ss.cpp"
// #include "search.cpp"
using namespace std;

typedef pair<double, double> point;
typedef set<point> PointSet;

int main(int argc, char* argv[]) {

    if (argc != 3) {
        cout << "Usage: " << argv[0] << " <construction_function> <p>" << endl;
        return 1;
    }

    // generar puntos:
    int p = atoi(argv[2]);
    cout<<"p: "<<p<<endl;
    int N = pow(2, p);
    PointSet points = generate_points(N);

    // construir el árbol:
    string f = argv[1];
    if (f == "cp") {
        MTree* T = cp(points);
    } else if (f == "ss") {
        //MTree* T = ss(points);
    } else {
        cout<<"Invalid construction function, use cp or ss"<<endl;
        return 1;
    }

    // hacer consultas:
    /* PointSet Q = generate_points(100);
    int r = 0.02;
    for (auto q : Q) {
        Query query = make_pair(q, r);
        PointSet result = search(T, query);
    } */

    return 0;
}