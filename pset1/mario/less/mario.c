#include<stdio.h>
#include<cs50.h>

int main(void)
{
    int h;
    //first submit a integer, then judge if it is authorized
    do
    {
        printf("Height:");
        h = get_int();
    }
    while (h < 0 || h > 23);


    if (h == 0) {}
    else
    {
        //authorized number of stairs
        //print blocks by #-symbol
        for (int i = 0; i < h; i++)//every layer has h+1 blocks
        {
            for (int space = 0; space < h - 1 - i; space++)
            {
                printf(" ");
            }
            for (int block = 0; block < i + 2; block++)
            {
                printf("#");
            }
            printf("\n");
        }
    }
}