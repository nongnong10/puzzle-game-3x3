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

int     Board::getID(){
    int P = 0;
    for (int i=0; i<3; ++i){
        for (int j=0; j<3; ++j){
            P = P * 10 + A[i][j];
        }
    }
    return P;
}

Board Board::moveUp(){
    if (blank < 3)  return Board(getID());
    Board newBoard = *this;
    int row = blank / 3;
    int col = blank % 3;
    swap (newBoard.A[row][col] , newBoard.A[row-1][col]);
    newBoard.blank -= 3;
    return newBoard;
}

Board Board::moveDown(){
    if (blank > 5)  return Board(getID());
    Board newBoard = *this;
    int row = blank / 3;
    int col = blank % 3;
    swap (newBoard.A[row][col] , newBoard.A[row+1][col]);
    newBoard.blank += 3;
    return newBoard;
}

Board Board::moveLeft(){
    if (blank % 3 == 0)  return Board(getID());
    Board newBoard = *this;
    int row = blank / 3;
    int col = blank % 3;
    swap (newBoard.A[row][col] , newBoard.A[row][col-1]);
    newBoard.blank -= 1;
    return newBoard;
}

Board Board::moveRight(){
    if (blank % 3 == 2)  return Board(getID());
    Board newBoard = *this;
    int row = blank / 3;
    int col = blank % 3;
    swap (newBoard.A[row][col] , newBoard.A[row][col+1]);
    newBoard.blank += 1;
    return newBoard;
}
