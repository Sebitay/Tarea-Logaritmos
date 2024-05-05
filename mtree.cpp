#include <bits/stdc++.h>
using namespace std;

typedef pair<double,double> point;

// Forward declaration
struct Node;

struct entry{
    point p;
    double radius;
    Node* a; // Pointer to Node
};

struct Node{
    vector<entry> entries;
};

struct MTree {
    Node* root;

    MTree() : root(nullptr) {}

    int height() {
        if (!root) return 0;
        return heightUtil(root);
    }

private:
    int heightUtil(Node* node) {
        if (!node) return 0;
        int maxHeight = 0;
        for (const entry& e : node->entries) {
            if (e.a)
                maxHeight = max(maxHeight, 1 + heightUtil(e.a));
        }
        return maxHeight;
    }
};

