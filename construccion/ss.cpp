#include "../mtree.cpp"
#include "../plot.cpp"
#include <bits/stdc++.h>
using namespace std;

int B = 10000;
int b = B/2;

typedef pair<double,double> point;

double dist(point p, point q){
    return sqrt((p.first - q.first)*(p.first - q.first) + (p.second - q.second)*(p.second - q.second));
}

struct cluster{
    vector<point> points;
    point medoide;

    cluster(const vector<point>& pts) : points(pts) {
        medoide = calculate_medoide();
    }

    // FUNCION PARA ENCONTRAR MEDOIDE
    point calculate_medoide(){
        double best_radius = 1e9;
        point best_medoide = {-1, -1};
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
                best_medoide = p;
            }
        }
    return best_medoide;
    };
};

cluster merge_clusters(cluster C, cluster D){
    vector<point> points;
    for (auto p : C.points){
        points.push_back(p);
    }
    for (auto p : D.points){
        points.push_back(p);
    }
    return cluster(points);
}


// FUNCION PARA ENCONTRAR LA DISTANCIA ENTRE CLUSTERS
double cluster_dist(cluster C, cluster D){
    return dist(C.medoide, D.medoide);
}


// FUNCION PARA ENCONTRAR EL VECINO MAS CERCANO
cluster vecino(vector<cluster> clusters, cluster C){
    double best_dist = 1e9;
    cluster vecino = clusters[0];
    for (cluster D : clusters){
        if (C.points != D.points){
            double d = cluster_dist(C, D);
            if (d < best_dist){
                best_dist = d;
                vecino = D;
            }
        }
    }
    return vecino;
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

double covering_radius(vector<point> points){
    double best_radius = 1e9;
    for(auto p : points){
        double max_radius = 0.0;
        for(auto q : points){
            double d = dist(p, q);
            if(d > max_radius)
                max_radius = d;
        }
        if(max_radius < best_radius)
            best_radius = max_radius;
    }
    return best_radius;
}

//FUNCION MIN MAX SPLIT
pair<cluster, cluster> min_max_split(cluster C){
    cluster C1 = cluster({}), C2 = cluster({});
    pair<vector<point>,vector<point>> best_points;
    double min_radius = 1e9;
    for(auto p : C.points){
        for(auto q : C.points){
            set<pair<double, point>> distancias_p, distancias_q;
            set<point> puntos_agregados;
            for(auto r : C.points){
                distancias_p.insert({dist(p, r), r});
                distancias_q.insert({dist(q, r), r});
                puntos_agregados.insert(r);
            }
            auto it_p = distancias_p.begin();
            auto it_q = distancias_q.begin();
            vector<point> puntos_c1, puntos_c2;
            while(it_p != distancias_p.end() && it_q != distancias_q.end()){
                puntos_c1.push_back(it_p->second);
                puntos_agregados.insert(it_p->second);

                while(puntos_agregados.find(it_q->second) != puntos_agregados.end()){
                    it_q++;
                }

                puntos_c2.push_back(it_q->second);
                puntos_agregados.insert(it_q->second);
                it_p++;
                it_q++;
            }
            double best_radius = max(covering_radius(puntos_c1), covering_radius(puntos_c2));
            if (best_radius < min_radius){
                min_radius = best_radius;
                best_points = {puntos_c1, puntos_c2};
            }
        }
    }
    return {cluster(best_points.first), cluster(best_points.second)};
}


vector<cluster> gen_cluster(cluster Cin){
    vector<cluster> C, Cout;

    for(auto p : Cin.points){
        C.push_back(cluster({p}));
    }

    while(C.size() > 1){
        pair<cluster, cluster> pares_cercanos = par_mas_cercano(C);
        if(pares_cercanos.first.points.size() + pares_cercanos.second.points.size() <= B){
            C.erase(remove(C.begin(), C.end(), pares_cercanos.first), C.end());
            C.erase(remove(C.begin(), C.end(), pares_cercanos.second), C.end());
            C.push_back(merge_clusters(pares_cercanos.first, pares_cercanos.second));
        }else{
            C.erase(remove(C.begin(), C.end(), pares_cercanos.first), C.end());
            Cout.push_back(pares_cercanos.first);
        }
    }

    cluster c_prima = cluster({});
    if(Cout.size() > 0){
        c_prima = vecino(Cout, C[0]);
    }
    cluster c_union = merge_clusters(c_prima, C[0]);
    if(c_prima.points.size() + C[0].points.size() <= B)
        Cout.push_back(c_union);
    else{
        pair<cluster, cluster> split = min_max_split(c_union);
        Cout.push_back(split.first);
        Cout.push_back(split.second);
    }
    return Cout;
};




int main(){
    vector<point> P = {{1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}, {6, 6}};
    return 0;
}