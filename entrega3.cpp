#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <algorithm>

using namespace std;

float random_float(){
    return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

int f_obj(vector<int> G, vector<int> X, int N){
    int suma=0;
    for(int j=0;j<N;++j){
        suma = suma + G[j]*X[j];
    }
    return suma;
}

float cf(vector<float> tau_0,vector<float> tau_1, int N){
    float suma=0;
    for(int i=0;i<N;++i){
        suma+=abs(tau_0[i]-tau_1[i]);
    }
    return suma/N;
}

bool feasible(vector<vector<int>> matrix, vector<int> R, vector<int> X, int N,int M){
    int suma;
    for(int i=0;i<M;++i){
        suma=0;
        for(int j=0;j<N;++j){
            suma += X[j]*matrix[i][j];
        }
        if (R[i]<suma){
            return false;
        }
    }
    return true;
}

int main(int argc, char** argv){
    int seed = atoi(argv[2]);
    srand(seed);

    /*INICIO: lectura del archivo*/
    string filename = argv[1];
    ifstream infile(filename);
    int N, M, optimo;

    infile >> N >> M >> optimo;

    vector<vector<int>> matrix(M, vector<int>(N));
    vector<int> G(N,0);
    vector<int> R(M,0);

    int j=0, i=0;
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
    /*FIN: lectura del archivo*/

    /*Greedy ordenador*/
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
    vector<pair<double, int>> target;
    target.reserve(id.size());
    transform(peso.begin(), peso.end(), id.begin(), back_inserter(target),[](double a, int b) { return make_pair(a, b); });
    sort(target.begin(), target.end(), greater<>());

    /*ant algorithm*/

    int n_ants=30;
    float p=0.03;
    float cf_current;
    float cf1=0.3, cf2=0.5, cf3=0.7, cf4=0.9, cf5=0.95;
    vector<int> X_global(N,0);
    vector<int> X_restart(N,0);
    vector<float> tau_0(N,0.5);
    vector<float> tau_1(N,0.5);

    float choice;
    vector <float> cfs(5,0);
    int check_feasible;

    for(int iter=0;iter<3000;++iter){
        vector<int> X_iter(N,0);
        for(i=0;i<n_ants;++i){
            vector<int> X_current(N,0);
            for(j=0;j<N;++j){
                choice = random_float();
                if (choice > tau_0[j]){
                    X_current[j]=1;
                }
            }
            /*clean solution*/
            /*DROP*/
            check_feasible=1;
            while (!feasible(matrix, R, X_current, N, M)){
                if (X_current[target[N-check_feasible].second]==1){
                    X_current[target[N-check_feasible].second]=0;
                }
                check_feasible++;
            }
            /*ADD*/
            for(check_feasible=0;check_feasible<N;++check_feasible){
                if(X_current[target[check_feasible].second]==0){
                    X_current[target[check_feasible].second]=1;
                    if (!feasible(matrix, R, X_current, N, M)){
                        X_current[target[check_feasible].second]=0;
                    }
                }
            }


            if(f_obj(G,X_current,N) > f_obj(G,X_iter,N)){
                X_iter = X_current;
            }

        }
        if(f_obj(G,X_iter,N) > f_obj(G,X_restart,N)){
            X_restart = X_iter;
        }
        if(f_obj(G,X_iter,N) > f_obj(G,X_global,N)){
            X_global = X_iter;
        }

        /*pheromone update*/
        /*evaporacion*/
        for(j=0;j<N;++j){
            tau_0[j]=(1-p)*tau_0[j];
            tau_1[j]=(1-p)*tau_1[j];
        }

        cf_current = cf(tau_0,tau_1, N);

        if (cf_current<cf1){
            for(j=0;j<N;++j){
                if (X_iter[j]==0){
                    tau_0[j] += p;
                }
                else{
                    tau_1[j] += p;
                }
            }
        }
        else if (cf_current<cf2){
            for(j=0;j<N;++j){
                if (X_iter[j]==0){
                    tau_0[j] += p*(2.0/3.0);
                }
                else{
                    tau_1[j] += p*(2.0/3.0);
                }

                if (X_restart[j]==0){
                    tau_0[j] += p*(1.0/3.0);
                }
                else{
                    tau_1[j] += p*(1.0/3.0);
                }
            }
        }
        else if (cf_current<cf3){
            for(j=0;j<N;++j){
                if (X_iter[j]==0){
                    tau_0[j] += p*(1.0/3.0);
                }
                else{
                    tau_1[j] += p*(1.0/3.0);
                }

                if (X_restart[j]==0){
                    tau_0[j] += p*(2.0/3.0);
                }
                else{
                    tau_1[j] += p*(2.0/3.0);
                }
            }
        }
        else if (cf_current<cf4){
            for(j=0;j<N;++j){
                if (X_restart[j]==0){
                    tau_0[j] += p;
                }
                else{
                    tau_1[j] += p;
                }
            }
        }
        else if (cf_current<cf5){
            for(j=0;j<N;++j){
                if (X_global[j]==0){
                    tau_0[j] += p;
                }
                else{
                    tau_1[j] += p;
                }
            }
        }
        else{
            ofstream writefile;
            writefile.open("results-"+filename);
            for(j=0;j<N;++j){
                X_restart[j]=0;
                tau_0[j] = 0.5*(1-p);
                tau_1[j] = 0.5*(1-p);

                if (X_global[j]==0){
                    tau_0[j] += p;
                }
                else{
                    tau_1[j] += p;
                }

                writefile << X_global[j] << " ";
            }
            writefile << "\n" << f_obj(G, X_global, N) << "\n";
            writefile.close();
            /*
            if (f_obj(G, X_global, N)==optimo){
                cout << "llegue al optimo" << endl;
                break;
            }
            */
        }
    }

    cout << "X: ";
    for(j=0;j<N;++j){
        cout << X_global[j] << ' ';
    }
    cout << endl;
    cout << "FO: " << f_obj(G, X_global, N);


    return 0;
}