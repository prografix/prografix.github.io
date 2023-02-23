
#include "math.h"
#include "File.h"
#include "float.h"

//*********************** 02.02.2005 ********************************
//
//   Чтение десятичного целого знакого числа из текстового файла
//       с - последний прочитанный символ
//
//*********************** 10.04.2006 ********************************

bool readIntDec ( IReadFile & file, char & c, int32 & i )
{
// Если в начале есть пробелы, то пропускаем их
    for (;;)
    {
        if ( ! file.getc ( &c ) ) return false;
        switch ( c )
        {
        case '\t':
        case '\n':
        case '\v':
        case '\r':
        case ' ': continue;
        default: goto m1;
        }
    }
m1: // Определяем знак числа
    bool isPlus = true;
    if ( c == '-' )
    {
        isPlus = false;
        if ( ! file.getc ( &c ) ) return false;
    }
    else
    if ( c == '+' )
    {
        if ( ! file.getc ( &c ) ) return false;
    }
// Определяем цифры числа
    bool isDigit = false;
    nat32 n = 0;
    do
    {
        switch ( c )
        {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            {
                const nat32 s = c - '0';
                if ( n >= 214748364 )
                {
                    if ( n > 214748364 ) return false;
                    if ( s > 8 ) return false;
                    if ( isPlus && s > 7 ) return false;
                }
                n *= 10;
                n += s;
                isDigit = true;
                break;
            }
        default: goto m2;
        }
    }
    while ( file.getc ( &c ) );
m2:
    if ( ! isDigit ) return false;
    if ( isPlus )
    {
        i = n;
    }
    else
    {
        if ( n < 2147483648 )
        {
            i = n;
            i = -i;
        }
        else
        {
            i = -2147483647 - 1;
        }
    }
    return true;
}

//*********************** 12.04.2006 ********************************
//
//   Чтение десятичного целого беззнакого числа из текстового файла
//       с - последний прочитанный символ
//
//*********************** 12.04.2006 ********************************

bool readIntDec ( IReadFile & file, char & c, nat32 & i )
{
// Если в начале есть пробелы, то пропускаем их
    for (;;)
    {
        if ( ! file.getc ( &c ) ) return false;
        switch ( c )
        {
        case '\t':
        case '\n':
        case '\v':
        case '\r':
        case ' ': continue;
        default: goto m1;
        }
    }
m1:;
// Определяем цифры числа
    bool isDigit = false;
    nat32 n = 0;
    do
    {
        switch ( c )
        {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            {
                const nat32 s = c - '0';
                if ( n >= 429496729 )
                {
                    if ( n > 429496729 ) return false;
                    if ( s > 5 ) return false;
                }
                n *= 10;
                n += s;
                isDigit = true;
                break;
            }
        default: goto m2;
        }
    }
    while ( file.getc ( &c ) );
m2:
    if ( ! isDigit ) return false;
    i = n;
    return true;
}

//*********************** 02.02.2005 ********************************
//
//   Чтение десятичного числа с плавающей точкой из текстового файла
//       с - последний прочитанный символ
//
//*********************** 19.11.2011 ********************************

bool readFltDec ( IReadFile & file, char & c, double & d )
{
    const double max10 = DBL_MAX / 10.;
// Если в начале есть пробелы, то пропускаем их
    for (;;)
    {
        if ( ! file.getc ( &c ) ) return false;
        switch ( c )
        {
        case '\t':
        case '\n':
        case '\v':
        case '\r':
        case ' ': continue;
        default: goto m1;
        }
    }
m1: // Определяем знак числа
    bool isPlus = true;
    if ( c == '-' )
    {
        isPlus = false;
        if ( ! file.getc ( &c ) ) return false;
    }
    else
    if ( c == '+' )
    {
        if ( ! file.getc ( &c ) ) return false;
    }
// Определяем целую часть числа
    double t = 0.;
	int32 l = 0;
    do
    {
        switch ( c )
        {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            {
                if ( t > max10 ) return false;
                t *= 10.;
                t += c - '0';
				++l;
                break;
            }
        default: goto m2;
        }
    }
    while ( file.getc ( &c ) );
m2:
    if ( l == 0 ) return false;
    if ( c == '.' )
    {
// Определяем дробную часть числа
        double a = 0., b = 1.;
        while ( file.getc ( &c ) )
        {
            switch ( c )
            {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                if ( b < 1e308 )
                {
                    b *= 10.;
                    a *= 10.;
                    a += ( c - '0' );
                }
                break;
            default: goto m3;
            }
        }
m3:     t += a / b;
    }
    if ( c == 'e' || c == 'E' )
    {
        int32 i;
        if ( ! readIntDec ( file, c, i ) ) return false;
        if ( i > 0 )
        {
            for ( int32 j = i; --j >= 0; )
            {
                if ( t > max10 ) return false;
                t *= 10.;
            }
        }
        if ( i < 0 )
        {
            const double min10 = DBL_MIN * 10.;
            for ( int32 j = i; ++j <= 0; )
            {
                if ( t < min10 ) return false;
                t /= 10.;
            }
        }
    }
    d = isPlus ? t : -t;
    return true;
}

//*********************** 02.02.2005 ********************************
//
//   Чтение десятичного числа с плавающей точкой из текстового файла
//       с - последний прочитанный символ
//
//*********************** 19.11.2011 ********************************

bool readFltDec ( IReadFile & file, char & c, float & d )
{
    const float max10 = FLT_MAX / 10.f;
// Если в начале есть пробелы, то пропускаем их
    for (;;)
    {
        if ( ! file.getc ( &c ) ) return false;
        switch ( c )
        {
        case '\t':
        case '\n':
        case '\v':
        case '\r':
        case ' ': continue;
        default: goto m1;
        }
    }
m1: // Определяем знак числа
    bool isPlus = true;
    if ( c == '-' )
    {
        isPlus = false;
        if ( ! file.getc ( &c ) ) return false;
    }
    else
    if ( c == '+' )
    {
        if ( ! file.getc ( &c ) ) return false;
    }
// Определяем целую часть числа
    float t = 0.f;
	int32 l = 0;
    do
    {
        switch ( c )
        {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            {
                if ( t > max10 ) return false;
                t *= 10.f;
                t += c - '0';
				++l;
                break;
            }
        default: goto m2;
        }
    }
    while ( file.getc ( &c ) );
m2:
    if ( l == 0 ) return false;
    if ( c == '.' )
    {
// Определяем дробную часть числа
        float a = 0.f, b = 1.f;
        while ( file.getc ( &c ) )
        {
            switch ( c )
            {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                if ( b < 1e38 )
                {
                    b *= 10.f;
                    a *= 10.f;
                    a += ( c - '0' );
                }
                break;
            default: goto m3;
            }
        }
m3:     t += a / b;
    }
    if ( c == 'e' || c == 'E' )
    {
        int32 i;
        if ( ! readIntDec ( file, c, i ) ) return false;
        if ( i > 0 )
        {
            for ( int32 j = i; --j >= 0; )
            {
                if ( t > max10 ) return false;
                t *= 10.f;
            }
        }
        if ( i < 0 )
        {
            const float min10 = FLT_MIN * 10.f;
            for ( int32 j = i; ++j <= 0; )
            {
                if ( t < min10 ) return false;
                t /= 10.f;
            }
        }
    }
    d = isPlus ? t : -t;
    return true;
}

//*********************** 03.03.2005 ********************************
//
//      Запись целого знакового числа в файл в десятичном виде
//      dig - минимальное к-во десятичных разрядов ( 1 <= dig <= 10 )
//      при необходимости слева дописываются нули
//      если plus == true, то для неотрицательных чисел пишется знак +
//
//*********************** 10.04.2006 *********************************

bool writeIntDec ( IWriteFile & file, int32 i, int32 dig, bool plus )
{
    if ( i < 0 )
    {
        if ( i == -2147483647 - 1 )
        {
            return file.write ( "-2147483648", 11, 1 ) == 1;
        }
        if ( ! file.putc ( "-" ) ) return false;
        i = - i;
    }
    else
    if ( plus )
    {
        if ( ! file.putc ( "+" ) ) return false;
    }
    if ( dig < 1 )
    {
        dig = 1;
    }
    else
    if ( dig > 10 )
    {
        dig = 10;
    }
    char buf[10];
    nat32 count = 10;
    while ( i > 0 || dig > 0 )
    {
        buf[--count] = '0' + char ( i % 10 );
        i /= 10;
        --dig;
    }
    return count < 10 ? file.write ( buf+count, 10-count, 1 ) == 1 : file.putc ( "0" );
}

//*********************** 04.04.2006 ********************************
//
//      Запись целого беззнакового числа в файл в десятичном виде
//      dig - минимальное к-во десятичных разрядов ( 1 <= dig <= 10 )
//      при необходимости слева дописываются нули
//      если plus == true, то вначале пишется знак +
//
//*********************** 22.07.2006 *********************************

bool writeIntDec ( IWriteFile & file, nat32 i, int32 dig, bool plus )
{
    if ( plus )
    {
        if ( ! file.putc ( "+" ) ) return false;
    }
    if ( dig < 1 )
    {
        dig = 1;
    }
    else
    if ( dig > 10 )
    {
        dig = 10;
    }
    char buf[10];
    nat32 count = 10;
    while ( i > 0 || dig > 0 )
    {
        buf[--count] = '0' + char ( i % 10 );
        i /= 10;
        --dig;
    }
    return count < 10 ? file.write ( buf+count, 10-count, 1 ) == 1 : file.putc ( "0" );
}

//*********************** 03.03.2005 *********************************
//
//      Запись числа c плавающей точкой в файл в десятичном виде
//      prec - к-во дробных десятичных разрядов ( 0 <= prec <= 310 )
//      если plus == true, то для неотрицательных чисел пишется знак +
//
//*********************** 24.05.2007 *********************************

namespace
{

bool writeFltDec_ ( IWriteFile & file, double d, nat32 prec, bool plus )
{
    if ( d < 0 )
    {
        if ( ! file.putc ( "-" ) ) return false;
        d = - d;
    }
    else
    if ( plus )
    {
        if ( ! file.putc ( "+" ) ) return false;
    }
    if ( prec < 308 ) // округляем число
    {
        double p = 2.;
        for ( nat32 i = 0; i < prec; ++i )
        {
            p *= 10.;
        }
        d += 1. / p;
    }
    char buf[310];
    nat32 count = 310;
    double f = floor ( d );
    d -= f; // теперь в d дробная часть
    while ( f > 0xffffffff ) // пока число не помещается в nat32
    {
        f /= 10.;
        const double t = floor ( f );
        buf[--count] = '0' + char ( 10. * ( f - t ) + 0.5 );
        f = t;
    }
    nat32 u = nat32 ( f );
    while ( u > 0 )
    {
        buf[--count] = '0' + char ( u % 10 );
        u /= 10;
    }
    if ( ! ( count < 310 ? file.write ( buf+count, 310-count, 1 ) == 1 : file.putc ( "0" ) ) ) return false;
    if ( ! file.putc ( "." ) ) return false;
    if ( prec <= 0 ) return true;
    if ( prec > 310 ) prec = 310;
    for ( nat32 i = 0; i < prec; ++i )
    {
        d *= 10.;
        double t = floor ( d );
        d -= t;
        buf[i] = '0' + char ( t );
    }
    return file.write ( buf, prec, 1 ) == 1;
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

FPCLASS fpclass ( const double & d )
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

} // namespace

bool writeFltDec ( IWriteFile & file, double d, nat32 prec, bool plus )
{
    switch ( fpclass ( d ) )
    {
    case FPCLASS_NINF: return writeStr ( file, "-1.#INF" );
    case FPCLASS_PINF: return writeStr ( file, "+1.#INF" );
    case FPCLASS_NIND: return writeStr ( file, "-1.#IND" );
    case FPCLASS_NQNAN: return writeStr ( file, "-1.#QNAN" );
    case FPCLASS_PQNAN: return writeStr ( file, "+1.#QNAN" );
    }
    return writeFltDec_ ( file, d, prec, plus );
}

//*********************** 31.03.2005 *********************************
//
//      Запись числа c плавающей точкой в файл в экспотенциально-десятичном виде
//      prec - к-во дробных десятичных разрядов для манциссы ( 0 <= prec <= 310 )
//      если plus == true, то для неотрицательных чисел пишется знак +
//
//*********************** 24.05.2007 *********************************

bool writeExpDec ( IWriteFile & file, double d, nat32 prec, bool plus )
{
    switch ( fpclass ( d ) )
    {
    case FPCLASS_NINF: return writeStr ( file, "-1.#INF" );
    case FPCLASS_PINF: return writeStr ( file, "+1.#INF" );
    case FPCLASS_NIND: return writeStr ( file, "-1.#IND" );
    case FPCLASS_NQNAN: return writeStr ( file, "-1.#QNAN" );
    case FPCLASS_PQNAN: return writeStr ( file, "+1.#QNAN" );
    }
    if ( d == 0 )
    {
        return writeFltDec_ ( file, d, prec, plus ) && writeStr ( file, "e+000" );
    }
    int32 i = 0;
    double a = fabs ( d );
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
    if ( d < 0 ) a = -a;
    return writeFltDec_ ( file, a, prec, plus ) && file.putc ( "e" ) && writeIntDec ( file, i, 3, true );
}

//*********************** 03.03.2005 ********************************
//
//      Запись строки в файл ( строка заканчивается нулём )
//
//*******************************************************************

bool writeStr ( IWriteFile & file, const char * str )
{
    nat32 count = 0;
    while ( str[count] != 0 ) ++count;
    return file.write ( str, count, 1 ) == 1;
}
