#include "otimizacao.h"
#include "random.h"
#include <bits/stdc++.h>
#include <cmath>


int ta;
using namespace std;
int atu,n1, n2, n3;
vector<bool> vis;
vector<int> livres;
int pos = 1;
int guarda =0;
int ale;
int lim=0;


//pertubacao aleatoria da solucao s melhorada
void pertubacao(Data &data, vector<int> &s, double &total, int n, const vector<int> &mls , const double &mltotal){
    double dif;
    int ale1, ale2, ale3, ale4;
    float meio = mls.size();
    meio = meio/2;
    int tamanho = mls.size();
    int parametro = ceil((mls.size()/10));
    int regulador;
    //escolha dos 4 nos aleatorios que delimitam os blocos a serem movimentados
    //obs: a separacao entre solucoes de tamanhos pares ou impares ocorre para definir o intervalo de escolha dos numeros
    if(mls.size()%2 ==0){
        ale2 = Random::getInt(2, (mls.size()/2)-1);

        ale3 = Random::getInt(mls.size()/2, mls.size()-3);

        ale1 = Random::getInt(1, ale2-1);

        ale4 = Random::getInt(ale3+1, tamanho-2);


    }
    else{

        ale2 = Random::getInt(2, floor(meio));

        ale3 = Random::getInt(ceil(meio), mls.size()-3);

        ale1 = Random::getInt(1, ale2-1);

        ale4 = Random::getInt(ale3+1, tamanho-2);


    }

    //separacao considerando que os blocos movimentados podem ser adjacentes
    if(ale2 != ale3-1){
            vector<int> antes(mls.begin(), mls.begin()+ale1);
            vector<int> sub1(mls.begin()+ale1, mls.begin()+ale2+1);
            vector<int> entre(mls.begin()+ale2+1, mls.begin() + ale3);
            vector<int> sub2(mls.begin()+ale3, mls.begin()+ale4+1);
            vector<int> depois(mls.begin()+ale4+1, mls.end());
            vector<int> cop;

            cop.insert(cop.end(), antes.begin(), antes.end());
            cop.insert(cop.end(), sub2.begin(), sub2.end());
            cop.insert(cop.end(), entre.begin(), entre.end());
            cop.insert(cop.end(), sub1.begin(), sub1.end());
            cop.insert(cop.end(), depois.begin(), depois.end());

            int sub1inicio = mls[ale1]; int sub1fim = mls[ale2]; int sub2inicio = mls[ale3]; int sub2fim = mls[ale4]; int sub1antes = mls[ale1-1]; int sub1depois = mls[ale2+1]; int sub2antes = mls[ale3-1]; int sub2depois = mls[ale4+1];
            dif = data.getDistance(sub1antes, sub2inicio) + data.getDistance(sub2fim, sub1depois) + data.getDistance(sub2antes, sub1inicio) + data.getDistance(sub1fim, sub2depois) - data.getDistance(sub1antes, sub1inicio) - data.getDistance(sub1fim, sub1depois) - data.getDistance(sub2antes, sub2inicio) - data.getDistance(sub2fim, sub2depois);
            s = cop;
            total = mltotal + dif;
    }
    else{
        vector<int> antes(mls.begin(), mls.begin()+ale1);
        vector<int> sub1(mls.begin()+ale1, mls.begin()+ale2+1);
        vector<int> sub2(mls.begin()+ale3, mls.begin()+ale4+1);
        vector<int> depois(mls.begin()+ale4+1, mls.end());
        vector<int> cop;

        cop.insert(cop.end(), antes.begin(), antes.end());
        cop.insert(cop.end(), sub2.begin(), sub2.end());
        cop.insert(cop.end(), sub1.begin(), sub1.end());
        cop.insert(cop.end(), depois.begin(), depois.end());

        int sub1inicio = mls[ale1]; int sub1fim = mls[ale2]; int sub2inicio = mls[ale3]; int sub2fim = mls[ale4]; int sub2depois = mls[ale4+1]; int sub1antes = mls[ale1-1];
        dif = data.getDistance(sub1antes, sub2inicio) + data.getDistance(sub2fim, sub1inicio) + data.getDistance(sub1fim, sub2depois) - data.getDistance(sub1antes, sub1inicio) - data.getDistance(sub1fim, sub2inicio) - data.getDistance(sub2fim, sub2depois);
        s = cop;
        total = mltotal +dif;
}}

//movimento swap
void impswap(Data &data, bool &melhora, vector<int> &s, double &total){
    double mdif = 0;
    int meli, melj;

    //i e j = nos que serao invertidos
    for(int i = 1; i < (s.size() - 1); i++){
        double dif = 0;
            int ati, depi, anti;
            ati = s[i]; depi = s[i+1]; anti = s[i-1];
                for(int j = 1 + i; j < s.size() - 1; j++){

                int atj, depj, antj;
                atj = s[j]; depj = s[j+1]; antj = s[j-1];
                if(j == i+1){
                    dif = data.getDistance(anti, atj) + data.getDistance(atj, ati) + data.getDistance(ati, depj) - data.getDistance(anti, ati) - data.getDistance(ati, atj) - data.getDistance(atj, depj);
                }
                  else{
                    dif = data.getDistance(anti, atj) + data.getDistance(atj, depi) + data.getDistance(antj, ati) + data.getDistance(ati, depj) - data.getDistance(anti, ati) - data.getDistance(ati, depi) - data.getDistance(antj, atj) - data.getDistance(atj, depj);
                }
                if(dif < mdif){
                    mdif = dif;
                    meli = i;
                    melj = j;
                }}}


    if(mdif < 0){
        swap(s[meli], s[melj]);
        total = total + mdif;
        melhora = true;
    }}

//movimento 2-opt
void twopt(Data &data, bool &melhora, vector<int> &s, double &total){
    double mdif = 0;
    int melj, meli;
    double dif = 0;

    // i = define o primeiro no que sera invertido
    // j = define o ultimo no que sera invertido
    for(int i= 1; i < (s.size()-1); i++){
        int ati,anti;
        ati = s[i];anti = s[i-1];
        for(int j = i + 2; j < (s.size()-1); j++){
            int atj, depj;
            dif = 0;
            atj = s[j]; depj = s[j+1];

            dif = data.getDistance(anti, atj) + data.getDistance(atj, ati) + data.getDistance(ati, depj) - data.getDistance(anti, ati) - data.getDistance(ati, atj) - data.getDistance(atj, depj);
            
            if(dif < mdif){
                mdif = dif;
                melj = j; meli = i;
            }}}

            if(mdif < 0){
                reverse(s.begin() + meli, s.begin() + melj + 1);
                total = total + mdif;
                melhora = true;
            }}

//movimentos reinsertion, 2-or-opt, 3-or-opt. a escolha de qual movimento e guiada pela variavel "qual"
void outros(Data &data, int qual, bool &melhora, vector<int> &s, double &total){

    //movimento reinsertion
    if(qual == 1){
        double mdif = 0;
        int melj, meli, ins;
        double dif = 0;

        // i = define o no movimentado
        // j = define a aresta onde o no sera inserido
        for(int i = 1; i < (s.size()-1); i++){
            int anti, ati, depi;
            ati = s[i];anti = s[i-1]; depi = s[i+1];
            for(int j = 1 ; j < s.size()-1; j++){
                if(j == i || j == i+1){
                    }
                else{
                    int atj, antj;
                    dif = 0;
                    atj = s[j]; antj = s[j-1];
                    dif = data.getDistance(anti, depi) + data.getDistance(antj, ati) + data.getDistance(ati, atj) - data.getDistance(anti, ati) - data.getDistance(ati, depi) - data.getDistance(antj, atj);

                if(dif < mdif){
                    mdif = dif;
                    melj = j;
                    meli = i;
                    ins = s[i];
                    }}}}

            if(mdif < 0){
                if(meli < melj){
                    total = total + mdif;
                    s.erase(s.begin() + meli);
                    s.insert(s.begin()+melj-1, ins);
                    melhora = true;
                }
                else{
                    total = total + mdif;
                    s.erase(s.begin() + meli);
                    s.insert(s.begin()+melj, ins);
                    melhora = true;
            }}}

    //movimento 2-or-opt
    if(qual == 2){
        double mdif=0;
        int melj, meli;

        // i = define o primeiro bloco a ser movimentado sendo ele i a i + 1
        // j = define a aresta onde o bloco sera inserido
        for(int i = 1; i < s.size() - 2; i++){
            int ati, anti, depi, ati2;
            ati = s[i];  anti = s[i-1]; ati2 = s[i+1]; depi = s[i+2];

            for(int j = 1;  j < s.size(); j++){
                 double dif = 0;
                int atj, atj2, depj, antj;
                atj= s[j]; depj = s[j+2]; antj = s[j-1];
                if( j == i+2|| j == i+1|| j == i){
                    }
                else{
                    double dif = 0;
                    int atj, depj, antj;
                    atj= s[j];  antj = s[j-1];
                    dif = data.getDistance(anti, depi) + data.getDistance(antj, ati) + data.getDistance(ati2, atj) - data.getDistance(anti, ati) - data.getDistance(ati2, depi) - data.getDistance(antj, atj);
                }
            if(mdif>dif){
                mdif = dif;
                melj = j;
                meli = i;
       }}}

       if(mdif < 0){
            vector<int> cop(s.begin() + meli, s.begin()+meli + 2);
            s.erase(s.begin()+meli, s.begin()+meli+2);
                if(meli < melj){
                s.insert(s.begin()+melj-2, cop.begin(), cop.end());}
                else{
                s.insert(s.begin()+melj, cop.begin(), cop.end());
                }
            total = total + mdif;
            melhora = true;
       }
       }
    //movimento 3-or-opt
    if(qual == 3){
    double mdif=0;
    int melj, meli;

    // i = define o primeiro bloco a ser movimentado sendo ele i a i + 2
    // j = define a aresta onde o bloco sera inserido

    for(int i = 1; i < s.size() - 3; i++){
        int ati, anti, depi, ati2;
        ati = s[i]; anti = s[i-1]; depi = s[i+3]; ati2 = s[i+2];

        for(int j = 1; j < s.size(); j++){
            if(j == i+3||j == i+2|| j == i+1|| j == i){
                }
            else{
                double dif = 0;
                int atj, antj;
                atj= s[j]; antj = s[j-1];

                dif = data.getDistance(anti, depi) + data.getDistance(antj, ati) + data.getDistance(ati2, atj) - data.getDistance(anti, ati) - data.getDistance(ati2, depi) - data.getDistance(antj, atj);
                if(mdif>dif){
                    mdif = dif;
                    meli = i;
                    melj = j;
   }}}}
      if(mdif < 0){
        vector<int> cop(s.begin() + meli, s.begin() + meli +3);

        s.erase(s.begin()+meli, s.begin()+meli+3);


        if(meli < melj){
            s.insert(s.begin()+melj-3, cop.begin(), cop.end());
        }
        else{
            s.insert(s.begin()+melj, cop.begin(), cop.end());
        }
        total = total + mdif;
        melhora = true;
   }}}

//melhora da solucao s atraves de movimentos definidos de forma aleatoria (baseado em rvnd)
void buscalocal(Data &data, vector<int> &s, double &total){
    bool melhora = true;
    vector<int> rvn = {1,2,3,4,5};
    while(!rvn.empty()){
        melhora = false;
        int r;
        r = Random::getInt(0, rvn.size()-1);
        switch(rvn[r]){
            case 1:
                impswap(data, melhora, s, total);
                break;

            case 2:
                twopt(data, melhora, s, total);
                 break;

            case 3:
                outros(data, 1, melhora, s, total);
                 break;

            case 4:
                outros(data, 2, melhora, s, total);
                break;

            case 5:
                outros(data, 3, melhora, s, total);
                break;
        }
        if(melhora == true){
                rvn = {1,2,3,4,5};
        }
        else{
            rvn.erase(rvn.begin()+r);
    }}}


//calculo do custo de insercao de um valor em cada posicao da solucao s
void custos(Data &data, int x, vector<pair<double, int>> &inse, vector<int> &s){
    for(int i = 1; i < (s.size() - 1); i++){
             int custo;
            custo = 0;
            custo = data.getDistance(s[i+1], x) + data.getDistance(s[i], x)  - data.getDistance(s[i], s[i+1]);
            inse.push_back({custo, i});
        }}

//escolha de um numero aleatorio com if-else impedindo a repeticao de numeros
void escolher(int &esc, int &c1, int &c2, vector<int> &livres){
    esc = Random::getInt(2, livres.size());
    if(esc != c1 && esc!= c2){
        for(int w = 0; w < livres.size(); w++){
            if(esc == livres[w]){
                livres.erase(livres.begin()+w);
                ta++;
                break;
            }}}
    else{
        escolher(esc, c1, c2, livres);
    }}

//escolha de 3 numeros aleatorios:
void escolher3(vector<int> &livres){
    n1 = 0; n2 = 0; n3 = 0;
    for(int i = 0; i<3;i++){

        if(i == 0){
            escolher(n1, n2, n3, livres);

        }
        if(i == 1){
            escolher(n2, n1, n3, livres);
    }
        if(i == 2){

            escolher(n3, n2, n1, livres);
}}}


// montagem aleatoria da solucao s inicial.

void construcao(Data &data, vector<int> &s, int n, double &total){

    // definicao do tamanho da lista de livres.
    livres.resize(n-1);

    //montagem da lista de livres,a funcao iota faz com que uma lista seja ocupada por um intervalo de x (ultimo valor fornecido a funcao) ate lista.tamanho
    iota(livres.begin(), livres.end(), 2);

    //escolhendo 3 numeros aleatorios
    escolher3(livres);

    //insercao dos 3 numeros aleatorios em posicoes aleatorias  da solucao s
    vector<int> l = {n1,n2,n3};
    int c1, pos;
    int calc = 0;
    int custo = 0;

    c1 = Random::getInt(0, l.size()-1);

    s.insert(s.begin()+1, l[c1]);

    custo = data.getDistance(s[0], s[1]) + data.getDistance(s[1], s[2]) - data.getDistance(s[0], s[2]);
    total = total +custo;

    l.erase(l.begin()+c1);

    c1 = Random::getInt(0, l.size()-1);

    pos = Random::getInt(1, 2);

    s.insert(s.begin()+pos, l[c1]);

    custo = data.getDistance(s[pos-1], s[pos]) + data.getDistance(s[pos], s[pos + 1]) - data.getDistance(s[pos-1], s[pos+1]);
    total = total +custo;

    l.erase(l.begin()+c1);

    c1 = Random::getInt(0, l.size()-1);

    pos = Random::getInt(1, 3);

    s.insert(s.begin() + pos, l[c1]);

    custo = data.getDistance(s[pos-1], s[pos]) + data.getDistance(s[pos], s[pos + 1]) - data.getDistance(s[pos-1], s[pos+1]);
    total = total +custo;

    //montagem aleatoria da solucao s
    while(!livres.empty()){
        vector<pair<double, int>> inse = vector<pair<double, int>>();
            int c;

            //chamada da funcao que calcula o custo da insercao do valor analisado em cada posicao da solucao
            custos(data, livres[0], inse, s);

            c = Random::getInt(0, inse.size()-1);

            sort(inse.begin(), inse.end());

            s.insert(s.begin() + inse[c].second + 1, livres[0]);

            total = total + inse[c].first;


            livres.erase(livres.begin());
        }}

