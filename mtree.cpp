#include <bits/stdc++.h>
using namespace std;

struct Node{
    pair<double,double> p;
    double radius;
    MTree *a;
};

struct MTree{
    vector<Node> nodes;
};