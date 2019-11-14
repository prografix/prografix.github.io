
#include "PseudoFile.h"

//************************* 15.10.2013 ********************************//
//
//                     class PseudoReadFile
//
//************************* 15.10.2013 ********************************//

nat PseudoReadFile::read ( void * ptr, const nat size, const nat count )
{
    if ( ! pos || ! ptr || ! size || ! count ) return 0;
    const nat n = size * count;
    bit8 * p = (bit8 *) ptr;
    for ( nat i = 0; i < n; ++i ) p[i] = *pos++;
    return count;
}

bool PseudoReadFile::getc ( void * ptr )
{
    if ( ! pos || ! ptr ) return false;
    *(bit8 *) ptr = *pos++;
    return true;
}

//************************* 13.09.2003 ********************************//
//
//                  class PseudoReadSeekFile
//
//************************* 16.11.2011 ********************************//

nat PseudoReadSeekFile::read ( void * ptr, const nat size, const nat count )
{
    if ( ! ptr || ! size || ! count ) return 0;
    nat m = ( buf.size() - pos ) / size;
    if ( m == 0 ) return 0;
    if ( m > count ) m = count;
    const nat n = size * m;
    bit8 * p = (bit8 *) ptr;
    for ( nat i = 0; i < n; ++i ) p[i] = buf[pos++];
    return m;
}

bool PseudoReadSeekFile::getc ( void * ptr )
{
    if ( ! ptr ) return false;
    if ( pos >= buf.size() ) return false;
    *(bit8 *) ptr = buf[pos++];
    return true;
}

bool PseudoReadSeekFile::seek_cur(long offset)
{
    offset += pos;
    if ( offset < 0 || offset > (long)buf.size() ) return false;
    pos = offset;
    return true;
}

bool PseudoReadSeekFile::seek_end(long offset)
{
    offset += buf.size();
    if ( offset < 0 || offset > (long)buf.size() ) return false;
    pos = offset;
    return true;
}

bool PseudoReadSeekFile::seek_set(long offset)
{
    if ( offset < 0 || offset > (long)buf.size() ) return false;
    pos = offset;
    return true;
}

//*********************** 13.09.2003 **********************************//
//
//                  class PseudoWriteFile
//
//************************* 25.06.2008 ********************************//

nat PseudoWriteFile::write(const void * ptr, const nat size, const nat count)
{
    if ( ! ptr || ! size || ! count ) return 0;
    suite.addAftLas ( CArrRef<bit8> ( (const bit8 *) ptr, size * count ) );
    return count;
}

bool PseudoWriteFile::putc(const void * ptr)
{
    if ( ! ptr ) return false;
    suite.inc() = *(const bit8 *) ptr;
    return true;
}

long PseudoWriteFile::tell() { return suite.size(); }

//************************ 06.01.2004 *********************************//
//
//                      class PseudoFile
//
//************************ 27.12.2010 *********************************//

PseudoFile::PseudoFile ( nat n, SuiteRef<bit8> & p ) : length(n), buf(p), pos(0)
{
    if ( buf.size() < 1 ) buf.resize ( 512 );
}

nat PseudoFile::read(void * ptr, const nat size, const nat count)
{
    if ( ! ptr || ! size || ! count ) return 0;
    nat m = ( length - pos ) / size;
    if ( m == 0 ) return 0;
    if ( m > count ) m = count;
    const nat n = size * m;
    bit8 * p = (bit8 *) ptr;
    for ( nat i = 0; i < n; ++i ) p[i] = buf[pos++];
    return m;
}

bool PseudoFile::getc(void * ptr)
{
    if ( ! ptr ) return false;
    if ( pos >= length ) return false;
    *(bit8 *) ptr = buf[pos++];
    return true;
}

bool PseudoFile::seek_cur(long offset)
{
    offset += pos;
    if ( offset < 0 || offset > (long)length ) return false;
    pos = offset;
    return true;
}

bool PseudoFile::seek_end(long offset)
{
    offset += length;
    if ( offset < 0 || offset > (long)length ) return false;
    pos = offset;
    return true;
}

bool PseudoFile::seek_set(long offset)
{
    if ( offset < 0 || offset > (long)length ) return false;
    pos = offset;
    return true;
}

nat PseudoFile::write(const void * ptr, const nat size, const nat count)
{
    if ( ! ptr || ! size || ! count ) return 0;
    const nat n = size * count;
    if ( pos + n > buf.size() ) buf.resize ( pos + n + buf.size() );
    const bit8 * p = (const bit8 *) ptr;
    for ( nat i = 0; i < n; ++i ) buf[pos++] = p[i];
    if ( length < pos ) length = pos;
    return count;
}

bool PseudoFile::putc(const void * ptr)
{
    if ( ! ptr ) return false;
    if ( pos >= buf.size() ) buf.resize ( pos + 16 + buf.size() );
    buf[pos++] = *(bit8 *) ptr;
    if ( length < pos ) length = pos;
    return true;
}
