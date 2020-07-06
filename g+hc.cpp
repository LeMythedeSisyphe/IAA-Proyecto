#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <algorithm>
#include <stdlib.h>
#include "IOProcedures.h"

using namespace std;

int f_eval(vector<int> G, vector<int> X, int c, int f, int N){
    int j=0;
    int suma=0;
    while(j<N){
        suma = suma + G[j]*X[j];
        j++;
    }
    suma = suma - (c*f);
    /*cout << c << ' ' << f << ' ' << suma << endl;*/
    return suma;
}

int f_r(vector<vector<int>> matrix, vector<int> R, vector<int> X, int N,int M){
    int i=0,j,suma,suma_total=0;
    while(i<M){
        j=0;
        suma=0;
        while(j<N){
            suma = suma + (matrix[i][j]*X[j]);
            j++;
        }
        suma = suma - R[i];
        if (suma>suma_total){
            /*suma_total = suma_total + suma;*/
            suma_total = suma;
        }
        i++;
    }
    return suma_total;
}

int main(int argc, char** argv){

    string filename = argv[1];
    ifstream infile(filename);
    int N, M, optimo;

    infile >> N >> M >> optimo;

    vector<vector<int>> matrix(M, vector<int>(N));
    vector<int> G(N,0);
    vector<int> R(M,0);

    int j = 0, i = 0, G_max=0;
    while(j<N){
        infile >> G[j];
        if (G[j]>G_max){
            G_max=G[j];
        }
        j++;
    }
    cout << "lei G";
    j=0;
    int Rij_min=-1;
    while(i<M){
        while(j<N){
            infile >> matrix[i][j];
            if (Rij_min==-1 && matrix[i][j] > 0){
                Rij_min=matrix[i][j];
            }
            if (matrix[i][j]<Rij_min && matrix[i][j]>0){
                Rij_min=matrix[i][j];
            }
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

    cout << "Gmax: " << G_max << " Rij_min: " << Rij_min << endl;

    /*Calculos*/
    /*
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
    */
    int c = (G_max+1)/Rij_min;

    cout << "c: " << c << endl;


    /*Greedy*/
    vector<int> Sc(N, 0);
    vector<double> peso(N,0);
    double suma=0;
    j=0,i=0;
    while(j<N){
        while(i<M){
            suma = suma + ((double)matrix[i][j]/(double)R[i]);
            i++;
        }
        peso[j] = G[j]/suma;
        suma=0;
        j++;
        i=0;
    }


    
    vector<int> id(N,0);
    generate(id.begin(), id.end(), [] {
        static int k = 0;
        return k++;
    });
    int profit = 0;

    vector<pair<double, int>> target;
    target.reserve(id.size());
    transform(peso.begin(), peso.end(), id.begin(), back_inserter(target),[](double a, int b) { return make_pair(a, b); });

    sort(target.begin(), target.end(), greater<>());

    j=0;
    bool FLAG;
    vector<int> R_prima(M,0);
    R_prima = R;
    while(j<N){
        
        FLAG = true;
        i=0;
        while(i<M){
            if (R_prima[i]<matrix[i][target[j].second]){
                FLAG=false;
                break;
            }
            i++;
        }

        if (FLAG){
            i=0;
            while(i<M){
                R_prima[i] = R_prima[i] - matrix[i][target[j].second];
                i++;
            }
            Sc[target[j].second]=1;
            profit = profit + G[target[j].second];
        }

        j++;
    }
    for(j=0;j<N;j++){
        cout << Sc[j] << ' ';
    }
    cout << endl;


    /*HC*/

    int f_res = f_r(matrix,R,Sc,N,M);
    int Sc_value = f_eval(G,Sc,c,f_res,N); 
    cout << "f: " << f_res << " sc_value: " << Sc_value << " profit: " << profit << endl;       
    FLAG = true;
    int max_v = Sc_value, pos_max_v = 0, f_v,s_v;
    while(FLAG){
        vector<vector<int>> vecindario(N, Sc);
        j=0;
        while(j<N){
            if (vecindario[j][j]==0){
                vecindario[j][j]=1;
            }
            else{
                vecindario[j][j]=0;
            }

            f_v = f_r(matrix,R,vecindario[j],N,M);
            s_v = f_eval(G,vecindario[j],c,f_v,N);
            i=0;
            cout << "Sv: " << "value: " << s_v << ' ';
            while(i<N){
                cout << Sc[i] << ' ';
                i++;
            }
            cout << endl;

            if (s_v>max_v){
                max_v=s_v;
                pos_max_v = j;
            }
            j++;
        }
        if (max_v > Sc_value){
            Sc_value=max_v;
            Sc = vecindario[pos_max_v];
        }
        else{
            FLAG=false;
        }
    }

    j=0;
    cout << "Sc: ";
    while(j<N){
        cout << Sc[j] << ' ';
        j++;
    }
    cout << endl << "optimo: " << optimo << endl;
    cout << "calculado: " << Sc_value << endl;
}
