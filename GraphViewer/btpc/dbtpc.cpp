//
// Eight+one-band binary tree predictive decoder
//
// Version 4.
//
// Copyright (c) John Robinson. 1994, 1995, 1997.
//
#include <stdio.h>
#include <string.h>
#include "compact.hpp"

unsigned char *initclip(const int topval);
void ycc_rgb(unsigned char ***components, const int& rows, const int& cols,
const unsigned char *clip, const char& expand);
void btpc_dec_plane(unsigned char **out,const int& XMAX, const int& YMAX,
const int& fromlevel, const int& tolevel, const int *spacing,
HuffmanDecoder *codein, const int& clipval);

int btpc_read_header ( IReadSeekFile & file, int& pnmtype, int& rows, int& cols, 
    int& maxval, int& numlevels, char& coltype, char& uvfilter, int *spacing )
{
    char cbuf[12];
    char newline;
    int n_coders;
    file.read(cbuf,1,7);
    cbuf[7] = 0;
    if (strcmp(cbuf,"btpc 4."))
    {
//      fprintf(stderr, "This is not a valid btpc 4 file\n");
        return -1;
    }
    char buf[32];
    file.read(buf,1,32);
    file.rewind();
    for(int k = 3; --k >= 0;)
    {
        char c;
        while ( file.getc(&c) && c != '\n' ) continue;
    }
    sscanf ( buf, "%s %d %d%c", cbuf, &rows, &cols, &newline );
    if (cbuf[0] == 'a')
    {
//      fprintf(stderr, "This picture is coded with an old version ");
//      fprintf(stderr, "of btpc 4.\nIn some (rare) cases, the ");
//      fprintf(stderr, "decoding will fail because of bugs in the\n");
//      fprintf(stderr, "earlier version. Carrying on anyway...\n");
        cbuf[0] = 'b';
    }
    if (cbuf[0] == 'b')
    {
        pnmtype = cbuf[1] - '0';// 1 for PGM, 3 for PPM
        numlevels = cbuf[2] - '0';
        n_coders = cbuf[3] - '0';
        coltype = cbuf[4];  // 'G'=GRB, 'Y'=YUV
        uvfilter = cbuf[5]; // 'B'=box, '0'=none
        if (n_coders != numcoders)
        {
//          fprintf(stderr, "Sorry. This picture is coded with ");
//          fprintf(stderr, "%d coders. I can handle ", n_coders);
//          fprintf(stderr, "%d coders only\n",numcoders);
            return -1;
        }
    }
    else
    {
//      fprintf(stderr, "This is a version of btpc 4 that I don't ");
//      fprintf(stderr, "recognize.\nI'm going to guess some ");
//      fprintf(stderr, "parameters to try\n");
//      fprintf(stderr, "to recover the monochrome component.\n");
        pnmtype = 1;
        numlevels = 4;
        coltype = 'G';
        uvfilter = '0';
    }
    unsigned char c;
    for ( int i = 1; i <= numlevels*2; i++ )
    {
        file.getc(&c);
        spacing[i] = c;
    }
    file.getc(&c);
    maxval = c;
    return 0;
}

bool dbtpc ( IReadSeekFile & file, int & nx, int & ny, unsigned char * & data, int & bpp )
{
    int pnmtype, truerows, truecols, maxval, numlevels;
    char coltype, uvfilter;
    int spacing[65];
    int i;
    int compnum;
    if ( btpc_read_header ( file, pnmtype, truerows, truecols,  maxval, 
        numlevels, coltype, uvfilter, spacing ) )
        return false;
    // Make XMAX and YMAX equal to next highest multiple of 16
    int YMAX = ((truerows+(1<<numlevels)-1)>>numlevels)<<numlevels;
    int XMAX = ((truecols+(1<<(numlevels+1))-1)>>(numlevels+1))<<(numlevels+1);
    // Set up coders
    ReadBitFile fio(file);
    HuffmanDecoder decoder[numcoders];
    for (i = 0; i < numcoders; i++) decoder[i].attach(&fio);
    // Allocate space for decoded picture
    // If decoding into an existing array (e.g. for immediate display)
    // create line pointers out[i], and point at rows of array.
    unsigned char **components[3];
    unsigned char **out;
    for (compnum = 0; compnum < pnmtype; compnum++)
    {
        components[compnum] = out = new unsigned char*[YMAX];
        for (i = 0; i < YMAX; i++)
        {
            out[i] = new unsigned char[XMAX];
            memset(out[i],0,XMAX);
        }
    }
    // Decode each plane of the picture
    for (compnum = 0; compnum < pnmtype; compnum++)
    {
        int tolevel = (compnum && (uvfilter != '0'));
        int clipval = tolevel ? 255 : maxval;
        // Above line ensures that UV components are correctly clipped
        btpc_dec_plane(components[compnum],XMAX,YMAX,numlevels,
        tolevel, spacing, decoder, clipval);
    }
    // Convert colourspace back if necessary
    if (coltype == 'Y')
    {
        unsigned char *clip = initclip(maxval);
        ycc_rgb(components,truerows,truecols,clip,uvfilter);
    }
//  writepnm(argv[2], (const unsigned char ***) components, 
//      truerows, truecols, maxval,pnmtype);
    bpp = pnmtype;
    nx = truecols;
    ny = truerows;
    data = new unsigned char[bpp*nx*ny];
    for (compnum = 0; compnum < pnmtype; compnum++)
    {
        unsigned char * p = data + compnum;
        out = components[compnum];
        for (i = 0; i < YMAX; i++) 
        {
            if ( i < truerows )
            {
                for (int j = 0; j < truecols; j++)
                {
                    *p = out[i][j];
                    p += bpp;
                }
            }
            delete[] out[i];
        }
        delete[] out;
    }
    return true;
}
