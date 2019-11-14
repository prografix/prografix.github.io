/***********************************************************************
	HA arithmetic coder
***********************************************************************/

class ArithEncoder
{
    nat16 h, l;
    int16 s, ppat;
    class IWriteFile & file;
public:
    ArithEncoder ( IWriteFile & f ) : file ( f )
    {
        h = 0xffff;
        l = s = 0;
        ppat = 1;
    }
    void out ( nat16 low, nat16 high, nat16 tot );
    void end ();
};

class ArithDecoder
{
    nat16 h, l, v;
    int16 gpat;
    class IReadFile & file;
public:
    ArithDecoder ( IReadFile & f );
    void in ( nat16 low, nat16 high, nat16 tot );
    nat16 threshold_val ( nat16 tot );
};