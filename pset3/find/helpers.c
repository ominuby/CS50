/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */

#include <cs50.h>
#include "stdio.h"
#include "helpers.h"
#include <math.h>

#define MAX_ARRAY_NUM 65536

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
    // Know that the max array's number will be less than 65536
    int number[MAX_ARRAY_NUM] = {0};
    for (int i = 0; i < n; i++)
    {
        number[values[i]]++;
    }
    // Put elements into new array
    int sorted[n];
    for (int i = 0; i < n; i++)
    {
        sorted[i] = 0;
    }
    //dynamic array needs to assign value by loop

    int s = 0;
    int max_array_n = MAX_ARRAY_NUM;
    for (int j = 0; j < max_array_n; j++)
    {
        if (number[j] != 0)
        {
            for (int d = 0; d < number[j]; d++)
            {
                sorted[s] = j;
                s++;
            }
        }


    }

    for (int i = 0; i < n; i++)
    {
       values[i] = sorted[i];
    }

    return;
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