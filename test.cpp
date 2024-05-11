#include "construccion/ss.cpp"
#include "plot.cpp"
#include <bits/stdc++.h>
#include <fstream> 
#include <chrono>
#include <cmath>
using namespace std;


int main(){
    int N = pow(2,10);
    vector<point> P = generate_points(N);
    vector<Node*> hojas;
    vector<Node*> internos;
    Node *T;
    hojas.push_back(new Node());
    internos.push_back(new Node());
    T = ss(P, hojas, internos);
    Query q = {{0.5,0.5},0.5};
    long long counter = 0;
    set<point> ss = search(T, q, counter);
    long long ss_counter = counter;
    T = cp(P);
    counter = 0;
    set<point> cp = search(T, q, counter);
    cout<< ss_counter << " - " << counter << endl;
}