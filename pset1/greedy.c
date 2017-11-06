#include<cs50.h>
#include<stdio.h>
#include<math.h>

int main(void)
{
    printf("O hai! How much change is owed?\n");
    float cash=get_float();
    while (cash < 0.00)
    {
        printf("How much change is owed?\n");
        cash = get_float();
    }
    int n = 0;
    int cent = round(cash*100);
    while(cent > 0)
    {
        if (cent >= 25)
        {
            cent = cent - 25;
            n++;
        }
        else if (cent >= 10)
        {
            cent = cent - 10;
            n++;
        }
        else if (cent >= 5)
        {
            cent = cent - 5;
            n++;
        }
        else
        {
            cent = cent - 1;
            n++;
        }
        printf("%d\n",cent);
    }
    printf("%d\n",n);
}