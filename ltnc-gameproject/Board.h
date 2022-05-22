#ifndef _BOARD_H_
#define _BOARD_H_

struct  Board{
    int blank;
    int A[3][3];

    Board();
    Board(int n);

    int getID();

    Board moveLeft();
    Board moveRight();
    Board moveUp();
    Board moveDown();
};

#endif // _BOARD_H_

