#ifndef OTIMIZACAO_H
#define OTIMIZACAO_H
#include "Data.h"
#include <iostream>
#include <vector>

using namespace std;


void construcao(Data &data, vector<int> &s, int n, double &total);
void buscalocal(Data &data, vector<int> &s, double &total);
void pertubacao(Data &data, vector<int> &s, double &total, int n, const vector<int> &mls , const double &mltotal);

#endif
