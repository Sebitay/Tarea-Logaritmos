#include <bits/stdc++.h>
#include <iostream>
#include "mtree.cpp"
using namespace std;

double dist(point p1, point p2) {
    return sqrt(pow(p1.first - p2.first, 2) + pow(p1.second - p2.second, 2));
}

PointSet search(Node *T, Query q) {
    PointSet result;
    // iterar sobre las entradas del nodo
    int i = 1;
    if (T->is_leaf)
        cout << "es hoja" << endl;
    else
        cout << "no es hoja" << endl;
    for (entry e : T->entries) {
        // si el nodo es una hoja
        if (!e.a) {
            if (dist(e.p, q.first) <= q.second) {
                result.insert(e.p);
                cout << "a" << i++ << endl;
            }
        } else {
            if (dist(e.p, q.first) <= e.radius + q.second) {
                cout << "b" << i++ << endl;
                PointSet childResult = search(e.a, q);
                result.insert(childResult.begin(), childResult.end());
            }
        }
    }

    return result;

}