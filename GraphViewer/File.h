
#ifndef _FILE_H
#define _FILE_H

#include "typedef.h"

class IReadFile
{
public:
    virtual nat  read ( void * p, const nat size, const nat count ) = 0;
    virtual bool getc ( void * p ) = 0;
    virtual ~IReadFile() {}
};

class IWriteFile
{
public:
    virtual nat write ( const void * p, const nat size, const nat count ) = 0;
    virtual bool putc ( const void * p ) = 0;
    virtual void flush() = 0;
    virtual ~IWriteFile() {}
};

class ISeekFile
{
public:
    virtual bool seek_set ( long offset ) = 0;
    virtual bool seek_end ( long offset ) = 0;
    virtual bool seek_cur ( long offset ) = 0;
    virtual void rewind() = 0;
    virtual long tell() = 0;
    virtual ~ISeekFile() {}
};

class IReadSeekFile : public IReadFile, public virtual ISeekFile {};

class IWriteSeekFile : public IWriteFile, public virtual ISeekFile {};

class IFile : public IReadSeekFile, public IWriteSeekFile {};


//*********************** 24.08.2011 ********************************
//
//               ласс-имитатор DummyWriteFile
//
//*********************** 24.08.2011 ********************************

class DummyWriteFile : public IWriteFile
{
public:
    nat write ( const void * p, const nat size, const nat count ) { return count; }
    bool putc ( const void * p ) { return true; }
    void flush() {}
};

//*********************** 02.02.2005 ********************************
//
//   „тение дес€тичного целого знакого числа из текстового файла
//       с - последний прочитанный символ
//
//*********************** 10.04.2006 ********************************

bool readIntDec ( IReadFile & file, char & c, int32 & i );

inline bool readIntDec ( IReadFile & file, int32 & i )
{
    char c;
    return readIntDec ( file, c, i );
}


//*********************** 12.04.2006 ********************************
//
//   „тение дес€тичного целого беззнакого числа из текстового файла
//       с - последний прочитанный символ
//
//*********************** 12.04.2006 ********************************

bool readIntDec ( IReadFile & file, char & c, nat32 & i );

inline bool readIntDec ( IReadFile & file, nat32 & i )
{
    char c;
    return readIntDec ( file, c, i );
}


//*********************** 02.02.2005 ********************************
//
//   „тение дес€тичного числа с плавающей точкой из текстового файла
//       с - последний прочитанный символ
//
//*********************** 10.04.2006 ********************************

bool readFltDec ( IReadFile & file, char & c, double & d );

inline bool readFltDec ( IReadFile & file, double & d )
{
    char c;
    return readFltDec ( file, c, d );
}


//*********************** 02.02.2005 ********************************
//
//   „тение дес€тичного числа с плавающей точкой из текстового файла
//       с - последний прочитанный символ
//
//*********************** 10.04.2006 ********************************

bool readFltDec ( IReadFile & file, char & c, float  & f );

inline bool readFltDec ( IReadFile & file, float & f )
{
    char c;
    return readFltDec ( file, c, f );
}


//*********************** 03.03.2005 *********************************
//
//      «апись целого знакового числа в файл в дес€тичном виде
//      dig - минимальное к-во дес€тичных разр€дов ( 1 <= dig <= 10 )
//      при необходимости слева дописываютс€ нули
//      если plus == true, то дл€ неотрицательных чисел пишетс€ знак +
//
//*********************** 10.04.2006 *********************************

bool writeIntDec ( IWriteFile & file, int32 i, int32 dig = 1, bool plus = false );


//*********************** 04.04.2006 *********************************
//
//      «апись целого беззнакового числа в файл в дес€тичном виде
//      dig - минимальное к-во дес€тичных разр€дов ( 1 <= dig <= 10 )
//      при необходимости слева дописываютс€ нули
//      если plus == true, то вначале пишетс€ знак +
//
//*********************** 10.04.2006 *********************************

bool writeIntDec ( IWriteFile & file, nat32 i, int32 dig = 1, bool plus = false );


//*********************** 03.03.2005 *********************************
//
//      «апись числа c плавающей точкой в файл в дес€тичном виде
//      prec - к-во дробных дес€тичных разр€дов ( 0 <= prec <= 310 )
//      если plus == true, то дл€ неотрицательных чисел пишетс€ знак +
//
//*********************** 24.05.2007 *********************************

bool writeFltDec ( IWriteFile & file, double d, nat32 prec, bool plus = false );


//*********************** 31.03.2005 *********************************
//
//      «апись числа c плавающей точкой в файл в экспотенциально-дес€тичном виде
//      prec - к-во дробных дес€тичных разр€дов дл€ манциссы ( 0 <= prec <= 310 )
//      если plus == true, то дл€ неотрицательных чисел пишетс€ знак +
//
//*********************** 24.05.2007 *********************************

bool writeExpDec ( IWriteFile & file, double d, nat32 prec, bool plus = false );


//*********************** 03.03.2005 ********************************
//
//       «апись строки в файл ( строка заканчиваетс€ нулЄм )
//
//*******************************************************************

bool writeStr ( IWriteFile & file, const char * str );

#endif