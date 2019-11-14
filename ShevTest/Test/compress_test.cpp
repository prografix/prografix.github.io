
#include "../Shev/lzh.h"
#include "../Shev/ShevArray.h"
#include "../Shev/PseudoFile.h"
#include "Display.h"

void compress_test ()
{
    FixArray<bit8, 1000> source;
    nat i;
    static nat k = 1;
    for ( i = 0; i < source.size(); ++i ) source[i] = i * k;
    k += 2;
    PseudoReadSeekFile sfile ( source );
    Suite<bit8> arr1;
    PseudoFile cfile ( 0, arr1 );
    lzh_pack ( sfile, cfile );
//    asc_pack ( sfile, cfile );
    cfile.rewind();
    Suite<bit8> arr2;
    PseudoWriteFile ofile ( arr2 );
    lzh_unpack ( cfile, ofile );
//    asc_unpack ( cfile, ofile );
    nat nn = ofile.tell();
    if ( source.size() != nn )
    {
        display << "ns != nn" << NL;
    }
    else
    {
        for ( i = 0; i < source.size(); ++i )
        {
            if ( source[i] != arr2[i] )
            {
                display << i << NL;
                break;
            }
        }
    }
    display << source.size() << cfile.tell() << "end" << NL;
}