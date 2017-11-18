#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include "bmp.h"

int main(int argc, char* argv[])
{
    // ensure proper useage
    if (argc != 4)
    {
        fprintf(stderr,"Usage: ./resize n infile outfile");
        return 1;
    }
    // file name
    char *infile = argv[2];
    char *outfile = argv[3];

    // Reading input file
    FILE *inptr = fopen (infile, "r");
        //ensure file can be opened
        if (inptr == NULL)
        {
            fprintf(stderr,"Could not open %s.\n", infile);
            return 2;
        }
    //Creating output file
    FILE *outptr = fopen (outfile, "w");

        //ensure file can be created
        if (outptr == NULL)
        {
            fclose(inptr);
            fprintf(stderr,"Could not open %s.\n", outfile);
            return 3;
        }

    // parse float information
    int factor = atoi (argv[1]);

    // the size is corresponding to input argument

        // read infile's BITMAPFILEHEADER
        BITMAPFILEHEADER bf;
        fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

        // read infile's BITMAPINFOHEADER
        BITMAPINFOHEADER bi;
        fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

        // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
        if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
            bi.biBitCount != 24 || bi.biCompression != 0)
        {
            fclose(outptr);
            fclose(inptr);
            fprintf(stderr, "Unsupported file format.\n");
            return 4;
        }

        // outfile's BITMAPFILEHEADER
        BITMAPFILEHEADER bf_out = bf;

        // outfile's BITMAPINFOHEADER
        BITMAPINFOHEADER bi_out = bi;

        // new length and width of bitmap
        int newWidth = bi.biWidth * factor;
//        int newHeight = abs(bi.biHeight) * factor;

        bi_out.biWidth = bi.biWidth * factor;
        bi_out.biHeight = bi.biHeight * factor;

        // determine padding for scanlines
        int padding_old = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
        int padding =  (4 - (newWidth * sizeof(RGBTRIPLE)) % 4) % 4;

        bi_out.biSizeImage = ((sizeof(RGBTRIPLE) * bi_out.biWidth + padding) * abs(bi_out.biHeight));
        bf_out.bfSize = bi_out.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

        // write outfile's BITMAPFILEHEADER
        fwrite(&bf_out,  sizeof(BITMAPFILEHEADER), 1, outptr);

        // write outfile's BITMAPINFOHEADER
        fwrite(&bi_out, sizeof(BITMAPINFOHEADER), 1, outptr);



    //Resize
        for (int i = 0; i < abs(bi.biHeight); i++)
        {
            //height pixel of new figure
            for (int h = 0; h < factor; h++)
            {
                // search appropriate position for loading
                fseek(inptr, sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + (bi.biWidth * 3 + padding_old) * i, SEEK_SET);
                for (int j = 0; j < bi.biWidth; j++)
                {
                    // temporary storage
                    RGBTRIPLE triple;
                    // read RGB triple from infile
                    fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

                    // width pixel of new figure
                    for (int w = 0; w < factor; w++)
                    {
                        // write RGB triple to outfile
                        fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                    }
                }
                // then add it back (to demonstrate how)
                for (int k = 0; k < padding; k++)
                {
                    fputc(0x00, outptr);
                }
            }
        }

    //closing files
    fclose(inptr);
    fclose(outptr);
    return 0;
}