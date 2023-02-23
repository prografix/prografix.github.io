#include <string.h>
#include "compact.hpp"
//
// Member functions for the compact data type implement a static Huffman coder.
//

void HuffmanDecoder::readtree()
{
    int i;
    longest = pfio->lookbits(4,0xf);
    pfio->skipbits(4);
    int tablesize = 1<<longest;
    longmask = tablesize - 1;
    if (!longest)
        num_symbols_of_length[0] = 1;
    else
        num_symbols_of_length[0] = 0;
    for (i = 1; i <= longest; i++)
    {
        num_symbols_of_length[i] = pfio->lookbits(8,0xff);
        pfio->skipbits(8);
    }
    int k = 0;
    for (i = 0; i <= longest; i++)
    {
        for (int j = 0; j < num_symbols_of_length[i]; j++)
        {
            ordered_symbols[k++] = pfio->lookbits(8,0xff);
            pfio->skipbits(8);
        }
    }

    if (!longest)   // Null table
    {
        memset(len,0,TABLESIZE);
        memset(dec,ordered_symbols[0],TABLESIZE);
        return;
    }
// First create temporary tables ordered by code length
    unsigned char codelength[257];
    unsigned short codebits[256];
    k = 0;
    for (i = 1; i <= longest; i++)
    {
        for (int j = 0; j < num_symbols_of_length[i]; j++) codelength[k++] = i;
    }
    codelength[k] = 0;  // Sentinal
    int lastsymbol = k;
    int codeword = 0;
    k = 0;
    int total = codelength[0];
    while(codelength[k])
    {
        while(codelength[k] == total)
        {
            codebits[k++] = codeword;
            codeword++;
        }
        codeword <<= 1;
        total++;
    }
// And finally install in lookup table
    k = 0;
    for (int length = 1; length <= longest; length++)
    {
        for (i = 1; i <= num_symbols_of_length[length]; i++)
        {
            int entry = codebits[k] << (longest-length);
            int j = 1 << (longest-length);
            while(j--)
            {
                dec[entry] = ordered_symbols[k];
                len[entry] = length;
                entry++;
            }
            ++k;
        }
    }
}

/*************************************************************/

void HuffmanEncoder::reset()
{
    for ( int i = 0; i < 256; ++i ) freq[i] = 0;
}

void HuffmanEncoder::put ( int symbol )
{
    int cnt = enclength[symbol];
    if ( cnt != 0 ) pfio->putbits ( encode[symbol], cnt );
}

// Write the Huffman tree to the output code file
void HuffmanEncoder::writetree()
{
    int i;
    pfio->putbit(longest & 0x8);
    pfio->putbit(longest & 0x4);
    pfio->putbit(longest & 0x2);
    pfio->putbit(longest & 0x1);
    for (i = 1; i <= longest; i++)
    {
        pfio->putbyte(num_symbols_of_length[i]);
    }
    // Then the literals
    int k = 0;
    for (i = 0; i <= longest; i++)
    {
        for (int j = 0; j < num_symbols_of_length[i]; j++)
        {
            pfio->putbyte(ordered_symbols[k++]);
        }
    }
//maketable_encode
    unsigned char codelength[257];
    unsigned short codebits[256];
    k = 0;
    for (i = 1; i <= longest; i++)
    {
        for (int j = 0; j < num_symbols_of_length[i]; j++) codelength[k++] = i;
    }
    codelength[k] = 0;  // Sentinal
    int lastsymbol = k;
    int codeword = 0;
    k = 0;
    int total = codelength[0];
    while(codelength[k])
    {
        while(codelength[k] == total)
        {
            codebits[k++] = codeword;
            codeword++;
        }
        codeword <<= 1;
        total++;
    }
    // And finally install in lookup table
    memset(enclength,0,256);    // So all unused slots will be 0

    for (k = 0; k < lastsymbol; k++)
    {
        encode[ordered_symbols[k]] = codebits[k];
        enclength[ordered_symbols[k]] = codelength[k];
    }
}

void HuffmanEncoder::buildtree()
{
    int i;
    int h1;
    int h2;
    int length[256];
    int descendents[256];
    for (i = 0; i < 256; i++)
    {
        length[i] = 0;
        descendents[i] = -1;
    }
    for(;;)
    {
        h1 = -1;
        h2 = -1;
        // Find smallest non-zero frequency
        for (i = 0; i < 256; i++)
        {
            if (freq[i])
            {
                if (h1 == -1 || freq[i] < freq[h1])
                {
                    if (h2 == -1 || freq[h1] < freq[h2]) h2 = h1;
                    h1 = i;
                }
                else if (h2 == -1 || freq[i] < freq[h2]) h2 = i;
            }
        }
        if (h2 == -1) // Found the root
            break;
        // Merge the two trees
        freq[h1] += freq[h2];
        freq[h2] = 0;
        length[h1]++;
        while (descendents[h1] >= 0)
        {
            h1 = descendents[h1];   // Go down chain
            length[h1]++;
        }
        //  Put h2 on this chain
        descendents[h1] = h2;
        // And go down its chain
        while (descendents[h1] >= 0)
        {
            h1 = descendents[h1];
            length[h1]++;
        }
    }
    if (h1 == -1) // Empty tree - create dummy char
    {
        longest = 0;
        num_symbols_of_length[0] = 1;
        ordered_symbols[0] = 0;
        return;
    }
    if (length[h1] == 0)    // one symbol
    {
        longest = 0;
        num_symbols_of_length[0] = 1;
        ordered_symbols[0] = h1;
        return;
    }
    for (i = 0; i < 32; i++)
        num_symbols_of_length[i] = 0;
    for (i = 0; i < 256; i++)
    {
        if (length[i])
            num_symbols_of_length[length[i]]++;
            // Assume never have case of 256 equal symbols
    }
    // Reduce to TABLEBITS-sized words using JPEG algorithm
    for (i = 31; i > TABLEBITS; i--)
    {
        while(num_symbols_of_length[i])
        {
            int j = i - 2;
            while (!num_symbols_of_length[j]) --j;
            num_symbols_of_length[i] -= 2;
            num_symbols_of_length[i-1]++;
            num_symbols_of_length[j+1] += 2;
            num_symbols_of_length[j]--;
        }
    }
    while(num_symbols_of_length[i] == 0) --i;
    // i is now length of longest codeword
    longest = i;
    int k = 0;
    for (i = 1; i <= 31; i++)
    {
        for (int j = 0; j < 256; j++)
        {
            if (length[j] == i) ordered_symbols[k++] = j;
        }
    }
}