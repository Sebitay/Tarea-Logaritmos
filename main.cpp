#include "construccion/ss.cpp"
#include "plot.cpp"
#include <bits/stdc++.h>
#include <fstream> 
#include <chrono>
#include <cmath>
using namespace std;

int main(){
    bool SS_flag = true;
    ofstream outFile("output.txt");
    if (!outFile.is_open()) {
        cerr << "Error opening the file." << endl;
        return 1;
    }
    for(int i = 10; i <= 25; i++){
        if(i>15) SS_flag = false;

        outFile << "Probando para N = 2^" << i << endl;
        cout << "Probando para N = 2^" << i <<endl;
        int N = pow(2,i);

        vector<point> P = generate_points(N);

        auto start = chrono::high_resolution_clock::now();
        vector<Node*> hojas;
        vector<Node*> internos;
        Node *T_ss;
        Node *T_cp;
        if (SS_flag){
            hojas.push_back(new Node());
            internos.push_back(new Node());
            T_ss = ss(P, hojas, internos);
        }
        auto stop = chrono::high_resolution_clock::now();
        auto create_time_ss = chrono::duration_cast<chrono::milliseconds>(stop - start);

        start = chrono::high_resolution_clock::now();
        T_cp = cp(P);
        stop = chrono::high_resolution_clock::now();
        auto create_time_cp = chrono::duration_cast<chrono::milliseconds>(stop - start);
        
        if(SS_flag)
            outFile << "create time SS = " << create_time_ss.count() << " ms\n" << endl;

        outFile << "create time CP = " << create_time_cp.count() << " ms\n" << endl;
        
        vector<Query> Q;
        for(int j = 0; j < 100; j++){
            point p = {generate_random_number(0,1), generate_random_number(0,1)};
            Q.push_back({p, 0.02});
        }   
        outFile << "Results" << endl;
        for(auto q : Q){
            long long counter_ss = 0;
            start = chrono::high_resolution_clock::now();
            int ss_size = 0;
            if (SS_flag){   
                set<point> ss = search(T_ss, q, counter_ss);
                ss_size = ss.size();
            }
            stop = chrono::high_resolution_clock::now();
            auto search_time_ss = chrono::duration_cast<chrono::milliseconds>(stop - start);

            long long counter_cp = 0;
            start = chrono::high_resolution_clock::now();
            set<point> cp = search(T_cp, q, counter_cp);
            stop = chrono::high_resolution_clock::now();
            auto search_time_cp = chrono::duration_cast<chrono::milliseconds>(stop - start);

            std::ostringstream oss;

            oss << "query = (" << q.first.first << "," << q.first.second << "), " << q.second;
            string text = oss.str();
            int length = text.length();
            for(int i = 0; i < 11-length/4; i++){
                oss << "\t";
            }
            oss << "|\t"; 
            outFile << oss.str(); oss.str("");
            oss << "time ss = " << search_time_ss.count() << " ms\t|\tcounter ss = " << counter_ss;
            oss << "\t|\ttime cp = " << search_time_cp.count() << " ms\t\t|\tcounter cp = " << counter_cp << "\t|\tcp_search: " <<cp.size()<<" ss_search: " << ss_size << endl;
            outFile << oss.str(); oss.str("");
        }
        outFile << endl;
    }
    outFile.close();
    return 0;
}