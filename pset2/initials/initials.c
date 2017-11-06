#include <stdio.h>
#include <cs50.h>
#include <string.h>

int main(void)
{
    string name = get_string();
    if (name[0] > 90)
    {
        printf("%c", name[0] - 32);
    }
    else
    {
        printf("%c", name[0] );
    }
    for (int i = 1, n = strlen(name); i < n; i++)
    {
        if ( name[i] == ' ')
        {
            if (name[i+1] > 90)
            {
                printf("%c", name[i+1] - 32);
            }
            else
            {
                printf("%c", name[i+1]);
            }
        }
    }
        printf("\n");
}