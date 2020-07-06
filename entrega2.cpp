#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <algorithm>

#include <numeric>
#include <chrono>


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

int f_obj(vector<vector<int>> matrix, vector<int> R, vector<int> X, vector<int> G,int N,int M){
    int f = f_r(matrix,R,X,N,M);
    if (f==0){
        int j=0;
        int suma=0;
        while(j<N){
            suma = suma + G[j]*X[j];
            j++;
        }
        return suma;
    }
    else{
        return -1;
    }
}

int random_n(int min, int max){
    int output;
    output = min + (rand() % static_cast<int>(max - min + 1));
    return output;
}

int main(int argc, char** argv){

    int seed = atoi(argv[2]);
    srand(seed);

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

    /*HC*/
    vector<int> Sbest(N, 0);
    int Sbest_value=0;
    int t=0;

    while(t<20){
        /*
        j=0;
        while(j<N){
            int value = random_n(0,1);
            Sc[j]=value;
            j++;
        }
        */
        vector<int> Sc(N, 0);
        int cant = random_n(0,N);
        j=0;
        while(j<cant){
            int pos_to_change = random_n(0,N-1);
            while (Sc[pos_to_change]==1){
                pos_to_change = random_n(0,N-1);
            }
            Sc[pos_to_change] = 1;
            j++;
        }
        int f_res = f_r(matrix,R,Sc,N,M);
        int Sc_value = f_eval(G,Sc,c,f_res,N);   
        int sum_of_elems = accumulate(Sc.begin(), Sc.end(), 0);
        /*cout << "Overflow: " << f_res << " F_eval: "<< Sc_value << " numero de elementos: " << float(sum_of_elems)/(float)N << endl;*/     
        bool FLAG = true;
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
        if(Sc_value > Sbest_value){
            Sbest_value=Sc_value;
            Sbest=Sc;
        }
        t++;
    }
    j=0;
    cout << "X: ";
    while(j<N){
        cout << Sbest[j] << ' ';
        j++;
    }
    cout << endl << "f: " <<  Sbest_value << endl;
}
