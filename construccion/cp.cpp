#include <bits/stdc++.h>
#include <iostream>
#include "../search.cpp"
using namespace std;

typedef pair<double, double> point;
typedef set<point> PointSet;

double distance(point p1, point p2) {
    return sqrt(pow(p1.first - p2.first, 2) + pow(p1.second - p2.second, 2));
}

void setCoveringRadius(Node *node) {
    if(!node) return;
    for (entry& e : node->entries) {
        if (e.a) {
            double max_dist = 0;
            for (entry& e2 : e.a->entries) {
                double dist = distance(e.p, e2.p);
                max_dist = max(max_dist, dist);
            }
            e.radius = max_dist;
            setCoveringRadius(e.a);
        }
    }
}

void setTsupPointers(Node *Tsup, vector<Node*>& Tprime, vector<point>& F){
    for(auto& entry : Tsup->entries){
        if(entry.a)
            setTsupPointers(entry.a, Tprime, F);
        else{
            auto it = find(F.begin(), F.end(), entry.p);
            // poner como hijo el subarbol correspondiente
            entry.a = Tprime[distance(F.begin(), it)];
        }
    }
};

void subtrees_h(Node *Tj, int h, point p, vector<Node*>& Tprime, vector<point>& F) {
    if(Tj->height() < h){
        cout << " - post 1" << endl;
        return;
    }
    if(Tj->height() == h){
        Tprime.push_back(Tj);
        F.push_back(p);
    }else{
        for(auto entry : Tj->entries){
            subtrees_h(entry.a, h, entry.p, Tprime, F);
        }
    }
    return;
};

Node* cp(vector<point> P) {
    // Si |P| ≤ B, se crea un árbol T, se insertan todos los puntos a T y se retorna T:
    if (P.size() <= B) {
        if (P.size() == 0) return nullptr;
        Node* T = new Node;
        for (auto& p : P) {
            entry e;
            e.p = p;
            e.radius = 0;
            e.a = nullptr;
            T->entries.push_back(e);
        }
        T->entries.size();
        return T;
    }
    int k = min(B, static_cast<int>(ceil((P.size() / (double)B))));
    vector<point> F;
    vector<vector<point>> Fk(k);
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
            Fk[idx].push_back(p);
            
        }
        // 4. Etapa de redistribución: Si algún Fj es tal que |Fj| < b:
        for (int i = k - 1; i > -1; i--) {
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
                    Fk[idx].push_back(p);

                }
                // Vaciamos F
                Fk[i].clear();
            }
        }
    } while(F.size() == 1);

    // 6. Se realiza recursivamente el algoritmo CP en cada Fj, obteniendo el árbol Tj
    vector<Node*> Tk;
    for (int i = 0; i < k; i++){
        if (Fk[i].size() == 0) continue;
        Tk.push_back(cp(Fk[i]));
    }

    for (int i = Tk.size() - 1; i > -1 ; i--) {
        // 7. Si la raíz del árbol es de un tamaño menor a b, se quita esa raíz, se elimina pfj de F y se trabaja
        // con sus subárboles como nuevos Tj, se añaden los puntos pertinentes a F.
        // revisar
        if (Tk[i]->entries.size() < b) {
            // Se elimina pfj de F
            for(auto& entry : Tk[i]->entries){
                if(entry.a){
                    Tk.push_back(entry.a);
                    F.push_back(entry.p);
                }
            }
            F.erase(remove(F.begin(), F.end(), F[i]));
            Tk.erase(remove(Tk.begin(), Tk.end(), Tk[i]));
            // Se trabaja con los subárboles de Tj como nuevos Tj   
        }  
    }   

    // 8. Etapa de balanceamiento: Se define h como la altura mínima de los árboles Tj. Se define T′
    // inicialmente como un conjunto vacío.
    /* cout<<"paso 8"<<endl; */
    int h = numeric_limits<int>::max();
    for (int j = 0; j < Tk.size(); j++) {
        if(!Tk[j]) cout<< "Vacio" << j << endl;
        h = min(h, Tk[j]->height());
    }
    //cout<<"h: "<<h<<endl; 
    // definir T'
    vector<Node*> Tprime;

    // 9. Por cada Tj, si su altura es igual a h, se añade a T′.
    // Si no, 9.1, 9.2 y 9.3
    /* cout<<"paso 9"<<endl; */
    
    for(int i = Tk.size() - 1; i > -1; i--){
        Node* tree = new Node;
        tree = Tk[i];
        if (!tree) continue;
        if(tree->height() == h)
            Tprime.push_back(tree);
        else{
            point p = F[i];
            F.erase(remove(F.begin(), F.end(), p));
            subtrees_h(tree, h, p, Tprime, F);
        }
    }
    

    // 10. Se define Tsup como el resultado de la llamada al algoritmo CP aplicado a F.
    /* cout<<"paso 10"<<endl; */
    Node* Tsup = new Node;
    Tsup = cp(F);

    // 11. Se une cada Tj ∈ T′ a su hoja en Tsup correspondiente al punto pfj ∈ F, obteniendo un nuevo árbol T.
    /* cout<<"paso 11"<<endl; */
    Node *T = new Node; 
    setTsupPointers(Tsup, Tprime, F);
    T = Tsup;

    // 12. Se setean los radios cobertores resultantes para cada entrada en este árbol.
    /* cout<<"paso 12"<<endl; */
    setCoveringRadius(T);

    // 13. Se retorna T
    /* cout<<"paso 13"<<endl; */
    return T;
}