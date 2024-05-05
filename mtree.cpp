#include <bits/stdc++.h>
using namespace std;

typedef pair<double,double> point;

struct entry;
struct Node;
struct MTree;

struct entry{
    point p;
    double radius;
    MTree* a; // Pointer to MTree
};

struct Node{
    vector<entry> entries;
};


struct MTree {
    Node* root;

    MTree() : root(nullptr) {}

    int height() {
        if (!root) return 0;
        return heightUtil(root);
    }

private:
    int heightUtil(Node* node) {
        if (!node) return 0;
        int maxHeight = 0;
        for (const entry& e : node->entries) {
            if (e.a)
                maxHeight = max(maxHeight, 1 + heightUtil(e.a->root));
        }
        return maxHeight;
    }
};





int main() {
    MTree tree;

    // Crear nodos y añadirlos al árbol
    Node* node1 = new Node;
    Node* node2 = new Node;
    Node* node3 = new Node;
    Node* node4 = new Node;
    Node* node5 = new Node;

    MTree* tree2 = new MTree;
    MTree* tree3 = new MTree;
    MTree* tree4 = new MTree;
    MTree* tree5 = new MTree;

    tree2->root = node2;
    tree3->root = node3;
    tree4->root = node4;
    
    // Asignar el nodo raíz
    tree.root = node1;

    // Crear algunas entradas que apunten a otros nodos
    entry entry1 = {{1.0, 2.0}, 1.0, tree2};
    entry entry2 = {{2.0, 3.0}, 1.0, tree3};
    entry entry3 = {{3.0, 4.0}, 1.0, tree4};

    // Agregar las entradas a node1
    node1->entries.push_back(entry1);
    node1->entries.push_back(entry2);
    node1->entries.push_back(entry3);

    tree5->root = node5;

    entry entry4 = {{4.0, 5.0}, 1.0, tree5};
    node2->entries.push_back(entry4);

    // Calcular y mostrar la altura del árbol
    cout << "Altura del arbol: " << tree.height() << endl;

    return 0;
}