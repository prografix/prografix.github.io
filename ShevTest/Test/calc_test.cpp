
#include "math.h"

#include "../Shev/tune.h"
#include "../Shev/mathem.h"
#include "../Shev/PseudoFile.h"
#include "../Shev/ShevArray.h"
#include "../Shev/Complex.h"

#include "display.h"

namespace {

int mu_func ( nat i )
{
    const nat n = 1000;
    int mu[n] = { 1,-1,-1, 0,-1, 1,-1, 0, 0, 1,-1, 0,-1, 1, 1, 0,-1, 0,-1, 0, //  20
                  1, 1,-1, 0, 0, 1, 0, 0,-1,-1,-1, 0, 1, 1, 1, 0,-1, 1, 1, 0, //  40
                 -1,-1,-1, 0, 0, 1,-1, 0, 0, 0, 1, 0,-1, 0, 1, 0, 1, 1,-1, 0, //  60
                 -1, 1, 0, 0, 1,-1,-1, 0, 1,-1,-1, 0,-1, 1, 0, 0, 1,-1,-1, 0, //  80
                  0, 1,-1, 0, 1, 1, 1, 0,-1, 0, 1, 0, 1, 1, 1, 0,-1, 0, 0, 0, // 100
                 -1,-1,-1, 0,-1, 1,-1, 0,-1,-1, 1, 0,-1,-1, 1, 0, 0, 1, 1, 0, // 120
                  0, 1, 1, 0, 0, 0,-1, 0, 1,-1,-1, 0, 1, 1, 0, 0,-1,-1,-1, 0, // 140
                  1, 1, 1, 0, 1, 1, 0, 0,-1, 0,-1, 0, 0,-1, 1, 0,-1, 1, 1, 0, // 160
                  1, 0,-1, 0,-1, 1,-1, 0, 0,-1, 0, 0,-1,-1, 0, 0, 1, 1,-1, 0, // 180
                 -1,-1, 1, 0, 1,-1, 1, 0, 0,-1,-1, 0,-1, 1,-1, 0,-1, 0,-1, 0, // 200
                  1, 1, 1, 0, 1, 1, 0, 0, 1, 1,-1, 0, 1, 1, 1, 0, 1, 1, 1, 0, // 220
                  1,-1,-1, 0, 0, 1,-1, 0,-1,-1,-1, 0,-1, 0, 1, 0, 1,-1,-1, 0, // 240
                 -1, 0, 0, 0, 0,-1, 1, 0, 1, 0,-1, 0, 1, 1,-1, 0,-1,-1, 1, 0, // 260
                  0, 1,-1, 0, 1,-1, 1, 0,-1, 0,-1, 0,-1, 1, 0, 0,-1, 1, 0, 0, // 280
                 -1,-1,-1, 0,-1,-1, 1, 0, 0,-1, 1, 0,-1, 0, 1, 0, 0, 1, 1, 0, // 300
                  1, 1, 1, 0, 1, 0,-1, 0, 1,-1,-1, 0,-1, 1, 0, 0,-1,-1, 1, 0, // 320
                  1,-1, 1, 0, 0, 1, 1, 0, 1, 1,-1, 0, 0, 1, 1, 0,-1, 0, 1, 0, // 340
                  1, 0, 0, 0,-1, 1,-1, 0,-1, 0, 0, 0,-1,-1, 1, 0,-1, 1,-1, 0, // 360
                  0, 1, 0, 0, 1,-1,-1, 0, 0,-1, 1, 0,-1,-1, 0, 0, 1, 0,-1, 0, // 380
                  1, 1,-1, 0,-1, 1, 0, 0,-1, 1, 1, 0, 1, 1, 1, 0,-1, 1,-1, 0, // 400
                 -1,-1, 1, 0, 0,-1, 1, 0,-1,-1, 1, 0, 1, 0, 1, 0, 1,-1,-1, 0, // 420
                 -1, 1, 0, 0, 0,-1, 1, 0,-1,-1,-1, 0,-1,-1,-1, 0, 1,-1,-1, 0, // 440
                  0,-1,-1, 0, 1, 1, 1, 0,-1, 0, 1, 0, 1, 1,-1, 0,-1, 1, 0, 0, // 460
                 -1, 1,-1, 0,-1, 1,-1, 0, 1,-1, 1, 0, 1,-1, 0, 0, 0, 1,-1, 0, // 480
                  1, 1,-1, 0, 1, 0,-1, 0, 1, 0,-1, 0, 1,-1, 0, 0, 1,-1,-1, 0, // 500
                  1, 1,-1, 0, 1,-1, 0, 0,-1, 1, 1, 0, 0, 1, 1, 0, 1,-1, 1, 0, // 520
                 -1, 0,-1, 0, 0, 1, 1, 0, 0,-1, 0, 0, 1,-1, 1, 0, 1, 1, 0, 0, // 540
                 -1, 1, 1, 0, 1, 1,-1, 0, 0, 0, 1, 0, 1, 1,-1, 0,-1, 0, 1, 0, // 560
                 -1, 1,-1, 0, 1, 1, 0, 0,-1, 1,-1, 0, 1,-1, 0, 0,-1, 0, 1, 0, // 580
                  1,-1, 1, 0, 0, 1,-1, 0, 1,-1, 1, 0,-1, 0,-1, 0, 1,-1,-1, 0, // 600
                 -1,-1, 0, 0, 0,-1,-1, 0,-1,-1, 1, 0,-1, 1,-1, 0,-1,-1,-1, 0, // 620
                  0, 1, 1, 0, 0, 1,-1, 0, 1, 0,-1, 0, 1, 1, 1, 0, 0,-1, 0, 0, // 640
                 -1,-1,-1, 0,-1,-1,-1, 0, 1, 0,-1, 0,-1,-1, 1, 0, 0,-1,-1, 0, // 660
                 -1, 1,-1, 0,-1, 0, 1, 0, 1,-1, 1, 0,-1, 1, 0, 0,-1,-1, 1, 0, // 680
                  1,-1,-1, 0, 1, 0, 1, 0, 1, 1,-1, 0, 0, 1, 1, 0, 1, 1, 1, 0, // 700
                 -1, 0, 1, 0,-1, 1, 1, 0,-1,-1, 0, 0, 1, 1,-1, 0, 1, 1,-1, 0, // 720
                  1, 0, 1, 0, 0, 0,-1, 0, 0,-1, 1, 0,-1, 1, 0, 0, 1, 0,-1, 0, // 740
                 -1,-1,-1, 0, 1, 1, 0, 0, 1, 0,-1, 0, 1,-1, 1, 0,-1, 1,-1, 0, // 760
                 -1,-1, 1, 0, 0, 1, 1, 0,-1, 1, 1, 0,-1, 0, 0, 0,-1, 1, 1, 0, // 780
                  1,-1, 0, 0, 1,-1,-1, 0, 1,-1, 1, 0, 1, 1,-1, 0,-1, 1, 1, 0, // 800
                  0, 1, 1, 0,-1,-1, 1, 0,-1, 0,-1, 0, 1,-1, 1, 0, 1, 1, 0, 0, // 820
                 -1,-1,-1, 0, 0,-1,-1, 0,-1,-1, 1, 0, 0,-1, 1, 0, 0, 1,-1, 0, // 840
                  0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0,-1,-1, 0, 0,-1, 1,-1, 0, // 860
                 -1, 1,-1, 0, 1, 1, 0, 0, 1, 1, 1, 0, 0,-1, 0, 0,-1, 1, 1, 0, // 880
                 -1, 0,-1, 0,-1, 1,-1, 0, 1,-1, 0, 0, 1,-1, 1, 0,-1, 1, 1, 0, // 900
                  1,-1,-1, 0, 1,-1,-1, 0, 0, 1,-1, 0, 1, 1,-1, 0, 1, 0,-1, 0, // 920
                  1, 1, 1, 0, 0, 1, 0, 0,-1, 1, 0, 0, 1, 1,-1, 0,-1,-1, 1, 0, // 940
                 -1,-1, 1, 0, 0,-1,-1, 0, 1, 0, 1, 0,-1, 0, 1, 0,-1, 1, 1, 0, // 960
                  0,-1, 0, 0, 1, 1,-1, 0,-1,-1,-1, 0, 1, 1, 0, 0,-1,-1, 1, 0, // 980
                  0, 1,-1, 0, 1,-1,-1, 0, 1, 0,-1, 0, 1,-1, 1, 0,-1, 1, 0, 0};//1000

    return ( i < 1 || i > n ) ? 0 : mu[i-1];
}

int la_func ( nat i )
{
    const nat n = 1000;
    int la[n] = { 1,-1,-1, 1,-1, 1,-1,-1, 1, 1,-1,-1,-1, 1, 1, 1,-1,-1,-1,-1, //  20
                  1, 1,-1, 1, 1, 1,-1,-1,-1,-1,-1,-1, 1, 1, 1, 1,-1, 1, 1, 1, //  40
                 -1,-1,-1,-1,-1, 1,-1,-1, 1,-1, 1,-1,-1, 1, 1, 1, 1, 1,-1, 1, //  60
                 -1, 1,-1, 1, 1,-1,-1,-1, 1,-1,-1,-1,-1, 1,-1,-1, 1,-1,-1,-1, //  80 
                  1, 1,-1, 1, 1, 1, 1, 1,-1, 1, 1,-1, 1, 1, 1, 1,-1,-1,-1, 1, // 100
                 -1,-1,-1, 1,-1, 1,-1,-1,-1,-1, 1,-1,-1,-1, 1,-1,-1, 1, 1,-1, // 120
                  1, 1, 1,-1,-1, 1,-1,-1, 1,-1,-1, 1, 1, 1, 1, 1,-1,-1,-1, 1, // 140
                  1, 1, 1, 1, 1, 1,-1,-1,-1, 1,-1, 1,-1,-1, 1, 1,-1, 1, 1, 1, // 160
                  1,-1,-1,-1,-1, 1,-1,-1, 1,-1,-1,-1,-1,-1,-1,-1, 1, 1,-1,-1, // 180
                 -1,-1, 1, 1, 1,-1, 1,-1, 1,-1,-1,-1,-1, 1,-1, 1,-1, 1,-1,-1, // 200
                  1, 1, 1, 1, 1, 1,-1,-1, 1, 1,-1,-1, 1, 1, 1, 1, 1, 1, 1, 1, // 220
                  1,-1,-1, 1, 1, 1,-1, 1,-1,-1,-1, 1,-1, 1, 1,-1, 1,-1,-1, 1, // 240
                 -1,-1,-1,-1,-1,-1, 1, 1, 1, 1,-1,-1, 1, 1,-1, 1,-1,-1, 1, 1, // 260
                 -1, 1,-1,-1, 1,-1, 1,-1,-1,-1,-1,-1,-1, 1,-1, 1,-1, 1,-1,-1, // 280
                 -1,-1,-1,-1,-1,-1, 1,-1, 1,-1, 1,-1,-1, 1, 1, 1, 1, 1, 1,-1, // 300
                  1, 1, 1,-1, 1, 1,-1, 1, 1,-1,-1,-1,-1, 1, 1,-1,-1,-1, 1,-1, // 320
                  1,-1, 1, 1,-1, 1, 1, 1, 1, 1,-1,-1,-1, 1, 1, 1,-1,-1, 1, 1, // 340
                  1, 1,-1, 1,-1, 1,-1, 1,-1, 1, 1, 1,-1,-1, 1,-1,-1, 1,-1, 1, // 360
                  1, 1,-1, 1, 1,-1,-1,-1,-1,-1, 1, 1,-1,-1, 1, 1, 1,-1,-1, 1, // 380
                  1, 1,-1, 1,-1, 1,-1,-1,-1, 1, 1,-1, 1, 1, 1,-1,-1, 1,-1, 1, // 400
                 -1,-1, 1,-1,-1,-1, 1,-1,-1,-1, 1,-1, 1, 1, 1, 1, 1,-1,-1,-1, // 420
                 -1, 1,-1, 1,-1,-1, 1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 1,-1,-1,-1, // 440
                  1,-1,-1, 1, 1, 1, 1,-1,-1,-1, 1,-1, 1, 1,-1,-1,-1, 1, 1, 1, // 460
                 -1, 1,-1,-1,-1, 1,-1,-1, 1,-1, 1, 1, 1,-1,-1, 1,-1, 1,-1,-1, // 480
                  1, 1,-1, 1, 1, 1,-1, 1, 1, 1,-1, 1, 1,-1, 1,-1, 1,-1,-1,-1, // 500
                  1, 1,-1, 1, 1,-1,-1,-1,-1, 1, 1,-1, 1, 1, 1, 1, 1,-1, 1,-1, // 520
                 -1, 1,-1,-1, 1, 1, 1, 1, 1,-1,-1, 1, 1,-1, 1, 1, 1, 1,-1, 1, // 540
                 -1, 1, 1, 1, 1, 1,-1,-1,-1, 1, 1,-1, 1, 1,-1,-1,-1, 1, 1, 1, // 560
                 -1, 1,-1, 1, 1, 1,-1, 1,-1, 1,-1, 1, 1,-1,-1, 1,-1,-1, 1, 1, // 580
                  1,-1, 1, 1, 1, 1,-1,-1, 1,-1, 1,-1,-1,-1,-1,-1, 1,-1,-1, 1, // 600
                 -1,-1,-1,-1,-1,-1,-1, 1,-1,-1, 1,-1,-1, 1,-1,-1,-1,-1,-1, 1, // 620
                  1, 1, 1, 1, 1, 1,-1,-1, 1,-1,-1, 1, 1, 1, 1, 1,-1,-1,-1, 1, // 640
                 -1,-1,-1, 1,-1,-1,-1,-1, 1, 1,-1,-1,-1,-1, 1,-1,-1,-1,-1,-1, // 660
                 -1, 1,-1, 1,-1, 1, 1,-1, 1,-1, 1,-1,-1, 1,-1, 1,-1,-1, 1,-1, // 680
                  1,-1,-1,-1, 1, 1, 1,-1, 1, 1,-1,-1, 1, 1, 1,-1, 1, 1, 1,-1, // 700
                 -1,-1, 1,-1,-1, 1, 1, 1,-1,-1,-1, 1, 1, 1,-1,-1, 1, 1,-1,-1, // 720
                  1,-1, 1,-1,-1, 1,-1,-1, 1,-1, 1, 1,-1, 1, 1, 1, 1, 1,-1, 1, // 740
                 -1,-1,-1,-1, 1, 1,-1, 1, 1,-1,-1,-1, 1,-1, 1, 1,-1, 1,-1,-1, // 760
                 -1,-1, 1,-1, 1, 1, 1,-1,-1, 1, 1,-1,-1, 1,-1, 1,-1, 1, 1,-1, // 780
                  1,-1, 1, 1, 1,-1,-1,-1, 1,-1, 1, 1, 1, 1,-1,-1,-1, 1, 1,-1, // 800
                 -1, 1, 1, 1,-1,-1, 1, 1,-1, 1,-1, 1, 1,-1, 1, 1, 1, 1, 1, 1, // 820
                 -1,-1,-1, 1, 1,-1,-1,-1,-1,-1, 1,-1,-1,-1, 1, 1, 1, 1,-1, 1, // 840
                  1, 1, 1,-1,-1, 1,-1,-1, 1, 1, 1, 1,-1,-1, 1, 1,-1, 1,-1, 1, // 860
                 -1, 1,-1, 1, 1, 1,-1, 1, 1, 1, 1, 1,-1,-1, 1, 1,-1, 1, 1, 1, // 880
                 -1,-1,-1, 1,-1, 1,-1,-1, 1,-1,-1,-1, 1,-1, 1, 1,-1, 1, 1, 1, // 900
                  1,-1,-1, 1, 1,-1,-1,-1,-1, 1,-1, 1, 1, 1,-1,-1, 1,-1,-1,-1, // 920
                  1, 1, 1,-1,-1, 1,-1, 1,-1, 1,-1,-1, 1, 1,-1, 1,-1,-1, 1, 1, // 940
                 -1,-1, 1,-1,-1,-1,-1, 1, 1, 1, 1,-1,-1, 1, 1,-1,-1, 1, 1, 1, // 960
                  1,-1,-1,-1, 1, 1,-1,-1,-1,-1,-1,-1, 1, 1, 1,-1,-1,-1, 1,-1, // 980
                 -1, 1,-1,-1, 1,-1,-1, 1, 1,-1,-1, 1, 1,-1, 1, 1,-1, 1, 1, 1}; // 1000

    return ( i < 1 || i > n ) ? 0 : la[i-1];
}

int la_test1 ( nat n )
{
    int sum = 1;
    for ( nat i = 2; i <= n; ++i )
    {
        if ( n % i == 0 ) sum += la_func ( i );
    }
    return sum;
}

int fi_func ( nat n )
{
    int sum = 0;
    for ( nat i = 1; i <= n; ++i )
    {
        if ( n % i == 0 ) sum += i * mu_func ( n / i );
    }
    return sum;
}

int bf_func ( nat n )
{
    int sum = 0;
    for ( nat i = 1; i <= n; ++i )
    {
        if ( n % i == 0 ) sum += i * mu_func ( i );
    }
    return sum;
}

int la_test2 ( nat n )
{
    int sum = 0;
    for ( nat i = 1; i <= n; ++i )
    {
        if ( n % i == 0 )
        {
            nat j = ( i-1 ) / 2;
            nat k = j % 2 == 0 ? 1 : -1;
            j = ( n/i-1 ) / 2;
            k *= j % 2 == 0 ? 1 : -1;
            sum += k * la_func ( i );
        }
    }
    return sum;
}

int fi_test ( nat n )
{
    int sum = 0;
    for ( nat i = 1; i <= n; ++i )
    {
        if ( n % i ) continue;
        nat j = n / i;
        sum += fi_func ( i ) * bf_func ( j );
    }
    return sum;
}

int fi_test ( nat n, nat k, nat s )
{
    int sum = 0;
    for ( nat i = 1; i <= n; ++i )
    {
        if ( n % i ) continue;
        nat j = n / i;
        if ( j % k == s ) continue;
        sum += i * mu_func ( j );
    }
    return sum;
}

int la_test ( nat n, nat k, nat s )
{
    int sum = 0;
    for ( nat i = s; i <= n; i += k )
    {
        if ( n % i ) continue;
        nat j = n / i;
        if ( j % k != s ) continue;
        sum += la_func ( i );
    }
    return sum;
}

int la_test3 ( nat n )
{
    int sum = 0;
    for ( nat i = 1; i <= n; i += 1 )
    {
        if ( n % i ) continue;
        nat j = n / i;
        if ( i % 3 == 0 ) continue;
        if ( j % 3 == 0 ) continue;
        int k = 1;
//        if ( j % 3 == 2 ) k = -1;
//        if ( i % 3 == 2 ) k *= -1;
        sum += la_func ( i ) * k;
    }
    return sum;
}

int mu_test3 ( nat n )
{
    int sum = 0;
    for ( nat i = 1; i <= n; i += 2 )
    {
        nat m = i * i;
        if ( n % m ) continue;
        nat j = n / m;
        if ( i % 2 == 0 ) continue;
        if ( j % 2 == 0 ) continue;
        int k = 1;
        if ( j % 4 == 3 ) k = -1;
        if ( i % 4 == 3 ) k *= -1;
        sum += mu_func ( j ) * k;
    }
    return sum;
}

int la2_func ( nat n )
{
    int sum = 0;
    for ( nat i = 1; i <= n; i += 2 )
    {
        nat m = i * i;
        if ( n % m ) continue;
        nat j = n / m;
        if ( i % 2 == 0 ) continue;
        if ( j % 2 == 0 ) continue;
        int k = 1;
        if ( j % 4 == 3 ) k = -1;
        if ( i % 4 == 3 ) k *= -1;
        sum += mu_func ( j ) * k;
    }
    if ( n % 4 == 3 ) return -sum;
    return sum;
}

int lb2_func ( nat n )
{
    if ( n == 1 ) return 1;
    int sum = 0;
    for ( nat i = 1; i < n; i += 2 )
    {
        if ( n % i ) continue;
        nat j = n / i;
        sum += la2_func ( j ) * lb2_func ( i );
    }
    return -sum;
}

int la2_test ( nat n )
{
    int sum = 0;
    for ( nat i = 1; i <= n; i += 2 )
    {
        if ( n % i ) continue;
        sum += la2_func ( i );
    }
    return sum;
}

int la2_test2 ( nat n )
{
    int sum = 0;
    for ( nat i = 1; i <= n; i += 2 )
    {
        if ( n % i ) continue;
        nat j = n / i;
        int ii = la2_func ( i );
        if ( i % 4 == 3 ) ii = -ii;
        sum += ii * abs ( mu_func ( j ) );
    }
    return sum;
}

int la_test4 ( nat n )
{
    int sum = 0;
    for ( nat i = 1; i <= n; i += 1 )
    {
        if ( n % i ) continue;
        nat j = n / i;
        if ( i % 2 == 0 ) continue;
        if ( j % 2 == 0 ) continue;
        int k = 1;
        if ( j % 4 == 3 ) k = -1;
        if ( i % 4 == 3 ) k *= -1;
        sum += la_func ( i ) * k;
    }
    return sum;
}

int la_test6 ( nat n )
{
    int sum = 0;
    for ( nat i = 1; i <= n; i += 1 )
    {
        if ( n % i ) continue;
        nat j = n / i;
        if ( i % 2 == 0 ) continue;
        if ( j % 2 == 0 ) continue;
        if ( i % 3 == 0 ) continue;
        if ( j % 3 == 0 ) continue;
        int k = 1;
        if ( j % 6 == 5 ) k = -1;
        if ( i % 6 == 5 ) k *= -1;
        sum += la_func ( i ) * k;
    }
    return sum;
}

int la_test8 ( nat n )
{
    int sum = 0;
    for ( nat i = 1; i <= n; i += 2 )
    {
        if ( n % i ) continue;
        nat j = n / i;
        int k = 1;
        if ( j % 8 == 7 || j % 8 == 5 ) k = -1;
        if ( i % 8 == 7 || i % 8 == 5 ) k *= -1;
        sum += la_func ( i ) * k;
    }
    return sum;
}

int mu_test1 ( nat n )
{
    int sum = 1;
    for ( nat i = 2; i <= n; ++i )
    {
        if ( n % i == 0 ) sum += mu_func ( i );
    }
    return sum;
}

void mu_test ( nat n )
{
    display << "mu_test begin" << NL;
    for ( nat i = 2; i <= n; ++i )
    {
        if ( mu_test1 ( i ) != 0 ) display << i << NL;;
    }
    display << "mu_test end" << NL;
}

void mu_gen ()
{
    nat32 i, j;
    const nat n = 300;
    Suite<int> a ( n, n );
    Suite<bit8> text;
    PseudoWriteFile file ( text );
    for ( i = 0; i < n; ++i ) a[i] = mu_func ( i + 1 );
    for ( i = 1; i <= 700; ++i )
    {
        a.inc() = 0;
        int sum = 1;
        nat m = a.size();
        for ( j = 2; j < m; ++j )
        {
            if ( m % j == 0 ) sum += a[j-1];
        }
        a.las() = -sum;
        if ( i % 20 == 1 ) printf ( file, "                 " );
        switch ( sum )
        {
        case 1: printf ( file, "-1," ); break;
        case 0: printf ( file, " 0," ); break;
        case -1:printf ( file, " 1," ); break;
        }
        if ( i % 20 == 0 )
        {
            printf ( file, " // % 3d\n", 300+i );
        }
    }
    text.add ( 0 );
    display << (char *) text() << NL;
}

void la_gen ()
{
    nat32 i, j;
    const nat n = 300;
    Suite<int> a ( n, n );
    Suite<bit8> text;
    PseudoWriteFile file ( text );
    for ( i = 0; i < n; ++i ) a[i] = la_func ( i + 1 );
    for ( i = 1; i <= 700; ++i )
    {
        a.inc() = 0;
        nat m = a.size();
        nat s = (nat) sqrt ( m + 0.1 );
        int sum = s * s == m ? 0 : 1;
        for ( j = 2; j < m; ++j )
        {
            if ( m % j == 0 ) sum += a[j-1];
        }
        a.las() = -sum;
        if ( i % 20 == 1 ) printf ( file, "                 " );
        switch ( sum )
        {
        case 1: printf ( file, "-1," ); break;
        case 0: printf ( file, " 0," ); break;
        case -1:printf ( file, " 1," ); break;
        }
        if ( i % 20 == 0 )
        {
            printf ( file, " // % 3d\n", 300+i );
        }
    }
    text.add ( 0 );
    display << (char *) text() << NL;
}

void alfa31_gen ()
{
    nat32 i, j;
    const nat n = 7*7*7;
    DynArray<int> a ( n );
    a[0] = 1;
    for ( i = 2; i <= n; ++i )
    {
        nat s = (nat) root3 ( i + 0.1 );
        int sum = s * s * s == i ? 0 : 1;
        for ( j = 2; j < i; ++j )
        {
            if ( i % j == 0 ) sum += a[j-1];
        }
        a[i-1] = -sum;
    //display << i << a[i-1] << NL;
    }
    for ( nat k = 1; k <= n; ++k )
    {
        int sum = 0;
        for ( i = 1; i <= k; i += 1 )
        {
            if ( k % i ) continue;
            nat j = k / i;
            if ( i % 2 == 0 ) continue;
            if ( j % 2 == 0 ) continue;
            int z = 1;
            if ( j % 4 == 3 ) z = -1;
            if ( i % 4 == 3 ) z *= -1;
            sum += a[i-1] * z;
        }
        if ( sum ) display << k << sum << NL;
    }
}

void alfa32_gen ()
{
    nat32 i, j;
    const nat n = 7*7*7;
    DynArray<int> a ( n );
    a[0] = 1;
    for ( i = 2; i <= n; ++i )
    {
        nat s = (nat) root3 ( i + 0.1 );
        int sum = s * s * s == i ? -1 : 0;
        for ( j = 2; j < i; ++j )
        {
            if ( i % ( j * j ) == 0 ) sum += a[i/(j*j)-1];
        }
        a[i-1] = -sum;
    //if(a[i-1]) display << i << a[i-1] << NL;
    }
    for ( nat k = 1; k <= n; ++k )
    {
        int sum = 0;
        for ( i = 1; i <= k; i += 1 )
        {
            if ( k % (i*i) ) continue;
            nat j = k / (i*i);
            if ( i % 2 == 0 ) continue;
            if ( j % 2 == 0 ) continue;
            int z = 1;
            if ( j % 4 == 3 ) z = -1;
            //if ( i % 4 == 3 ) z *= -1;
            sum += a[j-1] * z;
        }
        if ( sum ) display << k << sum << NL;
    }
}

class MuFunc
{
    Suite<int8> buf;
public:
    MuFunc()
    {
        buf.resize ( 1000 );
        for ( nat i = 0; i < buf.size(); ++i ) buf[i] = mu_func ( i+1 );
    }
    int operator() ( nat i )
    {
        while ( i > buf.size() )
        {
            buf.inc();
            int8 sum = 1;
            nat m = buf.size();
            for ( nat j = 2; j < m; ++j )
            {
                if ( m % j == 0 ) sum += buf[j-1];
            }
            buf.las() = -sum;
        }
        return buf[i-1];
    }
};

class LaFunc
{
    Suite<int8> buf;
public:
    LaFunc()
    {
        buf.resize ( 1000 );
        for ( nat i = 0; i < buf.size(); ++i ) buf[i] = la_func ( i+1 );
    }
    int operator() ( nat i )
    {
        while ( i > buf.size() )
        {
            buf.inc();
            nat m = buf.size();
            nat s = (nat) sqrt ( m + 0.1 );
            int8 sum = s * s == m ? 0 : 1;
            for ( nat j = 2; j < m; ++j )
            {
                if ( m % j == 0 ) sum += buf[j-1];
            }
            buf.las() = -sum;
        }
        return buf[i-1];
    }
};

void testMuFunc ( nat n )
{
    display << "testMuFunc begin" << NL;
    MuFunc mu;
    for ( nat i = 2; i <= n; ++i )
    {
        int sum = 1;
        for ( nat j = 2; j <= i; ++j )
        {
            if ( i % j == 0 ) sum += mu ( j );
        }
        if ( sum != 0 ) display << i << NL;
    }
    display << "testMuFunc end" << NL;
}

bool testSec ( nat ns, const int * s )
{
    for ( nat n = 2; n <= 1000; ++n )
    {
        int sum = 0;
        for ( nat j = 1; j <= n; ++j )
        {
            if ( n % j == 0 ) 
                sum += mu_func ( j ) * s[j%ns] * s[(n/j)%ns];
        }
        if ( sum != 0 )
            return false;
    }
    return true;
}

void testSec3()
{
    const nat n = 3;
    int s[n];
    s[1] = 1;
    for ( int i2 = -1; i2 <= 1; ++i2)
    {
        s[2] = i2;
        for ( int i3 = -1; i3 <= 1; ++i3 )
        {
            s[0] = i3;
            if ( testSec ( n, s ) )
            {
                display << s[1] << s[2] << s[0] << NL;
            }
        }
    }
}

void testSec4()
{
    const nat n = 4;
    const int k = 1;
    int s[n];
    s[1] = 1;
    for ( int i2 = -k; i2 <= k; ++i2)
    {
        s[2] = i2;
        for ( int i3 = -k; i3 <= k; ++i3 )
        {
            s[3] = i3;
            for ( int i4 = -k; i4 <= k; ++i4 )
            {
                s[0] = i4;
                if ( testSec ( n, s ) )
                {
                    display << s[1] << s[2] << s[3] << s[0] << NL;
                }
            }
        }
    }
}

void testSec5()
{
    const nat n = 5;
    const int k = 1;
    int s[n];
    s[1] = 1;
    for ( int i2 = -k; i2 <= k; ++i2)
    {
        s[2] = i2;
        for ( int i3 = -k; i3 <= k; ++i3 )
        {
            s[3] = i3;
            for ( int i4 = -k; i4 <= k; ++i4 )
            {
                s[4] = i4;
                for ( int i5 = -k; i5 <= k; ++i5 )
                {
                    s[0] = i5;
                    if ( testSec ( n, s ) )
                    {
                        display << s[1] << s[2] << s[3] << s[4] << s[0] << NL;
                    }
                }
            }
        }
    }
}

void testSec6()
{
    const nat n = 6;
    const int k = 1;
    int s[n];
    s[1] = 1;
    for ( int i2 = -k; i2 <= k; ++i2)
    {
        s[2] = i2;
        for ( int i3 = -k; i3 <= k; ++i3 )
        {
            s[3] = i3;
            for ( int i4 = -k; i4 <= k; ++i4 )
            {
                s[4] = i4;
                for ( int i5 = -k; i5 <= k; ++i5 )
                {
                    s[5] = i5;
                    for ( int i6 = -k; i6 <= k; ++i6 )
                    {
                        s[0] = i6;
                        if ( testSec ( n, s ) )
                        {
                            display << s[1] << s[2] << s[3] << s[4] << s[5] << s[0] << NL;
                        }
                    }
                }
            }
        }
    }
}

void testSec7()
{
    const nat n = 7;
    const int k = 1;
    int s[n];
    s[1] = 1;
    for ( int i2 = -k; i2 <= k; ++i2)
    {
        s[2] = i2;
        for ( int i3 = -k; i3 <= k; ++i3 )
        {
            s[3] = i3;
            for ( int i4 = -k; i4 <= k; ++i4 )
            {
                s[4] = i4;
                for ( int i5 = -k; i5 <= k; ++i5 )
                {
                    s[5] = i5;
                    for ( int i6 = -k; i6 <= k; ++i6 )
                    {
                        s[6] = i6;
                        for ( int i7 = -k; i7 <= k; ++i7 )
                        {
                            s[0] = i7;
                            if ( testSec ( n, s ) )
                            {
                                display << s[1] << s[2] << s[3] << s[4] << s[5] << s[6] << s[0] << NL;
                            }
                        }
                    }
                }
            }
        }
    }
}

void testSec8()
{
    const nat n = 8;
    const int k = 1;
    int s[n];
    s[1] = 1;
    for ( int i2 = -k; i2 <= k; ++i2)
    {
        s[2] = i2;
        for ( int i3 = -k; i3 <= k; ++i3 )
        {
            s[3] = i3;
            for ( int i4 = -k; i4 <= k; ++i4 )
            {
                s[4] = i4;
                for ( int i5 = -k; i5 <= k; ++i5 )
                {
                    s[5] = i5;
                    for ( int i6 = -k; i6 <= k; ++i6 )
                    {
                        s[6] = i6;
                        for ( int i7 = -k; i7 <= k; ++i7 )
                        {
                            s[7] = i7;
                            for ( int i8 = -k; i8 <= k; ++i8 )
                            {
                                s[0] = i8;
                                if ( testSec ( n, s ) )
                                {
                                    display << s[1] << s[2] << s[3] << s[4] << s[5] << s[6] << s[7] << s[0] << NL;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void testSec12()
{
    const nat n = 12;
    const int k = 1;
    int s[n];
    s[1] = 1;
    for ( int i2 = -k; i2 <= k; ++i2)
    {
        s[2] = i2;
        for ( int i3 = -k; i3 <= k; ++i3 )
        {
            s[3] = i3;
            for ( int i4 = -k; i4 <= k; ++i4 )
            {
                s[4] = i4;
                for ( int i5 = -k; i5 <= k; ++i5 )
                {
                    s[5] = i5;
                    for ( int i6 = -k; i6 <= k; ++i6 )
                    {
                        s[6] = i6;
                        for ( int i7 = -k; i7 <= k; ++i7 )
                        {
                            s[7] = i7;
                            for ( int i8 = -k; i8 <= k; ++i8 )
                            {
                                s[8] = i8;
                                for ( int i9 = -k; i9 <= k; ++i9 )
                                {
                                    s[9] = i9;
                                    for ( int i10 = -k; i10 <= k; ++i10 )
                                    {
                                        s[10] = i10;
                                        for ( int i11 = -k; i11 <= k; ++i11 )
                                        {
                                            s[11] = i11;
                                            for ( int i12 = -k; i12 <= k; ++i12 )
                                            {
                                                s[0] = i12;
                                                if ( testSec ( n, s ) )
                                                {
display << s[1] << s[2] << s[3] << s[4] << s[5] << s[6] << s[7] << s[8] << s[9] << s[10] << s[11] << s[0] << NL;
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
}

void testSec ()
{
    display << "testSec begin" << NL;
    testSec12();
    display << "testSec end" << NL;
}

double func1 ()
{
    double sum = 0;
    for ( nat n = 1; n <= 10000; ++n )
    {
        switch ( n % 8 )
        {
        case 3: sum += 1./ (n); break;
        case 7: sum -= 1./ (n); break;
        }
    }
    return sum;
}

double func2a ( MuFunc & mu )
{
    double sum = 0;
    for ( nat n = 1; n <= 20000; ++n )
    {
        switch ( n % 8 )
        {
        case 1: sum += mu ( n ) * 0.5 / (n); break;
        case 2: sum -= mu ( n ) * 1.0 / (n); break;
        case 3: sum += mu ( n ) * 1.5 / (n); break;
        case 5: sum -= mu ( n ) * 1.5 / (n); break;
        case 6: sum += mu ( n ) * 1.0 / (n); break;
        case 7: sum -= mu ( n ) * 0.5 / (n); break;
        }
    }
    return sum;
}

double funcA2 ( nat n, nat k )
{
    double sum = 0;
    for ( nat i = 1; i <= 100000; ++i )
    {
        if ( i % n == k ) sum += 1.0 / _pow2(double(i));
    }
    return sum;
}

double funcA3 ( nat n, nat k )
{
    double sum = 0;
    for ( nat i = 1; i <= 1000; ++i )
    {
        if ( i % n == k ) sum += 1.0 / _pow3(double(i));
    }
    return sum;
}

double funcA4 ( nat n, nat k )
{
    double sum = 0;
    for ( nat i = 1; i <= 1000; ++i )
    {
        if ( i % n == k ) sum += 1.0 / _pow4(double(i));
    }
    return sum;
}

double funcB1 ( MuFunc & mu, nat n, nat k )
{
    double sum = 0;
    for ( nat i = 1; i <= 30000; ++i )
    {
        if ( i % n == k )
        {
            sum += mu ( i ) / double(i);
        }
    }
    return sum;
}

double funcB2 ( nat n, nat k )
{
    double sum = 0;
    for ( nat i = 1; i <= 1000; ++i )
    {
        if ( i % n == k ) sum += mu_func ( i ) / _pow2(double(i));
    }
    return sum;
}

double funcB3 ( nat n, nat k )
{
    double sum = 0;
    for ( nat i = 1; i < 1000; ++i )
    {
        if ( i % n == k ) sum += mu_func ( i ) / _pow3(double(i));
    }
    return sum;
}

double sin1 ( nat n, nat k, double x )
{
    double sum = 0;
    for ( nat i = 1; i <= 1000000; ++i )
    {
        if ( i % n == k ) sum += sin(i*x) / i;
    }
    return sum;
}

double e0 ( double x )
{
    double sum = 0;
    for ( int i = 1; i < 1000; ++i )
    {
        const double t = exp ( -i * x );
        if ( i % 2 == 1 ) sum += mu_func ( i ) * t;
        if ( t < 1e-6 ) break;
    }
    return sum;
}

double tf ( double x )
{
    double sum = 0;
    for ( int i = 1; i < 100000; ++i )
    {
        if ( i % 2 == 0 ) continue;
        const double t = 1 / ( x * x + i * i );
        sum += t;
    }
    return sum;
}

double tf2 ( double x )
{
    double sum = 0;
    for ( int i = 1; i < 1000; ++i )
    {
        if ( i % 2 == 0 ) continue;
        const double t = M_PI * x / i;
        const double f = t * ( cos ( t ) - 1 ) / sin ( t );
        sum += mu_func ( i ) * f;
    }
    return sum;
}

double pn ( double x, nat n )
{
    double res = 1;
    for ( nat i = 0; i < n; ++i )
    {
        res *= ( x - i );
    }
    return res;
}

void func_test1()
{
    MuFunc mu;
    double res = 0;
    for ( nat i = 1; i < 10000; ++i )
    {
        if ( i % 4 == 1 )
        {
            res += abs(mu(i))/double(i);
        }
        else
        if ( i % 4 == 3 )
        {
            res -= abs(mu(i))/double(i);
        }
    }
    display << 100*res << 100*(2/M_PI) << NL;
}

void func_test2()
{
    double res = 0;
    for ( nat i = 1; i < 101; ++i )
    {
 //if ( mu_func(i) && mu_func(i) != la_func(i) ) display << "!=" << NL;
        if ( i % 8 == 1 )
        {
            res += fabs(mu_func(i))/_pow2(double(i));
        }
        else
        if ( i % 8 == 3 )
        {
            res -= fabs(mu_func(i))/_pow2(double(i));
        }
        else
        if ( i % 8 == 5 )
        {
            res -= fabs(mu_func(i))/_pow2(double(i));
        }
        else
        if ( i % 8 == 7 )
        {
            res += fabs(mu_func(i))/_pow2(double(i));
        }
    }
    display << 100/res << 100*(M_PI*M_PI/(6*sqrt(2))) << NL;
}

void func_test3()
{
    //MuFunc mu;
    double res = 0;
    for ( nat i = 1; i <= 200; ++i )
    {
        if ( i % 4 == 1 )
        {
            res += la_func(i)*1./_pow3(double(i));
        }
        if ( i % 4 == 3 )
        {
            res -= la_func(i)*1./_pow3(double(i));
        }
    }
    display << 10000*res << 10000*_pow3(M_PI)/(30) << NL;
}

inline double st1 ( double x )
{
    while ( x > 1 ) x -= 2;
    if ( !x || x == 1 ) return 0;
    return x > 0 ? 1 : -1;
}

inline double st2 ( double x )
{
    return st1 ( x + 0.5 );
}

// Многочлены Бернулли
double B1 ( double x ) { return x - 1./2; }
double B2 ( double x ) { return x*x - x + 1./6; }
double B3 ( double x ) { return x*x*x - 3*x*x/2 + x/2; }
double B4 ( double x ) { return x*x*x*x - 2*x*x*x + x*x - 1./30; }
double B5 ( double x ) { return x*x*x*x*x - 5*x*x*x*x/2 + 5*x*x*x/3 - x/6; }
double B6 ( double x ) { return x*x*x*x*x*x - 3*x*x*x*x*x + 5*x*x*x*x/2 - x*x/2 + 1./42.; }

/*                  Формулы

    S(i=1) 2 * x / ( x^2 + (2*M_PI*i)^2 ) = 1 / ( exp ( x ) - 1 ) - 1/x + 1/2

    S(i=0) 4 * x / ( x^2 + ((2*i+1)*M_PI)^2 ) = tanh ( x / 2 )
    f ( x ) = x * sqrt ( x ) * S(i=0) (2*i+1) * (-1)^i * exp ( - M_PI/4 * x^2 * (2*i+1)^2 )
    f ( x ) = f ( 1/x ), x > 0

    S(i=1) mu(i) * ( 1 / (exp(x/i) - 1) - i/x ) = 2 * x / ( x^2 + (2*M_PI)^2 )

    S(i=1) mu(i) / i * log ( 1 - x^i ) = -x
    S(i=1) mu(i) / i * log ( 1 + x^i ) = x - x * x
    S(i=1) mu(i) / i * log ( 2 * fabs ( sin ( i*x/2 ) ) ) = -cos(x)
    S(i=1) mu(i) / i * exp ( -x / i ) = 2 * S(i=1) mu(i) * i * x / ( (i*x)^2 + (2*M_PI)^2 ), x >= 0
    f ( x ) = sqrt ( x ) * S(i=1) mu(i) / i * exp ( - M_PI * x^2 / i^2 )
    f ( x ) = f ( 1/x ), x > 0

    S(i=0) mu(2*i+1) / (2*i+1) * arth ( x^(2*i+1) ) = x
    S(i=0) mu(2*i+1) / (2*i+1) * tanh ( x/(4*i+2) ) = 4 * x  / ( x^2 + M_PI^2 )

    S(i=0) mu(2*i+1) / (2*i+1) * (-1)^i * atan ( x^(2*i+1) ) = x
    S(i=0) mu(2*i+1) / (2*i+1) * (-1)^i / cosh ( x/(4*i+2) ) = 4 * M_PI  / ( x^2 + M_PI^2 )
    S(i=0) mu(2*i+1) / (2*i+1) * (-1)^i * arth ( sin(x*(2*i+1) ) = 2 * sin ( x )

    S(i=1) mu(i) / (i*i) * Li2 ( - x^i ) = x * x / 2 - x

*/

void func_test4()
{
    MuFunc mu;
    double res = 0;
    for ( nat i = 1; i <= 310; ++i )
    {
        /*for ( int j = 1; j <= 10000; j+=1 )
        {
            f += cos(j*x)/j * ( j % 2 ? 1 : -1 );
        }
        display << x << f << log(2*cos(0.5*x)) << NL;*
        for ( int j = 1; j <= 10000; j+=2 )
        {
            f += cos(j*x)/j;
        }
        display << x << f << -log(tan(0.5*x))/2 << NL;/
        for ( int j = 1; j <= 10000; j+=2 )
        {
            f += mu(j)*st1(j*x)/j;
        }
        display << i << 0.25*M_PI*f << sin(M_PI*x) << NL;*
        for ( int j = 1; j <= 10000; j+=2 )
        {
            f += mu(j)*st2(j*x)/j * ( j % 4 == 1 ? 1 : -1 );
        }
        display << i << 0.25*M_PI*f - cos(M_PI*x) << NL;*
        for ( int j = 1; j <= 10000; j+=2 )
        {
            f += mu(j)*tanh(x/(j+j))/j;
        }
        display << x << 4*x*M_PI/(x*x + M_PI*M_PI) << M_PI*f << NL;*
        for ( int j = 1; j <= 20000; j+=2 )
        {
            f += mu(j)/cosh(x/(j+j))/j * ( j % 4 == 1 ? 1 : -1 );
        }
        display << x << 4*M_PI/(x*x + M_PI*M_PI) << f << NL;/*
        for ( int j = 1; j <= 1000; j+=2 )
        {
            f += x/(x*x + M_PI*M_PI*j*j);
        }
        display << x << tanh(x/2) << 4*f << NL;/*
        for ( int j = 0; j <= 10000; j+=1 )
        {
            f += sin(x*(2*j+1))/(2*j+1) * ( j % 2 ? -1 : 1 );
        }
        display << x << f << log ( (1 + sin(x)) / (1 - sin(x)) ) / 4 << NL;/*
        for ( int j = 0; j <= 1000; j+=1 )
        {
            double u = sin(x*(2*j+1));
            f += mu(2*j+1) * log ( (1 + u) / (1 - u) ) /(2*j+1) * ( j % 2 ? -1 : 1 );
        }
        display << x << f << sin(x) * 4 << NL;*
        for ( int j = 1; j <= 10000; j+=2 )
        {
            double e = exp(-j*x)/j;
            f += e;
            if ( e < 1e-6 ) break;
        }
        display << x << f << 0.5*log ( (1 + exp(-x)) / (1 - exp(-x)) ) << NL;*/
    }
}

double sin2 ( double x )
{
    double res = 0;
    for ( double i = 1; i < 10000; ++i )
    {
        double t = i * i;
        res += sin ( x * t ) / t;
    }
    return res;
}

double cos2 ( double x )
{
    double res = 0;
    for ( int i = 1; i < 1000; ++i )
    {
        if ( i % 2 == 0 ) continue;
        double t = i * i;
        res += cos ( x * t ) / ( t * t );
    }
    return res;
}

double cos4 ( double x )
{
    double res = 0;
    for ( int i = 1; i < 1000; ++i )
    {
        if ( i % 2 == 0 ) continue;
        double t = i;
        res += cos ( x * t ) / _pow4 ( t );
    }
    return res;
}

inline double sin3func ( double x )
{
    double s = 0;
    for ( int i = 1; i < 1000; i += 3 )
    {
        s += sin ( M_PI * i * x ) / _pow3 ( double(i) );
    }
    return s;
}

inline double fs ( double x )
{
    while ( x > 1 ) x -= 2;
    if ( x == 1 ) return 0;
    return x;
}

inline double fs2 ( double x )
{
    while ( x >= 2 ) x -= 2;
    if ( ! x ) return 0;
    return 1 - x;
}

inline double sin3func2 ( double x )
{
    double s = 0;
    MuFunc mu;
    for ( int i = 1; i < 1000; i += 2 )
    {
        s += fs ( i*x ) * mu(i) / _pow3 ( double(i) );
    }
    return s;
}

inline double sin1func ( double x )
{
    double s = 0;
    for ( int i = 1; i < 100000; i += 2 )
    {
        s += sin ( M_PI * i * x ) * _pow2 ( fs(x) ) / double(i);
    }
    return s / ( M_PI );
}

double sum1 ( LaFunc & la, nat n, nat k )
{
    double sum = 0;
    for ( nat i = 1; i <= 30000; ++i )
    {
        if ( i % n == k ) sum += la ( i ) / double(i);
    }
    return sum;
}

double func_la2 ( nat n, nat k )
{
    double sum = 0;
    for ( nat i = 1; i <= 300; ++i )
    {
        if ( i % n == k ) sum += la_func ( i ) / _pow2 ( double(i) );
    }
    return sum;
}

double func_mu2 ( nat n, nat k )
{
    double sum = 0;
    for ( nat i = 1; i <= 300; ++i )
    {
        if ( i % n == k ) sum += mu_func ( i ) / _pow2 ( double(i) );
    }
    return sum;
}

void func_test5()
{
    LaFunc lf;
    double a1 = 0, a3 = 0, b1 = 0, b3 = 0;
    MuFunc mf;
    double c1 = 0, c3 = 0, d1 = 0, d3 = 0;
    for ( nat i = 1; i <= 20000; i += 2 )
    {
        if ( mf ( i ) )
        {
            if ( lf ( i ) > 0 )
            {
                ( i % 4 == 1 ? a1 : a3 ) += 1 / _pow2 ( double(i) );
            }
            else
            {
                ( i % 4 == 1 ? b1 : b3 ) += 1 / _pow2 ( double(i) );
            }
        }
        else
        {
            if ( lf ( i ) > 0 )
            {
                ( i % 4 == 1 ? c1 : c3 ) += 1 / _pow2 ( double(i) );
            }
            else
            {
                ( i % 4 == 1 ? d1 : d3 ) += 1 / _pow2 ( double(i) );
            }
        }
    }
    display << a1 / c1 << b1 / d1 << NL;
    display << a3 / c3 << b3 / d3 << NL;
}

double func_test6 ( MuFunc & mu, double x )
{
    double sum = 0;
    for ( int i = 1; i < 18000; i+=1 )
    {
        sum += mu(i) / cosh ( M_PI*x/i ) / i;
    }
    return x*sum;
}

double func_test6a ( MuFunc & mu, double x )
{
    double sum = 0;
    for ( int i = 1; i < 18000; i+=1 )
    {
        sum += mu(i) / cosh ( M_PI/x/i ) / i;
    }
    return sum;
}

void func_test7()
{
    double a = 0.5;
    for ( int i = 0; i <= 100; i+=1 )
    {
        double x = 0.01 * i;
        double sum1 = 0;
        for ( int j = -100; j <= 100; ++j ) sum1 += exp ( - a * fabs ( x + j ) );
        double sum2 = 0;
        for ( int n = -100; n <= 100; ++n ) sum2 += cos ( 2 * M_PI * n * x ) / ( a*a + _pow2 ( 2 * M_PI * n ) );
        sum2 *= 2 * a;
        display << x << sum1 << sum2 << cosh ( a * ( 0.5 - x ) ) / sinh ( 0.5 * a ) << NL;
    }
}

void func_test8()
{
    double a = 1;
    /*for ( int i = 1; i < 100; i+=1 )
    {
        double x = 0.01 * i;
        double sum1 = 0;
        //for ( int j = -100; j <= 100; ++j ) sum1 += exp ( - a * fabs ( x + j ) );
        double sum2 = 0;
        for ( int n = -1000; n <= 1000; ++n ) 
            sum2 +=  x / ( x*x + _pow2 ( 2 * M_PI * n ) ) / ( a*a + _pow2 ( 2 * M_PI * n ) );
        sum2 *= 2 * a;
        display << x << sum2 << ( x / tanh ( a/2 ) - a / tanh ( x/2 ) ) / ( x*x - a*a ) << NL;
    }*/
        double sum2 = 0;
        for ( int n = -1000; n <= 1000; ++n ) 
            sum2 +=  a / ( a*a + _pow2 ( 2 * M_PI * n ) ) * a / ( a*a + _pow2 ( 2 * M_PI * n ) );
        display << sum2 << ( sinh ( a ) + a ) / ( _pow2 ( sinh ( a/2 ) ) * 8 * a )<< NL;
}

void func_test9()
{
    double a = 2;
    for ( int i = 1; i < 100; i+=1 )
    {
        double x = 0.01 * i;
        double sum = 0;
        for ( int n = -10000; n <= 10000; ++n ) 
            sum += _pow3 ( sin ( a * M_PI * ( x + n ) ) / ( a * M_PI * ( x + n ) ) );
//        display << x << sum << 0.25 * ( cos ( 2 * M_PI * x ) + 3 ) << NL;
        display << x << sum << _pow2 ( 0.5 * ( cos ( 2 * M_PI * x ) + 1 ) )<< NL;
    }
}

double func9 ( double x )
{
    double sum = 0;
    for ( int i = 1; i < 10000; i+=1 )
    {
        double y = i * x;
        if ( i % 2 )
            sum -= M_PI / ( y * sinh ( M_PI * y ) ) - 1/(y*y);
        else
            sum += M_PI / ( y * sinh ( M_PI * y ) ) - 1/(y*y);
    }
    return sum*x;
}

void func_test10()
{
    for ( int i = 1; i < 10; i+=1 )
    {
        double x = i;
        display << x << func9 (x) << func9 (1/x) << x + 1/x << NL;
    }
}

double zeta ( int j )
{
    switch ( j )
    {
    case 2: return _pow2 ( M_PI ) / 6;
    case 3: return 1.2020569032;
    case 4: return _pow4 ( M_PI ) / 90;
    case 5: return 1.0369277551;
    case 6: return _pow6 ( M_PI ) / 945;
    case 7: return 1.0083492774;
    case 8: return _pow8 ( M_PI ) / 9450;
    }
    double sum = 1;
    for ( int i = 2; ; ++i )
    {
        double p = pow ( i, j );
        sum += 1 / p;
        if ( p > 1e16 ) break;
    }
    return sum;
}

double func11 ( MuFunc & mu, double x )
{
    double sum = 0;
    double q = sqrt ( M_PI );
    for ( int i = 1; i < 9000; i+=1 )
    {
        double t = q / i;
        sum += mu(i) * exp ( - t * t * x ) * t;
    }
    return sum;
}

double func11a ( double x )
{
    double sum = 0, t = 1;
    for ( int i = 1; i < 9000; i+=1 )
    {
        t *= - M_PI / i * x;
        double p = t / zeta ( 2*i + 1 );
        sum += p;
        if ( fabs ( p ) < 1e-9 ) break;
    }
    return sqrt ( M_PI ) * sum;
}

double func11b ( double x )
{
    double sum = 0, t = 1;
    for ( int i = 1; i < 9000; i+=1 )
    {
        t *= - M_PI / i * x;
        double p = t / zeta ( 2*i + 1 );
        sum += p;
        if ( fabs ( p ) < 1e-9 ) break;
    }
    return sqrt ( M_PI ) * sum;
}

double func11 ( MuFunc & mu, double x, int p )
{
    double sum = 0;
    double q = sqrt ( M_PI );
    for ( int i = 1; i < 9000; i+=1 )
    {
        double t = q / i;
        sum += mu(i) * exp ( - t * t * x ) * t * pow ( t, p+p );
    }
    return sum;
}

Complex func11 ( MuFunc & mu, Complex x )
{
    Complex sum ( 0, 0 );
    double q = sqrt ( M_PI );
    for ( int i = 1; i < 19000; i+=1 )
    {
        double t = q / i;
        sum += mu(i) * exp ( - t * t * x ) * t;
    }
    return sum;
}

double func12 ( double x )
{
    double sum = 0;
    for ( int i = 0; i < 900; i+=1 )
    {
        double q = i + i + 1;
        if ( i % 2 == 0 )
        {
            sum += q * exp ( - M_PI / 4 * x * x * q * q );
        }
        else
        {
            sum -= q * exp ( - M_PI / 4 * x * x * q * q );
        }
    }
    return x*sum;
}

double func12a ( MuFunc & mu, double x )
{
    double sum = 0;
    for ( int i = 0; i < 900; i+=1 )
    {
        double q = i + i + 1;
        q *= q;
        if ( i % 2 )
            sum += mu(i + i + 1) * exp ( - M_PI / 4 * x * x / q ) / q;
        else
            sum -= mu(i + i + 1) * exp ( - M_PI / 4 * x * x / q ) / q;
    }
    return x * sqrt ( x ) * sum;
}

double func13 ( double x )
{
    double sum = 0;
    for ( int i = 0; i < 9000; i+=1 )
    {
        double t = 2*i + 1;
        double p = x / ( x*x + t*t );
        if ( i % 2 )
            sum += p;
        else
            sum += p;
    }
    return sum;
}

double func13a ( MuFunc & mu, double x )
{
    double sum = 0;
    for ( int i = 1; i < 19000; i+=1 )
    {
        double s = i * x;
        double p = mu ( i ) * s / ( s*s + 1 );
        sum += p;
    }
    return sum;
}

double func13b ( MuFunc & mu, double x )
{
    double sum = 0;
    for ( int i = 1; i < 19000; i+=1 )
    {
        double s = x / i;
        double p = mu ( i ) * exp ( -M_2PI*s ) / i;
        sum += p;
    }
    return sum*M_PI;
}

double func13c ( MuFunc & mu, double x )
{
    double sum = 0;
    for ( int i = 1; i < 19000; i+=1 )
    {
        double s = x / i;
        sum += mu ( i ) * ( 1 / ( exp ( s ) - 1 ) - 1 / s ) / i;
    }
    return sum;
}

double Li2 ( double x ) // дилогарифм
{
    double sum = 0, y = 1;
    for ( nat i = 1; i < 1000; i+=1 )
    {
        y *= x;
        sum += y / ( i*i );
    }
    return sum;
}

double Li3 ( double x )
{
    double sum = 0, y = 1;
    for ( nat i = 1; i < 1000; i+=1 )
    {
        y *= x;
        sum += y / ( i*i*i );
    }
    return sum;
}

double Li4 ( double x )
{
    double sum = 0, y = 1;
    for ( nat i = 1; i < 100; i+=1 )
    {
        y *= x;
        sum += y / ( i*i*i*i );
    }
    return sum;
}

double Li0 ( double x )
{
    return x / ( 1 - x );
}

double Li_1 ( double x )
{
    return x / _pow2 ( 1 - x );
}

double func14 ( double a, double b, double x )
{
    double sum = 0;
    for ( int i = 0; i < 10000; i+=1 )
    {
        double y = M_PI*(2*i+1)/a;
        sum += ( x * cos ( y*b ) + y * sin(y*b) ) / ( x*x + y*y );
    }
    return 2 * sum;
}

double func14a ( double a, double b, double x )
{
    double sum = 0;
    for ( int i = 1; i < 10000; i+=1 )
    {
        double y = 2*M_PI*i/a;
        sum += y * sin(y*b) / ( x*x + y*y );
    }
    return 2 * sum;
}

double func14b ( double a, double b, double x )
{
    double sum = 0;
    for ( int i = 1; i < 10000; i+=1 )
    {
        double y = 2*M_PI*i/a;
        sum += x * cos ( y*b ) / ( x*x + y*y );
    }
    return 1 / x + 2 * sum;
}

void func_test()
{
    MuFunc mu;
    for ( int i = 1; i <= 200; i+=1 )
    {
        double x = 0.01 * i;
        //double y = 0, t = 1;
//        display << x << func14a ( 1, 0.25, x ) << 0.25 * ( cosh ( 0.75*x ) - cosh ( 0.25*x ) ) / _pow2 ( sinh(0.5*x) ) << NL;
//        display << x << func14 ( 1, 0.25, x ) << exp ( -0.25*x ) / ( 1 + exp(-x) ) << NL;
//        display << x << func13c ( mu, -x ) << 2*x / (x*x + 4*M_PI*M_PI) << NL;
        display << x << func13a ( mu, x ) << func13b ( mu, x ) << NL;
    }
}

} // namespace

void calc_test ()
{
//        display << func_la2 ( 2, 0 ) / (M_PI*M_PI) * 60 << NL;
//    alfa32_gen ();
    func_test();
//    testMuFunc ( 120000 );
}
