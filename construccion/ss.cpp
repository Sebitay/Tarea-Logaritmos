#include "../mtree.cpp"
#include "../plot.cpp"
#include <bits/stdc++.h>
using namespace std;

int B = 10000;
int b = B/2;

double dist(pair<double, double> p, pair<double, double> q){
    return sqrt((p.first - q.first)*(p.first - q.first) + (p.second - q.second)*(p.second - q.second));
}

struct cluster{
    vector<pair<double, double>> points;
    pair<double, double> medoide;

    cluster(const vector<pair<double, double>>& pts) : points(pts) {
        medoide = calculate_medoide();
    }

    // FUNCION PARA ENCONTRAR MEDOIDE
    pair<double, double> calculate_medoide(){
        double best_radius = 1e9;
        pair<double, double> *best_medoide;
        for (auto p : points){
            double max = 0.0;
            for (auto q : points){
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
    };
};


// FUNCION PARA ENCONTRAR LA DISTANCIA ENTRE CLUSTERS
double cluster_dist(cluster C, cluster D){
    return dist(C.medoide, D.medoide);
}


// FUNCION PARA ENCONTRAR EL VECINO MAS CERCANO
cluster vecino(vector<cluster> clusters, cluster C){
    double best_dist = 1e9;
    cluster *vecino;
    for (cluster D : clusters){
        if (C.points != D.points){
            double d = cluster_dist(C, D);
            if (d < best_dist){
                best_dist = d;
                vecino = &D;
            }
        }
    }
    return *vecino;
}


// FUNCION PARA ENCONTRAR EL PAR DE CLUSTERS MAS CERCANOS
pair<cluster, cluster> par_mas_cercano(vector<cluster> clusters){
    double best_dist = cluster_dist(clusters[0], clusters[1]);
    pair<cluster, cluster> best_pair = {clusters[0], clusters[1]};
    for (auto C : clusters){
        for (auto D : clusters){
            if (C.points != D.points){
                double d = cluster_dist(C, D);
                if (d < best_dist){
                    best_dist = d;
                    if(C.points.size() < D.points.size())
                        best_pair = {D, C};
                    else
                        best_pair = {C, D};
                }
            }
        }
    }
    return best_pair;
}


MTree ss(cluster Cin){
    vector<cluster> C;
    for(auto p : Cin.points){
        C.push_back(cluster({p}));
    }
    while(C.size() > 1){
        pair<cluster, cluster> pares_cercanos = par_mas_cercano(C);

    }
};