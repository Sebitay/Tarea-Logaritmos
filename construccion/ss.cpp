#include "../mtree.cpp"
#include "../plot.cpp"
#include <bits/stdc++.h>
using namespace std;

int B = 10000;
int b = B/2;

typedef vector<pair<double, double>> cluster;

double dist(pair<double, double> p, pair<double, double> q){
    return sqrt((p.first - q.first)*(p.first - q.first) + (p.second - q.second)*(p.second - q.second));
}

pair<double, double> medoide(cluster C){
    double best_radius = 1e9;
    pair<double, double> *best_medoide;
    for (auto p : C){
        double max = 0.0;
        for (auto q : C){
            double d = dist(p, q);
            if (d > max){
                max = d;
            }
        }
        if (max < best_radius){
            best_radius = max;
            best_medoide = &p;
        }
    }
    return *best_medoide;
}

int medoide_pos(cluster C){
    double best_radius = 1e9;
    int best_i = 0;
    int i = 0;
    for (auto p : C){
        double max = 0.0;
        for (auto q : C){
            double d = dist(p, q);
            if (d > max){
                max = d;
            }
        }
        if (max < best_radius){
            best_radius = max;
            
            best_i = i;
        }
        i++;
    }
    return best_i;
}





cluster gen_cluster(cluster Cin){
    vector<cluster> C;
    for(auto p : Cin){
        C.push_back({p});
    }
    while(C.size() > 1){
        
    }
}