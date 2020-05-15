#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <algorithm>

using namespace std;

int main(){
    string filename;
    cin >> filename;
    ifstream infile(filename);
    int N, M, optimo;

    infile >> N >> M >> optimo;

    vector<int> arrN(N, 0);
    vector<vector<int>> matrix(M, vector<int>(N));
    vector<int> valor(N,0);
    vector<int> res(M,0);

    int j = 0, i = 0;
    while(j<N){
        infile >> valor[j];
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
        infile >> res[i];
        i++;
    }

    /*Calculos*/

    vector<double> peso(N,0);
    double suma=0;
    j=0,i=0;
    while(j<N){
        while(i<M){
            suma = suma + ((double)matrix[i][j]/(double)res[i]);
            i++;
        }
        peso[j] = valor[j]/suma;
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
    while(j<N){
        
        FLAG = true;
        i=0;
        while(i<M){
            if (res[i]<matrix[i][target[j].second]){
                FLAG=false;
                break;
            }
            i++;
        }

        if (FLAG){
            i=0;
            while(i<M){
                res[i] = res[i] - matrix[i][target[j].second];
                i++;
            }
            arrN[target[j].second]=1;
            profit = profit + valor[target[j].second];
        }

        j++;
    }
    j=0;
    while(j<N){
        cout << arrN[j] << ' ';
        j++;
    }
    cout << endl << "optimo: " << optimo << endl << "calculado: " << profit;

    return 0;
}
