#include <bits/stdc++.h>
#include "Solution.h"
#include "Board.h"

using namespace std;

const int SIZE = 362880+10;

int rankP[SIZE];
int step[SIZE], pre[SIZE];
vector <int> avail;
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

    memset(step , -1 , sizeof(step));
    step[0] = 0;

    queue <int> Q;
    while (!Q.empty())  Q.pop();
    Q.push(0);

    while (!Q.empty()){
        int cur = Q.front();
        Q.pop();
        int P = rankP[cur];
        avail.push_back(P);
        //cerr<<P<<"\n";

        int newP = perToRank[Board(P).moveUp().getID()];
        if (step[newP] == -1){
            Q.push(newP);
            step[newP] = step[cur] + 1;
            pre[newP] = 1;
        }

        newP = perToRank[Board(P).moveDown().getID()];
        if (step[newP] == -1){
            Q.push(newP);
            step[newP] = step[cur] + 1;
            pre[newP] = 2;
        }

        newP = perToRank[Board(P).moveLeft().getID()];
        if (step[newP] == -1){
            Q.push(newP);
            step[newP] = step[cur] + 1;
            pre[newP] = 3;
        }

        newP = perToRank[Board(P).moveRight().getID()];
        if (step[newP] == -1){
            Q.push(newP);
            step[newP] = step[cur] + 1;
            pre[newP] = 4;
        }
    }
    //cerr<<1;

    /*
    //Check step and level
    int sav = 0;
    for (int a=0; a<avail.size(); ++a){
        int i = avail[a];
        if (step[perToRank[i]] != sav){
            sav = step[perToRank[i]];
            cout<<step[perToRank[i]]<<" : "<<perToRank[i]<<" -> "<<a<<"\n";
        }
        //cout<<step[perToRank[i]]<<" : "<<perToRank[i]<<" -> "<<i<<"\n";
    }*/
}

int     trace(int p){
    return pre[perToRank[p]];
}

int     random(int l, int r){
    return l + rand()%(r-l+1);
}

int     rd_level(int level){
    //181439 cases
    if (level == 1)     return avail[random(1 , 1000)];
    if (level == 2)     return avail[random(1000 , 10000)];
    if (level == 3)     return avail[random(10000 , 50000)];
    if (level == 4)     return avail[random(50000 , 100000)];
    if (level == 5)     return avail[random(100000 , 181439)];
}

