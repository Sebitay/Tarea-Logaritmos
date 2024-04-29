#include <bits/stdc++.h>
#include "../mtree.cpp"
#include <iostream>
using namespace std;
using PointSet = set<pair<double, double>>;

int B = 100000; // modificar

MTree cp(PointSet P){

    // 1. Si |P| ≤ B, se crea un árbol T , se insertan todos los puntos a T y se retorna T.:
    // (revisar pq lo hizo copilot)
    if (P.size() <= B){
        MTree T;
        for(auto p : P){
            Node n;
            n.p = p;
            n.radius = 0;
            n.a = nullptr;
            T.nodes.push_back(n);
        }
        return T;
    }


    // 2. De manera aleatoria se eligen k = min(B, n/B) puntos de P, que los llamaremos samples
    // pf1, . . . , pfk. Se insertan en un conjunto F de samples.
    // (revisar pq lo hizo copilot)
    int k = min(B, (int)P.size()/B);
    PointSet F;
    vector<pair<double, double>> samples;
    for(int i = 0; i < k; i++){
        int idx = rand() % P.size();
        auto it = P.begin();
        advance(it, idx);
        samples.push_back(*it);
        F.insert(*it);
    }
    
    // 3. Se le asigna a cada punto en P su sample más cercano. Con eso se puede construir k conjuntos
    // F1, . . . , Fk.

    // 4. Etapa de redistribución: Si algún Fj es tal que |Fj| < b:

    // 4.1 Quitamos pfj de F
    // 4.2 Por cada p ∈ Fj, le buscamos el sample pfl más cercano de F y lo añadimos a su conjunto Fl.


    // 5. Si |F| = 1, volver al paso 2.
    
    // 6. Se realiza recursivamente el algoritmo CP en cada Fj, obteniendo el árbol Tj


    // 7. Si la raíz del árbol es de un tamaño menor a b, se quita esa raíz, se elimina pfj de F y se trabaja
    // con sus subárboles como nuevos Tj , . . . , Tj+p−1, se añaden los puntos pertinentes a F.
    
    // 8. Etapa de balanceamiento: Se define h como la altura mínima de los árboles Tj. Se define T′
    // inicialmente como un conjunto vacío.

    // 9. Por cada Tj , si su altura es igual a h, se añade a T′. Si no se cumple:
    
    // 9.1 Se borra el punto pertinente en F.
    
    // 9.2 Se hace una búsqueda exhaustiva en Tj de todos los subárboles T1', . . . , Tp′ de altura igual
    // a h. Se insertan estos árboles a T′
    
    // 9.3 Se insertan los puntos raíz de T1′, . . . , Tp′, p′f1, . . . , p′fp en F
    
    // 10. Se define Tsup como el resultado de la llamada al algoritmo CP aplicado a F.
    
    // 11. Se une cada Tj ∈ T ′ a su hoja en Tsup correspondiente al punto pfj ∈ F, obteniendo un nuevo árbol T.

    // 12. Se setean los radios cobertores resultantes para cada entrada en este árbol.
    
    // 13. Se retorna T

    //MTree T;
    //return T;
}

/*

int main(){
    set<double> s = {1,2,3};
    cout << s.size();
    return 0;
}

*/