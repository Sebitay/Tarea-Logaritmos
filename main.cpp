#include <bits/stdc++.h>
#include <iostream>
#include <string>
#include "construccion/cp.cpp"
#include "construccion/ss.cpp"
// #include "search.cpp"
using namespace std;

typedef pair<double, double> point;
typedef set<point> PointSet;
typedef pair<point, double> Query;

double dist(point p1, point p2) {
    return sqrt(pow(p1.first - p2.first, 2) + pow(p1.second - p2.second, 2));
}

PointSet search(Node *T, Query q) {
    PointSet result;
    // iterar sobre las entradas del nodo
    int i = 1;
    for (entry e : T->entries) {
        // si el nodo es una hoja
        if (!e.a) {
            if (dist(e.p, q.first) <= q.second) {
                result.insert(e.p);
            }
        } else {
            if (dist(e.p, q.first) <= e.radius + q.second) {
                PointSet childResult = search(e.a, q);
                result.insert(childResult.begin(), childResult.end());
            }
        }
    }

    return result;

}

int main(int argc, char* argv[]) {

    if (argc != 3) {
        cout << "Usage: " << argv[0] << " <construction_function> <p>" << endl;
        return 1;
    }

    // generar puntos:
    int p = atoi(argv[2]);
    int N = pow(2, p);
    PointSet points = generate_points(N);

    // construir el árbol:
    auto start = std::chrono::high_resolution_clock::now();
    string f = argv[1];
    MTree T;
    if (f == "cp") {
        T = cp(points);
    } else if (f == "ss") {
        //MTree* T = ss(points);
    } else {
        cout<<"Invalid construction function, use cp or ss"<<endl;
        return 1;
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    auto hours = std::chrono::duration_cast<std::chrono::hours>(duration);
    duration -= hours;
    auto minutes = std::chrono::duration_cast<std::chrono::minutes>(duration);
    duration -= minutes;
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration);
    cout << "Tiempo de construccion del arbol: " << hours.count() << " horas, " << minutes.count() << " minutos y " << seconds.count() << " segundos" << std::endl;

    // iterar sobre todo el arbol, mostrando su radio cobertor:
    stack<Node*> st;
    st.push(T.root);
    while (!st.empty()) {
        Node* n = st.top();
        st.pop();
        for (entry e : n->entries) {
            cout << "Radio cobertor: " << e.radius << endl;
            cout<<"a: "<<e.a<<endl;
            if (e.a) {
                st.push(e.a);
            }
        }
    }

    // hacer consultas:
    /* PointSet Q = generate_points(100);
    int r = 0.02;
    for (auto q : Q) {
        Query query = make_pair(q, r);
        PointSet result = search(T, query);
    } */
    Query q = {{0.5, 0.5}, 0.5};
    set<point> s = search(T.root, q);
    cout << "result = "<< s.size() <<", expected = " << N*3.14*0.25 << endl;

    cout<<"Done!"<<endl;

    return 0;
}