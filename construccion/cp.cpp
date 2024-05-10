#include <bits/stdc++.h>
#include <iostream>
#include "../mtree.cpp"
#include "../plot.cpp"
using namespace std;

typedef pair<double, double> point;
typedef set<point> PointSet;

int B = 4096/sizeof(entry);
int b = B/2;


double distance(point p1, point p2) {
    return sqrt(pow(p1.first - p2.first, 2) + pow(p1.second - p2.second, 2));
}

void setCoveringRadius(Node& node) {
    for (auto& e : node.entries) {
        if (e.a) {
            // cout<<"setCoveringRadius"<<endl;
            double max_dist = 0;
            for (auto& e2 : e.a->entries) {
                double dist = distance(e.p, e2.p);
                max_dist = max(max_dist, dist);
            }
            // cout<<"max_dist: "<<max_dist<<endl;
            e.radius = max_dist;
            setCoveringRadius(*e.a);
        }
    }
}

vector<MTree> subtrees_h(Node& node, int h) {
    // crear una cola
    queue<Node*> q;
    vector<MTree> subtrees;
    q.push(&node);
    while (!q.empty()) {
        // sacar el primer elemento de la cola
        for (auto& e : q.front()->entries) {
            if (e.a) {
                MTree T = MTree();
                T.root = e.a;
                int height = T.height();
                if (height != h) {
                    // añadir a la cola
                    q.push(e.a);
                } else {
                    subtrees.push_back(T);
                }      
            }
        }
        q.pop();
    }
    return subtrees;
}

MTree cp(PointSet P) {
    // Si |P| ≤ B, se crea un árbol T, se insertan todos los puntos a T y se retorna T:
    if (P.size() <= B) {
        MTree* T = new MTree;
        T->root = new Node;
        for (auto& p : P) {
            entry e;
            e.p = p;
            e.radius = 0;
            e.a = nullptr;
            T->root->entries.push_back(e);
        }
        return *T;
    }
    int k = min(B, static_cast<int>(ceil((P.size() / (double)B))));
    vector<point> F;
    vector<PointSet> Fk(k);
    do{
        // 2. De manera aleatoria se eligen k = min(B, n/B) puntos de P, que los llamaremos samples
        // pf1, . . . , pfk. Se insertan en un conjunto F de samples.
        F.clear();
        for (int i = 0; i < k; i++) {
            if(Fk[i].size() > 0)
                Fk[i].clear();
            int idx = rand() % P.size();
            auto it = next(P.begin(), idx);
            F.push_back(*it);
        }

        // 3. Se le asigna a cada punto en P su sample más cercano. Con eso se puede construir k conjuntos
        // F1, . . . , Fk.
        for (auto p : P) {
            double min_dist = numeric_limits<double>::max();
            int idx = -1;
            int i = 0;
            for (auto& Fp : F) {
                // calcular la distancia entre p y F[i]:
                double dist = distance(p, Fp);

                // actualizar el mínimo:
                if (dist < min_dist) {
                    min_dist = dist;
                    idx = i;
                }
                i++;
            }
            // añadir p a Fk[idx]:
            Fk[idx].insert(p);
            
        }
        // 4. Etapa de redistribución: Si algún Fj es tal que |Fj| < b:
        for (int i = 0; i < k; i++) {
            if (Fk[i].size() < b) {
                // 4.1 Quitamos pfj de F
                F.erase(remove(F.begin(), F.end(), F[i]));

                // 4.2 Por cada p ∈ Fj, le buscamos el sample pfl más cercano de F y lo añadimos a su conjunto Fl.
                for (auto p : Fk[i]) {
                    double min_dist = numeric_limits<double>::max();
                    int idx = -1;
                    for (int j = 0; j < k; j++) {
                        if (j == i) continue; // no considerar el mismo sample
                        // calcular la distancia entre p y F:
                        double dist = distance(p, F[j]);
                        // actualizar el mínimo:
                        if (dist < min_dist && dist != 0) {
                            min_dist = dist;
                            idx = j;
                        }
                    }
                    // añadir p a Fk[idx]:
                    Fk[idx].insert(p);

                }
                // Vaciamos F
                Fk[i].clear();
            }
        }
    } while(F.size() == 1);

    // 6. Se realiza recursivamente el algoritmo CP en cada Fj, obteniendo el árbol Tj
    vector<MTree> Tk;
    for (int j = 0; j < k; j++) {
        if (Fk[j].size() == 0) continue;
        MTree Tj = cp(Fk[j]);
        Tk.push_back(Tj);
  
        // 7. Si la raíz del árbol es de un tamaño menor a b, se quita esa raíz, se elimina pfj de F y se trabaja
        // con sus subárboles como nuevos Tj, se añaden los puntos pertinentes a F.
        // revisar
        if (Tj.root->entries.size() < b) {
            // Se elimina pfj de F
            F.erase(remove(F.begin(), F.end(), F[j]));
            // Se trabaja con los subárboles de Tj como nuevos Tj
            for (auto& e : Tj.root->entries) {
                Tk.push_back(cp(Fk[j]));
            }
            // Se añaden los puntos pertinentes a F
            // (revisar si estos son los puntos pertienentes)
            for (auto& e : Tj.root->entries) {
                F.push_back(e.p);
            }
            // Se quita la raíz de Tj
            Tj.root = nullptr;
        }
    }   

    // 8. Etapa de balanceamiento: Se define h como la altura mínima de los árboles Tj. Se define T′
    // inicialmente como un conjunto vacío.
    /* cout<<"paso 8"<<endl; */
    int h = numeric_limits<int>::max();
    for (int j = 0; j < k; j++) {
        h = min(h, Tk[j].height());
    }
    //cout<<"h: "<<h<<endl; 
    // definir T'
    vector<MTree> Tprime;

    // 9. Por cada Tj, si su altura es igual a h, se añade a T′.
    // Si no, 9.1, 9.2 y 9.3
    /* cout<<"paso 9"<<endl; */
    for (int j = 0; j < k; j++) {
        if (Tk[j].height() == h) {
            Tprime.push_back(Tk[j]);
        } else {
            // 9.1 Se borra el punto pertinente en F.
            F.erase(remove(F.begin(), F.end(), F[j]));

            // 9.2 Se hace una búsqueda exhaustiva en Tj de todos los subárboles T1', . . . , Tp′ de altura igual
            // a h. Se insertan estos árboles a T′
            // split Tj into p sub-trees T1 ,..., Tp of height hmi
            vector<MTree> Tp = subtrees_h(*Tk[j].root, h);
            for (auto& T : Tp) {
                Tprime.push_back(T);
            }

            // 9.3 Se añaden los puntos pertinentes a F.
            for (auto& T : Tp) {
                for (auto& e : T.root->entries) {
                    F.push_back(e.p);
                }
            }
        }
    }

    // 10. Se define Tsup como el resultado de la llamada al algoritmo CP aplicado a F.
    /* cout<<"paso 10"<<endl; */
    PointSet F_set(F.begin(), F.end());
    MTree Tsup = cp(F_set);

    // 11. Se une cada Tj ∈ T′ a su hoja en Tsup correspondiente al punto pfj ∈ F, obteniendo un nuevo árbol T.
    /* cout<<"paso 11"<<endl; */
    MTree *T = new MTree;
    vector<entry> entries;
    for (auto& e : Tsup.root->entries) {
        entry new_entry;
        new_entry.p = e.p;
        new_entry.radius = e.radius;
        new_entry.a = nullptr;  
        // buscar la posicion de e.p en F
        auto it = find(F.begin(), F.end(), new_entry.p);
        // poner como hijo el subarbol correspondiente
        new_entry.a = Tk[distance(F.begin(), it)].root;        
    }
    T->root = Tsup.root;

    // 12. Se setean los radios cobertores resultantes para cada entrada en este árbol.
    /* cout<<"paso 12"<<endl; */
    setCoveringRadius(*(T->root));

    // 13. Se retorna T
    /* cout<<"paso 13"<<endl; */
    return *T;
}