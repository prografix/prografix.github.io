
#ifndef _PSEUDO_FILE_H
#define _PSEUDO_FILE_H

#include "File.h"
#include "ShevArray.h"

//************************* 15.10.2013 ********************************//
//
//                     class PseudoReadFile
//
//************************* 15.10.2013 ********************************//

class PseudoReadFile : public IReadFile
{
    const bit8 * pos;
public:
    explicit PseudoReadFile ( const bit8 * p ) : pos(p) {}
    nat read ( void * p, const nat size, const nat count );
    bool getc ( void * p );
};

//************************* 13.09.2003 ********************************//
//
//                  class PseudoReadSeekFile
//
//************************* 16.11.2011 ********************************//

class PseudoReadSeekFile : public IReadSeekFile
{
    CArrRef<bit8> buf;
    nat pos;
public:
    explicit PseudoReadSeekFile ( CArrRef<bit8> p ) : buf(p), pos(0) {}
    nat read(void * p, const nat size, const nat count);
    bool getc(void * p);
    bool seek_set(long offset);
    bool seek_end(long offset);
    bool seek_cur(long offset);
    long tell() { return pos; }
    void rewind () { pos = 0; }
};

//************************* 13.09.2003 ********************************//
//
//                  class PseudoWriteFile
//
//************************* 25.06.2008 ********************************//

class PseudoWriteFile : public IWriteFile
{
    SuiteRef<bit8> & suite;
// Запрет конструктора копии и оператора присваивания:
    PseudoWriteFile ( PseudoWriteFile & );
    void operator = ( PseudoWriteFile & );
public:
    explicit PseudoWriteFile ( SuiteRef<bit8> & p ) : suite(p) {}
    nat write(const void * p, const nat size, const nat count);
    bool putc(const void * p);
    void flush() {}
    long tell();
};

//************************* 06.01.2004 ********************************//
//
//                      class PseudoFile
//
//************************* 27.12.2010 ********************************//

class PseudoFile : public IFile
{
    SuiteRef<bit8> & buf;
    nat length, pos;
// Запрет конструктора копии и оператора присваивания:
    PseudoFile ( PseudoFile & );
    void operator = ( PseudoFile & );
public:
    PseudoFile ( nat n, SuiteRef<bit8> & p );
    nat read(void * p, const nat size, const nat count);
    bool getc(void * p);
    nat write(const void * p, const nat size, const nat count);
    bool putc(const void * p);
    void flush() {}
    bool seek_set(long offset);
    bool seek_end(long offset);
    bool seek_cur(long offset);
    void rewind() { pos = 0; }
    long tell() { return pos; }
};

#endif