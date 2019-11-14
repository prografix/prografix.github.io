
#include <math.h>
#include <stdarg.h>
#include "File.h"
#include "ShevArray.h"

void writeInt ( SuiteRef<char> & buf, int32 i, int32 width, bool plus, bool zero )
{
    const nat k = buf.size();
    bool minus = false;
    if ( i < 0 )
    {
        if ( i == -2147483647 - 1 )
        {
            buf.addAftLas ( CArrRef<char> ( "8463847412", 10 ) );
            width -= 10;
        }
        else
        {
            i = - i;
        }
        minus = true;
    }
    if ( buf.size() == k )
    {
        if ( ! i )
        {
            buf.inc() = '0';
            --width;
        }
        else
        {
            while ( i > 0 )
            {
                buf.inc() = '0' + char ( i % 10 );
                i /= 10;
                --width;
            }
        }
    }
    if ( zero )
    {
        while ( width > 1 )
        {
            buf.inc() = '0';
            --width;
        }
        if ( minus )
        {
            buf.inc() = '-';
        }
        else
        if ( plus )
        {
            buf.inc() = '+';
        }
        else
        if ( width > 0 )
        {
            buf.inc() = '0';
        }
    }
    else
    {
        if ( minus )
        {
            buf.inc() = '-';
        }
        else
        if ( plus )
        {
            buf.inc() = '+';
        }
    }
    ArrRef<char> ( buf, k, buf.size()-k ).reverse();
}

void writeInt ( SuiteRef<char> & buf, nat32 i, int32 width, bool plus, bool zero )
{
    const nat k = buf.size();
    if ( ! i )
    {
        buf.inc() = '0';
        --width;
    }
    else
    {
        while ( i > 0 )
        {
            buf.inc() = '0' + char ( i % 10 );
            i /= 10;
            --width;
        }
    }
    if ( zero )
    {
        while ( width > 1 )
        {
            buf.inc() = '0';
            --width;
        }
        if ( plus )
        {
            buf.inc() = '+';
        }
        else
        if ( width > 0 )
        {
            buf.inc() = '0';
        }
    }
    else
    {
        if ( plus )
        {
            buf.inc() = '+';
        }
    }
    ArrRef<char> ( buf, k, buf.size()-k ).reverse();
}

void writeHex ( SuiteRef<char> & buf, nat32 i, int32 width, char a, bool zero )
{
    const nat k = buf.size();
    if ( ! i )
    {
        buf.inc() = '0';
        --width;
    }
    else
    {
        a -= 10;
        while ( i > 0 )
        {
            const char c = char ( i % 16 );
            buf.inc() = c + ( c < 10 ? '0' : a );
            i /= 16;
            --width;
        }
    }
    if ( zero )
    {
        while ( width > 0 )
        {
            buf.inc() = '0';
            --width;
        }
    }
    ArrRef<char> ( buf, k, buf.size()-k ).reverse();
}

void copy ( double d, Suite<nat32> & arr )
{
    int ip;
    double t = frexp ( d, &ip );
    ip -= 53;
    nat64 n64 = (int64) ldexp ( t, 53 );
    nat32 * p32 = (nat32 *) & n64;
    arr.inc() = p32[0];
    if ( p32[1] ) arr.inc() = p32[1];
    nat k = ip / 32;
    if ( k > 0 )
    {
        arr.inc(k) <<= 2;
        for ( nat i = 0; i < k; ++i ) arr[i] = 0;
    }
    k = ip % 32;
    if ( !k ) return;
    nat32 n32 = 0;
    for ( nat i = 0; i < arr.size(); ++i )
    {
        n64 = arr[i];
        n64 <<= k;
        arr[i] = p32[0] + n32;
        n32 = p32[1];
    }
    if ( n32 ) arr.inc() = n32;
}

nat32 divide10 ( ArrRef<nat32> arr )
{
    nat64 n64 = 0;
    nat32 * p32 = (nat32 *) & n64;
    for ( nat i = 0; i < arr.size(); ++i )
    {
        nat32 & n = arr.las(i);
        p32[0] = n;
        n = nat32 ( n64 / 10 );
        p32[1] = nat32 ( n64 % 10 );
    }
    return p32[1];
}

void writeFltDec_ ( SuiteRef<char> & buf, real64 d, nat32 prec, bool plus, bool round )
{
    char sign = '-';
    bool isSign = false;
    if ( d < 0 )
    {
        isSign = true;
        d = - d;
    }
    else
    if ( plus )
    {
        isSign = true;
        sign = '+';
    }
    if ( round && prec < 308 ) // округляем число
    {
        real64 p = 2.;
        for ( nat32 i = 0; i < prec; ++i ) p *= 10.;
        d += 1. / p;
    }
    real64 f = modf ( d, &d ); // теперь в d целая часть числа, а в f - дробная
    if ( d > 0 )
    {
        const nat k = buf.size();
        if ( d < ldexp ( 1, 63 ) ) // если число поместится в int64
        {
            nat64 u = nat64 ( d );
            while ( u > 0 )
            {
                buf.inc() = '0' + char ( u % 10 );
                u /= 10;
            }
        }
        else
        {
            Suite<nat32> arr;
            copy ( d, arr );
            while ( arr.size() > 0 )
            {
                buf.inc() = '0' + char ( divide10 ( arr ) );
                if ( ! arr.las() ) arr.dec();
            }
        }
        if ( isSign ) buf.inc() = sign;
        ArrRef<char> ( buf, k, buf.size()-k ).reverse();
    }
    else
    {
        if ( isSign ) buf.inc() = sign;
        buf.inc() = '0';
    }
    buf.inc() = '.';
    for ( nat32 i = 0; i < prec; ++i )
    {
        real64 t;
        f = modf ( 10*f, &t );
        buf.inc() = '0' + char ( t );
    }
}

enum FPCLASS
{
    FPCLASS_NINF,
    FPCLASS_PINF,
    FPCLASS_NIND,
    FPCLASS_NQNAN,
    FPCLASS_PQNAN,
    FPCLASS_OTHER
};

FPCLASS fpclass ( const real64 & d )
{
    const nat32 * i = (const nat32 *)& d;
    if ( ( i[1] & 0x7FF00000 ) != 0x7FF00000 ) return FPCLASS_OTHER;
    const nat32 s = i[1] & 0x80000000;
    if ( i[0] == 0 )
    {
        if ( ( i[1] & 0x0FFFFF ) == 0 )
        {
            return s == 0 ? FPCLASS_PINF : FPCLASS_NINF;
        }
        if ( ( i[1] & 0x0FFFFF ) == 0x080000 )
        {
            return s == 0 ? FPCLASS_PQNAN : FPCLASS_NIND;
        }
    }
    return s == 0 ? FPCLASS_PQNAN : FPCLASS_NQNAN;
}

void writeFltDec ( SuiteRef<char> & buf, real64 d, nat32 prec, bool plus )
{
    const char * p = 0;
    switch ( fpclass ( d ) )
    {
    case FPCLASS_NINF:  p = "-1.#INF";  break;
    case FPCLASS_PINF:  p = "+1.#INF";  break;
    case FPCLASS_NIND:  p = "-1.#IND";  break;
    case FPCLASS_NQNAN: p = "-1.#QNAN"; break;
    case FPCLASS_PQNAN: p = "+1.#QNAN"; break;
    }
    if ( p )
    {
        for ( nat i = 0; p[i]; ++i ) buf.inc() = p[i];
    }
    else writeFltDec_ ( buf, d, prec, plus, true );
}

void writeExpDec ( SuiteRef<char> & buf, real64 d, nat32 prec, bool plus )
{
    const char * p = 0;
    switch ( fpclass ( d ) )
    {
    case FPCLASS_NINF:  p = "-1.#INF";  break;
    case FPCLASS_PINF:  p = "+1.#INF";  break;
    case FPCLASS_NIND:  p = "-1.#IND";  break;
    case FPCLASS_NQNAN: p = "-1.#QNAN"; break;
    case FPCLASS_PQNAN: p = "+1.#QNAN"; break;
    }
    if ( p )
    {
        for ( nat i = 0; p[i]; ++i ) buf.inc() = p[i];
        return;
    }
    if ( d == 0 )
    {
        writeFltDec_ ( buf, d, prec, plus, false );
        p = "e+000";
        for ( nat i = 0; p[i]; ++i ) buf.inc() = p[i];
        return;
    }
    int32 i = 0;
    real64 a = fabs ( d );
    while ( a >= 10. )
    {
        a  /= 10.;
        ++i;
    }
    while ( a < 1. )
    {
        a  *= 10.;
        --i;
    }
    if ( prec < 308 ) // округляем число
    {
        real64 p = 2.;
        for ( nat32 j = 0; j < prec; ++j ) p *= 10.;
        a += 1. / p;
        if ( a >= 10 ) a = 1., ++i;
    }
    if ( d < 0 ) a = -a;
    writeFltDec_ ( buf, a, prec, plus, false );
    buf.inc() = 'e';
    writeInt ( buf, i, 4, true, true );
}

/*-----------------------------------------------------------------------*

__vprinter is a table-driven design, for speed and flexibility. There are
two tables.  The first table classifies all 7-bit ASCII chars and then the
second table is the switch table which points to the function blocks which
handle the various classes of characters.

All characters with the 8th bit set are currently classed as don't cares,
which is the class of character also used for normal alphabetics.  All
characters less than ' ' (0x20) are also classed as don't cares.

*------------------------------------------------------------------------*/

typedef
        enum
        {
                _si,    /* sign fill +/-        */
                _af,    /* alternate form       */
                _ar,    /* format (width or precision) by argument */
                _lj,    /* left justify         */

                _pr,    /* precision            */
                _nu,    /* numeral              */
                _lo,    /* long                 */
                _ld,    /* long double, __int64 */
                _sh,    /* short, narrow        */
                _fz,    /* fill zeros           */

                _de,    /* decimal              */
                _oc,    /* octal                */
                _un,    /* unsigned decimal     */
                _he,    /* hexadecimal          */

                _pt,    /* pointer              */
                _fl,    /* float                */
                _ch,    /* char                 */
                _st,    /* string               */
                _Ch,    /* wide char            */
                _St,    /* wide string          */

                _ns,    /* number sent          */
                _zz,    /* terminator           */
                _dc,    /* don't care           */
                _pc,    /* percent              */

                _ne,    /* near pointer         */
                _fa,    /* far pointer          */
                _wi,    /* wide (for I64)       */
        } characterClass;

        /*  Here is the table of classes, indexed by character. */

static bit8 printCtype [96] =
{
/*       SP   !   "   #   $   %   &   '   (   )   *   +   ,   -   .   /  */
        _si,_dc,_dc,_af,_dc,_pc,_dc,_dc,_dc,_dc,_ar,_si,_dc,_lj,_pr,_dc,

/*        0   1   2   3   4   5   6   7   8   9   :   ;   <   =   >   ?  */
        _fz,_nu,_nu,_nu,_nu,_nu,_nu,_nu,_nu,_nu,_dc,_dc,_dc,_dc,_dc,_dc,

/*        _   A   B   C   D   E   F   G   H   I   J   K   L   M   N   O  */
        _dc,_dc,_dc,_Ch,_dc,_fl,_fl,_fl,_sh,_wi,_dc,_dc,_ld,_dc,_ne,_dc,

/*        P   Q   R   S   T   U   V   W   X   Y   Z   [   \   ]   ^   _  */
        _dc,_dc,_dc,_St,_dc,_dc,_dc,_dc,_he,_dc,_dc,_dc,_dc,_dc,_dc,_dc,

/*        `   a   b   c   d   e   f   g   h   i   j   k   l   m   n   o  */
        _dc,_dc,_dc,_ch,_de,_fl,_fl,_fl,_sh,_de,_dc,_dc,_lo,_dc,_ns,_oc,

/*        p   q   r   s   t   u   v   w   x   y   z   {   |   }   ~ DEL  */
        _pt,_dc,_dc,_st,_dc,_un,_dc,_dc,_he,_dc,_dc,_dc,_dc,_dc,_dc,_dc,
};


/*---------------------------------------------------------------------*

Name            __vprinter - sends formatted output

Usage           int   __vprinter (putnF  *putter,
                                         void   *outP,
                                         const char *formP,
                                         int __use_nsize,
                                         size_t __nsize,
                                         va_list *argP)

Prototype in    _printf.h

Description     The list of arguments *argP is combined with literal text in
                the format string *formP according to format specifications
                inside the format string.

                The supplied procedure *putter is used to generate the output.
                It is required to take the string S, which has been
                constructed by __vprinter, and copy it to the destination
                outP.  The destination may be a string, a FILE, or whatever
                other class of construct the caller requires.  It is possible
                for several calls to be made to *putter since the buffer S
                is of limited size.

                *putter is required to preserve  SI, DI.

                The only purpose of the outP argument is to be passed through
                to putter.

                The object at *argP is a record of unknown structure, which
                structure is interpreted with the aid of the format string.
                Each field of the structure may be an integer, long, double,
                or string (char *).  Chars may appear but occupy integer-sized
                cells.  Floats, character arrays, and structures may not
                appear.

                __use_nsize is normally 0.  The (v)snprintf series of
                functions, will set __use_nsize to 1 and will also pass a
                valid maximum size value in the __nsize parameter.  This
                __nsize value represents the maximum length of the output
                buffer. The entire format string is processed and then the
                number of characters writen (via *outP) is returned except:

                  1. When the __nsize value is less than the number of chars
                     needed to be written.  In this case the extra
                     characters are counted and ignored (not written to the
                     output via *outP) and the number of characters that
                     COULD HAVE BEEN WRITTEN if the __nsize value had been
                     sufficiently large.

                  2. When __nsize is 0, then no characters are written, in
                     fact, outP can even be NULL.  The format string is
                     processed as normal and the number of characters needed
                     to store this string is calculated and returned.


Return value    The result of the function is a count of the characters sent to
                *outP (subject to the previously mentions restrictions and
                enhancements).

                There is no error indication.  When an incorrect conversion
                spec is encountered __vprinter copies the format as a literal
                (since it is assumed that alignment with the argument list has
                been lost), beginning with the '%' which introduced the bad
                format.

                If the destination outP is of limited size, for example a
                string or a full disk, __vprinter does not know.  Overflowing
                the destination causes undefined results.  In some cases
                *putter is able to handle overflows safely, but that is not
                the concern of __vprinter.

                The syntax of the format string is:

                format ::=      ([literal] ['%' conversion ])* ;

                conversion ::=  '%' | [flag]* [width] ['.' precision]
                                      ['l'] type ;

                flag ::=        '-' | '+' | ' ' | '#' | '0' ;

                width ::=       '*' | number ;
                precision ::=   '.' ('*' | number) ;

                type ::=        'd'|'i'|'o'|'u'|'x'|'n'|'X'|'f'|'e'|'E'|
                                'g'|'G'|'c'|'s'|'p'|'N'|'F'

*---------------------------------------------------------------------*/
static bool printf_va ( IWriteFile & file, const char *formP, va_list argP )
{
    enum syntaxStages
    {
        flagStage, wideStage, dotStage, precStage, ellStage, typeStage,
    };

    enum flagBits
    {
        altFormatBit = 0x0001, /* the '#' flag                 */
        leftJustBit  = 0x0002, /* the '-' flag                 */
//      notZeroBit   = 0x0004, /* 0 (octal) or 0x (hex) prefix */
//      fillZerosBit = 0x0008, /* zero fill width              */
        isLongBit    = 0x0010, /* long-type argument           */
        farPtrBit    = 0x0020, /* far pointers                 */
        alt0xBit     = 0x0040, /* '#' confirmed for %x format  */
//      floatBit     = 0x0080, /* float arg 4 bytes not 8!     */
        LongDoubleBit= 0x0100, /* signal a long double argument*/
        isShortBit   = 0x0200, /* short-type argument          */
    };

    unsigned flagSet;
    char    fc;                     /* format char, from fmt string */
    bool    isSigned;               /* chooses signed/unsigned ints */
    int     width;
    int     precision;
    nat64   temp64;                  /* used to be long, now it's int64 */
    int     radix;
    CmbSuite<char, 9> buf;

/*
General outline of the method:

First the string is scanned, and conversion specifications detected.

The preliminary fields of a conversion (flags, width, precision, long)
are detected and noted.

The argument is fetched and converted, under the optional guidance of
the values of the preliminary fields.  With the sole exception of the
's' conversion, the converted strings are first placed in the tempStr
buffer.

The contents of the tempStr (or the argument string for 's') are copied
to the output, following the guidance of the preliminary fields in
matters such as zero fill, field width, and justification.
*/

    for (;;)
    {


NEXT:
        for (;;)
        {
            /* This code is arranged to give in-line flow to the most frequent
             * case, literal transcription from *formP to *outP.
             */
            if ((fc = *formP++) == '\0')
                goto respond;             /* end of format string */
            if (fc == '%')         /* '%' char begins a conversion */
            {
                if ((fc = *formP) == '%')
                    formP++;              /* but "%%" is just a literal '%'. */
                else
                    break;
            }
            file.putc ( &fc );            /* copy literal character */
        }

        /* If arrived here then a conversion specification has been
         * encountered.
         */
        nat stage = flagStage;
        bool plusSign  = false;
        bool fillZeros = false;     /* zero fill width              */
        bool space = false;
        flagSet = farPtrBit;
        width = precision = -1;
        buf.resize();

                /*==================================*/
                /* loop to here when scanning flags */
                /*==================================*/

        for (;;)
        {
            fc = *formP++;              /* get next format character */
            if (fc < ' ' ||      /* filter out controls */
                fc > '\x7F')     /* or highs */
                goto abandon;

/**************************************************************************
 *                 Main character classification switch                   *
 **************************************************************************/

            switch (printCtype[(bit8)(fc - ' ')])
            {
            case (_af):                 /* when '#' was seen            */
                if (stage > flagStage)
                    goto abandon;
                flagSet |= altFormatBit;
                continue;

            case (_lj):                 /* when '-' was seen            */
                if (stage > flagStage)
                    goto abandon;
                flagSet |= leftJustBit;
                continue;

            case (_si):                 /* when ' ' or '+' was seen     */
                if (stage > flagStage)
                    goto abandon;
                ( fc == '+' ? plusSign : space ) = true;
                continue;

            case (_ne):                 /* near pointer                 */
                flagSet &= ~farPtrBit;
                stage = ellStage;
                continue;

            case (_fa):                 /* far pointer                  */
                flagSet |= farPtrBit;
                stage = ellStage;
                continue;

            case (_fz):                 /* leading width '0' is a flag  */
                if (stage > flagStage)
                    goto case_nu;       /*   else it is just a digit    */
                if ((flagSet & leftJustBit) == 0)
                {
                    fillZeros = true;
                }
                continue;

            case (_ar):
                /* When '*' is seen it causes the next argument to be
                 * taken, depending on the stage, as the width or
                 * precision.
                 */
                {
                    unsigned int tempI = va_arg(argP,int);
                    if ( stage < wideStage )
                    {
                        if ((int)tempI < 0)      /* is the width negative?       */
                        {
                            width = -(int)tempI;
                            flagSet |= leftJustBit;
                        }
                        else
                            width = tempI;
                        stage = wideStage + 1;
                    }
                    else
                    {
                        if (stage != precStage)
                            goto abandon;
                        precision = tempI;
                        stage++;
                    }
                }
                continue;

            case (_pr):                 /* when '.' is seen, precision  */
                if (stage >= precStage) /* should follow                */
                    goto abandon;
                stage = precStage;
                precision++;            /* if no digits, ANSI says zero */
                continue;
/*
        When a numeral is seen, it may be either part of a width, or
        part of the precision, depending on the stage.
*/
            case (_nu):                     /* when 0..9 seen               */
case_nu:
                fc -= '0';          /* turn '0'-'9' to 0-9          */
                if (stage <= wideStage)     /* is it part of a width spec?  */
                {
                    stage = wideStage;
                    if (width == -1)        /* first width digit ?          */
                        width = fc;    /*   default width was -1       */
                    else
                        width = width * 10 + fc;
                }
                else                        /* no, see if it's a precision  */
                {
                    if (stage != precStage) /* is it part of precision spec */
                        goto abandon;       /* no, it's just a literal      */

                    /* At this point we know that the precision specifier
                     * '.' has been seen, so we know that the precision
                     * is zero (set at '.') or greater.
                     */
                    precision = precision * 10 + fc;
                }
                continue;

            case (_lo):                     /* 'l' was seen (long)          */
                flagSet |= isLongBit;
                stage = ellStage;
                continue;

            case (_ld):                     /* 'L' was seen (long double or */
                                            /*  __int64)                    */
                flagSet = (flagSet | LongDoubleBit) & ~isLongBit;
                stage = ellStage;
                continue;

            case (_sh):                     /* 'h' or 'H' was seen (short   */
                                            /*  or narrow)                  */
                flagSet = (flagSet | isShortBit) & ~(isLongBit);
                stage = ellStage;
                continue;

            case (_wi):                     /* 'I' was seen (wide)           */
                if (formP[0] == ('6') && formP[1] == ('4'))
                  /* handles the Microsoft I64 format */
                {
                    formP += 2;
                    flagSet = (flagSet | LongDoubleBit) & ~(isLongBit | isShortBit);
                    stage = ellStage;
                }
                else
                if (formP[0] == ('3') && formP[1] == ('2'))
                  /* handles the Microsoft I32 format */
                {
                    formP += 2;
                    flagSet = (flagSet | isLongBit) & ~(LongDoubleBit | isShortBit);
                    stage = ellStage;
                }
                else
                if (formP[0] == ('1') && formP[1] == ('6'))
                  /* handles the Microsoft I16 format */
                {
                    formP += 2;
                    flagSet = (flagSet | isShortBit) & ~(LongDoubleBit | isLongBit);
                    stage = ellStage;
                }
                else
                if (formP[0] == ('8'))
                  /* handles the Microsoft I8 format */
                {
                    formP += 1;
                    /* make it a regular int since that's how a char is passed */
                    flagSet &= ~(LongDoubleBit | isLongBit | isShortBit);
                    stage = ellStage;
                }
                continue;

/*--------------------------------------------------------------------------
The previous cases covered all the possible flags.  Now the following
cases deal with the different argument types.

Remember fc contains a copy of the original character.
--------------------------------------------------------------------------*/

        /*==========================================================*/
        /* The first group of cases is for the integer conversions. */
        /*==========================================================*/

            case (_oc):                 /* octal                        */
                radix = 8;
                goto NoSign;
            case (_un):                 /* unsigned                     */
                radix = 10;
                goto NoSign;
            case (_pt):                 /* pointer                      */
                radix = 16;
                width = 8;
                fillZeros = true;
                fc = 'X';
                goto NoSign;
            case (_he):                 /* hex                          */
                radix = 16;
                                        /* Adjust for aAbBcC etc later  */
NoSign:
                plusSign = false;       /* It's an unsigned operand     */
                isSigned = 0;
                goto toAscii;

            case (_de):                 /* decimal                      */
                radix = 10;
                isSigned = 1;
toAscii:
                if (flagSet & LongDoubleBit) /* context here means __int64 */
                    temp64 = va_arg(argP,unsigned __int64);
                else if (flagSet & isLongBit)
                {
                    unsigned long tempL = va_arg(argP,unsigned long);
                    if (isSigned)       /* check for sign extension     */
                        temp64 = (unsigned __int64)(long)tempL;
                    else
                        temp64 = (unsigned __int64)tempL;
                }
                else if (flagSet & isShortBit)
                {
                    unsigned short tempS = va_arg(argP,short);
                    if (isSigned)       /* check for sign extension     */
                        temp64 = (unsigned __int64)(short)tempS;
                    else
                        temp64 = (unsigned __int64)tempS;
                }
                else
                {
                    unsigned int tempI = va_arg(argP,int);
                    if (isSigned)       /* check for sign extension     */
                        temp64 = (unsigned __int64)(int)tempI;
                    else
                        temp64 = (unsigned __int64)tempI;
                }

/*-------------------------------------------------------------------------
        "Normal" integer output cases wind up down here somewhere.
-------------------------------------------------------------------------*/

                switch ( radix )
                {
                case 10:
                    if ( isSigned )
                    {
                        writeInt ( buf, (int32) temp64, width, plusSign, fillZeros );
                    }
                    else
                    {
                        writeInt ( buf, (nat32) temp64, width, plusSign, fillZeros );
                    }
                    break;
                case 16:
                    writeHex ( buf, (nat32) temp64, width, fc - 'X' + 'A', fillZeros );
                    break;
                }
                goto CopySpace;

            case (_Ch):                 /* char, opposite */
                if (!(flagSet & (isShortBit | isLongBit)))
                flagSet |= isLongBit;

                /* fall through to _ch */

            case (_ch):                 /* char, normal  */

                /* The 'c' conversion takes a character as parameter.
                 * Note, however, that the character occupies an
                 * (int) sized cell in the argument list.
                 *
                 * Note: We must handle both narrow and wide versions
                 * depending on the flags specified and the version called:
                 *
                 * Format           printf          wprintf
                 * ----------------------------------------
                 * %c               narrow          wide
                 * %C               wide            narrow
                 * %hc              narrow          narrow
                 * %hC              narrow          narrow
                 * %lc              wide            wide
                 * %lC              wide            wide
                 *
                 *
                 */
                 /* Use default char size for normal operation */
//                tempStr[0] = (char)va_arg(argP,int);
//                tempStr[1] = ('\0');
//                cP = tempStr;
                goto CopyLen;

            case (_St):                 /* string, opposite */
                /* fall through to _st */

            case (_st):                 /* string, normal   */

                /* The 's' conversion takes a string (char *) as
                 * argument and copies the string to the output
                 * buffer.
                 *
                 * Note: We must handle both narrow and wide versions
                 * depending on the flags specified and the version called:
                 *
                 * Format           printf          wprintf
                 * ----------------------------------------
                 * %s               narrow          wide
                 * %S               wide            narrow
                 * %hs              narrow          narrow
                 * %hS              narrow          narrow
                 * %ls              wide            wide
                 * %lS              wide            wide
                 *
                 */
                {
                    const char * cP = va_arg(argP,char *);
                    if ( precision < 0 )
                    {
                        nat i = 0;
                        while ( cP[i] ) buf.inc() = cP[i], ++i;
                    }
                    else
                        buf = CArrRef<char> ( cP, precision );
                }

                goto CopyLen;

            case (_fl):                 /* float        */
                {
                    const double dbl = va_arg ( argP, double );
                    if ( fc == 'f' || fc == 'F' )
                        writeFltDec ( buf, dbl, precision < 0 ? 6 : precision, plusSign );
                    else
                        writeExpDec ( buf, dbl, precision < 0 ? 6 : precision, plusSign );
                }

CopySpace:
                
                if ( space && buf[0] != ' ' && buf[0] != '+' && buf[0] != '-' )
                {
                    buf.inc() = ' ';
                    buf >>= 1;
                }

CopyLen:
                // Is the field to be right-filled?
                if ( flagSet & leftJustBit )
                {
                    while ( (int)buf.size() < width ) buf.inc() = ' ';
                }
                else
                // If result is NOT left justified, insert leading spaces.
                {
                    int k = width - (int)buf.size();
                    if ( k > 0 )
                    {
                        while ( (int)buf.size() < width ) buf.inc() = ' ';
                        buf >>= k;
                    }
                }

                /* Need alternate hex form?
                 *
                if (flagSet & alt0xBit)
                {                       /* Yes, Send "0x" or "0X"       *
                    file.putc ( "0" );
                    file.putc ( &fc );  /* fc is 'x' or 'X'             *
                }*/

                if ( file.write ( buf(), buf.size(), 1 ) != 1 ) return false;

                // If arrive here, the conversion has been done and copied to output.
                goto NEXT;

            case (_ns) :                 /* number sent */
                //cP = va_arg(argP,char *);
                /*if (flagSet & isLongBit)
                    *((long *)cP) = totalSent;
                else if (flagSet & isShortBit)
                    *((short *)cP) = totalSent;
                else
                    *((int *)cP) = totalSent;*/
                goto NEXT;


            case (_zz):
            case (_dc):
            case (_pc):
                /* \0 characters, unexpected end of format string,
                 * ordinary "don't care" chars in the wrong position,
                 * '%' percent characters in the wrong position
                 */
                goto abandon;

            }               /* end switch */
        }
    }

    /* If the format goes badly wrong, then copy it literally to the output
     * and abandon the conversion.
     */

abandon:
    return false;


    /* If arrived here then the function has finished
     * (either correctly or not).
     */

respond:

    file.flush();
    return true;
}

bool printf ( IWriteFile & file, const char * format, ... )
{
  va_list arg;
  va_start (arg, format);
  bool done = printf_va ( file, format, arg );
  va_end (arg);
  return done;
}