
#include <stdio.h>
#include "compact.hpp"

void rgb_ycc ( unsigned char ***components, int ymax, int xmax, int smooth );
void makescratch ( const int xmax, const int ymax );
void deletescratch ( const int ymax );
void btpc_enc_plane ( unsigned char ** in, const int xmax, const int ymax,
    const int fromlevel, const int tolevel, const int * const spacing, 
    HuffmanEncoder *codeout, const int clipval );

//
// Calculation of quantization levels from quality parameter
//
int calcq ( int maxval, int quality, int *qvals, int numbands )
 {
    if ((quality > 100) || (quality < 0)) return -1;
    int topval = ( 100 - quality ) / 2;
    // topval will be the quantizer spacing on the finest band
    topval *= maxval;
    topval /= 255;
    if (!topval)
    {   // Quality == 100 or scaling makes it so
        for (int i = 0; i < numbands; i++) *qvals++ = 1;
        return 1;
    }
    float qratio;
    if (topval >= 40)
    {
        topval += (topval-25) + 3*(topval-40);
        qratio = 0.75f;
    }
    else if (topval > 25)
    {
        topval += (topval-25);
        qratio = 0.75f;
    }
    else
        qratio = 0.8f;
    qvals[numbands-1] = topval;
    for (int i = numbands-2; i >= 0; i--)
        qvals[i] = (int)((((float)qvals[i+1])*qratio)+0.99999);
    return 1;
}

bool cbtpc ( int nx, int ny, const unsigned char * data,
             int bpp, int maxval, int quality, IWriteFile & file )
{
    int i, n = nx * ny;
    unsigned char **components[3];
    unsigned char * temp = new unsigned char[bpp*n];
    {
        unsigned char * t = temp;
        for ( i = n; --i >= 0; ) t[i] = data[bpp*i];
        components[0] = new unsigned char *[ny];
        for ( i = 0; i < ny; ++i ) components[0][i] = t + nx*i;
    }
    if ( bpp > 1 )
    {
        unsigned char * t = temp + n;
        for ( int i = n; --i >= 0; ) t[i] = data[bpp*i+1];
        components[1] = new unsigned char *[ny];
        for ( i = 0; i < ny; ++i ) components[1][i] = t + nx*i;
    }
    if ( bpp > 2 )
    {
        unsigned char * t = temp + n + n;
        for ( int i = n; --i >= 0; ) t[i] = data[bpp*i+2];
        components[2] = new unsigned char *[ny];
        for ( i = 0; i < ny; ++i ) components[2][i] = t + nx*i;
    }
    const int numlevels = 4;  // Number of levels in tree
    int compnum;
    char coltype = 'G'; // GRB or monochrome
    char uvfilter = '0';    // No downsampling
    // Get picture into unsigned char arrays
    // Make ymax equal to next highest multiple of blocksize
    int ymax = ((ny+(1<<numlevels)-1)>>numlevels)<<numlevels;
    // Make xmax equal to next highest multiple of 2*blocksize
    // Do this so that points can be processed in pairs in binary tree
    int xmax = ((nx+(1<<(numlevels+1))-1)>>(numlevels+1))<<(numlevels+1);
    int spacing[65];    // Quantizer spacings
    if ( calcq(maxval, quality, spacing+1, numlevels*2) < 0 )
    {
//      fprintf(stderr,"Quality parameter must be between 0 and 100\n");
        return 0;
    }
    if ((bpp == 3)&&(spacing[numlevels*2] >= 2))
    {
        coltype = 'Y';
        uvfilter = 'B'; // Box filter
        rgb_ycc(components,ymax,xmax,1);
    }
    // Write header
    if (bpp == 1)
    {
        char buf[80];
        int n = sprintf(buf,"btpc 4.b1%c%cG0\n%d\n%d\n", 
            numlevels+'0', numcoders+'0', ny, nx);
        file.write ( buf, 1, n );
    }
    else
    {
        char buf[80];
        int n = sprintf(buf,"btpc 4.b3%c%c%c%c\n%d\n%d\n", 
            numlevels+'0', numcoders+'0', coltype, uvfilter, ny, nx);
        file.write ( buf, 1, n );
    }
    // Output all the quantizer spacing parameters
    for (i = 1; i <=numlevels*2; i++)
        file.putc(spacing+i);
    // And the maxval parameter
    file.putc(&maxval);
    // Set up the huffman coders
    WriteBitFile fio(file);
    HuffmanEncoder encoder[numcoders];
    for (i = 0; i < numcoders; i++) encoder[i].attach(&fio);
    // Encode each plane of the picture
    makescratch(xmax, ymax);
    for (compnum = 0; compnum < bpp; compnum++)
    {
        int tolevel = (compnum && (uvfilter != '0'));
        int clipval = tolevel ? 255 : maxval;
        btpc_enc_plane(components[compnum],xmax,ymax,numlevels, tolevel, spacing, encoder, clipval);
    }
    fio.flush();
    deletescratch(ymax);
    if ( bpp > 2 ) delete[] components[2];
    if ( bpp > 1 ) delete[] components[1];
    delete[] components[0];
    delete[] temp;
    return 1;
}

