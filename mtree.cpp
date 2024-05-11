#include <bits/stdc++.h>
using namespace std;

typedef pair<double, double> point;
typedef set<point> PointSet;
typedef pair<point, double> Query;

struct entry;
struct Node;

struct MTree{
    Node * root;
};

struct Node{
    vector<entry> entries;
    bool is_leaf = false;
};

struct entry{
    point p;
    double radius;
    Node* a; // Pointer to Node
};