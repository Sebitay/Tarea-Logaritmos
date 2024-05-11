#include "../search.cpp"
#include "../plot.cpp"
#include <bits/stdc++.h>
#include <cmath>
using namespace std;

int B = 4096/sizeof(entry);
int b = B/2;

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

bool operator==(const cluster& lhs, const cluster& rhs) {
    // Compare the points set of each cluster
    return lhs.points == rhs.points;
}

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
    int i = 0;
    for (auto C = clusters.begin(); C != clusters.end(); C++){
        for (auto D = next(C); D != clusters.end(); D++){
            if (C->points != D->points){
                double d = dist(C->medoide, D->medoide);
                if (d < best_dist){
                    best_dist = d;
                    if(C->points.size() < D->points.size()){
                        best_pair = {*D, *C};
                    }
                    else{
                        best_pair = {*C, *D};
                    }
                }
            }
        }
    }
    return best_pair;
}


pair<cluster, cluster> par_mas_cercano_dc(vector<cluster> clusters) {
    // Base case: If there are only two clusters, return the closest pair
    if (clusters.size()<= 3) {
        return par_mas_cercano(clusters);
    }

    // Sort clusters based on medoide's x-coordinate
    sort(clusters.begin(), clusters.end(), [](const cluster& a, const cluster& b) {
        return a.medoide.first < b.medoide.first;
    });

    // Divide the clusters into two halves
    int mid = clusters.size() / 2;
    vector<cluster> left_clusters(clusters.begin(), clusters.begin() + mid);
    vector<cluster> right_clusters(clusters.begin() + mid, clusters.end());
    

    // Recursively find closest pair of clusters in each half
    pair<cluster, cluster> closest_left = par_mas_cercano_dc(left_clusters);
    pair<cluster, cluster> closest_right = par_mas_cercano_dc(right_clusters);


    // Find the closest pair that spans both halves
    double min_dist = min(cluster_dist(closest_left.first, closest_left.second),
                          cluster_dist(closest_right.first, closest_right.second));


    // Find the clusters within the strip
    vector<cluster> strip;
    for (const auto& c : clusters) {
        if (abs(c.medoide.first - clusters[mid].medoide.first) < min_dist) {
            strip.push_back(c);
        }
    }

    // Find the closest pair in the strip
    pair<cluster, cluster> closest_in_strip = {cluster({}), cluster({})};
    min_dist = DBL_MAX;
    if (!strip.empty()) {
        for (size_t i = 0; i < strip.size(); ++i) {
            for (size_t j = i + 1; j < strip.size() && strip[j].medoide.second - strip[i].medoide.second < min_dist; ++j) {
                double dist = cluster_dist(strip[i], strip[j]);
                if (dist < min_dist) {
                    min_dist = dist;
                    closest_in_strip = {strip[i], strip[j]};
                }
            }
            // Early termination if min_dist becomes 0
            if (min_dist == 0) {
                break;
            }
        }
    }

    // Return the closest pair among the three possibilities
    if (min(cluster_dist(closest_left.first, closest_left.second), cluster_dist(closest_right.first, closest_right.second)) < min_dist) {
        return min(cluster_dist(closest_left.first, closest_left.second), cluster_dist(closest_right.first, closest_right.second)) == cluster_dist(closest_left.first, closest_left.second) ? closest_left : closest_right;
    } else {
        return closest_in_strip;
    }
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
    int i = 0;
    for(auto p = C.points.begin(); p != C.points.end(); p++){
        for(auto q = next(p); q != C.points.end(); q++){
            if(p == q)
                break;
            set<pair<double, point>> distancias_p, distancias_q;
            set<point> puntos_agregados;
            for(auto r : C.points){
                if (r != *p)
                    distancias_p.insert({dist(*p, r), r});
                if (r != *q)
                    distancias_q.insert({dist(*q, r), r});
                puntos_agregados.insert(r);
            }
            auto it_p = distancias_p.begin();
            auto it_q = distancias_q.begin();
            vector<point> puntos_c1, puntos_c2;
            while(it_p != distancias_p.end() && it_q != distancias_q.end()){
                puntos_c1.push_back(it_p->second);
                puntos_agregados.insert(it_p->second);
                
                while(puntos_agregados.find(it_q->second) != puntos_agregados.end() && it_q != distancias_q.end()){
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
        pair<cluster, cluster> pares_cercanos = par_mas_cercano_dc(C);
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
    if(c_prima.points.size() + C[0].points.size() <= B){
        Cout.push_back(c_union);
    }
    else{
        cout<< "Min max split policy" << endl;
        pair<cluster, cluster> split = min_max_split(c_union);
        Cout.push_back(split.first);
        Cout.push_back(split.second);
    }
    return Cout;
};


entry output_hoja(cluster Cin, Node *hoja){
    double r = 0;
    for (auto p : Cin.points){
        hoja->entries.push_back({p, 0, nullptr});
        r = max(r, dist(p, Cin.medoide));
    }
    return entry{Cin.medoide, r, hoja};
}


entry output_interno(Node Cmra, Node *interno){
    vector<point> Cin;
    double R = 0;
    vector<entry> entries = Cmra.entries;
    for (auto entry : entries)
        Cin.push_back(entry.p);
    cluster Cin_cluster = cluster(Cin);
    for (auto entry : entries){
        R = max(R, dist(entry.p, Cin_cluster.medoide) + entry.radius);
        interno->entries.push_back({entry.p, entry.radius, entry.a});
    }
    return entry{Cin_cluster.medoide, R, interno};
}

Node *ss(vector<point> P, vector<Node*>& hojas, vector<Node*>& internos){
    cout<< "Inicio" << endl;
    cluster Cin = cluster(P);
    int hoja_i = 0, interno_i = 0;
    if (Cin.points.size() <= B){
        output_hoja(Cin, hojas[hoja_i]);
        return hojas[hoja_i];
    }
    cout<< "Generacion de cluster" << endl;
    vector<cluster> Cout = gen_cluster(Cin);
    Node C = Node();

    for (cluster c : Cout){
        C.entries.push_back(output_hoja(c, hojas[hoja_i]));
        hojas.push_back(new Node());
        hoja_i++;
    }

    while (C.entries.size() > B){
        vector<Node> Cmra;
        vector<point> Cin;
        for (auto entry : C.entries)
            Cin.push_back(entry.p);
        Cout = gen_cluster(cluster(Cin));
        for (auto c : Cout){
            Node s = Node();
            for (auto entry : C.entries){
                for(auto point : c.points){
                    if(point == entry.p)
                        s.entries.push_back(entry);
                }
            }
            Cmra.push_back(s);
        }
        C.entries.clear();
        for(auto node : Cmra){
            C.entries.push_back(output_interno(node, internos[interno_i]));
            internos.push_back(new Node());
            interno_i++;
        }
    }
    output_interno(C, internos[interno_i]);
    return internos[interno_i];
}