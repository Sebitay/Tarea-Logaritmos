#include <bits/stdc++.h>
#include <iostream>
#include "mtree.cpp"
using namespace std;

PointSet search(Node *T, Query q, long long& counter) {
    counter++;
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
                PointSet childResult = search(e.a, q, counter);
                result.insert(childResult.begin(), childResult.end());
            }
        }
    }
    return result;
}