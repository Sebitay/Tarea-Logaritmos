#include <bits/stdc++.h>
#include <iostream>
#include "../mtree.cpp"
#include "../plot.cpp"
using namespace std;

typedef pair<double, double> point;
typedef set<point> PointSet;

//int B = 2;
int B = 4096/sizeof(point);
int b = B/2;


double distance(point p1, point p2) {
    return sqrt(pow(p1.first - p2.first, 2) + pow(p1.second - p2.second, 2));
}


/* puntos a revisar:
7.3-7.4
9.2-9.3
11
12
 */
MTree* cp(PointSet P) {
    
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
        return T;
    }

    // 2. De manera aleatoria se eligen k = min(B, n/B) puntos de P, que los llamaremos samples
    // pf1, . . . , pfk. Se insertan en un conjunto F de samples.
    int k = min(B, static_cast<int>(P.size() / B));
    vector<point> F;
    for (int i = 0; i < k; i++) {
        int idx = rand() % P.size();
        auto it = next(P.begin(), idx);
        F.push_back(*it);
    }

/*     // show the samples
    cout << "samples" << endl;
    for (auto p : F) {
        cout << p.first << '-' << p.second << endl;
    }
 */

    // 3. Se le asigna a cada punto en P su sample más cercano. Con eso se puede construir k conjuntos
    // F1, . . . , Fk.
    vector<PointSet> Fk(k);
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
/*     // show the sets
    cout << endl;
    for (int i = 0; i < k; i++) {
        // show the points
        cout << "F" << i << endl;
        for (auto p : Fk[i]) {
            cout << p.first << '-' << p.second << endl;
        }
    }
 */
    

    // 4. Etapa de redistribución: Si algún Fj es tal que |Fj| < B:
    for (int i = 0; i < k; i++) {
        if (Fk[i].size() < B) {
            // 4.1 Quitamos pfj de F
            F.erase(find(F.begin(), F.end(), F[i]));

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

    // eliminar los valores vacios de fk

/*     // show the sets
    cout << endl;
    cout << "redistribucion" << endl;
    for (int i = 0; i < k; i++) {
        // show the points
        cout << "F" << i << endl;
        for (auto p : Fk[i]) {
            cout << p.first << '-' << p.second << endl;
        }
    }
 */
    
    // 5. Si |F| = 1, volver al paso 2.
    //cout << endl << "size F: " << F.size() << endl;
    if (F.size() == 1) {
        return cp(P);
    }

    // 6. Se realiza recursivamente el algoritmo CP en cada Fj, obteniendo el árbol Tj
    vector<MTree> Tk(k);
    for (int j = 0; j < k; j++) {
        if (Fk[j].size() == 0) continue;
        MTree* Tj = cp(Fk[j]);
        Tk[j] = *Tj;
  

        // 7. Si la raíz del árbol es de un tamaño menor a b, se quita esa raíz, se elimina pfj de F y se trabaja
        // con sus subárboles como nuevos Tj, se añaden los puntos pertinentes a F.
        // revisar
        if (Tj->root->entries.size() < b) {

            // 7.1 Se quita la raíz de Tj
            Node* root = Tj->root;
            Tj->root = nullptr;
            
            // 7.2 Se elimina pfj de F
            F.erase(find(F.begin(), F.end(), F[j]));

            // 7.3 Se trabaja con los subárboles de Tj como nuevos Tj
            for (auto& e : Tj->root->entries) {
                Tk.push_back(*cp(Fk[j]));
            }

            // 7.4 Se añaden los puntos pertinentes a F
            for (auto& e : Tj->root->entries) {
                F.push_back(e.p);
            }
            delete root;

        }
    }   

    // 8. Etapa de balanceamiento: Se define h como la altura mínima de los árboles Tj. Se define T′
    // inicialmente como un conjunto vacío.

    int h = numeric_limits<int>::max();
    for (int j = 0; j < k; j++) {
        h = min(h, Tk[j].height());
    }
    // definir T'
    vector<MTree> Tprime;

    // 9. Por cada Tj, si su altura es igual a h, se añade a T′. Si no se cumple:
    for (int j = 0; j < k; j++) {
        if (Tk[j].height() == h) {
            Tprime.push_back(Tk[j]);
        } else {

            // 9.1 Se borra el punto pertinente en F.
            F.erase(find(F.begin(), F.end(), F[j]));

            // 9.2 Se hace una búsqueda exhaustiva en Tj de todos los subárboles T1', . . . , Tp′ de altura igual
            // a h. Se insertan estos árboles a T′


            // 9.3 Se insertan los puntos raíz de T1′, . . . , Tp′, p′f1, . . . , p′fp en F

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
    // 12. Se setean los radios cobertores resultantes para cada entrada en este árbol.
    for (auto& e : T->root->entries) {
        double max_dist = 0;
        for (auto& e2 : T->root->entries) {
            if (e2.a == e.a) {
                max_dist = max(max_dist, distance(e.p, e2.p));
            }
        }
        e.radius = max_dist;
    }
    
    // 13. Se retorna T
    return T;
}

/* int main() {
    //int N = 4;
    int N = pow(2, 15);
    PointSet points = generate_points(N);
    // show the points
    /*     
    cout << "points" << endl;
    for (auto p : points) {
        cout << p.first << '-' << p.second << endl;
    } 
    
   MTree* T = cp(points);
   return 0;
} */
