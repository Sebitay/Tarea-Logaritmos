#include <bits/stdc++.h>
#include <iostream>
#include "../mtree.cpp"
#include "../plot.cpp"
using namespace std;

typedef pair<double, double> point;
typedef set<point> PointSet;

int B = 4096/sizeof(point);
int b = B/2;


double distance(point p1, point p2) {
    return sqrt(pow(p1.first - p2.first, 2) + pow(p1.second - p2.second, 2));
}

MTree* cp(PointSet P) {
    // Si |P| ≤ B, se crea un árbol T, se insertan todos los puntos a T y se retorna T:
    cout<< "inicio" << endl;
    if (P.size() <= B) {
        cout<<"paso 1"<<endl;
        MTree* T = new MTree;
        T->root = new Node;
        for (auto& p : P) {
            entry e;
            e.p = p;
            e.radius = 0;
            e.a = nullptr;
            T->root->entries.push_back(e);
        }
        return T;
    }
    int k = min(B, static_cast<int>(P.size() / B));
    vector<point> F;
    vector<PointSet> Fk(k);
    do{
        cout<< "paso 2" << endl;
        // 2. De manera aleatoria se eligen k = min(B, n/B) puntos de P, que los llamaremos samples
        // pf1, . . . , pfk. Se insertan en un conjunto F de samples.
        F.clear();
        for (int i = 0; i < k; i++) {
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
        // 4. Etapa de redistribución: Si algún Fj es tal que |Fj| < B:
        for (int i = 0; i < k; i++) {
            if (Fk[i].size() < B) {
                cout<< "paso 4"<< endl;
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
    }while(F.size() == 1);

    // 6. Se realiza recursivamente el algoritmo CP en cada Fj, obteniendo el árbol Tj
    vector<MTree> Tk(k);
    cout<< "paso 6: k=" << k << endl;
    for (int j = 0; j < k; j++) {
        cout<< Fk[j].size() << endl;
        if (Fk[j].size() == 0) continue;
        cout<<"recursion"<<endl;
        MTree* Tj = cp(Fk[j]);
        Tk[j] = *Tj;
  
        // 7. Si la raíz del árbol es de un tamaño menor a b, se quita esa raíz, se elimina pfj de F y se trabaja
        // con sus subárboles como nuevos Tj, se añaden los puntos pertinentes a F.
        // revisar
        if (Tj->root->entries.size() < b) {
            cout<<"paso 7"<<endl;
            // Se quita la raíz de Tj
            Node* root = Tj->root;
            Tj->root = nullptr;
            // Se elimina pfj de F
            F.erase(remove(F.begin(), F.end(), F[j]));
            // Se trabaja con los subárboles de Tj como nuevos Tj
            for (auto& e : Tj->root->entries) {
                Tk.push_back(*cp(Fk[j]));
            }
            // Se añaden los puntos pertinentes a F
            for (auto& e : Tj->root->entries) {
                F.push_back(e.p);
            }
        }
    }   

    // 8. Etapa de balanceamiento: Se define h como la altura mínima de los árboles Tj. Se define T′
    // inicialmente como un conjunto vacío.
    cout<<"paso 8"<<endl;
    int h = numeric_limits<int>::max();
    for (int j = 0; j < k; j++) {
        h = min(h, Tk[j].height());
    }
    // definir T'
    vector<MTree> Tprime;

    // 9. Por cada Tj, si su altura es igual a h, se añade a T′.
    cout<<'9'<<endl;
    for (int j = 0; j < k; j++) {
        if (Tk[j].height() == h) {
            Tprime.push_back(Tk[j]);
        } else {
            // 9.1 Se borra el punto pertinente en F.
            F.erase(find(F.begin(), F.end(), F[j]));

            // 9.2 Se hace una búsqueda exhaustiva en Tj de todos los subárboles T1', . . . , Tp′ de altura igual
            // a h. Se insertan estos árboles a T′
            for (auto& e : Tk[j].root->entries) {
                MTree* Tp = new MTree;
                Tp->root = e.a;
                if (Tp->height() == h) {
                    Tprime.push_back(*Tp);
                    // 9.3 Se insertan los puntos raíz de T1′, . . . , Tp′, p′f1, . . . , p′fp en F
                    F.push_back(e.p);
                }
            }         
        }
    }

    // 10. Se define Tsup como el resultado de la llamada al algoritmo CP aplicado a F.
    PointSet F_set(F.begin(), F.end());
    MTree* Tsup = cp(F_set);

    // 11. Se une cada Tj ∈ T′ a su hoja en Tsup correspondiente al punto pfj ∈ F, obteniendo un nuevo árbol T.
    MTree* T = new MTree;
    for (auto& Tj : Tprime) {
        // buscar el punto pfj en F
        for (auto& e : Tj.root->entries) {
            // buscar el punto en F
            auto it = F_set.find(e.p);
            if (it != F_set.end()) {
                // unir Tj a su hoja en Tsup
                // buscar el punto en Tsup
                for (auto& e2 : Tsup->root->entries) {
                    if (e2.p == e.p) {
                        e.a = e2.a;
                        e.radius = e2.radius;
                    }
                }
            }
        }
    }
    T->root = Tsup->root;

    // 12. Se setean los radios cobertores resultantes para cada entrada en este árbol.
    // hacerlo recursivamente para todas las entradas (revisar esto)
    for (auto& e : T->root->entries) {
        if (e.a) {
            // calcular el radio cobertor
            double max_dist = 0;
            for (auto& e2 : e.a->entries) {
                double dist = distance(e.p, e2.p);
                max_dist = max(max_dist, dist + e2.radius);
            }
            e.radius = max_dist;
        }
    }
    
    // 13. Se retorna T
    return T;
}