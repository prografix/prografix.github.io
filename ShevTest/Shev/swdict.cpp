/***********************************************************************
	HA sliding window dictionary
***********************************************************************/

#include "file.h"
#include "swdict.h"

#define HSIZE	16384
#define HSHIFT	3
#define HASH(p) ((b[p]^((b[p+1]^(b[p+2]<<HSHIFT))<<HSHIFT))&(HSIZE-1))
#define MAXCNT	1024

SWD1::SWD1(nat16 maxl, nat16 bufl, IReadFile & f) : file(f)
{	
    register int16 i;
    
    iblen=maxl; 
    cblen=bufl;
    blen=cblen+iblen;
    ll = new nat16[blen];
    best = new nat16[blen];
    ccnt = new nat16[HSIZE];
    cr = new nat16[HSIZE];
    b = new unsigned char[blen+iblen-1];
    for (i=0;i<HSIZE;++i) ccnt[i]=0; 
    binb=bbf=bbl=inptr=0;
    while (bbl<iblen)
    {
	    if ( ! file.getc(b+inptr) ) break;
	    inptr++;
	    bbl++;
    }
    mlf = MINLEN-1;
}

SWD1::~SWD1()
{
    delete[] b;
    delete[] cr;
    delete[] ccnt;
    delete[] best;
    delete[] ll;
}

void SWD1::accept()
{
    register int16 i,j;
    j = mlf - 2;
    do
    {   		/* Relies on non changed mlf !!! */
	    if (binb==cblen) --ccnt[HASH(inptr)];
	    else ++binb;
	    i=HASH(bbf);
	    ll[bbf]=cr[i];
	    cr[i]=bbf;
	    best[bbf]=30000;
	    ccnt[i]++;
	    if (++bbf==blen) bbf=0;
        unsigned char c;
	    if ( ! file.getc(&c) )
        {
	        --bbl;
	        if(++inptr==blen) inptr = 0;
	        continue;
	    }
	    if (inptr<iblen-1)
        {
	        b[inptr+blen] = b[inptr] = c;
	        ++inptr;
	    }
	    else
        {
	        b[inptr] = c;
	        if(++inptr == blen) inptr = 0;
	    }
    }
    while (--j);
    mlf = MINLEN - 1;
}

void SWD1::findbest()
{
    register nat16 i,ref,cnt,ptr,start_len;
    
    i=HASH(bbf);
    if ((cnt=ccnt[i]++)>MAXCNT) cnt=MAXCNT;
    ptr=ll[bbf]=cr[i];
    cr[i]=bbf;
    _char=b[bbf];
    if ((start_len = mlf)>=bbl)
    {
	    if (bbl==0) _char = -1;
	    best[bbf]=30000;
    }
    else
    {
	    for (ref=b[bbf+mlf-1];cnt--;ptr=ll[ptr])
        {
	        if (b[ptr+mlf-1]==ref && b[ptr]==b[bbf] && b[ptr+1]==b[bbf+1])
            {
		        {
		            register unsigned char *p1=b+ptr+3,*p2=b+bbf+3;
		            for (i=3;i<bbl;++i)
                    {
			            if (*p1++!=*p2++) break; 
		            }
		        }
		        if (i<=mlf) continue;
		        bpos=ptr;				
		        if ((mlf=i)==bbl || best[ptr]<i) break;
		        ref=b[bbf+mlf-1];
	        }
	    }
	    best[bbf] = mlf;
	    if (mlf>start_len)
        {
	        if (bpos<bbf) bpos = bbf - bpos - 1;
	        else bpos = blen - 1 - bpos + bbf;
	    }
    }
    if (binb==cblen) --ccnt[HASH(inptr)];
    else ++binb;
    if (++bbf==blen) bbf=0;
    unsigned char c;
    if ( ! file.getc(&c) )
    {
	    --bbl;
	    if(++inptr==blen) inptr=0;
	    return;
    }
    if (inptr<iblen-1)
    {
	    b[inptr+blen]=b[inptr]=c;
	    ++inptr;
    }
    else
    {
	    b[inptr]=c;
	    if (++inptr==blen) inptr=0;
    }
}	

SWD2::SWD2 ( nat16 bufl, IWriteFile & f ) : file(f)
{
    cblen = bufl;
    b = new unsigned char[cblen];
    bbf = 0;
}

SWD2::~SWD2()
{
    delete[] b;
}

void SWD2::dpair(nat16 l, nat16 p)
{
    if (bbf>p) p=bbf-1-p;
    else p=cblen-1-p+bbf;
    while (l--)
    {
	    b[bbf] = b[p];
        file.putc(b+p);
	    if (++bbf==cblen) bbf = 0;
	    if (++p==cblen) p = 0;
    }
}

void SWD2::dchar(unsigned char c)
{
    b[bbf]=c;
    file.putc(&c);
    if (++bbf==cblen) bbf=0;
}
