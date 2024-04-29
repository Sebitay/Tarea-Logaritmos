#include <bits/stdc++.h>
using namespace std;


struct Node;

struct MTree{
    vector<Node> nodes;
};

struct Node{
    pair<double,double> p;
    double radius;
    MTree *a;
};

int main(){
    return 0;
}