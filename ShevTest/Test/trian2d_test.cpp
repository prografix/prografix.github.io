
#include "math.h"

#include <afx.h> 
#define new DEBUG_NEW

#include "../Shev/template.h"
#include "../Shev/trian2t.h"
#include "../Shev/trian2d.h"
#include "../Shev/func1t.h"
#include "../Shev/func2t.h"
#include "../Shev/func2d.h"
#include "../Shev/opti2d.h"
#include "../Shev/Vector2d.h"
#include "../Shev/RealFile.h"
#include "../Shev/ShevList.h"
#include "../Shev/ShevArray.h"
#include "../draw.h"

#include "display.h"

double timeInSec();

namespace Test
{

bool testSemiRib3 ( const nat nr, const SemiRib * rib )
{
    if ( nr < 3 || nr % 3 != 0 ) return false;
    nat i, k = 0;
    for ( i = 0; i < nr; ++i )
    {
        const SemiRib & a = rib[i];
        if ( a.next >= nr )
        {
            break;
        }
        const SemiRib & b = rib[a.next];
        if ( b.next >= nr )
        {
            break;
        }
        const SemiRib & c = rib[b.next];
        if ( c.next != i )
        {
            break;
        }
        if ( a.twin < nr )
        {
            ++k;
            if ( rib[a.twin].vert != b.vert )
            break;
        }
        if ( b.twin < nr )
        {
            ++k;
            if ( rib[b.twin].vert != c.vert )
            break;
        }
        if ( c.twin < nr )
        {
            ++k;
            if ( rib[c.twin].vert != a.vert )
            break;
        }
    }
    return i == nr && k + 3 == nr;
};

/*
bool trianTest ( const List1u & list, const SemiRib * rib )
{
    bool ok = true;
    Show<Item1u> show ( list );
    if ( show.top() )
    do
    {
        const Item1u * item = show.cur();
        const nat a1 = item->d;
        const nat b1 = rib[a1].next;
        const nat c1 = rib[b1].next;
        const nat a2 = rib[a1].twin;
        const nat b2 = rib[a2].next;
        const nat c2 = rib[b2].next;
        if ( rib[a1].vert != rib[b2].vert ||
             rib[a2].vert != rib[b1].vert )
        {
            ok = false;
        }
        if ( rib[a1].facet != rib[b1].facet || rib[c1].facet != rib[b1].facet )
        {
            ok = false;
        }
        if ( rib[a2].facet != rib[b2].facet || rib[c2].facet != rib[b2].facet )
        {
            ok = false;
        }
    }
    while ( show.next() );
    return ok;
};

bool testSemiRib3 ( const nat nr, const SemiRib * rib )
{
    if ( nr < 3 || nr % 3 != 0 ) return false;
    nat i, k = 0;
    for ( i = 0; i < nr; ++i )
    {
        const SemiRib & a = rib[i];
        if ( a.next >= nr )
        {
            break;
        }
        const SemiRib & b = rib[a.next];
        if ( b.next >= nr )
        {
            break;
        }
        const SemiRib & c = rib[b.next];
        if ( c.next != i )
        {
            break;
        }
        if ( a.twin < nr )
        {
            ++k;
            if ( rib[a.twin].vert != b.vert )
            break;
        }
        if ( b.twin < nr )
        {
            ++k;
            if ( rib[b.twin].vert != c.vert )
            break;
        }
        if ( c.twin < nr )
        {
            ++k;
            if ( rib[c.twin].vert != a.vert )
            break;
        }
    }
    return i == nr && k + 3 == nr;
};
*/

#define MAX_MIN

void trian_test1()
{
    const nat n = 14;
    FixArray<Vector2d, n> buf;
    Suite<Set3<nat> > res1 ( n - 2 );
    Suite<Set3<nat> > res2 ( n - 2 );
#ifdef MAX_MIN
//    const TQ_Area<double, Vector2d> quality ( vert );
    const TQ_MinAngle<double, Vector2d> quality ( buf );
#else
    const TQ_AdivR2<double, Vector2d> quality ( buf );
#endif
    for ( nat nv = 4; nv <= n; ++nv )
    {
        ArrRef<Vector2d> vert ( buf, 0, nv );
        double time1 = 0;
        double time2 = 0;
        for ( nat j = 0; j < 10; ++j )
        {
            randPolygon ( vert );
//if(nv < 7 ) continue;
//if(j < 1 ) continue;
            if(1)
            {
                if ( ! trianTestNat1L1Area ( vert ) )
                {
                    trianTestNat1L1Area ( vert );
                }
                if ( ! trianTestNat1L1AdivR2 ( vert ) )
                {
                    trianTestNat1L1AdivR2 ( vert );
                }
                if ( ! trianTestNat1L1MinTan ( vert ) )
                {
                    trianTestNat1L1MinTan ( vert );
                }
            }
            double t1 = timeInSec();
#ifdef MAX_MIN
            nat k1 = trianNat1L1MaxMinArea ( vert, res1 ).size();
//            nat k1 = trianNat2L1MaxMinAngle ( nv, vert, res1 );
#else
            nat k1 = trian2 ( nv, vert, (Set3<int>*) res1 );
#endif
            time1 += timeInSec() - t1;
            double t2 = timeInSec();
#ifdef MAX_MIN
//            nat k2 = trianG1MaxMinArea ( nv, vert, res2 );
            nat k2 = trianG1MaxMinTan ( vert, res2 ).size();
#else
            nat k2 = trianG1MaxSumAdivR2 ( vert, res2 );
#endif
            time2 += timeInSec() - t2;
            if ( k1 != k2 )
                display << "k1 != k2" << NL;
            double sum = 0, sum1 = 0, sum2 = 0, min1 = 2, min2 = 2, 
                area = tempArea2<double, Vector2d> ( vert );
            for ( nat i = 0; i < k2; ++i )
            {
                const Vector2d & a = vert[res1[i].a];
                const Vector2d & b = vert[res1[i].b];
                const Vector2d & c = vert[res1[i].c];
                double s = ( b - a ) % ( c - b );
                if ( s <= 0 )
                {
                    double t = s;
                }
                sum += s;
                double t1 = quality ( res1[i].a, res1[i].b, res1[i].c );
                double t2 = quality ( res2[i].a, res2[i].b, res2[i].c );
                sum1 += t1;
                sum2 += t2;
                if ( min1 > t1 ) min1 = t1;
                if ( min2 > t2 ) min2 = t2;
            }
            if ( fabs ( sum - area ) > 1e-15 )
            {
                double t = area - sum;
            }
            if ( fabs ( sum1 - sum2 ) > 1e-15 )
            {
                double t = sum1 - sum2;
            }
            if ( fabs ( min1 - min2 ) > 1e-15 )
            {
                double t = min1 - min2;/*
                    for ( nat i = 0; i < k1; ++i )
                    {
                double t1 = quality ( res1[i].a, res1[i].b, res1[i].c );
                double t2 = quality ( res2[i].a, res2[i].b, res2[i].c );
                            const Set3<nat> & t = res2[i];
                            display << vert[t.a] << NL;
                            display << vert[t.b] << NL;
                            display << vert[t.c] << NL;
                            display << vert[t.a] << NL << NL;
                    }*/
            }
        }
        display << nv << time1 << time2 << NL;
    }
}

void trian_test2()
{
    const char * filename = "data/cont 99.txt";
    RealFile file ( filename, "r" );
    if ( ! file.isValid() )
    {
        display << "Not open file" << filename << NL;
        return;
    }
    nat32 nv;
    int n1=0, n2=0;
    Suite<Vector2d> vert;
    Suite<Set3<nat> > res1, res2;
    while ( readIntDec ( file, nv ) )
    {
        vert.resize ( nv );
        int32 x, y;
        nat32 i;
        for ( i = 0; i < nv; ++i )
        {
            if ( ! readIntDec ( file, x ) || ! readIntDec ( file, y ) ) break;
            vert[i].x = x;
            vert[i].y = y;
        }
        if ( i == nv && nv < 400 )
        {
            if(1)
            {
                if ( ! trianTestNat1L1Area ( vert ) )
                {
                    trianTestNat1L1Area ( vert );
                }
                if ( ! trianTestNat1L1AdivR2 ( vert ) )
                {
                    trianTestNat1L1AdivR2 ( vert );
                }
                if ( ! trianTestNat1L1MinTan ( vert ) )
                {
                    trianTestNat1L1MinTan ( vert );
                }
            }
            double t1 = timeInSec();
//            nat k1 = trianNat1L1MaxMinArea ( nv, vert, res1 );
//            nat k1 = trianNat1L1MaxMinAdivR2 ( nv, vert, res1 );
//            nat k1 = trianNat1L1MaxSumAdivR2 ( nv, vert, res1 );
            nat k1 = trianNat1L1MaxSumMinAngle ( vert, res1 ).size();
//            nat k1 = trianNat1L1MaxMinTan ( nv, vert, res1 );
//            nat k1 = trian2 ( nv, vert, (Set3<int> *) res1 );
            double time1 = timeInSec() - t1;
            double t2 = timeInSec();
//            nat k2 = trianG1MaxMinArea ( nv, vert, res2 );
//            nat k2 = trianG1MaxSumAdivR2 ( nv, vert, res2 );
//            nat k2 = trianG1MaxSumAdivR2 ( nv, vert, res2 );
            nat k2 = trianG1MaxSumMinAngle ( vert, res2 ).size();
            double time2 = timeInSec() - t2;
//            const TQ_Area<double, Vector2d> quality ( vert );
//            const TQ_AdivR2<double, Vector2d> quality ( vert );
            const TQ_MinAngle<double, Vector2d> quality ( vert );
            double max1 = 1e9, max2 = 1e9, sum1 = 0, sum2 = 0, sum = 0, 
                area = tempArea2<double, Vector2d> ( vert );
            for ( i = 0; i < k2; ++i )
            {
                const Vector2d & a = vert[res1[i].a];
                const Vector2d & b = vert[res1[i].b];
                const Vector2d & c = vert[res1[i].c];
                double s = ( b - a ) % ( c - b );
                if ( s <= 0 )
                {
                    double t = s;
                }
                sum += s;
                double t1 = quality ( res1[i].a, res1[i].b, res1[i].c );
                double t2 = quality ( res2[i].a, res2[i].b, res2[i].c );
                if ( max1 > t1 ) max1 = t1;
                if ( max2 > t2 ) max2 = t2;
                sum1 += t1;
                sum2 += t2;
            }
            if ( fabs ( sum - area ) > 1e-15 )
            {
                double t = area - sum;
            }
            if ( fabs ( max1 - max2 ) > 1e-15 )
            {
//                display << (int)nv << max1 << max2 << max2 - max1 << NL;
                double t = max1 - max2;/*
                ok = false;/*
                {
                    trianNat1L1MaxMinArea ( nv, vert, res1 );
                    for ( nat i = 0; i < k1; ++i )
                    {
                            const Set3<nat> & t = res2[i];
                            display << vert[t.a] << NL;
                            display << vert[t.b] << NL;
                            display << vert[t.c] << NL;
                            display << vert[t.a] << NL << NL;
                    }
                }*/
//                ++n2;
            }
            if ( fabs ( sum1 - sum2 ) > 9e-15 )
            {
                double t = sum1 - sum2;
                ++n2;
//                trianNat1L1MaxSumMinAngle ( nv, vert, res1 );
//                display << (int)nv << sum1 << sum2 << sum2 - sum1 << NL;
                if(0)
                {
                    for ( nat i = 0; i < k1; ++i )
                    {
                            const Set3<nat> & t = res2[i];
                            display << vert[t.a] << NL;
                            display << vert[t.b] << NL;
                            display << vert[t.c] << NL;
                            display << vert[t.a] << NL << NL;
                    }
                    return;
                }
            }
            ++n1;
//            if(!ok) display << (int)nv << max1 << max2 << NL;
            display << (int)nv << time2 << time1 << NL;
        }
    }
    display << n1-n2 << n2 << NL;
}

void trian_test3()
{
    display << "trian_test" << NL;
    const nat nv = 5, m = 100;
    Suite<Set3<nat> > res1 ( nv-2 ), res2 ( nv-2 );
    for ( nat j = 0; j < m; ++j )
    {
        FixArray<Vector2d, nv> vert;
        randPolygon ( vert );
        if ( j == m )
        {
            for ( nat i = 0; i < nv; ++i )
                display << vert[i] << NL;
        }
        const TQ_AdivR2<double, Vector2d> quality ( vert );
        nat k1 = trianNat1L1MaxMinAdivR2 ( vert, res1 ).size();
        nat k2 = trianG1MaxSumAdivR2 ( vert, res2 ).size();
        if ( k1 == k2 )
        {
            double sum1 = 0.;
            double sum2 = 0.;
            for ( nat i = 0; i < k1; ++i )
            {
                sum1 += quality ( res1[i].a, res1[i].b, res1[i].c );
                sum2 += quality ( res2[i].a, res2[i].b, res2[i].c );
                if ( j == 3575 )
                {
//                    display << quality ( res1[i] ) << quality ( res2[i] ) << NL;
//                    display << res1[i].a << res1[i].b << res1[i].c << res2[i].a << res2[i].b << res2[i].c << NL;
/*                    const Set3<nat> & t = res2[i];
                    display << vert[t.a] << NL;
                    display << vert[t.b] << NL;
                    display << vert[t.c] << NL;
                    display << vert[t.a] << NL << NL;*/
                }
            }
            if ( fabs ( sum1 - sum2 ) > 1e-15 )
            {
                display << j << sum2 - sum1 << NL;
            }
            else
            {
//                display << j << NL;
            }
        }
        else
            display << "k1 != k2" << NL;
    }
    display << "end" << NL;
}

void convexParts1 ( CCArrRef<Vector2d> & poly, List< ListItem<ShevList> > & res )
{
    nat i;
    ListItem<ShevList> * list = new ListItem<ShevList>;
    for ( i = 0; i < poly.size(); ++i ) list->addAftCur ( new ShevItem ( i ) );
    List< ListItem<ShevList> > llist;
    llist.addAftCur ( list );
    if ( poly.size() < 4 )
    {
        swap ( llist, res );
        return;
    }
    res.delAll();
    const Min2a<double> merge;
    const TQ_MinTan<double, Vector2d> quality ( poly );
    Suite< Set3<nat> > trip ( poly.size() - 2 );
    tempTrianNat1L1<double, Vector2d, double> ( quality, merge, poly, trip );
    DynArray<ShevList> diag ( poly.size() );
    for ( i = 0; i < trip.size(); ++i )
    {
        const Set3<nat> & t = trip[i];
        if ( t.b > t.a + 1 )
        {
            diag[t.a].addAftCur ( new ShevItem ( t.b ) );
            diag[t.b].addAftCur ( new ShevItem ( t.a ) );
        }
        if ( t.c > t.b + 1 )
        {
            diag[t.b].addAftCur ( new ShevItem ( t.c ) );
            diag[t.c].addAftCur ( new ShevItem ( t.b ) );
        }
        if ( t.a > t.c + 1 )
        {
            diag[t.c].addAftCur ( new ShevItem ( t.a ) );
            diag[t.a].addAftCur ( new ShevItem ( t.c ) );
        }
    }
    while ( llist.size() > 0 )
    {
m1:     ShevList * list = llist.cur();
        if ( list->size() > 3 )
        {
            list->top();
            do
            {
                ShevItem * pre = list->cprev();
                ShevItem * cur = list->cnext();
                ShevItem * nex = list->cnext();
                list->cprev();
                if ( ( poly[pre->info] - poly[cur->info] ) %
                     ( poly[nex->info] - poly[cur->info] ) < 0 ) continue;
                ShevList & sla = diag[cur->info];
                if ( sla.top() == 0 )
                    continue;
                ShevItem * ita = 0;
                if ( sla.size() == 1 )
                {
                    ShevItem * it = sla.cur();
                    if ( list->fir ( Info == it->info ) != 0 )
                    {
                        ita = it;
                    }
                }
                else
                {
                    double min;
                    const Vector2d u ( ( poly[pre->info] - poly[cur->info] ).setNorm2() -
                                       ( poly[nex->info] - poly[cur->info] ).setNorm2() );
                    do
                    {
                        ShevItem * it = sla.cur();
                        if ( list->fir ( Info == it->info ) != 0 )
                        {
                            const Vector2d v ( ( poly[it->info] - poly[cur->info] ).setNorm2() );
                            const double d = fabs ( u * v );
                            if ( ita == 0 || min > d )
                            {
                                min = d;
                                ita = it;
                            }
                        }
                    }
                    while ( sla.next() );
                }
                if ( ita == 0 )
                    continue;
                sla.jump ( ita );
                ShevList & slb = diag[ita->info];
                if ( ! slb.findFir ( Info == cur->info ) )
                    continue;
                ShevItem * itb = slb.cur();
                list->makeCurFir();
                list->findLas ( Info == ita->info );
                ListItem<ShevList> * temp = new ListItem<ShevList>;
                list->movAftCurAftLas ( *temp );
                slb.movCurAftLas ( *temp );
                sla.movCurBefFir ( *temp );
                llist.addAftLas ( temp );
                goto m1;
            }
            while ( list->next() );
        }
        llist.movCurAftLas ( res );
    }
}

void convexParts_test ()
{
    const nat n = 9;
    FixArray<Vector2d,n> poly;
    nat i;
    for ( i = 0; i < 1; ++i ) randPolygon ( poly );
    List< ListItem<ShevList> > parts;
    convexParts1 ( poly, parts );
    nat n1 = 0;
    if ( parts.top() )
    do
    {
        ShevList * sl = parts.cur();
        if ( sl->top() )
        do
        {
            ShevItem * pre = sl->cprev();
            ShevItem * cur = sl->cnext();
            Segment2d seg;
            seg.a.x = poly[cur->info].x;
            seg.a.y = poly[cur->info].y;
            seg.b.x = poly[pre->info].x;
            seg.b.y = poly[pre->info].y;
            if ( cur->info > pre->info )
            {
                if ( cur->info - pre->info > 1 )
                {
                    draw ( seg, 1, 1, 0 );
                    ++n1;
                }
            }
            else
            {
                if ( n + cur->info - pre->info > 1 )
                {
                    draw ( seg, 1, 1, 0 );
                    ++n1;
                }
            }
        }
        while ( sl->next() );
    }
    while ( parts.next() );
    convexParts ( poly, parts );
    nat n2 = 0;
    if ( parts.top() )
    do
    {
        ShevList * sl = parts.cur();
        if ( sl->top() )
        do
        {
            ShevItem * pre = sl->cprev();
            ShevItem * cur = sl->cnext();
            Segment2d seg;
            seg.a.x = poly[cur->info].x;
            seg.a.y = poly[cur->info].y;
            seg.b.x = poly[pre->info].x;
            seg.b.y = poly[pre->info].y;
            if ( cur->info > pre->info )
            {
                if ( cur->info - pre->info > 1 )
                {
                    draw ( seg, 1, 0, 0 );
                    ++n2;
                }
            }
            else
            {
                if ( n + cur->info - pre->info > 1 )
                {
                    draw ( seg, 1, 0, 0 );
                    ++n2;
                }
            }
        }
        while ( sl->next() );
    }
    while ( parts.next() );
    drawPolygon ( poly, 0, 1, 1 );
    if ( n1 != n2 ) display << n1/2 << n2/2 << NL;
}

void convexParts_test2 ()
{
    FixArray<Vector2d, 12> buf;
    nat m1 = 0, m2 = 0;
    for ( nat n = 3; n < 13; ++n )
    {
        ArrRef<Vector2d> poly ( buf, 0, n );
        for ( nat i = 0; i < 100; ++i )
        {
            randPolygon ( poly );
            List< ListItem<ShevList> > parts;
            convexParts1 ( poly, parts );
            nat n1 = parts.size();
            convexParts ( poly, parts );
            nat n2 = parts.size();
/*            if ( n1 < n2 )
            {
                display << n1 << n2 << NL;
                if ( parts.top() )
                do
                {
                    ShevList * sl = parts.cur();
                    if ( sl->top() )
                    do
                    {
                        ShevItem * pre = sl->cprev();
                        ShevItem * cur = sl->cnext();
                        Segment2d seg;
                        seg.a.x = poly[cur->info].x;
                        seg.a.y = poly[cur->info].y;
                        seg.b.x = poly[pre->info].x;
                        seg.b.y = poly[pre->info].y;
                        if ( cur->info > pre->info )
                        {
                            if ( cur->info - pre->info > 1 )
                            {
                                draw ( seg, 1, 0, 0 );
                            }
                        }
                        else
                        {
                            if ( n + cur->info - pre->info > 1 )
                            {
                                draw ( seg, 1, 0, 0 );
                            }
                        }
                    }
                    while ( sl->next() );
                }
                while ( parts.next() );
                drawPolygon ( poly, 0, 1, 1 );
                return;
            }*/
            if ( n1 < n2 ) ++m1;
            if ( n1 > n2 ) ++m2;
        }
    }
    display << m1 << m2 << NL;
    display << "end" << NL;
}

void trianSeidel_test()
{
    const nat n = 9;
    Suite<Vector2d> vert;
    randPolygon ( vert.resize(n) );
    vert.reverse() *= 0.8;
    const double a = 0.9;
    vert.inc() = Vector2d (  a,  a );
    vert.inc() = Vector2d ( -a,  a );
    vert.inc() = Vector2d ( -a, -a );
    vert.inc() = Vector2d (  a, -a );
    FixArray<nat, 2> cntr;
    cntr[0] = n;
    cntr[1] = 4;
    Suite<Set3<nat> > res;
    trianSeidel ( cntr, vert, res );
    nat i;
    for ( i = 0; i < res.size(); ++i )
    {
        const Set3<nat> & s = res[i];
        draw ( Segment2d ( vert[s.a], vert[s.b] ), 1, 1, 0 );
        draw ( Segment2d ( vert[s.b], vert[s.c] ), 1, 1, 0 );
        draw ( Segment2d ( vert[s.c], vert[s.a] ), 1, 1, 0 );
    }
    const Vector2d * p = vert();
    for ( i = 0; i < cntr.size(); ++i )
    {
        drawPolygon ( CArrRef<Vector2d>(p, cntr[i]), 0, 1, 1 );
        p += cntr[i];
    }
}
/*
void drawPoly ( CCArrRef<Vector2d> & vertex, CCArrRef<ChainNode> & node, nat i )
{
    Suite<Vector2d> vert;
    vert.add ( vertex[node[i].index] );
    nat t = node[i].next;
    while ( t != i )
    {
        vert.add ( vertex[node[t].index] );
        t = node[t].next;
    }
    drawPolygon ( vert, 1, 0, 0 );
}

void putMessage ( const char * str )
{
    if ( str )
        display << str << NL;
}

void test ( CCArrRef<Vector2d> & vertex, CCArrRef<ChainNode> & node, List3n & sweepLine )
{
    for ( nat i = 0; i < node.size(); ++i )
    {
        const ChainNode & ni = node[i];
        if ( node[ni.next].prev != i ) putMessage("error");
        if ( node[ni.prev].next != i ) putMessage("error");
        if ( ni.segm && ni.segm->a != i && ni.segm->b != i ) putMessage("error");
    }
    putMessage ( sweepLine.test() );
    if ( sweepLine.top() )
    do
    {
        Item3n * p = sweepLine.cur();
        if ( node[p->a].segm != p ) putMessage("error");
        if ( node[p->b].segm != p ) putMessage("error");
        if ( vertex[node[p->a].index].x > vertex[node[node[p->a].next].index].x ) putMessage("error");
        if ( vertex[node[p->b].index].x < vertex[node[node[p->b].next].index].x ) putMessage("error");
        if ( vertex[node[p->a].index].x > vertex[node[p->c].index].x ) putMessage("error");
        if ( vertex[node[p->b].index].x < vertex[node[p->c].index].x ) putMessage("error");
    }
    while ( sweepLine.next() );
}
*/
class Vertex : public Vector2d
{
public:
    nat index, prev, next;
    int order;
};

class Polygon
{
public:
    nat size, vert;
};

class Interval : public ShevItem
{
public:
    nat v, e1, e2;
};

enum TypeEven { START_TYPE, BEND_TYPE, END_TYPE };

inline TypeEven typeEven ( CArrRef<Vertex> vertex, nat v )
{
    const Vertex & b = vertex[v];
    const Vertex & a = vertex[b.prev];
    const Vertex & c = vertex[b.next];
    if ( b.order < a.order && b.order < c.order ) return START_TYPE;
    if ( b.order > a.order && b.order > c.order ) return END_TYPE;
    return BEND_TYPE;
}

enum LocateType { BELOW, INTO, OVER };

LocateType locate ( Suite<Vertex> & vertex, nat v, List<Interval> & sweepLine )
{
    const Vector2d & c = vertex[v];
    if ( sweepLine.top() )
    do
    {
        const Interval * p = sweepLine.cur();
        const Vertex & a1 = vertex[p->e1];
        const Vertex & b1 = vertex[a1.next];
        if ( a1.x == b1.x )
        {
            if ( c.y < b1.y ) return BELOW;
        }
        else
        {
            if ( ( b1 - a1 ) % ( c - b1 ) < 0 ) return BELOW;
        }
        const Vertex & a2 = vertex[p->e2];
        const Vertex & b2 = vertex[a2.prev];
        if ( a2.x == b2.x )
        {
            if ( c.y <= b2.y ) return INTO;
        }
        else
        {
            if ( ( b2 - a2 ) % ( c - b2 ) <= 0 ) return INTO;
        }
    }
    while ( sweepLine.next() );
    return OVER;
}

void startTransition ( Suite<Vertex> & vertex, nat v, List<Interval> & sweepLine )
{
    Interval * p = new Interval;
    p->v = p->e1 = p->e2 = v;
    switch ( locate ( vertex, v, sweepLine ) )
    {
    case BELOW:
        sweepLine.addBefCur ( p ); break;
    case INTO:
        {
            nat a = vertex.size();
            Vertex & va = vertex.inc();
            nat b = vertex.size();
            Vertex & vb = vertex.inc();
            Interval * t = sweepLine.cur();
            Vertex & wa = vertex[v];
            Vertex & wb = vertex[t->v];
            va.x = wa.x;
            va.y = wa.y;
            va.index = wa.index;
            va.order = wa.order;
            vb.x = wb.x;
            vb.y = wb.y;
            vb.index = wb.index;
            vb.order = wb.order;
            va.next = wa.next;
            vertex[va.next].prev = vb.next = a;
            vb.prev = wb.prev;
            vertex[vb.prev].next = va.prev = b;
            wa.next = t->v;
            wb.prev = v;
            p->e1 = p->v = a;
            p->e2 = t->e2 == wa.next ? b : t->e2;
            t->e2 = t->v = v;
        }
    case OVER:
        sweepLine.addAftCur ( p ); break;
    }
}

void bendTransition ( Suite<Vertex> & vertex, nat v, List<Interval> & sweepLine )
{
    if ( sweepLine.top() )
    do
    {
        Interval * p = sweepLine.cur();
        if ( v == vertex[p->e1].next )
        {
            p->v = p->e1 = v;
            return;
        }
        if ( v == vertex[p->e2].prev )
        {
            p->v = p->e2 = v;
            return;
        }
    }
    while ( sweepLine.next() );
}

nat sizeOf ( CArrRef<Vertex> vertex, nat v )
{
    nat n = 1, t = vertex[v].next;
    while ( t != v )
    {
        t = vertex[t].next;
        ++n;
    }
    return n;
}

void endTransition ( Suite<Vertex> & vertex, nat v, List<Interval> & sweepLine, Suite<Polygon> & res )
{
    if ( sweepLine.top() )
    do
    {
        Interval * p = sweepLine.cur();
        if ( v == vertex[p->e1].next )
        {
            Polygon & poly = res.inc();
            poly.vert = v;
            poly.size = sizeOf ( vertex, v );
            sweepLine.delCur();
            return;
        }
        if ( v == vertex[p->e2].prev )
        {
            Interval * t = sweepLine.next();
            p->v = v;
            p->e2 = t->e2;
            sweepLine.delCur();
            return;
        }
    }
    while ( sweepLine.next() );
}

void semiregularize ( const Polygon & poly, Suite<Vertex> & vertex, Suite<Polygon> & res )
{
    DynArray<SortItem<int, nat> > schedule ( poly.size );
    nat i, v = poly.vert;
    for ( i = 0; i < poly.size; ++i )
    {
        SortItem<int, nat> & si = schedule[i];
        si.head = vertex[v].order;
        si.tail = v;
        v = vertex[v].next;
    }
    quickSort123 ( schedule );
    List<Interval> sweepLine;
    for ( i = 0; i < schedule.size(); ++i )
    {
        const nat v = schedule[i].tail;
        switch ( typeEven ( vertex, v ) )
        {
        case START_TYPE: startTransition ( vertex, v, sweepLine ); break;
        case BEND_TYPE: bendTransition ( vertex, v, sweepLine ); break;
        case END_TYPE: endTransition ( vertex, v, sweepLine, res ); break;
        }
    }
}

void regularize ( const Polygon & poly, Suite<Vertex> & vertex, Suite<Polygon> & res )
{
    if ( poly.size < 4 )
    {
        res.inc() = poly;
        return;
    }
    Suite<Polygon> temp;
    semiregularize ( poly, vertex, temp );
    nat i;
    vertex << NegXY();
    for ( i = 0; i < vertex.size(); ++i ) vertex[i].order *= -1;
    for ( i = 0; i < temp.size(); ++i )
    {
        semiregularize ( temp[i], vertex, res );
    }
}

void triangulate_monotone_polygons ( CArrRef<Polygon> monpolys, CArrRef<Vertex> vertex, SuiteRef<Set3<nat> > & res )
{
    Suite<nat> buf;
    for ( nat i = 0; i < monpolys.size(); ++i )
    {
        const Polygon & poly = monpolys[i];
        if ( poly.size == 3 )
	    {
            const Vertex & v = vertex[poly.vert];
            Set3<nat> & s = res.inc();
	        s.a = v.index;
	        s.b = vertex[v.next].index;
	        s.c = vertex[v.prev].index;
	    }
        else
	    {
            buf.resize();
            buf.inc() = poly.vert;
            nat prev = vertex[poly.vert].prev;
            nat next = vertex[poly.vert].next;
            if ( vertex[prev].order < vertex[next].order )
            {
                buf.inc() = next;
                next = vertex[next].next;
            }
            else
            {
                buf.inc() = prev;
                prev = vertex[prev].prev;
            }
            nat v;
            for ( nat i = 3; i < poly.size; ++i )
            {
                if ( vertex[prev].order < vertex[next].order )
                {
                    v = next;
                    next = vertex[next].next;
                }
                else
                {
                    v = prev;
                    prev = vertex[prev].prev;
                }
                if ( vertex[v].prev == buf[0] || vertex[v].next == buf[0] )
                {
                    const nat a = buf.las();
                    while ( buf.size() > 1 )
                    {
                        const nat i0 = buf.las(0);
                        const nat i1 = buf.las(1);
                        Set3<nat> & s = res.inc();
                        if ( vertex[v].prev == buf[0] )
                        {
	                        s.a = vertex[i0].index;
	                        s.b = vertex[i1].index;
                        }
                        else
                        {
	                        s.a = vertex[i1].index;
	                        s.b = vertex[i0].index;
                        }
	                    s.c = vertex[v].index;
                        buf.dec();
                    }
                    buf[0] = a;
                }
                else
                {
                    const Vector2d & v2 = vertex[v];
                    const bool order = vertex[v].next == buf.las();
                    while ( buf.size() > 1 )
                    {
                        const nat i0 = buf.las(0);
                        const nat i1 = buf.las(1);
                        const Vector2d & v0 = vertex[i0];
                        const Vector2d & v1 = vertex[i1];
                        const double t = ( v1 - v0 ) % ( v2 - v1 );
                        if ( t != 0 && ( t < 0 == order ) ) break;
                        Set3<nat> & s = res.inc();
                        if ( order )
                        {
	                        s.a = vertex[i0].index;
	                        s.b = vertex[i1].index;
                        }
                        else
                        {
	                        s.a = vertex[i1].index;
	                        s.b = vertex[i0].index;
                        }
	                    s.c = vertex[v].index;
                        buf.dec();
                    }
                }
                buf.inc() = v;
            }
            if ( vertex[prev].order < vertex[next].order )
            {
                v = next;
                next = vertex[next].next;
            }
            else
            {
                v = prev;
                prev = vertex[prev].prev;
            }
            while ( buf.size() > 1 )
            {
                const nat i0 = buf.las(0);
                const nat i1 = buf.las(1);
                Set3<nat> & s = res.inc();
                if ( vertex[v].prev == buf[0] )
                {
	                s.a = vertex[i0].index;
	                s.b = vertex[i1].index;
                }
                else
                {
	                s.a = vertex[i1].index;
	                s.b = vertex[i0].index;
                }
	            s.c = vertex[v].index;
                buf.dec();
            }
        }
    }
}

SuiteRef<Set3<nat> > & trianSweepLine ( CArrRef<Vector2d> vert, SuiteRef<Set3<nat> > & res )
{
    nat i;
    res.resize();
    const nat nv = vert.size();
    if ( nv < 3 ) return res;
    Suite<Vertex> vertex ( nv, nv );
    DynArray<SortItem<double, nat> > schedule ( nv );
    for ( i = 0; i < nv; ++i )
    {
        const Vector2d v = vert[i];
        Vertex & vi = vertex[i];
        vi.x = v.x;
        vi.y = v.y;
        vi.next = i + 1;
        vi.prev = i - 1;
        vi.index = i;
        SortItem<double, nat> & si = schedule[i];
        si.head = v.x;
        si.tail = i;
    }
    vertex[0].prev = nv - 1;
    vertex.las().next = 0;
    quickSort123 ( schedule );
    for ( i = 0; i < vertex.size(); ++i )
    {
        vertex[schedule[i].tail].order = i;
    }
// ����� ���������� ��������������
    Polygon poly;
    poly.vert = 0;
    poly.size = nv;
    Suite<Polygon> temp;
    regularize ( poly, vertex, temp );
// ������������ ���������� ���������������
    triangulate_monotone_polygons ( temp, vertex, res );
    return res;
}

void trianSweepLine_test()
{
    nat i;
    const nat n = 7;
    Suite<Vector2d> vert;
    for ( i = 0; i < 1; ++i ) randPolygon ( vert.resize(n) );
    vert.reverse();
    Def<Segment2d> s = dimensions ( vert );
    vert -= 0.5 * ( s.a + s.b );
//for ( i = 0; i < vert.size(); ++i ) display << vert[i] << NL;
    FixArray<nat, 2> cntr;
    cntr[0] = vert.size();
    cntr[1] = 4;
    FixArray<Vector2d, 4> quad;
    quad[0].x = -1;
    quad[0].y = -1;
    quad[1].x =  1;
    quad[1].y = -1;
    quad[2].x =  1;
    quad[2].y =  1;
    quad[3].x = -1;
    quad[3].y =  1;
    vert.addAftLas ( quad );
    vert *= 0.9;
    Suite<Set3<nat> > res;
    ::trianSweepLine ( cntr, vert, res );
    if(1)
    {
        for ( i = 0; i < res.size(); ++i )
        {
            const Set3<nat> & s = res[i];
            draw ( Segment2d ( vert[s.a], vert[s.b] ), 0.5f, 0.5f, 0.5f );
            draw ( Segment2d ( vert[s.b], vert[s.c] ), 0.5f, 0.5f, 0.5f );
            draw ( Segment2d ( vert[s.c], vert[s.a] ), 0.5f, 0.5f, 0.5f );
            double a = ( vert[s.a] - vert[s.b] ) % ( vert[s.b] - vert[s.c] );
if ( a < 0 ) display << a << NL;
        }
    }
    rebuildDelauney ( vert, res );
    if(1)
    {
        for ( i = 0; i < res.size(); ++i )
        {
            const Set3<nat> & s = res[i];
            draw ( Segment2d ( vert[s.a], vert[s.b] ), 1, 1, 0 );
            draw ( Segment2d ( vert[s.b], vert[s.c] ), 1, 1, 0 );
            draw ( Segment2d ( vert[s.c], vert[s.a] ), 1, 1, 0 );
            double a = ( vert[s.a] - vert[s.b] ) % ( vert[s.b] - vert[s.c] );
if ( a < 0 ) display << a << NL;
        }
    }
    /*Def<Circle2d> c = maxCircleInPolygon ( vert );
    vert -= c.o;
    vert *= 1.0;
    vert += c.o;
    drawPolygon ( vert, 0, 1, 1 );*/
    nat k = 0;
    for ( i = 0; i < cntr.size(); ++i )
    {
        drawPolygon ( CArrRef<Vector2d> ( vert, k, cntr[i] ), 0, 1, 1 );
        k += cntr[i];
    }
}

void trianSweepLineN_test()
{
    nat i;
    const nat n = 9;
    Suite<Vector2d> vert;
    FixArray<nat, 1> cntr;
    for ( i = 0; i < 1000; ++i )
    {
        randPolygon ( vert.resize(n) );
        cntr[0] = n;
        Suite<Set3<nat> > res;
        ::trianSweepLine ( cntr, vert, res );
    }
display << "end" << NL;
}

void trianSweepLine_test2()
{
    nat i;
    const nat n = 8;
    Suite<Vector2d> vert;
    randPolygon ( vert.resize(n) );
#if 0
    vert.reverse() *= 0.8;
    const double a = 0.9;
    vert.inc() = Vector2d (  a,  a );
    vert.inc() = Vector2d ( -a,  a );
    vert.inc() = Vector2d ( -a, -a );
    vert.inc() = Vector2d (  a, -a );
    FixArray<nat, 2> cntr;
    cntr[0] = n;
    cntr[1] = 4;
#else
    FixArray<nat, 1> cntr;
    cntr[0] = n;
#endif
    Suite<Set3<nat> > res;
    ::trianSweepLine ( cntr, vert, res );
    if(1)
    {
        for ( i = 0; i < res.size(); ++i )
        {
            const Set3<nat> & s = res[i];
            draw ( Segment2d ( vert[s.a], vert[s.b] ), 1, 1, 0 );
            draw ( Segment2d ( vert[s.b], vert[s.c] ), 1, 1, 0 );
            draw ( Segment2d ( vert[s.c], vert[s.a] ), 1, 1, 0 );
//display << ( vert[s.a] - vert[s.b] ) % ( vert[s.b] - vert[s.c] ) << NL;
        }
        const Vector2d * p = vert();
        for ( i = 0; i < cntr.size(); ++i )
        {
            drawPolygon ( CArrRef<Vector2d>(p, cntr[i]), 0, 1, 1 );
            p += cntr[i];
        }
    }
}

void trianSeidel_test2 ( const Spin2d & spin, const char * filename )
{
    RealFile file ( filename, "r" );
    if ( ! file.isValid() )
    {
        display << "Not open file" << filename << NL;
        return;
    }
    nat32 nv;
    int n1=0, n2=0;
    Suite<SortItem<nat, double> > stat;
    Suite<Vector2d> vert;
    Suite<Set3<nat> > res;
    List< ListItem<ShevList> > plist;
    while ( readIntDec ( file, nv ) )
    {
        vert.resize ( nv );
        int32 x, y;
        nat32 i;
        for ( i = 0; i < nv; ++i )
        {
            if ( ! readIntDec ( file, x ) || ! readIntDec ( file, y ) ) break;
            vert[i].x = x;
            vert[i].y = y;
        }
        if ( i == nv )
        {
//        if ( nv > 60 ) continue;
        vert *= spin;
//display << nv << area ( vert ) << NL;
            DynArray<SemiRib> rib ( 3 * nv - 6 );
//            trianSeidel ( vert, res );
//            ::trianSweepLine ( vert, res );
            double t1 = timeInSec();
//            rebuildDelauney ( vert, res );
            convexParts ( vert, plist );
//            ::trianSweepLine ( vert, res );
//            tempTrianNat1<double, Vector2d> ( vert, rib );
//            trianNat1L1MaxMinTan ( vert, res );
            double t2 = timeInSec();
            stat.inc() = SortItem<nat, double> ( nv, t2-t1 );
            /*double max1 = 1e9, max2 = 1e9, sum1 = 0, sum2 = 0, sum = 0, 
                area = tempArea2<double, Vector2d> ( vert );
            for ( i = 0; i < res.size(); ++i )
            {
                const Vector2d & a = vert[res[i].a];
                const Vector2d & b = vert[res[i].b];
                const Vector2d & c = vert[res[i].c];
                double s = ( b - a ) % ( c - b );
                if ( s < 0 )
                {
                    display << nv << s << "<= 0" << NL;
                    ::trianSweepLine ( cntr, vert, res );
//            for ( nat j = 0; j < nv; ++j ) display << vert[j] << NL;
                    /*if(1)
                    {
                        Vector2d v = amean ( *vert );
                        vert -= v;
                        if(0)
                        for ( i = 0; i < res.size(); ++i )
                        {
                            const Set3<nat> & s = res[i];
                            draw ( Segment2d ( vert[s.a], vert[s.b] ), 1, 1, 0 );
                            draw ( Segment2d ( vert[s.b], vert[s.c] ), 1, 1, 0 );
                            draw ( Segment2d ( vert[s.c], vert[s.a] ), 1, 1, 0 );
                //display << ( vert[s.a] - vert[s.b] ) % ( vert[s.b] - vert[s.c] ) << NL;
                        }
                        const Vector2d * p = vert();
                        for ( i = 0; i < cntr.size(); ++i )
                        {
                            drawPolygon ( CArrRef<Vector2d>(p, cntr[i]), 0, 1, 1 );
                            p += cntr[i];
                        }
                        drawPoint ( a, 1, 0, 0 );
                        drawPoint ( b, 1, 0, 0 );
                        drawPoint ( c, 1, 0, 0 );
                    }*
                    return;
                }
                sum += s;
            }
            if ( fabs ( sum - area ) > 1e-15 )
            {
                double t = area - sum;
            }*/
            ++n1;
        }
        //else
            //display << (int)nv << "error" << NL;
    }
    if ( stat.size() > 0 )
    {
        insertSort123 ( stat );
        nat32 i = 0, n = 1;
        double sum = stat[i].tail;
        for(;;)
        {
            nat32 i1 = i + 1;
            if ( i1 == stat.size() || stat[i].head != stat[i1].head )
            {
                display << stat[i].head << sum / n << NL;
                if ( i1 == stat.size() ) break;
                n = 0;
                sum = 0;
            }
            sum += stat[i1].tail;
            ++n;
            i = i1;
        }
    }
    display << "end" << NL;
}

void trianSeidel_test2()
{
    trianSeidel_test2 ( Spin2d()    , "data/cont 99.txt" );
    trianSeidel_test2 ( Spin2d::d090, "data/cont 99.txt" );
    trianSeidel_test2 ( Spin2d::d180, "data/cont 99.txt" );
    trianSeidel_test2 ( Spin2d::d270, "data/cont 99.txt" );
    trianSeidel_test2 ( Spin2d()    , "data/cont 707.txt" );
    trianSeidel_test2 ( Spin2d::d090, "data/cont 707.txt" );
    trianSeidel_test2 ( Spin2d::d180, "data/cont 707.txt" );
    trianSeidel_test2 ( Spin2d::d270, "data/cont 707.txt" );
}

} // namespace

using namespace Test;

void trian2d_test ()
{
    drawNewList2d();
//    trian_test2();
//    convexParts_test();
    trianSeidel_test2();
//    trianSweepLine_test();
    endNewList();
}