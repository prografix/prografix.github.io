
#include "math.h"

#include "../Shev/tune.h"
#include "../Shev/atrian.h"
#include "../Shev/trian3d.h"
#include "../draw.h"

#include "display.h"

double timeInSec();

namespace
{

void trian_test()
{
    const nat n = 114;
    FixArray<Vector3d, n> vert;
    CmbArray<Set3<nat>, n-2> res;
    nat i;
    for ( i = 0; i < n; ++i )
    {
        const double a = i * M_2PI / n;
        Vector3d & v = vert[i];
        v.x = sin(a);
        v.y = cos(a);
        v.z = v.x * v.x - 0.5;
    }
    trian ( vert, res );
    for ( i = 0; i < res.size(); ++i )
    {
        const Set3<nat> & t = res[i];
        draw ( Segment3d ( vert[t.a], vert[t.b] ), 0, 1, 1 );
        draw ( Segment3d ( vert[t.b], vert[t.c] ), 0, 1, 1 );
        draw ( Segment3d ( vert[t.c], vert[t.a] ), 0, 1, 1 );
    }
}

void trian2_test()
{
    const nat n = 55;
    FixArray<Vector3d, n> vert1, vert2;
    CmbArray<Set3<nat>, n+n-2> res;
    nat i;
    for ( i = 0; i < n; ++i )
    {
        const double a = i * M_2PI / ( n - 1 );
        Vector3d & v1 = vert1[i];
        v1.x = ( a - M_PI ) / 3;
        v1.y = 2*(0.5 - sin(0.5*a));
        v1.z = -0.6;
        Vector3d & v2 = vert2[i];
        v2.x = 0.5*v1.x;
        v2.y = 0.5*v1.y;
        v2.z = -v1.z;
    }
    trian ( vert1, vert2, false, res );
    for ( i = 0; i < res.size(); ++i )
    {
        const Set3<nat> & t = res[i];
        const Vector3d & va = t.a >= n ? vert2[t.a-n] : vert1[t.a];
        const Vector3d & vb = t.b >= n ? vert2[t.b-n] : vert1[t.b];
        const Vector3d & vc = t.c >= n ? vert2[t.c-n] : vert1[t.c];
        draw ( Segment3d ( va, vb ), 0, 1, 1 );
        draw ( Segment3d ( vb, vc ), 0, 1, 1 );
        draw ( Segment3d ( vc, va ), 0, 1, 1 );
    }
    for ( i = 1; i < n; ++i )
    {
        draw ( Segment3d ( vert1[i], vert1[i-1] ), 1, 1, 0 );
        draw ( Segment3d ( vert2[i], vert2[i-1] ), 1, 0, 0 );
    }
}

} // namespace

template <class F, class A1, class A2=A1, class A3=A2, class A4=A3>
class Func4a // Функция четырёх аргументов
{
public:
    virtual F operator () ( A1 a1, A2 a2, A3 a3, A4 a4 ) const = 0;
};

template <> class TriDiag<void>
{
public:
    nat rib, facet1, facet2; // Индексы полуребра и соседних граней
};

void optiL ( Func4a<bool,nat> & change, ArrRef<SemiRib> & rib )
{
    const nat nr = rib.size();
    if ( nr < 6 || nr % 3 != 0 ) return;
    const nat nf = nr / 3;
    nat i;
    DynArray<nat> dinx ( nr, nr );
    Suite<TriDiag<void> > diag ( nr/2 ); // Массив диагоналей
    for ( i = 0; i < nf; ++i )
    {
        const nat ia = 3 * i;
        const SemiRib & a = rib[ia];
        const nat ib = a.next;
        const SemiRib & b = rib[ib];
        const nat ic = b.next;
        const SemiRib & c = rib[ic];
        if ( a.twin < nr )
        {
            dinx[ia] = dinx[a.twin] = diag.size();
            TriDiag<void> & d = diag.inc();
            d.facet1 = i;
            d.facet2 = a.twin/3;
            d.rib = ia;
        }
        if ( b.twin < nr )
        {
            dinx[ib] = dinx[b.twin] = diag.size();
            TriDiag<void> & d = diag.inc();
            d.facet1 = i;
            d.facet2 = b.twin/3;
            d.rib = ib;
        }
        if ( c.twin < nr )
        {
            dinx[ic] = dinx[c.twin] = diag.size();
            TriDiag<void> & d = diag.inc();
            d.facet1 = i;
            d.facet2 = c.twin/3;
            d.rib = ic;
        }
    }
    const nat nd = diag.size();
    Suite<nat> stack ( nd, nd ); // Очередь на проверку оптимальности диагоналей
    for ( i = 0; i < nd; ++i ) stack[i] = i;
    while ( stack.size() > 0 )
    {
        TriDiag<void> & d = diag[stack.las()];
        stack.dec();
        const nat a1 = d.rib;
        const nat b1 = rib[a1].next;
        const nat c1 = rib[b1].next;
        const nat a2 = rib[a1].twin;
        const nat b2 = rib[a2].next;
        const nat c2 = rib[b2].next;
        if ( change ( b1, c1, b2, c2 ) )
        {
            rib[a1].vert = rib[c1].vert;
            rib[a1].next = c2;
            rib[c2].next = b1;
            rib[b1].next = a1;
            rib[a2].vert = rib[c2].vert;
            rib[a2].next = c1;
            rib[c1].next = b2;
            rib[b2].next = a2;
            i = dinx[c1];
            if ( i < nr )
            {
                TriDiag<void> & di = diag[i];
                if ( di.rib >= nr )
                {
                    di.rib -= nr;
                    stack.inc() = i;
                }
                ( di.facet1 == d.facet1 ? di.facet1 : di.facet2 ) = d.facet2;
            }
            i = dinx[b1];
            if ( i < nr )
            {
                TriDiag<void> & di = diag[i];
                if ( di.rib >= nr )
                {
                    di.rib -= nr;
                    stack.inc() = i;
                }
            }
            i = dinx[c2];
            if ( i < nr )
            {
                TriDiag<void> & di = diag[i];
                if ( di.rib >= nr )
                {
                    di.rib -= nr;
                    stack.inc() = i;
                }
                ( di.facet1 == d.facet2 ? di.facet1 : di.facet2 ) = d.facet1;
            }
            i = dinx[b2];
            if ( i < nr )
            {
                TriDiag<void> & di = diag[i];
                if ( di.rib >= nr )
                {
                    di.rib -= nr;
                    stack.inc() = i;
                }
            }
        }
        d.rib += nr;
    }
}

void trian3d_test ()
{
    drawNewList2d();
    trian2_test();
    endNewList ();
}