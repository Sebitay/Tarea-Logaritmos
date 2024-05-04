#include <bits/stdc++.h>
using namespace std;

typedef pair<double,double> point;

struct entry;

struct Node;

struct MTree {
    Node* root;

    MTree() : root(nullptr) {}
};

struct Node{
    vector<entry> entries;
};

struct entry{
    point p;
    double radius;
    MTree* a; // Pointer to MTree
};