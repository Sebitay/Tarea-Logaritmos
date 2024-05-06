#include "../plot.cpp"
#include "../search.cpp"
#include <bits/stdc++.h>
#include <cmath>
using namespace std;

int B = 4096/sizeof(entry);
int b = B/2;

struct cluster{
    set<point> points;
    point medoide;

    cluster(const set<point>& pts) : points(pts) {
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
    set<point> points;
    for (auto p : C.points){
        points.insert(p);
    }
    for (auto p : D.points){
        points.insert(p);
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



double covering_radius(set<point> points){
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
    pair<set<point>,set<point>> best_points;
    double min_radius = 1e9;
    int i = 0;
    cout << C.points.size() << endl;
    for(auto p = C.points.begin(); p != C.points.end(); p++){
        cout<< i++ << endl;
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
            set<point> puntos_c1, puntos_c2;
            while(it_p != distancias_p.end() && it_q != distancias_q.end()){
                puntos_c1.insert(it_p->second);
                puntos_agregados.insert(it_p->second);
                
                while(puntos_agregados.find(it_q->second) != puntos_agregados.end() && it_q != distancias_q.end()){
                    it_q++;
                }

                puntos_c2.insert(it_q->second);
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
    cout<< '1' << endl;
    for(auto p : Cin.points){
        C.push_back(cluster({p}));
    }
    cout<< '2' << endl;
    while(C.size() > 1){
        cout<< C.size() << endl;
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
    cout<< '3' << endl;

    cluster c_prima = cluster({});
    if(Cout.size() > 0){
        c_prima = vecino(Cout, C[0]);
    }
    cout<< '4' << endl;
    cluster c_union = merge_clusters(c_prima, C[0]);
    cout<< '5' << endl;
    if(c_prima.points.size() + C[0].points.size() <= B){
        Cout.push_back(c_union);
        cout<< "6.1" << endl;
    }
    else{
        pair<cluster, cluster> split = min_max_split(c_union);
        cout<< "6.2" << endl;
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
    hoja->is_leaf = true;
    return entry{Cin.medoide, r, hoja};
}


entry output_interno(Node Cmra, Node *interno){
    set<point> Cin;
    double R = 0;
    vector<entry> entries = Cmra.entries;
    for (auto entry : entries)
        Cin.insert(entry.p);
    cluster Cin_cluster = cluster(Cin);
    for (auto entry : entries){
        R = max(R, dist(entry.p, Cin_cluster.medoide) + entry.radius);
        interno->entries.push_back({entry.p, entry.radius, entry.a});
        if(entry.a->is_leaf)
            cout << "hoja " <<entry.a->entries.size() << endl;
        else
            cout << "interno " <<entry.a->entries.size() << endl;
    }
    return entry{Cin_cluster.medoide, R, interno};
}

Node *ss(set<point> P, vector<Node*>& hojas, vector<Node*>& internos){
    cluster Cin = cluster(P);
    int hoja_i = 0, interno_i = 0;
    if (Cin.points.size() <= B){
        output_hoja(Cin, hojas[hoja_i]);
        return hojas[hoja_i];
    }
    vector<cluster> Cout = gen_cluster(Cin);
    Node C = Node();

    cout << "--- creacion hojas ---" << endl;
    for (cluster c : Cout){
        C.entries.push_back(output_hoja(c, hojas[hoja_i]));
        cout << "actual " << C.entries[hoja_i].a->entries.size() << " - primero " << C.entries[0].a->entries.size() << endl;
        hojas.push_back(new Node());
        hoja_i++;
    }
    cout << "----------------------" << endl;

    int i = 0;
    cout << "--- hojas en C ---" << endl;
    for (auto entry : C.entries){
        cout << "en C "<< entry.a->entries.size() << endl;
        cout << "hoja "<< hojas[i]->entries.size() << endl;
        i++;
    }
    cout << "------------------" << endl;


    cout<< '6' << endl;
    while (C.entries.size() > B){
        vector<Node> Cmra;
        set<point> Cin;
        for (auto entry : C.entries)
            Cin.insert(entry.p);
        cout<< '7' << endl;
        Cout = gen_cluster(cluster(Cin));
        cout<< '8' << endl;
        for (auto c : Cout){
            Node s = Node();
            for (auto entry : C.entries){
                if (c.points.find(entry.p) != c.points.end())
                    s.entries.push_back(entry);
            }
            Cmra.push_back(s);
        }
        cout<< '9' << endl;
        C.entries.clear();
        for(auto node : Cmra){
            cout << "--- output_interno --- " << endl;
            C.entries.push_back(output_interno(node, internos[interno_i]));
            internos.push_back(new Node());
            interno_i++;
            cout << "---------------------- " << endl;
        }
        cout << "10" << endl;
    }
    cout << "11" << endl;
    output_interno(C, internos[interno_i]);
    cout << "12" << endl;
    cout << internos[interno_i]->entries.size() << endl;
    return internos[interno_i];
}

int main(){
    cout<< sizeof(entry) << endl;
    int N = pow(2,12);
    set<point> P = generate_points(N);
    vector<Node*> hojas;
    vector<Node*> internos;
    MTree T;
    hojas.push_back(new Node());
    internos.push_back(new Node());
    T.root = ss(P, hojas, internos);
    cout << T.root->entries.size() << endl;
    Query q = {{0.5, 0.5}, 0.5};
    set<point> s = search(T.root, q);
    cout << "result = "<< s.size() <<", expected = " << N*3.14*0.25 << endl;
    return 0;
}