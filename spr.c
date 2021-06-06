//
//  main.c
//  spr (Knights problem)
//
//  Created by Michael Roth in January 2014.
//  CC-by-nc-sa roth::net Michael Roth. All rights reserved.
//  

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

int board[20][20];              // maximum board size +4
int size = 8;                     // default board size
int move[8][2];                 // array of moves
int movelist[256];              // maximum movelist
int posx = 2;                     // starting pos x
int posy = 2;                     // starting pos y
int depth = 0;                    // current depth
int maxdepth = 0;                 // maximum depth
unsigned long long movecount = 0; // move counter
struct timeval starttime,
    nowtime;         // timer

void boardInit(void)
{
    int bsize = size + 4;
    int x, y;

    for (x = 0; x < bsize; x++)
    {
        for (y = 0; y < bsize; y++)
        {
            if (x <= 1 || x >= size + 2 || y <= 1 || y >= size + 2)
            {
                board[x][y] = -1; // Ausserhalb des Brettes
            }
            else
            {
                board[x][y] = 0;
            }
        }
    }
    board[2][2] = 1;
}

void movesInit(void)
{
    move[0][0] = 1;
    move[0][1] = 2;

    move[1][0] = 2;
    move[1][1] = 1;

    move[2][0] = 2;
    move[2][1] = -1;

    move[3][0] = 1;
    move[3][1] = -2;

    move[4][0] = -1;
    move[4][1] = -2;

    move[5][0] = -2;
    move[5][1] = -1;

    move[6][0] = -2;
    move[6][1] = 1;

    move[7][0] = -1;
    move[7][1] = 2;
}

void boardPrintFull(void)
{
    int bsize = size + 4; // total board size
    int x, y, z, movespersecond;
    double timespan;

    printf("\a\n\n");

    gettimeofday(&nowtime, NULL);

    for (z = 2; z < bsize - 2; z++)
    {
        printf("----");
    }
    printf("-\n");

    for (y = bsize - 3; y >= 2; y--)
    {
        for (x = 2; x < bsize - 2; x++)
        {
            printf("|%3d", board[x][y]);

        }
        printf("|\n");

        for (z = 2; z < bsize - 2; z++)
        {
            printf("----");
        }
        printf("-\n");
    }
    timespan = (double)nowtime.tv_sec - starttime.tv_sec + (double)(nowtime.tv_usec - starttime.tv_usec) / 1000000;
    movespersecond = movecount / timespan;
    printf("Depth: %d   Time: %lfs\nAverage calculationspeed: %d moves/s\n", depth, timespan, movespersecond);
}


void tryallmoves(void) // Recursive function to move the knight on the board
{
    int i;
    for (i = 0; i < 8; i++)
    {

        if (board[posx + move[i][0]][posy + move[i][1]] == 0)
        {
            posx = posx + move[i][0];
            posy = posy + move[i][1];
            movecount++;
            if (movecount % 100000000 == 0) printf("Calculated moves: %llu Million Moves, Max. Depth:%d \n", movecount / 1000000, maxdepth);

            movelist[depth] = i;
            depth++;
            board[posx][posy] = depth + 1;
            if (depth > maxdepth)
            {
                maxdepth = depth;
                boardPrintFull();
            }
            if ((depth < size * size - 1) && (depth > 0)) tryallmoves();
        }

    }

    // Now we're taking back the last move...
    board[posx][posy] = 0;
    posx = posx - move[movelist[depth - 1]][0];
    posy = posy - move[movelist[depth - 1]][1];
    depth--;
    return;
}


int main(int argc, const char* argv[])
{
    printf("\nKnights problem 1.1 by micky 1993-2014 · CC-by-nc-sa\n");
    printf("\nUsage: spr n (where n is a number from 3 to 16 to define the size of the board)\n");

    if (argc == 1) exit(0);

    size = atoi(argv[1]);

    if ((size < 3) || (size > 16))
    {
        printf("Size of the board must be 3...16\n");
        exit(0);
    }

    boardInit(); // Initialises the board and its surrounding fields
    movesInit(); // Initialises the moves

    printf("\nStarting calculation of moves...\n\n");

    gettimeofday(&starttime, NULL);

    tryallmoves();

    printf("Finished! I performed %llu moves on the board\n", movecount);
    return 0;
}