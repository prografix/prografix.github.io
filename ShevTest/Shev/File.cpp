
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

bool readFltDec ( IReadFile & file, char & c, real64 & d )
{
    const real64 max10 = DBL_MAX / 10.;
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
    real64 t = 0.;
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
        real64 a = 0., b = 1.;
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
            const real64 min10 = DBL_MIN * 10.;
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

bool readFltDec ( IReadFile & file, char & c, real32 & d )
{
    const real32 max10 = FLT_MAX / 10.f;
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
    real32 t = 0.f;
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
        real32 a = 0.f, b = 1.f;
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
            const real32 min10 = FLT_MIN * 10.f;
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
