#include "construccion/ss.cpp"
#include <bits/stdc++.h>
#include <fstream> 
#include <chrono>
#include <cmath>
using namespace std;

int main(){
    ofstream outFile("output.txt");
    if (!outFile.is_open()) {
        cerr << "Error opening the file." << endl;
        return 1;
    }
    outFile << "CC\n" << endl;
    for(int i = 10; i <= 25; i++){
        outFile << "Probando para N = 2^" << i << endl;
        cout << "Probando para N = 2^" << i <<endl;
        int N = pow(2,i);

        set<point> P = generate_points(N);

        auto start = chrono::high_resolution_clock::now();
        vector<Node*> hojas;
        vector<Node*> internos;
        MTree T;

        hojas.push_back(new Node());
        internos.push_back(new Node());
        T.root = ss(P, hojas, internos);
        auto stop = chrono::high_resolution_clock::now();

        auto create_time = chrono::duration_cast<chrono::milliseconds>(stop - start);
        outFile << "create time = " << create_time.count() << " ms\n" << endl;

        vector<Query> Q;
        for(int j = 0; j < 100; j++){
            point p = {generate_random_number(0,1), generate_random_number(0,1)};
            double r = generate_random_number(0,min(min(p.first, 1-p.first), min(p.second, 1-p.second)));
            Q.push_back({p, r});
        }   
        outFile << "Results" << endl;
        for(auto q : Q){
            long long counter = 0;

            start = chrono::high_resolution_clock::now();
            set<point> s = search(T.root, q, counter);
            stop = chrono::high_resolution_clock::now();

            auto search_time = chrono::duration_cast<chrono::milliseconds>(stop - start);
            std::ostringstream oss;

            oss << "query = (" << q.first.first << "," << q.first.second << "), " << q.second;
            string text = oss.str();
            int length = text.length();
            for(int i = 0; i < 11-length/4; i++){
                oss << "\t";
            }
            oss << "|\t"; 
            outFile << oss.str(); oss.str("");

            oss << "result = "<< s.size() <<", expected = " << N*3.14*q.second*q.second;
            text = oss.str();
            length = text.length();
            for(int i = 0; i < 10-length/4; i++){
                oss << "\t";
            }
            oss << "|\t"; 
            outFile << oss.str(); oss.str("");

            oss << "time = " << search_time.count() << " ms\t\t|\tcounter = " << counter << endl;
            outFile << oss.str(); oss.str("");
        }
        outFile << endl;
    }
    outFile.close();
    return 0;
}