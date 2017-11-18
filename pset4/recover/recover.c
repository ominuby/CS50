#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    //ensure proper usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover image");
        return 1;
    }
    // file to read
    char *card = argv[1];
    FILE *cptr = fopen (card, "r");
    //ensure input figure can be opened
    if (cptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", card);
        return 2;
    }

    int count = 0;
    int found = 0;
    unsigned char buffer[512];
    FILE *outptr = NULL;
    while(fread(&buffer, sizeof(buffer), 1, cptr))
    {
        //repeat until end of card
        //start a new JPEG?
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xe0) == 0xe0)
        {
            found = 1;
            //close last JPEG
            if(outptr != NULL)
            {
                fclose(outptr);
            }
            // New JPEG to write
            char filename[10];
            sprintf(filename,"%03i.jpg",count);
            outptr = fopen(filename,"w");
            //read 512 bytes into a buffer
            count++;
        }
        //already found a JPEG?
        if (found != 0)
        {
            fwrite(&buffer, sizeof(buffer), 1, outptr);
        }

    }
    // close remaining files
    fclose(cptr);
    return 0;
}