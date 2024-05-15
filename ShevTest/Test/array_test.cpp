
#include "../Shev/TempExt.h"
#include "../Shev/ShevArray.h"

#include "display.h"

namespace
{

void shift ()
{
    CmbSuite<nat, 9> a;
    nat i, k, n;
    for ( n = 2; n <= 9; ++n )
    {
        a.resize ( n );
        for ( k = 1; k <= n; ++k )
        {
            for ( i = 0; i < a.size(); ++i ) a[i] = i;
            a <<= k;
            display << a << NL;
        }
        display << NL;
    }
}

void reverse ()
{
    FixArray<nat, 7> a;
    nat i;
    for ( i = 0; i < a.size(); ++i ) a[i] = i+1;
    display << a << NL;
    a.reverse();
    display << a << NL;
    a << Pow2();
    display << a << NL;
}

} // end of namespace

void outOfRange ( const char * name, nat size, nat index )
{
    display << "!!! outOfRange in" << name << size << index << NL;
}

void array_test ()
{
    nat a[2], b[2];
    ArrRef<nat> a1 ( a, 2), a2 ( b, 2 );
    a1 = a2;
    shift ();
//    reverse ();
}