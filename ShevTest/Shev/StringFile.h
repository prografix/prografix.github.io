
#pragma once

#include "File.h"
#include "ShevArray.h"

//************************* 01.08.2017 ********************************//
//
//                    class StringWriteFile
//
//************************* 01.08.2017 ********************************//

class StringWriteFile : public IWriteFile
{
    Suite<char> suite;
// Запрет конструктора копии и оператора присваивания:
    StringWriteFile ( StringWriteFile & );
    void operator = ( StringWriteFile & );
public:
    StringWriteFile () { suite.inc() = 0; }

    nat write ( const void * p, const nat size, const nat count )
    {
        const nat n = size * count;
        const nat k = suite.size() - 1;
        suite.inc ( n );
        const char * s = (const char *) p;
        for ( nat i = 0; i < n; ++i ) suite[i+k] = s[i];
        suite.las() = 0;
        return count;
    }

    bool putc ( const void * p )
    {
        suite.las() = *(const char *) p;
        suite.inc() = 0;
        return true;
    }

    void flush() {}

    const char * operator () () const
    {
        return suite();
    }

    nat size () const
    {
        return suite.size() - 1;
    }

    void clear()
    {
        suite.resize(1).las() = 0;
    }
};