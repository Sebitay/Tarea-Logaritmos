#include <bits/stdc++.h>
using namespace std;

typedef pair<double, double> point;
typedef set<point> PointSet;
typedef pair<point, double> Query;

struct entry;
struct Node;
struct MTree;

struct entry{
    point p;
    double radius;
    Node* a; // Pointer to Node
};

struct Node{
    vector<entry> entries;

    int height() {
        return heightUtil(entries);
    }

private:
    int heightUtil(vector<entry> entries) {
        int maxHeight = 0;
        for (const entry& e : entries) {
            if (e.a)
                maxHeight = max(maxHeight, 1 + heightUtil(e.a->entries));
        }
        return maxHeight;
    }
};

struct MTree {
    Node* root;

    MTree() : root(nullptr) {}
};

int B = 4096/sizeof(entry);
int b = B/2;

double dist(point p1, point p2) {
    return sqrt(pow(p1.first - p2.first, 2) + pow(p1.second - p2.second, 2));
}