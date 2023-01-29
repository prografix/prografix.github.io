
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
//              Класс-имитатор DummyWriteFile
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
//   Чтение десятичного целого знакого числа из текстового файла
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
//   Чтение десятичного целого беззнакого числа из текстового файла
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
//   Чтение десятичного числа с плавающей точкой из текстового файла
//       с - последний прочитанный символ
//
//*********************** 10.04.2006 ********************************

bool readFltDec ( IReadFile & file, char & c, real64 & d );

inline bool readFltDec ( IReadFile & file, real64 & d )
{
    char c;
    return readFltDec ( file, c, d );
}


//*********************** 02.02.2005 ********************************
//
//   Чтение десятичного числа с плавающей точкой из текстового файла
//       с - последний прочитанный символ
//
//*********************** 10.04.2006 ********************************

bool readFltDec ( IReadFile & file, char & c, real32  & f );

inline bool readFltDec ( IReadFile & file, real32 & f )
{
    char c;
    return readFltDec ( file, c, f );
}


//*********************** 01.10.2017 ********************************
//
//          Запись данных в файл с форматом
//
//*********************** 01.10.2017 ********************************

bool printf ( IWriteFile & file, const char * format, ... );

#endif