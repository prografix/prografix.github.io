
#include "../Shev/sorting.h"
#include "../Shev/rand.h"
#include "../Shev/func1t.h"
#include "../Shev/func1d.h"
#include "../Shev/median.h"

#include "display.h"

double timeInSec();

namespace
{

nat isum;

template <class T> inline void sort123k ( T & a0, T & a1, T & a2 )
{
++isum;
    sort123 ( a1, a2 );
++isum;
    if ( a0 > a1 )
    {
        _swap ( a0, a1 );
++isum;
        sort123 ( a1, a2 );
    }
}

template <class T> inline void sort123k ( T & a0, T & a1, T & a2, T & a3 )
{
++isum;
    sort123 ( a0, a3 );
++isum;
    sort123 ( a1, a2 );
++isum;
    if ( a2 > a3 )
    {
        _swap ( a2, a3 );
++isum;
        if ( a1 > a2 ) _swap ( a1, a2 );
        else
        {
++isum;
            sort123 ( a0, a1 );
        }
    }
    else
    {
++isum;
        if ( a0 > a1 )
        {
            _swap ( a0, a1 );
++isum;
            sort123 ( a1, a2 );
        }
    }
}

template <class T> inline void sort123k ( T & a0, T & a1, T & a2, T & a3, T & a4 )
{
isum+=1;
    sort123 ( a1, a2 );
isum+=1;
    sort123 ( a3, a4 );
isum+=1;
    if ( a2 > a4 )
    {
        _swap ( a1, a3 );
        _swap ( a2, a4 );
    }
isum+=1;
    if ( a0 > a2 )
    {
isum+=1;
        if ( a0 > a4 )
        {
            _next ( a3, a0, a4 );
isum+=1;
            if ( a0 > a1 )
            {
                _swap ( a0, a1 );
isum+=1;
                sort123 ( a1, a2 );
            }
        }
        else
        {
isum+=1;
            if ( a2 > a3 )
            {
                _swap ( a0, a3 );
isum+=1;
                sort123 ( a0, a1 );
            }
            else
            {
                _next ( a1, a0, a2 );
isum+=1;
                sort123 ( a2, a3 );
            }
        }
    }
    else
    {
isum+=1;
        sort123 ( a0, a1 );
isum+=1;
        if ( a1 > a3 )
        {
            _next ( a1, a2, a3 );
isum+=1;
            sort123 ( a0, a1 );
        }
        else
        {
isum+=1;
            sort123 ( a2, a3 );
        }
    }
}

nat median3k ( const double * a )
{
isum=1;
    if ( a[0] <= a[1] )
    {
++isum;
        if ( a[1] <= a[2] ) return 1;
++isum;
        return a[0] >= a[2] ? 0 : 2;
    }
++isum;
    if ( a[1] >= a[2] ) return 1;
++isum;
    return a[0] <= a[2] ? 0 : 2;
}

nat median5k ( const double * a )
{
    nat i1, i2, i3;
isum=1;
    if ( a[1] <= a[2] )
    {
++isum;
        if ( a[2] <= a[3] )
        {
            i1 = 1;
            i2 = 2;
            i3 = 3;
        }
        else
        {
++isum;
            if ( a[1] <= a[3] )
            {
                i1 = 1;
                i2 = 3;
                i3 = 2;
            }
            else
            {
                i1 = 3;
                i2 = 1;
                i3 = 2;
            }
        }
    }
    else
    {
++isum;
        if ( a[1] <= a[3] )
        {
            i1 = 2;
            i2 = 1;
            i3 = 3;
        }
        else
        {
++isum;
            if ( a[2] <= a[3] )
            {
                i1 = 2;
                i2 = 3;
                i3 = 1;
            }
            else
            {
                i1 = 3;
                i2 = 2;
                i3 = 1;
            }
        }
    }

++isum;
    if ( a[i2] < a[4] )
    {
++isum;
        if ( a[i2] < a[0] )
        {
++isum;
            if ( a[4] < a[0] )
            {
++isum;
                if ( a[4] > a[i3] )
                    return i3;
                else
                    return 4;
            }
            else
            {
++isum;
                if ( a[0] > a[i3] )
                    return i3;
                else
                    return 0;
            }
        }
    }
    else
    {
++isum;
        if ( a[i2] > a[0] )
        {
++isum;
            if ( a[4] > a[0] )
            {
++isum;
                if ( a[4] < a[i1] )
                    return i1;
                else
                    return 4;
            }
            else
            {
++isum;
                if ( a[0] < a[i1] )
                    return i1;
                else
                    return 0;
            }
        }
    }

    return i2;
}

nat median7b ( const double * a ) // 9.581
{
    nat i1, i2, i3, i4, i5;

isum=1;
    if ( a[2] <= a[3] )
    {
++isum;
        if ( a[3] <= a[4] )
        {
            i2 = 2;
            i3 = 3;
            i4 = 4;
        }
        else
        {
++isum;
            if ( a[2] <= a[4] )
            {
                i2 = 2;
                i3 = 4;
                i4 = 3;
            }
            else
            {
                i2 = 4;
                i3 = 2;
                i4 = 3;
            }
        }
    }
    else
    {
++isum;
        if ( a[2] <= a[4] )
        {
            i2 = 3;
            i3 = 2;
            i4 = 4;
        }
        else
        {
++isum;
            if ( a[3] <= a[4] )
            {
                i2 = 3;
                i3 = 4;
                i4 = 2;
            }
            else
            {
                i2 = 4;
                i3 = 3;
                i4 = 2;
            }
        }
    }

++isum;
    if ( a[i3] < a[5] )
    {
++isum;
        if ( a[i3] < a[1] )
        {
            i1 = i2;
            i2 = i3;
++isum;
            if ( a[5] < a[1] )
            {
                i5 = 1;
++isum;
                if ( a[5] > a[i4] )
                {
                    i3 = i4;
                    i4 = 5;
                }
                else
                {
                    i3 = 5;
                }
            }
            else
            {
                i5 = 5;
++isum;
                if ( a[1] > a[i4] )
                {
                    i3 = i4;
                    i4 = 1;
                }
                else
                {
                    i3 = 1;
                }
            }
        }
        else
        {
            i1 = 1;
            i5 = 5;
        }
    }
    else
    {
++isum;
        if ( a[i3] > a[1] )
        {
            i5 = i4;
            i4 = i3;
++isum;
            if ( a[5] > a[1] )
            {
                i1 = 1;
++isum;
                if ( a[5] < a[i2] )
                {
                    i3 = i2;
                    i2 = 5;
                }
                else
                {
                    i3 = 5;
                }
            }
            else
            {
                i1 = 5;
++isum;
                if ( a[1] < a[i2] )
                {
                    i3 = i2;
                    i2 = 1;
                }
                else
                {
                    i3 = 1;
                }
            }
        }
        else
        {
            i1 = 5;
            i5 = 1;
        }
    }

++isum;
    if ( a[i3] < a[6] )
    {
++isum;
        if ( a[i3] < a[0] )
        {
            i3 = i4;
++isum;
            if ( a[i3] > a[i5] ) i3 = i5;
++isum;
            if ( a[i3] > a[6] ) i3 = 6;
++isum;
            if ( a[i3] > a[0] ) i3 = 0;
        }
    }
    else
    {
++isum;
        if ( a[i3] > a[0] )
        {
            i3 = i2;
++isum;
            if ( a[i3] < a[i1] ) i3 = i1;
++isum;
            if ( a[i3] < a[0] ) i3 = 0;
++isum;
            if ( a[i3] < a[6] ) i3 = 6;
        }
    }

    return i3;
}

nat median7k ( const nat * a )
{
    nat i1, i2, i3, i4, i5, i6, i7 = 6;
isum=1;
    if ( a[0] <= a[1] )
    {
        i1 = 0;
        i2 = 1;
    }
    else
    {
        i1 = 1;
        i2 = 0;
    }

++isum;
    if ( a[2] <= a[3] )
    {
        i3 = 2;
        i4 = 3;
    }
    else
    {
        i3 = 3;
        i4 = 2;
    }

++isum;
    if ( a[4] <= a[5] )
    {
        i5 = 4;
        i6 = 5;
    }
    else
    {
        i5 = 5;
        i6 = 4;
    }

++isum;
    if ( a[i2] > a[i4] )
    {
        nat i = i2;
        i2 = i4;
        i4 = i;
        i = i1;
        i1 = i3;
        i3 = i;
    }

++isum;
    if ( a[i2] <= a[i6] )
    {
++isum;
        if ( a[i3] > a[i5] )
        {
            nat i = i6;
            i6 = i4;
            i4 = i;
            i = i5;
            i5 = i3;
            i3 = i;
        }
++isum;
        if ( a[i2] <= a[i7] )
        {
++isum;
            if ( a[i5] <= a[i7] )
            {
++isum;
                if ( a[i4] <= a[i5] ) return i4;
++isum;
                return a[i2] < a[i5] ? i5 : i2;
            }
++isum;
            if ( a[i4] <= a[i7] ) return i4;
++isum;
            return a[i3] < a[6] ? 6 : i3;
        }
++isum;
        if ( a[i2] <= a[i5] )
        {
++isum;
            return a[i2] < a[i3] ? i3 : i2;
        }
        else
        {
++isum;
            if ( a[i1] <= a[i5] )
            {
++isum;
                return a[i5] < a[6] ? 6 : i5;
            }
++isum;
            return a[i1] < a[6] ? 6 : i1;
        }
    }
++isum;
    if ( a[i3] <= a[6] )
    {
++isum;
        if ( a[i6] <= a[6] )
        {
++isum;
            if ( a[i1] <= a[i3] )
            {
++isum;
                if ( a[i2] <= a[i3] ) return i2;
++isum;
                return a[i3] < a[i6] ? i6 : i3;
            }
++isum;
            if ( a[i1] >= a[6] ) return 6;
++isum;
            return a[i1] < a[i6] ? i6 : i1;
        }
++isum;
        if ( a[i1] <= a[i6] )
        {
++isum;
            if ( a[i1] <= a[i5] )
            {
++isum;
                return a[i5] < a[6] ? 6 : i5;
            }
++isum;
            return a[i1] < a[6] ? 6 : i1;
        }
++isum;
        return i6;
    }
++isum;
    if ( a[i3] <= a[i6] )
    {
++isum;
        if ( a[i1] <= a[i6] )
        {
++isum;
            if ( a[i1] <= a[i5] )
            {
++isum;
                return a[i3] < a[i5] ? i5 : i3;
            }
++isum;
            return a[i1] < a[i3] ? i3 : i1;
        }
++isum;
        return i6;
    }
++isum;
    if ( a[i1] <= a[i7] )
    {
++isum;
        if ( a[i2] <= a[i7] ) return i2;
++isum;
        return a[i6] < a[i7] ? i7 : i6;
    }
    if ( a[i1] <= a[i3] )
    {
++isum;
        return a[i1] < a[i6] ? i6 : i1;
    }
    return i3;
}

nat median9k ( const double * a )
{
    nat i0, i1, i2; // первая тройка

isum=1;
    if ( a[0] <= a[1] )
    {
++isum;
        if ( a[1] <= a[2] )
        {
            i0 = 0;
            i1 = 1;
            i2 = 2;
        }
        else
        {
++isum;
            if ( a[0] <= a[2] )
            {
                i0 = 0;
                i1 = 2;
                i2 = 1;
            }
            else
            {
                i0 = 2;
                i1 = 0;
                i2 = 1;
            }
        }
    }
    else
    {
++isum;
        if ( a[0] <= a[2] )
        {
            i0 = 1;
            i1 = 0;
            i2 = 2;
        }
        else
        {
++isum;
            if ( a[1] <= a[2] )
            {
                i0 = 1;
                i1 = 2;
                i2 = 0;
            }
            else
            {
                i0 = 2;
                i1 = 1;
                i2 = 0;
            }
        }
    }

    nat i3, i4, i5; // вторая тройка

++isum;
    if ( a[3] <= a[4] )
    {
++isum;
        if ( a[4] <= a[5] )
        {
            i3 = 3;
            i4 = 4;
            i5 = 5;
        }
        else
        {
++isum;
            if ( a[3] <= a[5] )
            {
                i3 = 3;
                i4 = 5;
                i5 = 4;
            }
            else
            {
                i3 = 5;
                i4 = 3;
                i5 = 4;
            }
        }
    }
    else
    {
++isum;
        if ( a[3] <= a[5] )
        {
            i3 = 4;
            i4 = 3;
            i5 = 5;
        }
        else
        {
++isum;
            if ( a[4] <= a[5] )
            {
                i3 = 4;
                i4 = 5;
                i5 = 3;
            }
            else
            {
                i3 = 5;
                i4 = 4;
                i5 = 3;
            }
        }
    }

    nat i6, i7, i8; // третья тройка

++isum;
    if ( a[6] <= a[7] )
    {
++isum;
        if ( a[7] <= a[8] )
        {
            i6 = 6;
            i7 = 7;
            i8 = 8;
        }
        else
        {
++isum;
            if ( a[6] <= a[8] )
            {
                i6 = 6;
                i7 = 8;
                i8 = 7;
            }
            else
            {
                i6 = 8;
                i7 = 6;
                i8 = 7;
            }
        }
    }
    else
    {
++isum;
        if ( a[6] <= a[8] )
        {
            i6 = 7;
            i7 = 6;
            i8 = 8;
        }
        else
        {
++isum;
            if ( a[7] <= a[8] )
            {
                i6 = 7;
                i7 = 8;
                i8 = 6;
            }
            else
            {
                i6 = 8;
                i7 = 7;
                i8 = 6;
            }
        }
    }
    
    nat j0, j1, j2, j3, j4; // пятёрка оставшихся
++isum;
    if ( a[i1] <= a[i4] )
    {
++isum;
        if ( a[i4] <= a[i7] )
        {
            j0 = i2;
            j1 = i3;
            j2 = i4;
            j3 = i5;
            j4 = i6;
        }
        else
        {
++isum;
            if ( a[i1] <= a[i7] )
            {
                j0 = i2;
                j1 = i6;
                j2 = i7;
                j3 = i8;
                j4 = i3;
            }
            else
            {
                j0 = i8;
                j1 = i0;
                j2 = i1;
                j3 = i2;
                j4 = i3;
            }
        }
    }
    else
    {
++isum;
        if ( a[i1] <= a[i7] )
        {
            j0 = i5;
            j1 = i0;
            j2 = i1;
            j3 = i2;
            j4 = i6;
        }
        else
        {
++isum;
            if ( a[i4] <= a[i7] )
            {
                j0 = i5;
                j1 = i6;
                j2 = i7;
                j3 = i8;
                j4 = i0;
            }
            else
            {
                j0 = i8;
                j1 = i3;
                j2 = i4;
                j3 = i5;
                j4 = i0;
            }
        }
    }

// медиана из пяти
++isum;
    if ( a[j2] < a[j4] )
    {
++isum;
        if ( a[j2] < a[j0] )
        {
++isum;
            if ( a[j4] < a[j0] )
            {
++isum;
                if ( a[j4] > a[j3] )
                    return j3;
                else
                    return j4;
            }
            else
            {
++isum;
                if ( a[j0] > a[j3] )
                    return j3;
                else
                    return j0;
            }
        }
    }
    else
    {
++isum;
        if ( a[j2] > a[j0] )
        {
++isum;
            if ( a[j4] > a[j0] )
            {
++isum;
                if ( a[j4] < a[j1] )
                    return j1;
                else
                    return j4;
            }
            else
            {
++isum;
                if ( a[j0] < a[j1] )
                    return j1;
                else
                    return j0;
            }
        }
    }

    return j2;
}

void median3_test ()
{
    nat k2 = 0;
    nat k3 = 0;
    nat kn = 0;
    for ( nat i1 = 1; i1 <= 3; ++i1 )
    {
        for ( nat i2 = 1; i2 <= 3; ++i2 )
        {
            if ( i1 == i2 ) continue;
            for ( nat i3 = 1; i3 <= 3; ++i3 )
            {
                if ( i1 == i3 || i2 == i3 ) continue;
                double a[3];
                a[0] = i1;
                a[1] = i2;
                a[2] = i3;
                if ( a[median3k ( a )] != 2 )
                {
                    median3k ( a );
                    display << "err" << NL;
                }
                switch(isum)
                {
                case 2: ++k2; break;
                case 3: ++k3; break;
                default: ++kn; break;
                }
            }
        }
    }
    display << k2 << k3 << kn << NL;
    display << (2*k2+3*k3)/6. << NL;
    display << "end" << NL;
}

void median4_test ()
{
    double a[4];
    for ( nat i1 = 1; i1 <= 4; ++i1 )
    {
        a[0] = i1;
        for ( nat i2 = 1; i2 <= 4; ++i2 )
        {
            if ( i1 == i2 ) continue;
            a[1] = i2;
            for ( nat i3 = 1; i3 <= 4; ++i3 )
            {
                if ( i1 == i3 || i2 == i3 ) continue;
                a[2] = i3;
                for ( nat i4 = 1; i4 <= 4; ++i4 )
                {
                    if ( i1 == i4 || i2 == i4 || i3 == i4 ) continue;
                    a[3] = i4;
                    if ( _median4 ( a ) != 2.5 )
                    {
                        _median4 ( a );
                        display << "err" << NL;
                    }
                }
            }
        }
    }
    display << "end" << NL;
}

void median5_test ()
{
    nat k4 = 0;
    nat k5 = 0;
    nat k6 = 0;
    nat k7 = 0;
    nat kn = 0;
    for ( nat i1 = 1; i1 <= 5; ++i1 )
    {
        for ( nat i2 = 1; i2 <= 5; ++i2 )
        {
            if ( i1 == i2 ) continue;
            for ( nat i3 = 1; i3 <= 5; ++i3 )
            {
                if ( i1 == i3 || i2 == i3 ) continue;
                for ( nat i4 = 1; i4 <= 5; ++i4 )
                {
                    if ( i1 == i4 || i2 == i4 || i3 == i4 ) continue;
                    for ( nat i5 = 1; i5 <= 5; ++i5 )
                    {
                        if ( i1 == i5 || i2 == i5 || i3 == i5 || i4 == i5 ) continue;
                        double a[5];
                        a[0] = i1;
                        a[1] = i2;
                        a[2] = i3;
                        a[3] = i4;
                        a[4] = i5;
                        if ( a[median5k ( a )] != 3 )
                        {
                            median5k ( a );
                            display << "err" << NL;
                        }
                        switch(isum)
                        {
                        case 4: ++k4; break;
                        case 5: ++k5; break;
                        case 6: ++k6; break;
                        case 7: ++k7; break;
                        default: ++kn; break;
                        }/*
                        if(kn)
                        {
                            median5 ( a );
                        }*/
                    }
                }
            }
        }
    }
    display << k4 << k5 << k6 << k7 << kn << NL;
    display << (4*k4+5*k5+6*k6+7*k7)/120. << NL;
    display << "end" << NL;
}

template<class T> T median6k ( const T * a )
{
    SemiRef<const T> a0(a[0]), a1(a[1]), a2(a[2]),
                     a3(a[3]), a4(a[4]), a5(a[5]);
isum=0;
    sort123k ( a0, a1, a2 );
    sort123k ( a3, a4, a5 );
isum+=3;
    if ( a1 > a4 )
        return ( _max ( a0, a4 ) + _min ( a1, a5 ) ) / 2;
    else
        return ( _max ( a1, a3 ) + _min ( a2, a4 ) ) / 2;
}

void median6_test ()
{
    nat k7 = 0;
    nat k8 = 0;
    nat k9 = 0;
    nat kn = 0;
    for ( nat i1 = 1; i1 <= 6; ++i1 )
    {
        for ( nat i2 = 1; i2 <= 6; ++i2 )
        {
            if ( i1 == i2 ) continue;
            for ( nat i3 = 1; i3 <= 6; ++i3 )
            {
                if ( i1 == i3 || i2 == i3 ) continue;
                for ( nat i4 = 1; i4 <= 6; ++i4 )
                {
                    if ( i1 == i4 || i2 == i4 || i3 == i4 ) continue;
                    for ( nat i5 = 1; i5 <= 6; ++i5 )
                    {
                        if ( i1 == i5 || i2 == i5 || i3 == i5 || i4 == i5 ) continue;
                        for ( nat i6 = 1; i6 <= 6; ++i6 )
                        {
                            if ( i1 == i6 || i2 == i6 || i3 == i6 || i4 == i6 || i5 == i6 ) continue;
                            double a[6];
                            a[0] = i1;
                            a[1] = i2;
                            a[2] = i3;
                            a[3] = i4;
                            a[4] = i5;
                            a[5] = i6;
                            if ( median6k ( a ) != 3.5 )
                            {
                                median6k ( a );
                                display << "err" << NL;
                            }
                            switch(isum)
                            {
                            case 7: ++k7; break;
                            case 8: ++k8; break;
                            case 9: ++k9; break;
                            default: ++kn; break;
                            }
                            if(kn)
                            {
                                median6k ( a );
                            }
                            if ( median6k ( a ) != _median6 ( a ) ) display << "median6" << NL;
                        }
                    }
                }
            }
        }
    }
    if ( k7 ) display << "7 (" << k7 << ")" << NL;
    if ( k8 ) display << "8 (" << k8 << ")" << NL;
    if ( k9 ) display << "9 (" << k9 << ")" << NL;
    if ( kn ) display << "other (" << kn << ")" << NL;
    display << (7*k7+8*k8+9*k9) / 720. << NL;
    nat kk = k7 + k8 + k9 + kn;
    if ( kk != 720 ) display << "kk != 720" << NL;
}
//7 ( 80 ) 
//8 ( 320 ) 
//9 ( 320 ) 
//8.333 

void median7_test ()
{
    nat k6 = 0;
    nat k7 = 0;
    nat k8 = 0;
    nat k9 = 0;
    nat k10 = 0;
    nat k11 = 0;
    nat k12 = 0;
    nat k13 = 0;
    nat k14 = 0;
    nat kn = 0;
    for ( nat i1 = 1; i1 <= 7; ++i1 )
    {
        for ( nat i2 = 1; i2 <= 7; ++i2 )
        {
            if ( i1 == i2 ) continue;
            for ( nat i3 = 1; i3 <= 7; ++i3 )
            {
                if ( i1 == i3 || i2 == i3 ) continue;
                for ( nat i4 = 1; i4 <= 7; ++i4 )
                {
                    if ( i1 == i4 || i2 == i4 || i3 == i4 ) continue;
                    for ( nat i5 = 1; i5 <= 7; ++i5 )
                    {
                        if ( i1 == i5 || i2 == i5 || i3 == i5 || i4 == i5 ) continue;
                        for ( nat i6 = 1; i6 <= 7; ++i6 )
                        {
                            if ( i1 == i6 || i2 == i6 || i3 == i6 || i4 == i6 || i5 == i6 ) continue;
                            for ( nat i7 = 1; i7 <= 7; ++i7 )
                            {
                                if ( i1 == i7 || i2 == i7 || i3 == i7 || i4 == i7 || i5 == i7 || i6 == i7 ) continue;
                                double a[7];
                                a[0] = i1;
                                a[1] = i2;
                                a[2] = i3;
                                a[3] = i4;
                                a[4] = i5;
                                a[5] = i6;
                                a[6] = i7;
                                if ( a[median7b ( a )] != 4 )
                                {
                                    median7b ( a );
                                    display << "err" << NL;
                                }
                                switch(isum)
                                {
                                case 6: ++k6; break;
                                case 7: ++k7; break;
                                case 8: ++k8; break;
                                case 9: ++k9; break;
                                case 10: ++k10; break;
                                case 11: ++k11; break;
                                case 12: ++k12; break;
                                case 13: ++k13; break;
                                case 14: ++k14; break;
                                default: ++kn; break;
                                }
                                if(kn)
                                {
                                    median7b ( a );
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    if ( k6 ) display << "6 (" << k6 << ")" << NL;
    if ( k7 ) display << "7 (" << k7 << ")" << NL;
    if ( k8 ) display << "8 (" << k8 << ")" << NL;
    if ( k9 ) display << "9 (" << k9 << ")" << NL;
    if ( k10 ) display << "10 (" << k10 << ")" << NL;
    if ( k11 ) display << "11 (" << k11 << ")" << NL;
    if ( k12 ) display << "12 (" << k12 << ")" << NL;
    if ( k13 ) display << "13 (" << k13 << ")" << NL;
    if ( k14 ) display << "14 (" << k14 << ")" << NL;
    if ( kn ) display << "other (" << kn << ")" << NL;
    display << (6*k6+7*k7+8*k8+9*k9+10*k10+11*k11+12*k12+13*k13+14*k14)/5040. << NL;
    nat kk = k6 + k7 + k8 + k9 + k10 + k11 + k12 + k13 + k14 + kn;
    if ( kk != 5040 ) display << "kk != 5040" << NL;
}

template <class T> T median8k ( const T * a )
{
    SemiRef<const T> a0(a[0]), a1(a[1]), a2(a[2]), a3(a[3]),
                     a4(a[4]), a5(a[5]), a6(a[6]), a7(a[7]);
isum = 0;
// Упорядочивание первой четвёрки
    sort123k ( a0, a1, a2, a3 );
// Упорядочивание второй четвёрки
    sort123k ( a4, a5, a6, a7 );
// Отбрасывание крайних чисел и вычисление медианы
++isum;
    if ( a2 < a6 )
    {
++isum;
        if ( a2 < a5 )
        {
++isum;
++isum;
            return ( _max ( a2, a4 ) + _min ( a3, a5 ) ) / 2;
        }
        else
        {
++isum;
            return ( _max ( a1, a5 ) + a2 ) / 2;
        }
    }
    else
    {
++isum;
        if ( a1 < a6 )
        {
++isum;
            return ( _max ( a1, a5 ) + a6 ) / 2;
        }
        else
        {
++isum;
++isum;
            return ( _max ( a0, a6 ) + _min ( a1, a7 ) ) / 2;
        }
    }
}

void median8_test ()
{
    nat i11 = 0;
    nat i12 = 0;
    nat i13 = 0;
    nat i14 = 0;
    nat in = 0;
    nat nn = 0;
    for ( nat i1 = 1; i1 <= 8; ++i1 )
    {
        for ( nat i2 = 1; i2 <= 8; ++i2 )
        {
            if ( i1 == i2 ) continue;
            for ( nat i3 = 1; i3 <= 8; ++i3 )
            {
                if ( i1 == i3 || i2 == i3 ) continue;
                for ( nat i4 = 1; i4 <= 8; ++i4 )
                {
                    if ( i1 == i4 || i2 == i4 || i3 == i4 ) continue;
                    for ( nat i5 = 1; i5 <= 8; ++i5 )
                    {
                        if ( i1 == i5 || i2 == i5 || i3 == i5 || i4 == i5 ) continue;
                        for ( nat i6 = 1; i6 <= 8; ++i6 )
                        {
                            if ( i1 == i6 || i2 == i6 || i3 == i6 || i4 == i6 || i5 == i6 ) continue;
                            for ( nat i7 = 1; i7 <= 8; ++i7 )
                            {
                                if ( i1 == i7 || i2 == i7 || i3 == i7 || i4 == i7 || i5 == i7 || i6 == i7 ) continue;
                                for ( nat i8 = 1; i8 <= 8; ++i8 )
                                {
                                    if ( i1 == i8 || i2 == i8 || i3 == i8 || i4 == i8 ||
                                         i5 == i8 || i6 == i8 || i7 == i8 ) continue;
                                    double a[8];
                                    a[0] = i1;
                                    a[1] = i2;
                                    a[2] = i3;
                                    a[3] = i4;
                                    a[4] = i5;
                                    a[5] = i6;
                                    a[6] = i7;
                                    a[7] = i8;
                                    if ( median8k ( a ) != 4.5 )
                                    {
                                        median8k ( a );
                                        display << "err" << NL;
                                    }
                                    nn += isum;
                                    switch(isum)
                                    {
                                    case 11: ++i11; break;
                                    case 12: ++i12; break;
                                    case 13: ++i13; break;
                                    case 14: ++i14; break;
                                    default: ++in; break;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    display << i11 << i12 << i13 << i14 << in << NL;
    display << ( i11*11 + i12*12 + i13*13 + i14*14 ) / 40320. << nn  / 40320. << NL;
    display << "end" << NL;
//  2304 11392 17920 8704 0 
//  12.819 
}

void median9_test ()
{
    nat i10 = 0;
    nat i11 = 0;
    nat i12 = 0;
    nat i13 = 0;
    nat i14 = 0;
    nat i15 = 0;
    nat i16 = 0;
    nat in = 0;
    nat nn = 0;
    for ( nat i1 = 1; i1 <= 9; ++i1 )
    {
        for ( nat i2 = 1; i2 <= 9; ++i2 )
        {
            if ( i1 == i2 ) continue;
            for ( nat i3 = 1; i3 <= 9; ++i3 )
            {
                if ( i1 == i3 || i2 == i3 ) continue;
                for ( nat i4 = 1; i4 <= 9; ++i4 )
                {
                    if ( i1 == i4 || i2 == i4 || i3 == i4 ) continue;
                    for ( nat i5 = 1; i5 <= 9; ++i5 )
                    {
                        if ( i1 == i5 || i2 == i5 || i3 == i5 || i4 == i5 ) continue;
                        for ( nat i6 = 1; i6 <= 9; ++i6 )
                        {
                            if ( i1 == i6 || i2 == i6 || i3 == i6 || i4 == i6 || i5 == i6 ) continue;
                            for ( nat i7 = 1; i7 <= 9; ++i7 )
                            {
                                if ( i1 == i7 || i2 == i7 || i3 == i7 || i4 == i7 || i5 == i7 || i6 == i7 ) continue;
                                for ( nat i8 = 1; i8 <= 9; ++i8 )
                                {
                                    if ( i1 == i8 || i2 == i8 || i3 == i8 || i4 == i8 ||
                                         i5 == i8 || i6 == i8 || i7 == i8 ) continue;
                                    for ( nat i9 = 1; i9 <= 9; ++i9 )
                                    {
                                        if ( i1 == i9 || i2 == i9 || i3 == i9 || i4 == i9 ||
                                             i5 == i9 || i6 == i9 || i7 == i9 || i8 == i9 ) continue;
                                        double a[9];
                                        a[0] = i1;
                                        a[1] = i2;
                                        a[2] = i3;
                                        a[3] = i4;
                                        a[4] = i5;
                                        a[5] = i6;
                                        a[6] = i7;
                                        a[7] = i8;
                                        a[8] = i9;
                                        if ( _median9b ( a ) != 5 )
                                        {
                                            median9k ( a );
                                            display << "err" << NL;
                                        }
                                        nn += isum;
                                        switch(isum)
                                        {
                                        case 10: ++i10; break;
                                        case 11: ++i11; break;
                                        case 12: ++i12; break;
                                        case 13: ++i13; break;
                                        case 14: ++i14; break;
                                        case 15: ++i15; break;
                                        case 16: ++i16; break;
                                        default: ++in; break;
                                        }/*
                                        if(in)
                                        {
                                            median9 ( a );
                                        }*/
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    display << i10 << i11 << i12 << i13 << i14 << i15 << i16 << in << NL;
    display << (i10*10 + i11*11 + i12*12 + i13*13 + i14*14 + i15*15 + i16*16 ) / 362880. << NL;
//    display << nn / 362880. << NL;
    display << "end" << NL;
}

void median9_time_test()
{
    static PRand rand;
    FixArray<double, 9> y;
    double sum1 = 0;
    double sum2 = 0;
    double m1, m2;
    for ( nat j = 1; j <= 10000; ++j )
    {
		nat i;
        for ( i = 0; i < 9; ++i ) y[i] = nat(10*rand());
        nat nn = 100;
        double t1 = timeInSec();
        for ( i = 0; i < nn; ++i ) m1 = _median9a(y());
        double t2 = timeInSec();
        for ( i = 0; i < nn; ++i ) m2 = _median9b(y());
        double t3 = timeInSec();
        if ( m1 != m2 )
            display << "err" << NL;
        sum1 += (t2-t1);
        sum2 += (t3-t2);
    }
    display << sum2/sum1 << NL;
}

template <class T> T median10k ( const T * a )
{
    SemiRef<const T> a0(a[0]), a1(a[1]), a2(a[2]), a3(a[3]), a4(a[4]),
                     a5(a[5]), a6(a[6]), a7(a[7]), a8(a[8]), a9(a[9]);
isum=0;
    sort123k ( a0, a1, a2, a3, a4 );
    sort123k ( a5, a6, a7, a8, a9 );
isum+=4;
    if ( a2 < a7 )
    {
        if ( a3 < a6)
            return ( _max ( a3, a5 ) + _min ( a4, a6 ) ) / 2;
        else
            return ( _max ( a2, a6 ) + _min ( a3, a7 ) ) / 2;
    }
    else
    {
        if ( a1 < a8 )
            return ( _max ( a1, a7 ) + _min ( a2, a8 ) ) / 2;
        else
            return ( _max ( a0, a8 ) + _min ( a1, a9 ) ) / 2;
    }
}

void median10_test ()
{
    nat n16 = 0;
    nat n17 = 0;
    nat n18 = 0;
    nat in = 0;
    nat nn = 0;
    for ( nat i1 = 1; i1 <= 10; ++i1 )
    {
        for ( nat i2 = 1; i2 <= 10; ++i2 )
        {
            if ( i1 == i2 ) continue;
            for ( nat i3 = 1; i3 <= 10; ++i3 )
            {
                if ( i1 == i3 || i2 == i3 ) continue;
                for ( nat i4 = 1; i4 <= 10; ++i4 )
                {
                    if ( i1 == i4 || i2 == i4 || i3 == i4 ) continue;
                    for ( nat i5 = 1; i5 <= 10; ++i5 )
                    {
                        if ( i1 == i5 || i2 == i5 || i3 == i5 || i4 == i5 ) continue;
                        for ( nat i6 = 1; i6 <= 10; ++i6 )
                        {
                            if ( i1 == i6 || i2 == i6 || i3 == i6 || i4 == i6 || i5 == i6 ) continue;
                            for ( nat i7 = 1; i7 <= 10; ++i7 )
                            {
                                if ( i1 == i7 || i2 == i7 || i3 == i7 || i4 == i7 || i5 == i7 || i6 == i7 ) continue;
                                for ( nat i8 = 1; i8 <= 10; ++i8 )
                                {
                                    if ( i1 == i8 || i2 == i8 || i3 == i8 || i4 == i8 ||
                                         i5 == i8 || i6 == i8 || i7 == i8 ) continue;
                                    for ( nat i9 = 1; i9 <= 10; ++i9 )
                                    {
                                        if ( i1 == i9 || i2 == i9 || i3 == i9 || i4 == i9 ||
                                             i5 == i9 || i6 == i9 || i7 == i9 || i8 == i9 ) continue;
                                        for ( nat i10 = 1; i10 <= 10; ++i10 )
                                        {
                                            if ( i1 == i10 || i2 == i10 || i3 == i10 || i4 == i10 || i5 == i10 || 
                                                 i6 == i10 || i7 == i10 || i8 == i10 || i9 == i10 ) continue;
                                            double a[10];
                                            a[0] = i1;
                                            a[1] = i2;
                                            a[2] = i3;
                                            a[3] = i4;
                                            a[4] = i5;
                                            a[5] = i6;
                                            a[6] = i7;
                                            a[7] = i8;
                                            a[8] = i9;
                                            a[9] = i10;
                                            if ( median10k ( a ) != 5.5 )
                                            {
                                                _median10 ( a );
                                                display << "err" << NL;
                                            }
                                            ++nn;
                                            int j = isum;
                                            switch(isum)
                                            {
                                            case 16: ++n16; break;
                                            case 17: ++n17; break;
                                            case 18: ++n18; break;
                                            default: 
                                                ++in;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    display << n16 << n17 << n18 << NL;
    display << ( n16*16 + n17*17 + n18*18 ) / double(nn) << nn << NL;
// 16128 451584 3161088
// 17.867 3628800 
}

nat median11k ( const double * a )
{
    nat i, i1, i2, i3, i4, i5, i6, i7;
isum=1;
    if ( a[0] <= a[1] )
    {
        i1 = 0;
        i2 = 1;
    }
    else
    {
        i1 = 1;
        i2 = 0;
    }
++isum;
    if ( a[2] <= a[3] )
    {
        i3 = 2;
        i4 = 3;
    }
    else
    {
        i3 = 3;
        i4 = 2;
    }
++isum;
    if ( a[i2] > a[i4] )
    {
        _swap ( i1, i3 );
        _swap ( i2, i4 );
    }
++isum;
    if ( a[4] <= a[5] )
    {
        i5 = 4;
        i6 = 5;
    }
    else
    {
        i5 = 5;
        i6 = 4;
    }

    i = 5;
    for (;;)
    {
m1:     if ( ++i == 11 )
        {
++isum;
            return a[i4] > a[i6] ? i4 : i6;
        }
++isum;
        if ( a[i6] > a[i] )
        {
            i7 = i6;
            i6 = i;
++isum;
            if ( a[i4] > a[i7] ) goto m3;
++isum;
            if ( a[i5] > a[i6] ) _swap ( i5, i6 );
        }
        else
        {
            i7 = i;
++isum;
            if ( a[i4] > a[i7] ) goto m2;
        }
    }
    for (;;)
    {
m2:     if ( ++i == 11 )
        {
++isum;
            if ( a[i2] > a[i3] )
            {
++isum;
                return a[i2] > a[i7] ? i2 : i7;
            }
            else
            {
++isum;
                return a[i3] > a[i7] ? i3 : i7;
            }
        }
++isum;
        if ( a[i3] > a[i] )
        {
            i4 = i3;
            i3 = i;
        }
        else
        {
            i4 = i;
        }
++isum;
        if ( a[i2] > a[i4] )
        {
            _swap ( i1, i3 );
            _swap ( i2, i4 );
        }
++isum;
        if ( a[i4] <= a[i7] ) goto m1;
    }
    for (;;)
    {
m3:     if ( ++i == 11 )
        {
++isum;
            if ( a[i2] > a[i3] )
            {
++isum;
                return a[i2] > a[i7] ? i2 : i7;
            }
            else
            {
++isum;
                return a[i3] > a[i7] ? i3 : i7;
            }
        }
++isum;
        if ( a[i3] > a[i] )
        {
            i4 = i3;
            i3 = i;
        }
        else
        {
            i4 = i;
        }
++isum;
        if ( a[i2] > a[i4] )
        {
            _swap ( i1, i3 );
            _swap ( i2, i4 );
        }
++isum;
        if ( a[i4] <= a[i7] )
        {
++isum;
            if ( a[i5] > a[i6] ) _swap ( i5, i6 );
            goto m1;
        }
    }
}

void median11_test ()
{
    nat n15 = 0;
    nat n16 = 0;
    nat n17 = 0;
    nat n18 = 0;
    nat n19 = 0;
    nat n20 = 0;
    nat in = 0;
    nat nn = 0;
    for ( nat i1 = 1; i1 <= 11; ++i1 )
    {
        for ( nat i2 = 1; i2 <= 11; ++i2 )
        {
            if ( i1 == i2 ) continue;
            for ( nat i3 = 1; i3 <= 11; ++i3 )
            {
                if ( i1 == i3 || i2 == i3 ) continue;
                for ( nat i4 = 1; i4 <= 11; ++i4 )
                {
                    if ( i1 == i4 || i2 == i4 || i3 == i4 ) continue;
                    for ( nat i5 = 1; i5 <= 11; ++i5 )
                    {
                        if ( i1 == i5 || i2 == i5 || i3 == i5 || i4 == i5 ) continue;
                        for ( nat i6 = 1; i6 <= 11; ++i6 )
                        {
                            if ( i1 == i6 || i2 == i6 || i3 == i6 || i4 == i6 || i5 == i6 ) continue;
                            for ( nat i7 = 1; i7 <= 11; ++i7 )
                            {
                                if ( i1 == i7 || i2 == i7 || i3 == i7 || i4 == i7 || i5 == i7 || i6 == i7 ) continue;
                                for ( nat i8 = 1; i8 <= 11; ++i8 )
                                {
                                    if ( i1 == i8 || i2 == i8 || i3 == i8 || i4 == i8 ||
                                         i5 == i8 || i6 == i8 || i7 == i8 ) continue;
                                    for ( nat i9 = 1; i9 <= 11; ++i9 )
                                    {
                                        if ( i1 == i9 || i2 == i9 || i3 == i9 || i4 == i9 ||
                                             i5 == i9 || i6 == i9 || i7 == i9 || i8 == i9 ) continue;
                                        for ( nat i10 = 1; i10 <= 11; ++i10 )
                                        {
                                            if ( i1 == i10 || i2 == i10 || i3 == i10 || i4 == i10 || i5 == i10 || 
                                                 i6 == i10 || i7 == i10 || i8 == i10 || i9 == i10 ) continue;
                                            for ( nat i11 = 1; i11 <= 11; ++i11 )
                                            {
                                                if ( i1 == i11 || i2 == i11 || i3 == i11 || i4 == i11 || i5 == i11 || 
                                                     i6 == i11 || i7 == i11 || i8 == i11 || i9 == i11 || i10== i11 ) continue;
                                                double a[11];
                                                a[0] = i1;
                                                a[1] = i2;
                                                a[2] = i3;
                                                a[3] = i4;
                                                a[4] = i5;
                                                a[5] = i6;
                                                a[6] = i7;
                                                a[7] = i8;
                                                a[8] = i9;
                                                a[9] = i10;
                                                a[10] = i11;
                                                if ( a[median11k ( a )] != 6 )
                                                {
                                                    median11k ( a );
                                                    display << "err" << NL;
                                                }
                                                ++nn;
                                                int j = isum;
                                                switch(isum)
                                                {
                                                case 15: ++n15; break;
                                                case 16: ++n16; break;
                                                case 17: ++n17; break;
                                                case 18: ++n18; break;
                                                case 19: ++n19; break;
                                                case 20: ++n20; break;
                                                default: 
                                                    ++in;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    display << n15 << n16 << n17 << n18 << n19 << n20 << NL;
    display << ( n15*15 + n16*16 + n17*17 + n18*18 + n19*19 + n20*20 ) / double(nn) << nn << NL;
// 86400 740160 2952000 7401600 12988800 15747840 
// 18.997 39916800 
}

void median11_time_test ()
{
    static PRand rand;
    FixArray<double, 11> a; 
    double time1 = 0.;
    double time2 = 0.;
    nat j;
    for ( nat i = 0; i < 1000; ++i )
    {
        a << rand;
        Def<double> m1, m2;
        double t1 = timeInSec();
        for ( j = 1; j <= 100; ++j ) m1 = _median11(a());
        double t2 = timeInSec();
        for ( j = 1; j <= 100; ++j ) m2 = selectR ( a, 5 );
        double t3 = timeInSec();
        if ( m1 != m2 )
        {
            display << "error" << NL;
        }
        time1 += t2 - t1;
        time2 += t3 - t2;
    }
    display << time2/time1 << time1 << time2 << NL;
}

void median13_test ()
{
    static PRand rand;
    FixArray<double, 13> a; 
    double time1 = 0.;
    double time2 = 0.;
    nat j;
    for ( nat i = 0; i < 1000; ++i )
    {
        a << rand;
        Def<double> m1, m2;
        double t1 = timeInSec();
        for ( j = 1; j <= 100; ++j ) m1 = _median13(a());
        double t2 = timeInSec();
        for ( j = 1; j <= 100; ++j ) m2 = selectR ( a, 6 );
        double t3 = timeInSec();
        if ( m1 != m2 )
        {
            display << "error" << NL;
        }
        time1 += t2 - t1;
        time2 += t3 - t2;
    }
    display << time2/time1 << time1 << time2 << NL;
}

void select_test ()
{
    for ( nat k = 0; k < 7; ++k )
    {
        for ( nat i1 = 1; i1 <= 7; ++i1 )
        {
            for ( nat i2 = 1; i2 <= 7; ++i2 )
            {
                for ( nat i3 = 1; i3 <= 7; ++i3 )
                {
                    for ( nat i4 = 1; i4 <= 7; ++i4 )
                    {
                        for ( nat i5 = 1; i5 <= 7; ++i5 )
                        {
                            for ( nat i6 = 1; i6 <= 7; ++i6 )
                            {
                                for ( nat i7 = 1; i7 <= 7; ++i7 )
                                {
                                    FixArray<double, 7> a;
                                    a[0] = i1;
                                    a[1] = i2;
                                    a[2] = i3;
                                    a[3] = i4;
                                    a[4] = i5;
                                    a[5] = i6;
                                    a[6] = i7;
                                    Def<double> res = selectR ( a, k );
                                    select ( a, k );
                                    double v = a[k];
                                    insertSort123 ( a );
                                    if ( a[k] != v )
                                    {
                                        display << "err" << NL;
                                    }
                                    if ( a[k] != res )
                                    {
                                        res = a[k];
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    display << "end" << NL;
}

void select_test1()
{
    static PRand rand;
    const nat n = 1000;
    FixArray<double, n> buf;
    double sum1 = 0;
    double sum2 = 0;
    for ( nat j = 1; j <= 10000; ++j )
    {
        const nat m = n - rand.number(n);
        ArrRef<double> y ( buf, 0, m );
		nat i;
        for ( i = 0; i < m; ++i ) y[i] = (10*rand());
        nat nn = 1;
        double t1 = timeInSec();
        Def<double> res1, res2;
        for ( i = 0; i < nn; ++i ) res1 = selectR ( y, m/2 );
        double t2 = timeInSec();
        for ( i = 0; i < nn; ++i )
        {
            select ( y, m/2 );
            res2 = y[m/2];
        }
        double t3 = timeInSec();
//display << m << (t2-t1) << (t3-t2) << NL;
        sum1 += (t2-t1) / m;
        sum2 += (t3-t2) / m;
        if ( res1 != res2 )  display << "!=" << NL;
//return;
//    display << m << nsr << nsr/double(m) << NL;
//        break;
    }
    display << n*sum1 << n*sum2 << NL;
    display << "end" << NL;
}

void selectW_test()
{
    static PRand rand;
    const nat n = 100;
    CmbSuite<double, n> y, w;
    for ( nat j = 1; j < n; ++j )
    {
        const nat m = j;//n - nat(n*rand());
        y.resize ( m );
        w.resize ( m );
        nat i;
        for ( i = 0; i < m; ++i )
        {
            y[i] = nat(10*rand());
            w[i] = nat(10*rand());
        }
//        double tt = 0.;
        i = m / 2;
        for ( i = 0; i <= m; ++i )
        {
            const double p = double(i)/m;
//        double t = timeInSec();
            Def<nat> im = selectR ( y, w, p );
            if ( ! im.isDef )
            {
                selectR ( y, w, p );
                continue;
            }
//        tt += timeInSec() - t;
            double sum1 = 0;
            double sum2 = 0;
            double sum3 = 0;
            for ( nat k = 0; k < m; ++k )
            {
                if ( y[k] < y[im] ) sum1 += w[k];
                if ( y[k] > y[im] ) sum2 += w[k];
                sum3 += w[k];
            }
            if ( sum1 > p * sum3 || sum2 > ( 1 - p + 1e-16 ) * sum3 )
                 display << "err" << NL;
        }
//return;
//    display << m << tt << NL;
//        break;
    }
    display << "end" << NL;
}

void mergeSort_test ()
{
    static PRand rand;
    const nat n = 10000;
    FixArray<double, n> a, b, c, d;
	nat i, j, nn;
    for ( nn = 0; nn <= 999; nn += 1 )
    {
        double si = 0;
        double sq = 0;
        double sm = 0;
        for ( j = 0; j < 5; ++j )
        {
            for ( i = 0; i < nn; ++i )
            {
                a[i] = b[i] = c[i] = ( 1000 * rand() );
            }
            double t1 = timeInSec();
            insertSort123 ( ArrRef<double> ( a, 0, nn ) );
            double t2 = timeInSec();
            quickSort123 ( ArrRef<double> ( b, 0, nn ) );
            double t3 = timeInSec();
            mergeSort123 ( ArrRef<double> ( c, 0, nn ), d );
            double t4 = timeInSec();
            for ( i = 0; i < nn; ++i )
            {
                if ( a[i] != b[i] || a[i] != c[i] )
                {
                    display << "ERR" << NL;
                }
            }
            si += t2 - t1;
            sq += t3 - t2;
            sm += t4 - t3;
    //break;
        }
        display << nn << si / sm << sq / sm << NL;
    }
}

void sort5_test ()
{
    const nat n = 5;
    nat a[n];
    for ( nat i1 = 1; i1 <= n; ++i1 )
    {
        for ( nat i2 = 1; i2 <= n; ++i2 )
        {
            if ( i1 == i2 ) continue;
            for ( nat i3 = 1; i3 <= n; ++i3 )
            {
                if ( i1 == i3 || i2 == i3 ) continue;
                for ( nat i4 = 1; i4 <= n; ++i4 )
                {
                    if ( i1 == i4 || i2 == i4 || i3 == i4 ) continue;
                    for ( nat i5 = 1; i5 <= n; ++i5 )
                    {
                        if ( i1 == i5 || i2 == i5 || i3 == i5 || i4 == i5 ) continue;
                        a[0] = i1;
                        a[1] = i2;
                        a[2] = i3;
                        a[3] = i4;
                        a[4] = i5;
                        sort5_123 ( a );
                    }
                }
            }
        }
    }
    display << "end" << NL;
}

void sort6_test ()
{
    const nat n = 6;
    for ( nat i1 = 1; i1 <= n; ++i1 )
    {
        for ( nat i2 = 1; i2 <= n; ++i2 )
        {
            if ( i1 == i2 ) continue;
            for ( nat i3 = 1; i3 <= n; ++i3 )
            {
                if ( i1 == i3 || i2 == i3 ) continue;
                for ( nat i4 = 1; i4 <= n; ++i4 )
                {
                    if ( i1 == i4 || i2 == i4 || i3 == i4 ) continue;
                    for ( nat i5 = 1; i5 <= n; ++i5 )
                    {
                        if ( i1 == i5 || i2 == i5 || i3 == i5 || i4 == i5 ) continue;
                        for ( nat i6 = 1; i6 <= n; ++i6 )
                        {
                            if ( i1 == i6 || i2 == i6 || i3 == i6 || i4 == i6 || i5 == i6 ) continue;
                            nat a[n];
                            a[0] = i1;
                            a[1] = i2;
                            a[2] = i3;
                            a[3] = i4;
                            a[4] = i5;
                            a[5] = i6;
#if 0
                            sort6_123 ( a );
                            for ( nat i = 1; i < n; ++i )
                            {
                                if ( a[i-1] >= a[i] )
                                {
                                    display << "err" << NL;
                                    a[0] = i1;
                                    a[1] = i2;
                                    a[2] = i3;
                                    a[3] = i4;
                                    a[4] = i5;
                                    a[5] = i6;
                                    sort6_123 ( a );
                                }
                            }
#else
                            sort6_321 ( a );
                            for ( nat i = 1; i < n; ++i )
                            {
                                if ( a[i-1] <= a[i] ) display << "err" << NL;
                            }
#endif
                        }
                    }
                }
            }
        }
    }
    display << "end" << NL;
}

void sort_test2 ()
{
    static PRand rand;
    const nat n = 6;
    FixArray<double, n> a, b;
    double si = 0;
    double so = 0;
    nat i, j;
    for ( j = 0; j < 1000; ++j )
    {
        for ( i = 0; i < n; ++i )
        {
            a[i] = b[i] = nat ( 1000 * rand() );
        }
        double t1 = timeInSec();
        insertSort123 ( b );
        double t2 = timeInSec();
        sort6_123 ( &a[0] );
        double t3 = timeInSec();
        for ( i = 0; i < n; ++i )
        {
            if ( a[i] != b[i] )
            {
                display << "err" << NL;
            }
        }
        si += t2 - t1;
        so += t3 - t2;
//break;
    }
    display << si / so << si << so << NL;
}

void sorting_test ()
{
    nat i1, i2, i3, b[6];
    for ( i1 = 1; i1 <= 3; ++i1 )
    {
        for ( i2 = 1; i2 <= 3; ++i2 )
        {
            if ( i1 == i2 ) continue;
            for ( i3 = 1; i3 <= 3; ++i3 )
            {
                if ( i1 == i3 || i2 == i3 ) continue;
                b[0] = i1; b[1] = i2; b[2] = i3;
                sort3_123 ( b );
                if ( b[0] > b[1] || b[1] > b[2] ) display << "err3" << NL;
                b[0] = i1; b[1] = i2; b[2] = i3;
                sort3_321 ( b );
                if ( b[0] < b[1] || b[1] < b[2] ) display << "err3" << NL;
            }
        }
    }
    nat i4, n =4;
    for ( i1 = 1; i1 <= n; ++i1 )
    {
        for ( i2 = 1; i2 <= n; ++i2 )
        {
            if ( i1 == i2 ) continue;
            for ( i3 = 1; i3 <= n; ++i3 )
            {
                if ( i1 == i3 || i2 == i3 ) continue;
                for ( i4 = 1; i4 <= n; ++i4 )
                {
                    if ( i1 == i4 || i2 == i4 || i3 == i4 ) continue;
                    b[0] = i1; b[1] = i2; b[2] = i3; b[3] = i4;
                    sort4_123 ( b );
                    if ( b[0] > b[1] || b[1] > b[2] || b[2] > b[3] ) display << "err4" << NL;
                    b[0] = i1; b[1] = i2; b[2] = i3; b[3] = i4;
                    sort4_321 ( b );
                    if ( b[0] < b[1] || b[1] < b[2] || b[2] < b[3] ) display << "err4" << NL;
                }
            }
        }
    }
    n = 5;
    for ( i1 = 1; i1 <= n; ++i1 )
    {
        for ( i2 = 1; i2 <= n; ++i2 )
        {
            if ( i1 == i2 ) continue;
            for ( i3 = 1; i3 <= n; ++i3 )
            {
                if ( i1 == i3 || i2 == i3 ) continue;
                for ( i4 = 1; i4 <= n; ++i4 )
                {
                    if ( i1 == i4 || i2 == i4 || i3 == i4 ) continue;
                    for ( nat i5 = 1; i5 <= n; ++i5 )
                    {
                        if ( i1 == i5 || i2 == i5 || i3 == i5 || i4 == i5 ) continue;
                        b[0] = i1; b[1] = i2; b[2] = i3; b[3] = i4; b[4] = i5;
                        sort5_123 ( b );
                        if ( b[0] > b[1] || b[1] > b[2] || b[2] > b[3] || b[3] > b[4] ) display << "err5" << NL;
                        b[0] = i1; b[1] = i2; b[2] = i3; b[3] = i4; b[4] = i5;
                        sort5_321 ( b );
                        if ( b[0] < b[1] || b[1] < b[2] || b[2] < b[3] || b[3] < b[4] ) display << "err5" << NL;
                    }
                }
            }
        }
    }
    n = 6;
    for ( i1 = 1; i1 <= n; ++i1 )
    {
        for ( i2 = 1; i2 <= n; ++i2 )
        {
            if ( i1 == i2 ) continue;
            for ( i3 = 1; i3 <= n; ++i3 )
            {
                if ( i1 == i3 || i2 == i3 ) continue;
                for ( i4 = 1; i4 <= n; ++i4 )
                {
                    if ( i1 == i4 || i2 == i4 || i3 == i4 ) continue;
                    for ( nat i5 = 1; i5 <= n; ++i5 )
                    {
                        if ( i1 == i5 || i2 == i5 || i3 == i5 || i4 == i5 ) continue;
                        for ( nat i6 = 1; i6 <= n; ++i6 )
                        {
                            if ( i1 == i5 || i2 == i5 || i3 == i5 || i4 == i5 ) continue;
                            b[0] = i1; b[1] = i2; b[2] = i3; b[3] = i4; b[4] = i5; b[5] = i6;
                            sort6_123 ( b );
                            if ( b[0] > b[1] || b[1] > b[2] || b[2] > b[3] || b[3] > b[4] || b[4] > b[5] ) display << "err6" << NL;
                            b[0] = i1; b[1] = i2; b[2] = i3; b[3] = i4; b[4] = i5; b[5] = i6;
                            sort6_321 ( b );
                            if ( b[0] < b[1] || b[1] < b[2] || b[2] < b[3] || b[3] < b[4] || b[4] < b[5] ) display << "err6" << NL;
                        }
                    }
                }
            }
        }
    }
    display << "end" << NL;
}

void selectS_test()
{
    static PRand rand;
    FixArray<SortItem<double>, 5> s;
    nat i;
    for ( i = 0; i < s.size(); ++i )
    {
        SortItem<double> & item = s[i];
        item.head = rand();
        item.tail = rand();
    }
    for ( i = 0; i <= 100; ++i )
    {
        double p = i / 100.;
        Def<double> r1 = selectS ( s, p, 0 );
        Def<double> r2 = selectT ( s, p, 1./16 );
        Def<double> r3 = selectS ( s, p, 1./16 );
        display << p << r1 << r2 << r3 << NL;
    }
}

void median_test()
{
    static PRand rand;
    CmbSuite<double, 20> y;
    for ( nat n = 3; n <= 20; ++n )
    {
        y.resize(n);
		nat i, j;
        for ( j = 0; j < 10; ++j )
        {
            for ( i = 0; i < n; ++i ) y[i] = i;
            shuffle ( y, rand );
            double x = median ( y );
            nat k = 0;
            for ( i = 0; i < n; ++i )
            {
                if ( x > y[i] ) ++k;
            }
            if ( k != n/2 )
                k = k;
        }
    }
    display << "end" << NL;
}

} // namespace

void func1d_test ()
{
//    sorting_test ();
//    median_test();
//    median9_time_test();
//    median6_test ();
//    median13_test ();
    selectW_test();
//    select_test();
//    sort5_test ();
//    mergeSort_test ();
}
