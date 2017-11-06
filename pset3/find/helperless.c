/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */

#include <cs50.h>

#include "helpers.h"
#include <math.h>

//swap function declaration
void swap(int *a, int *b);
int Bsearch (int value, int values[], int low, int up);
/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[],  int n)
{
    // TODO: implement a searching algorithm
    //Binary search
    int ans = Bsearch (value, values, 0, n - 1);
    if (ans < 1) return true;
    else return false;
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    // TODO: implement a sorting algorithm
    //Bubble sort
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n - 1; j++)
        {
            if (values[j] > values [j + 1]) swap (&values[j], &values [j + 1]);
        }
    }

    return;
}

void swap(int *a, int *b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

int Bsearch (int value, int values[], int low, int up)
{
    if (low <= up)
    {
        if (values[(low + up) / 2] > value )
        {
            return Bsearch( value,  values,  low, (low + up) / 2 - 1);
        }
        else if (values[(low + up) / 2] < value )
        {
            return Bsearch( value,  values,  (low + up) / 2 + 1, up);
        }
        else
        {
            return 0;
        }
    }
    return 1;

}