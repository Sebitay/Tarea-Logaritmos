#include <bits/stdc++.h>
#include "../mtree.cpp"
#include "../plot.cpp"
#include <iostream>
using namespace std;
using PointSet = vector<pair<double, double>>; // ver si es que es set o vector

int B = 2; // modificar

MTree cp(PointSet P){

    // 1. Si |P| ≤ B, se crea un árbol T , se insertan todos los puntos a T y se retorna T.:
    if (P.size() <= B){
        MTree T;
        for(auto p : P){
            Node n;
            n.p = p;
            n.radius = 0; // ver que radio poner
            n.a = nullptr; // ver que puntero poner
            T.nodes.push_back(n);
        }
        return T;
    }


    // 2. De manera aleatoria se eligen k = min(B, n/B) puntos de P, que los llamaremos samples
    // pf1, . . . , pfk. Se insertan en un conjunto F de samples.
    // (falta lidiar con el tema de los repetidos)
    int k = min(B, (int)P.size()/B);
    PointSet F;
    //srand(time(0));
    for(int i = 0; i < k; i++){
        int idx = rand() % P.size();
        F.push_back(P[idx]);
    }
    // show the samples
    cout<<endl;
    for(int i =0; i < k; i++){
        cout<<'s'<<i<<':'<<F[i].first<<'-'<<F[i].second<<endl;
    }
    
    
    // 3. Se le asigna a cada punto en P su sample más cercano. Con eso se puede construir k conjuntos
    // F1, . . . , Fk.
    vector<PointSet> Fk(k);
    cout<<Fk.size()<<endl;
    for(auto p : P){
        double min_dist = 1e9;
        int idx = -1;
        for(int i = 0; i < k; i++){
            // calcular la distancia entre p y F[i]:
            double dist = sqrt(pow(p.first - F[i].first, 2) + pow(p.second - F[i].second, 2));
            cout<<dist<<endl;
            // actualizar el mínimo:
            if(dist < min_dist){
                min_dist = dist;
                idx = i;
            }
        }
        cout<<"min:"<<min_dist<<endl;
        cout<<endl;
        // añadir p a Fk[idx]:
        Fk[idx].push_back(p);
    }
    // show the sets
    cout<<endl;
    for(int i = 0; i < k; i++){
        cout<<"F"<<i<<endl;
        for(auto p : Fk[i]){
            cout<<p.first<<'-'<<p.second<<endl;
        }
    }

    // 4. Etapa de redistribución: Si algún Fj es tal que |Fj| < b:

    

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
    MTree T;


    // 12. Se setean los radios cobertores resultantes para cada entrada en este árbol.
    
    // 13. Se retorna T
    return T;
}


int main(){
    int N = 4;
    PointSet points = generate_points(N);
    for(int i =0; i < N; i++){
        cout<<'P'<<i<<':'<<points[i].first<<'-'<<points[i].second<<endl;
    }
    MTree T = cp(points);
    // show the nodes of the tree
    //for(auto n : T.nodes){
      //  cout<<n.p.first<<'-'<<n.p.second<<endl;
    //}
    return 0;
}