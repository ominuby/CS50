#include<stdio.h>
#include<cs50.h>
#include<string.h>

int main(int argc, string argv[])
{   //avoid too much input argument
    if(argc != 2)
    {
        printf("Usage: ./caesar k\n");
        return 1;
    }
    else
    {
        int k = atoi(argv[1]);
        string pt;//declare the string type out of the while loop

        //avoid input is not a number or it is negative
        do
        {
            printf("plaintext:");
            pt = get_string();
        } while(k < 0);

        string ct = pt;
        for (int i = 0, n = strlen(pt); i < n; i++)
        {

            //capital
            if ((int)pt[i] > 64 && (int)pt[i] < 91 )
            {
                ct[i] = (pt[i] - 64 + k) % 26 + 64;
            }
            //lowercase
            else if ((int)pt[i] > 96 && (int)pt[i] < 123)
            {
                ct[i] = (pt[i] - 96 + k) % 26 + 96;
            }
        }
        printf("ciphertext: %s\n", ct);

        return 0;
    }
}