/***********************************************************************
	HA sliding window dictionary
***********************************************************************/

#define MINLEN 	3	/* Minimum possible match length for this implementation */

class SWD1
{
    nat16 blen,iblen;
    nat16 cblen,binb;
    nat16 bbf,bbl,inptr;
    nat16 *ccnt, *ll, *cr, *best;
    unsigned char * b;
    class IReadFile & file;
public:
    nat16 bpos, mlf;
    int16 _char;

    SWD1(nat16 maxl, nat16 bufl, IReadFile & f);
                	/* maxl=max len to be found  */
					/* bufl=dictionary buffer len */
					/* bufl+2*maxl-1<32768 !!! */
    void findbest();
    void accept();
    ~SWD1();
};

class SWD2
{
    nat16 cblen;
    nat16 bbf;
    unsigned char * b;
    class IWriteFile & file;
public:
    SWD2(nat16 bufl, IWriteFile & f);
    void dpair(nat16 l, nat16 p);
    void dchar(unsigned char c);
    ~SWD2();
};
