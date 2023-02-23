
//******** RealFile.cpp **********
//
//    13.09.2003    version 2
//    24.10.2003    version 2.1
//    14.02.2006    version 2.2
//
//********************************

#include "stdio.h"
#include "RealFile.h"

RealFile::RealFile(const char * filename, const char * mode)
{
    file = fopen(filename, mode);
}

RealFile::~RealFile()
{
    if ( file ) fclose(file);
}

unsigned int RealFile::read(void * ptr, const unsigned int size, const unsigned int count)
{
    if ( !file ) return 0;
    return fread(ptr, size, count, file);
}

unsigned int RealFile::write(const void * ptr, const unsigned int size, const unsigned int count)
{
    if ( !file ) return 0;
    return fwrite(ptr, size, count, file);
}

bool RealFile::getc(void * ptr)
{
    if ( !file ) return false;
    int i = ::getc(file);
    if ( i < 0 ) return false;
    *(char *) ptr = (char) i;
    return true;
}

bool RealFile::putc(const void * ptr)
{
    return file && fputc(*(char *) ptr, file) >= 0;
}

bool RealFile::seek_cur(long offset)
{
    return file && fseek(file, offset, SEEK_CUR) == 0;
}

bool RealFile::seek_end(long offset)
{
    return file && fseek(file, offset, SEEK_END) == 0;
}

bool RealFile::seek_set(long offset)
{
    return file && fseek(file, offset, SEEK_SET) == 0;
}

long RealFile::tell()
{
    return file ? ftell(file) : -1;
}

void RealFile::rewind()
{
    if(file) ::rewind(file);
}

void RealFile::flush()
{
    if(file) fflush(file);
}
