
#include "math.h"
#include "stdio.h"
#include "float.h"

#include "../Shev/rand.h"
#include "../Shev/PseudoFile.h"
#include "../Shev/StringFile.h"
#include "../Shev/ShevArray.h"

#include "display.h"

namespace {

void file_test1 ()
{
    StringWriteFile file;
    static PRand rand;
    for ( int i=0; i < 40000; ++i )
    {
        double a = 2. * rand() - 1;
        if ( !a ) continue;
        a = rand() / a;
//if(i!=233) continue;
        if ( fabs ( a ) < 300 ) a *= exp ( a );
        //a = ldexp ( 1, 63 );
        //if ( fabs ( a ) > 1 ) a = 1 / a;
        file.clear();
        if ( printf ( file, "%.19f ", a ) )
        {
            double b;
            //file.rewind();
            //if ( readFltDec ( file, b ) )
            {
                b = atof ( file() );
                char buf[310];
                sprintf ( buf, "%.19f", a );
                double c = atof ( buf );
                if ( b != c )
                {
                    double e1 = b - c;
                    double e2 = a - c;
                    double e3 = b - a;
//                    if ( fabs(e1/a) > 1e-15 && fabs(e2) > fabs(e3) )
//                    if ( fabs(e1) > 1.1e-9 && fabs(e1/a) > 2e-15 && fabs(e2) < fabs(e3) )
                    if ( fabs(e1/a) > 1e-15 && fabs(e2) < fabs(e3))
                    {
//                    display << i << NL << file() << NL << buf << NL;
                        display << i << fabs(e1/a) << NL;
//                        file.rewind();
                        //printf ( file, "%.9f", a );
                    }
                }
            }
/*            else
            {
                display << "err" << NL;
            }*/
        }
        else
        {
            display << "err" << NL;
        }
    }//*/
    display << "end" << NL;
}

void writeFltDec_test()
{
    Suite<bit8> arr;
//    static QRand rand ( 7 );
    PseudoFile file ( 350, arr );
    double d = 1e70;//DBL_MAX;//100*rand();
    //d -= floor(d);
/*    if ( writeFltDec ( file, d, 0 ) && file.putc ( "" ) )
    {
        display << d << NL;
        display << (char*) &arr[0] << NL;
        display << file.tell() << NL;
    }
    else
        display << "err" << NL;*/
    //for ( nat i = 0; i <= 18; ++i )
    {
        file.rewind();
        printf ( file, "%f", d );
        file.putc ( "" );
        display << d << (char*) arr() << NL;
        d *= 10;
    }
}

void printf_test()
{
    char buf[32];
    StringWriteFile file;
    static PRand rand;
    double d = 100.*rand();
    /*int i = rand.number(333);
    nat n = i;
    i = -i;
    printf ( file, "n = %d ha-ha %u %s %.3f", i, n, "hi-hi", d );
    display << file() << NL;
    file.clear();
    printf ( file, "e = %.3e E = %.3E", d, d );
    display << file() << NL;*
    printf ( file, "%5d", 123 );
    display << file() << NL;
    sprintf ( buf, "%5d", 123 );
    display << buf << NL;
    file.clear();
    printf ( file, "%5d", -123 );
    display << file() << NL;
    sprintf ( buf, "%5d", -123 );
    display << buf << NL;
    file.clear();
    printf ( file, "%+5d", 123 );
    display << file() << NL;
    sprintf ( buf, "%+5d", 123 );
    display << buf << NL;
    file.clear();
    printf ( file, "%05d", 123 );
    display << file() << NL;
    sprintf ( buf, "%05d", 123 );
    display << buf << NL;
    file.clear();
    printf ( file, "%-5d", 123 );
    display << file() << NL;
    sprintf ( buf, "%-5d", 123 );
    display << buf << NL;
    file.clear();
    printf ( file, "%5u", 123 );
    display << file() << NL;
    sprintf ( buf, "%5u", 123 );
    display << buf << NL;
    file.clear();
    printf ( file, "%7.3f", d );
    display << file() << NL;
    sprintf ( buf, "%7.3f", d );
    display << buf << NL;
    file.clear();
    printf ( file, "% 4.3f", d );
    display << file() << NL;
    sprintf ( buf, "% 4.3f", d );
    display << buf << NL;
    file.clear();*
    printf ( file, "%7.3e", d );
    display << file() << NL;
    sprintf ( buf, "%7.3e", d );
    display << buf << NL;
    file.clear();*/
    printf ( file, "ha-ha % s ha-ha ", "hi-hi" );
    display << file() << NL;
    sprintf ( buf, "ha-ha % s ha-ha ", "hi-hi" );
    display << buf << NL;
    file.clear();
    printf ( file, "% 1d", 123 );
    display << file() << NL;
    sprintf ( buf, "% 1d", 123 );
    display << buf << NL;
    file.clear();
}

} // namespace

void file_test ()
{
    file_test1 ();
//    writeFltDec_test();
//    printf_test();
}
