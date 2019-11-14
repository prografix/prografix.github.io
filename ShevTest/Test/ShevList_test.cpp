
#include "../Shev/ShevList.h"
#include "../Shev/lists.h"
#include "../Shev/rand.h"

#include "Display.h"

double timeInSec();

namespace
{

void mov1_test ()
{
    ShevList list1;
    ShevList list2;
//    ShevList & list2 = list1;
    static PRand  rand;
    for ( nat k = 0; k < 100; ++k )
    {
        for ( nat n1 = 0; n1 <= 19; ++n1 )
        {
            for ( nat n2 = 0; n2 <= 19; ++n2 )
            {
                nat i;
                for ( i = 0; i < n1; ++i )
                {
                    list1.addAftCur ( new ShevItem ( rand.number(2) ) );
                }
                if ( & list1 != & list2 )
                {
                    for ( i = 0; i < n2; ++i )
                    {
                        list2.addAftCur ( new ShevItem ( 2 ) );
                    }
                }/*
                if ( k == 1 && n1 == 7 && n2 == 0 )
                {
                    i = list1.count ( Info == 1 );
                }*/
                if ( k % 2 == 1 )
                {
                    list1.mov321BefFir ( Info == 0, list2 );
                }
                else
                {
                    list1.mov321AftLas ( Info == 0, list2 );
                }
                const char * str1 = list1.test();
                const char * str2 = list2.test();
                if ( str1 )
                {
                    display << str1 << NL;
                }
                if ( str2 )
                {
                    display << str1 << NL;
                }
                if ( & list1 != & list2 )
                {
                    i = list1.count ( Info == 0 );
                    if ( i > 0 ) display << k << n1 << n2 << NL;
                }
                if ( list2.size() > 1 )
                {
                   list2.top();
                   while ( list2.next() )
                   {
                       ShevItem * p = list2.prev();
                       ShevItem * c = list2.next();
                       if ( k % 2 == 1 )
                       {
                           if ( p->info > c->info )
                               display << ">" << NL;
                       }
                       else
                       {
                           if ( p->info < c->info )
                               display << "<" << NL;
                       }
                   }
                }
                list1.delAll();
                list2.delAll();
            }
        }
    }
    display << "end" << NL;
}

void mov2_test ()
{
    ShevList list1;
    ShevList list2;
    static PRand  rand;
    for ( nat k = 0; k < 100; ++k )
    {
        for ( nat n1 = 0; n1 <= 9; ++n1 )
        {
            for ( nat n2 = 0; n2 <= 9; ++n2 )
            {
                nat i;
                for ( i = 0; i < n1; ++i )
                {
                    list1.addAftCur ( new ShevItem );
                }
                for ( i = 0; i < n2; ++i )
                {
                    list2.addAftCur ( new ShevItem );
                }
                if ( k % 2 == 0 ) list2.top();
                nat m = rand.number(n1);
                if(0)
                {
                    if ( k % 2 == 1 ) list1.top();
                    list1.movLasAftLas ( m, list2 );
                }
                else
                {
                    for ( i = 0; i < m; ++i ) list1.prev();
                    list1.movAftCurAftLas ( list2 );
                }
                const char * str1 = list1.test();
                const char * str2 = list2.test();
                if ( str1 )
                {
                    display << str1 << NL;
                }
                if ( str2 )
                {
                    display << str2 << NL;
                }
                list1.delAll();
                list2.delAll();
            }
        }
    }
    display << "end" << NL;
}

void sort_test ()
{
    static PRand  rand;
    for ( nat n = 2; n <= 20; ++n )
    {
        for ( nat j = 1; j <= 100; ++j )
        {
            List1n list;
            for ( nat i = 0; i < n; ++i )
            {
                nat s = nat ( 20. * rand() );
                list.addAftCur ( new Item1n ( s, s ) );
            }
            sort123 ( list );
            const char * str = list.test();
            if ( str != 0 )
            {
                display << str << NL;
                return;
            }
            list.top();
            while ( list.next() )
            {
                Item1n * p = list.prev();
                Item1n * c = list.next();
                if ( p->a > c->a )
                {
                    display << n << j << NL;
                    return;
                }
            }
        }
    }
    display << "end" << NL;
}

void sort_test2 ()
{
    static PRand  rand;
    for ( nat n = 2; n <= 100; ++n )
    {
        double time1 = 0, time2 = 0;
        for ( nat j = 1; j <= 100; ++j )
        {
            List1n list;
            for ( nat i = 0; i < n; ++i )
            {
                nat s = nat ( 20. * rand() );
                list.addAftCur ( new Item1n ( s, i ) );
            }
            double t1 = timeInSec();
            sort123 ( list );
            double t2 = timeInSec();
            list.sort123();
            double t3 = timeInSec();
            time1 += t2 - t1;
            time2 += t3 - t2;
        }
        display << n << time1 << time2 << NL;
    }
}

void infoActGen_test()
{
    ShevList list;
    list.addAftCur ( new ShevItem );
    list.cur()->info = 1;
    list.addAftCur ( new ShevItem );
    list.cur()->info = 2;
    list.addAftCur ( new ShevItem );
    list.cur()->info = 3;
    list.addAftCur ( new ShevItem );
    list.cur()->info = 4;
    list.addAftCur ( new ShevItem );
    list.cur()->info = 5;
    nat i;
    for ( i = 0; i <= 6; ++i )
    {/*
        display << list.count ( Info == i );
        display << list.count ( Info != i );
        display << list.count ( Info <  i );
        display << list.count ( Info >  i );
        display << list.count ( Info <= i );
        display << list.count ( Info >= i );
        display << list.fir ( Info != i )->info;
        display << list.las ( Info != i )->info;*/
        display << list.count ( Info < i && Info > i );
        display << list.count ( Info < i || Info > i );
        display << list.count ( ! ( Info == i ) );
        display << NL;
    }
}

void alter_test()
{
    ShevList list;
    list.addAftCur ( new ShevItem );
    list.addAftCur ( new ShevItem );
    list.addAftCur ( new ShevItem );
    list.addAftCur ( new ShevItem );
    list.addAftCur ( new ShevItem );
    list.for_each123 ( InfoAP ( 1, 1 ) );
/*    list.for_some123 ( Info <= 3, Info = 2 ); 
    display << list.count ( Info == 2 ); 
    list.for_some321 ( Info >= 3, Info *= 2 ); 
    display << list.count ( Info == 8 ); 
    list.for_each123 ( Info = 1 );
    display << list.count ( Info == 1 );
    list.for_each321 ( Info = 2 );
    display << list.count ( Info == 2 );
    list.for_each321 ( If ( Info == 2, Info = 3, none ) );*/
    list.top();
    do
    {
        display << list.cur()->info << NL;
    }
    while ( list.next() );
}

void list_test1()
{
    for ( nat i = 0; i <= 3; ++i )
    {
        for ( nat j = 0; j <= 3; ++j )
        {
            ShevList list1, list2;
            while ( list1.size() < i ) list1.addAftLas ( new ShevItem );
            while ( list2.size() < j ) list2.addAftLas ( new ShevItem );
//            list1.addAllBefFir ( list2 );
//            list1.addAllBefCur ( list2 );
            list1.addAllAftCur ( list2 );
//            list1.addAllAftLas ( list2 );
            const char * s1 = list1.test();
            const char * s2 = list2.test();
            if ( s1 ) display << i << j << "list1" << s1 << NL;
            if ( s2 ) display << i << j << "list2" << s2 << NL;
        }
    }
    display << "end" << NL;
}

void list_test()
{
    ShevList list1, list2;
    for ( nat i = 0; i < 3; ++i )
    {
        list1.addAftLas ( new ShevItem (i+1) );
        list2.addAftLas ( new ShevItem (i+4) );
    }
    list1.top();
    list1.next();
    list2.top();
//    list2.next();
    list1.movBefCurBefCur ( list2 );
//    list1.movAftCurAftCur ( list2 );
    const char * s1 = list1.test();
    const char * s2 = list2.test();
    display << "list1" << NL;
    if ( s1 ) display << s1 << NL;
    else
    {
        if ( list1.top() )
        do
        {
            display << list1.cur()->info;
        }
        while ( list1.next() );
        display << NL;
    }
    display << "list2" << NL;
    if ( s2 ) display << s2 << NL;
    else
    {
        if ( list2.top() )
        do
        {
            display << list2.cur()->info;
        }
        while ( list2.next() );
        display << NL;
    }
    display << "end" << NL;
}

void all_test()
{
    mov1_test();
    mov2_test();
    sort_test();
    list_test();
    infoActGen_test();
    alter_test();
    display << "all test end" << NL;
}

} // namespace

void shevlist_test()
{
//    all_test();
    list_test();
//    alter_test();
//    mov2_test ();
//    sort_test2 ();
//    infoActGen_test();
}