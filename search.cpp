#include <bits/stdc++.h>
#include <iostream>
#include "mtree.cpp"
using namespace std;

typedef pair<double, double> point;
typedef set<point> PointSet;
typedef pair<point, double> Query;

double dist(point p1, point p2) {
    return sqrt(pow(p1.first - p2.first, 2) + pow(p1.second - p2.second, 2));
}

PointSet search(Node *T, Query q) {
    PointSet result;
    return result;
}