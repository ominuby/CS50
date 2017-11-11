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
    float factor = atof (argv[1]);
    printf("%f\n",factor);
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
        // new size of bitmap
        int newWidth = bi.biWidth * factor;
        int newHeight = abs(bi.biHeight) * factor;
        // write outfile's BITMAPFILEHEADER
        fwrite(&bf,  sizeof(BITMAPFILEHEADER), 1, outptr);

        // write outfile's BITMAPINFOHEADER
        fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // determine padding for scanlines
    int padding =  (4 - (newWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    //Resize
    // enlarge
//    if (factor >= 1)
//    {
        for (int i = 0; i < newHeight; i++)
        {
          // iterate over pixels in scanline
            for (int j = 0; j < newWidth; j++)
           {
                // temporary storage
                RGBTRIPLE triple;

                // read RGB triple from infile
                if (i % (int)factor == 0 && j % (int)factor == 0)
                {
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
                }
                // write RGB triple to outfile
                fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
            }

            // skip over padding, if any
            fseek(inptr, padding, SEEK_CUR);

            // then add it back (to demonstrate how)
            for (int k = 0; k < padding; k++)
            {
                fputc(0x00, outptr);
            }
        }
//    }
//    else // shrink
//    {

//    }

    //closing files
    fclose(inptr);
    fclose(outptr);
    return 0;
}