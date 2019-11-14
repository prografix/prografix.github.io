/***********************************************************************
	HA arithmetic coder
***********************************************************************/

#include "file.h"
#include "acoder.h"

/***********************************************************************
  Bit I/O
***********************************************************************/

#define putbit(b) 	{ ppat<<=1;				\
			  if (b) ppat|=1;			\
			  if (ppat&0x100) {			\
				file.putc(&ppat);		\
				ppat=1;				\
			  }					\
			}


#define getbit(b) 	{ gpat<<=1;				\
			  if (!(gpat&0xff)) {			\
                gpat = 0;               \
                if ( ! file.getc(&gpat) ) gpat=0x100;	\
				else {				\
					gpat<<=1;		\
					gpat|=1;		\
				}				\
			  }					\
			  b|=(gpat&0x100)>>8;			\
			}

/***********************************************************************
  Arithmetic encoding
***********************************************************************/

void ArithEncoder::out ( nat16 low, nat16 high, nat16 tot )
{    
    register nat32 r;
    
    r=(nat32)(h-l)+1;
    h=(nat16)(r*high/tot-1)+l;
    l+=(nat16)(r*low/tot);
    if (!((h^l)&0x8000))
    {
	    putbit(l&0x8000);
	    while(s)
        {
	        --s;
	        putbit(~l&0x8000);
	    }
	    l<<=1;
	    h<<=1;
	    h|=1;
	    while (!((h^l)&0x8000))
        {
	        putbit(l&0x8000);
	        l<<=1;
	        h<<=1;
	        h|=1;
	    }
    }
    while ((l&0x4000)&&!(h&0x4000))
    {
	    ++s;
	    l<<=1;
	    l&=0x7fff;
	    h<<=1;
	    h|=0x8001;
    }
}

void ArithEncoder::end()
{
    ++s;
    putbit(l&0x4000);
    while (s--)
    {
	    putbit(~l&0x4000);
    }
    if (ppat==1) return;
    while(!(ppat&0x100)) ppat<<=1;
    file.putc(&ppat);
}

/***********************************************************************
  Arithmetic decoding
***********************************************************************/

ArithDecoder::ArithDecoder ( IReadFile & f ) : file ( f )
{
    h=0xffff;
    l=0;
    gpat=0;
    v = 0;
    file.getc(&v);
    v <<= 8;
    file.getc(&v);
}

void ArithDecoder::in(nat16 low, nat16 high, nat16 tot)
{
    register nat32 r;
    
    r=(nat32)(h-l)+1;
    h=(nat16)(r*high/tot-1)+l;
    l+=(nat16)(r*low/tot);
    while (!((h^l)&0x8000))
    {
	    l<<=1;
	    h<<=1;
	    h|=1;
	    v<<=1;
	    getbit(v);
    }
    while ((l&0x4000)&&!(h&0x4000))
    {
	    l<<=1;
	    l&=0x7fff;
	    h<<=1;
	    h|=0x8001;
	    v<<=1;
	    v^=0x8000;
	    getbit(v);
    }
}

nat16 ArithDecoder::threshold_val ( nat16 tot )
{	
    return nat16 ( ( nat32 ( v - l + 1 ) * tot - 1 ) / nat32 ( h - l + 1 ) );
}