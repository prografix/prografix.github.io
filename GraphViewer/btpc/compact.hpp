//
//
// Binary Tree Predictive Coding.
// John Robinson, 1994, 1995.
//
// Header file for static huffman coder.
//

#include "../File.h"

class ReadBitFile
{
    IReadFile & file;
// Input is double buffered: first get a chunk into buf, then get bits into bitbuf
    int charsleft;
    unsigned long   bitbuf;
    int valid;  // bits left in bitbuf
    unsigned char *pbuf;
    unsigned char buf[2048];
public:
    ReadBitFile ( IReadFile & p ) : file(p)
    { 
        bitbuf = 0; charsleft = 0; valid = 0;
    }
    int getbit()
    {
        if (!valid)
        {
            bitbuf = (charsleft-- ? *pbuf++ : 
                (fillbuf() && charsleft--) ? *pbuf++ : 0);
            valid = 8;
        }
        return (int) ((bitbuf >> --valid) & 1);
    }
    int fillbuf()
    {
        return(charsleft = file.read((pbuf = buf),1,2048));
    }
    void loadforlook()
    {
        bitbuf = (bitbuf<<8) + (charsleft-- ? *pbuf++ : 
            (fillbuf() && charsleft--) ? *pbuf++ : 0);
    }
    int lookbits(const int& bits, const int& mask)
    {
        if (valid < bits)
        {
            loadforlook();
            valid += 8;
        }
        if (valid < bits)
        {
            loadforlook();
            valid += 8;
        }
        return (int) ((bitbuf >> (valid-bits)) & mask);
    }
    void skipbits(const int bits)
    {
        valid -= bits;
    }
    int get(unsigned char *declength, unsigned char *decode, int bits, const int mask)
    {
        register int v = valid;
        if (v < bits)
        {
            loadforlook();
            if ((v += 8) < bits)
            {
                loadforlook();
                v += 8;
            }
        }
        bits = (int) ((bitbuf >> (v-bits)) & mask);
        valid = v - declength[bits];
        return decode[bits];
    }
};

class WriteBitFile
{
    IWriteFile & file;
// Output works a byte at a time and is therefore simple
    int outbyte;
    int outbits; // Number of output bits
public:
    WriteBitFile ( IWriteFile & p ) : file(p), outbyte(0), outbits(0) {}

    void putbit(int val)
    {
        outbyte <<= 1;
        if (val)
            outbyte++;
        outbits++;
        if ((outbits &= 0x7) == 0)
            file.putc(&outbyte);
    }

    void putbits(int val, int numbits)
    {
        int space = 8 - outbits;
        if (numbits > space)
        {
            outbyte <<= space;
            outbyte |= val>>(numbits = numbits-space);
            file.putc(&outbyte);
            outbits = 0;
            space = 8;
            val &= (1<<numbits) - 1;
            if (numbits > space)
            { 
            // Now space = 8 and outbyte is empty
                outbyte = val >> (numbits = numbits - space);
                file.putc(&outbyte);
                val &= (1<<numbits) - 1;
            }
        }
        // Now guaranteed that numbits <= space
        outbyte <<= numbits;
        outbyte |= val;
        outbits += numbits;
        if ((outbits &= 0x7) == 0) file.putc(&outbyte);
    }

    void putbyte(unsigned char val)
    {
        int thisbyte = 8 - outbits;
        outbyte <<= thisbyte;
        outbyte |= (val >> outbits);
        file.putc(&outbyte);
        outbyte = val;
    }

    void flush()
    {
        if (outbits)
        {
            outbyte <<= 8 - outbits;
            file.putc(&outbyte);
            outbits = 0;
        }
        file.flush();
    }
};

const int TABLEBITS = 12;
const int MASK = (1<<TABLEBITS)-1;
const int TABLESIZE = (1<<TABLEBITS);

class HuffmanEncoder
{
private:
    WriteBitFile * pfio;
    int longest;
    int longmask;
    unsigned char num_symbols_of_length[32];
    unsigned char ordered_symbols[256];
    long freq[256];
    unsigned short encode[256];
    unsigned char enclength[256];

public:
    HuffmanEncoder()
    {
        pfio = 0; 
        longest = 0;
        reset();
    }
    void attach(WriteBitFile *f) { pfio = f; }
    void putbit(int val) { pfio->putbit(val); }
    void reset();
    void inc(int symbol) {freq[symbol]++;};
//  void dec(int symbol) {freq[symbol]--;};
    void buildtree(void);
    void writetree(void);
    void put(int inbyte);
};

class HuffmanDecoder
{
private:
    ReadBitFile *pfio;
    int longest;
    int longmask;
    unsigned char num_symbols_of_length[32];
    unsigned char ordered_symbols[256];
    unsigned char dec[TABLESIZE];
    unsigned char len[TABLESIZE];
public:
    HuffmanDecoder()
    {
        pfio = 0; 
        longest = 0;
    }
    void attach(ReadBitFile *f) { pfio = f; }
    void readtree(void);
    int getbit() { return pfio->getbit(); }
    int get()
    {
        return pfio->get(len, dec, longest, longmask);
    }
};

const int numcoders = 4;
