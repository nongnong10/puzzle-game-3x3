#include <bits/stdc++.h>
#include "Solution.h"

using namespace std;

const int SIZE = 362880+10;

int rankP[SIZE];
map <int,int> perToRank;

void    initListPer(){
    int cnt = 0;
    rankP[cnt] = 123456789;
    perToRank[123456789] = cnt;

    vector <int> V;
    for (int i=1; i<=9; ++i){
        V.push_back(i);
    }

    //Sinh ra tat ca hoan vi theo thu tu
    while (next_permutation(V.begin() , V.end())){
        int val = 0;
        for (auto i: V){
            val = val*10 + i;
        }
        rankP[++cnt] = val;
        perToRank[val] = cnt;
        //cout<<cnt<<" = "<<rankP[cnt]<<"\n";
    }
}

void    BFS(){
    //In ra tat ca hoan vi cua 9 so
    initListPer();
    cout<<perToRank[123456879];
}
