#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <algorithm>
#include <stdlib.h>
#include "IOProcedures.h"

using namespace std;
int Seed;

int main(int argc, char** argv){
    Capture_Params(argc,argv);
    srand48(Seed);

    cout << "La semilla ingresada es -->" << Seed << endl;
    cout << "Para definirla usamos srand(Seed)" << endl;

    string filename;
    cin >> filename;
    ifstream infile(filename);
    int N, M, optimo;

    infile >> N >> M >> optimo;

    vector<int> arrN(N, 0);
    vector<vector<int>> matrix(M, vector<int>(N));
    vector<int> G(N,0);
    vector<int> R(M,0);

    int j = 0, i = 0;
    while(j<N){
        infile >> G[j];
        j++;
    }
    j=0;
    while(i<M){
        while(j<N){
            infile >> matrix[i][j];
            j++;
        }
        i++;
        j=0;
    }
    i=0;
    while(i<M){
        infile >> R[i];
        i++;
    }

    /*Calculos*/
    double c=0.0,c_j;
    j=0;
    while(j<N){
        c_j=0;
        i=0;
        while(i<M){
            c_j = c_j + (double)matrix[i][j];
            i++;
        }
        c_j = c_j/M;
        c = c + (double)G[j]/c_j;
        j++;
    }
    c = c/N;

    cout << N << endl << M << endl << c << endl;
}
