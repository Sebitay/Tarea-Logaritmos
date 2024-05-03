#include <bits/stdc++.h>
using namespace std;

typedef pair<double,double> point;

struct MTree {
    Node* root;
};

struct Node{
    vector<entry> entries;
};

struct entry{
    point p;
    double radius;
    MTree* a; // Pointer to MTree
};