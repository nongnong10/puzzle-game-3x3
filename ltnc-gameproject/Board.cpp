#include <bits/stdc++.h>
#include "Board.h"

using namespace std;

Board::Board(int n){
    int len = 8;
    while (n > 0){
        A[len/3][len%3] = n%10;
        n /= 10;
        len--;
    }

    for (int i=0; i<3; ++i){
        for (int j=0; j<3; ++j){
            if (A[i][j] == 1)   blank = i*3 + j;
        }
    }
}
