#include "Data.h"
#include "otimizacao.h"
#include "random.h"
#include <bits/stdc++.h>
#include <chrono>
using namespace std;

vector<int> seque;
vector<int> mseque;
double total;
double mtotal = 10e18;
int iterILS;

int main(int argc, char** argv) {
    auto cronometroi = chrono::steady_clock::now();
    Random::randomize(time(NULL));
    auto data = Data(argc, argv[1]);
    data.read();
    size_t n = data.getDimension();

    int maxIter  = 50;
    int maxIterILS;
    if(n >= 150){
        maxIterILS = n/2;
    }
    else{
        maxIterILS = n;
    }
for(int i = 0; i < maxIter; i++){
// solucao e valor inicial � iniciado
    seque = {1,1};
    total = 0;

    vector<int> mlseque;
    double mltotal;

// construcao gera a primeira sequencia
    construcao(data, seque, n, total);

    mlseque = seque;
    mltotal = total;

    if(total < mtotal){
        mseque = seque;
        mtotal = total;
    }

    iterILS = 0;

    while(iterILS <= maxIterILS){

//uso da busca local baseada em movimentos para aprimorar a solucao
     buscalocal(data, seque, total);

//checa se busca local montou um novo melhor local
     if(total < mltotal){
        mlseque = seque;
        mltotal = total;
        iterILS = 0;
     }

    iterILS++;
     pertubacao(data, seque, total, n, mlseque, mltotal);
    }

    //comparacao melhor local x melhor
    if(mltotal < mtotal){
        mseque = mlseque;
        mtotal = mltotal;
     }}


    for(int i =0; i < mseque.size(); i++){
     cout << mseque[i] << "->";
    }
    auto cronometrof = chrono::steady_clock::now();
    chrono::duration<double> cronometro = cronometrof - cronometroi;
    cout << "\n" << "Custo:" << mtotal;
    cout << "\nTempo de execucao: " << cronometro.count();
    return 0;
}
