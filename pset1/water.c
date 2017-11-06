#include<cs50.h>
#include<stdio.h>

int main(void)
{
    printf("Minutes:");
    int min=get_int();
    printf("Bottles:%d\n",min*12);
}