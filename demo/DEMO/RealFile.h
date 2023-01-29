
//********* RealFile.h ***********
//
//    13.09.2003    version 2
//    24.10.2003    version 2.1
//    14.02.2006    version 2.2
//
//********************************

#ifndef _REAL_FILE_H
#define _REAL_FILE_H

#include "File.h"

class RealFile : public IFile
{
    struct _iobuf * file;
// Запрет конструктора копии и оператора присваивания:
    RealFile ( RealFile & );
    void operator = ( RealFile & );
public:
    RealFile(const char * filename, const char * mode);
    bool isValid() const { return file != 0; }
    unsigned int read (void * p, const unsigned int size, const unsigned int count);
    unsigned int write(const void * p, const unsigned int size, const unsigned int count);
    bool getc(void * p);
    bool putc(const void * p);
    bool seek_set(long offset);
    bool seek_end(long offset);
    bool seek_cur(long offset);
    long tell();
    void rewind();
    void flush();
    ~RealFile();
};

#endif