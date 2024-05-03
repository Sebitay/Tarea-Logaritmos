#include <bits/stdc++.h>
using namespace std;


typedef pair<double,double> point;

struct Node;

// Forward declaration of NodeComparator
struct NodeComparator;

typedef set<Node*, NodeComparator> MTree; // Use NodeComparator for set comparison

struct Node{
    point p;
    double radius;
    MTree* a; // Pointer to MTree
};

// Define NodeComparator
struct NodeComparator {
    bool operator()(const Node* lhs, const Node* rhs) const {
        // Compare based on the point coordinates
        if (lhs->p.first != rhs->p.first)
            return lhs->p.first < rhs->p.first;
        return lhs->p.second < rhs->p.second;
    }
};