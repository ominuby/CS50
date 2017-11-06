/**
 * fifteen.c
 *
 * Implements Game of Fifteen (generalized to d x d).
 *
 * Usage: fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [DIM_MIN,DIM_MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */

#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 9

// board
int board[DIM_MAX][DIM_MAX];

// dimensions
int d;

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);

void swap(int* a, int *b);

int main(int argc, string argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    // open log
    FILE *file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }

    // greet user with instructions
    greet();

    // initialize the board
    init();

    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // log the current state of the board (for testing)

        for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                fprintf(file, "%i", board[i][j]);
                if (j < d - 1)
                {
                    fprintf(file, "|");
                }
            }
            fprintf(file, "\n");
        }
        fflush(file);


        // check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = get_int();

        // quit if user inputs 0 (for testing)
        if (tile == 0)
        {
            break;
        }

        // log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        // sleep thread for animation's sake
        usleep(500000);
    }

    // close log
    fclose(file);

    // success
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(2000000);//unit: microsecond
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).
 */
void init(void)
{
    // TODO
    //board represented by 2-D array
    //size of board
    //contain the starting state, which starts in descending order, left to right top to buttom
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            board[i][j] = d * d - d * i - j - 1;
        }
    }

    //if dimension is even, swap 1 & 2
    if (! (d % 2))
    {
        board[d - 1][d - 2] = 2;
        board[d - 1][d - 3] = 1;
    }

}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    // TODO
    //print the current state of the board
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            //print a blank space before single digit
            if (board[i][j] < 10 && board[i][j] > 0 )
            {
                printf("%2i ",board[i][j]);
            }
            else if (board[i][j] > 9)
            {
                printf("%2i ",board[i][j]);
            }
            else
            {
                printf(" _ ");
            }
        }
        printf("\n");
    }

}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false.
 */
bool move(int tile)//given tile number
{
    // TODO
    int Xloc_tile = 0;
    int Yloc_tile = 0;
    int Xloc_blank = 0;
    int Yloc_blank = 0;

    //find the tile location
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            if (board[i][j] == tile)
            {
                Xloc_tile = i;
                Yloc_tile = j;
            }
            else if (board[i][j] == 0)
            {
                Xloc_blank = i;
                Yloc_blank = j;
            }
            else {}
        }
        printf("\n");
    }
    //judge if the block is movable
    if (abs(Xloc_tile - Xloc_blank) + abs(Yloc_tile - Yloc_blank) == 1 )
    {
        swap(&board[Xloc_tile][Yloc_tile],&board[Xloc_blank][Yloc_blank]);
        return true;
    }
    else
    {
        return false;
    }

}

void swap(int *a, int *b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

/**
 * Returns true if game is won (i.e., board is in winning configuration),
 * else false.
 */
bool won(void)
{
    // TODO
    //iterate over board and check value
    bool flag = true;
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            if ((i + 1) * (j + 1) < d * d)
            {
               flag = flag && (board[i][j] == i * d + j + 1);
            }

        }
        printf("\n");
    }
    return flag;
}
