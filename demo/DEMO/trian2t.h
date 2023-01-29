
#pragma once

#include "atrian.h"

//**************************** 11.07.2007 *********************************//
//
//      Триангуляция многоугольника методом tempTrianNat1
//
//**************************** 27.08.2011 *********************************//

struct Node1
{
    nat next, info;
};

template <class Scalar, class Vector> 
nat tempTrianNat1 ( CCArrRef<Vector> & vert, ArrRef<SemiRib> & res )
{
    nat nv = vert.size();
    if ( nv <= 2 ) return 0;
    const nat nr = 3 * nv - 6;
    if ( nv == 3 )
    {
        SemiRib & ra = res[0];
        ra.next = 1;
        ra.twin = nr;
        ra.vert = 0;
        SemiRib & rb = res[1];
        rb.next = 2;
        rb.twin = nr;
        rb.vert = 1;
        SemiRib & rc = res[2];
        rc.next = 0;
        rc.twin = nr;
        rc.vert = 2;
        return nr;
    }
    DynArray<Node1> node ( nv );
    nat i;
    for ( i = 0; i < nv; ++i )
    {
        node[i].next = i + 1;
        node[i].info = nr;
    }
    node[nv-1].next = 0;
    nat k = 0, ip = 0;
// Строим неотрицательные треугольники с проверкой на пересечения с другими рёбрами
    while ( nv > 3 )
    {
        for ( i = 0; i < nv; ++i )
        {
            nat ic = node[ip].next;
            nat in = node[ic].next;
            const Vector & a = vert[ip];
            const Vector & b = vert[ic];
            const Vector & c = vert[in];
            const Vector ba ( b - a );
            const Vector cb ( c - b );
            Scalar a1 = ba % cb;
            if ( a1 < 0 || ( a1 == 0 && cb * ba > 0 ) )
            {
                ip = ic;
                continue;
            }
            if ( a1 > 0 )
            {
// Проверка на пересечения с другими рёбрами
                const Vector ac ( a - c );
                Scalar b1 = 0;
                Scalar c1 = 0;
                Vector v1 = c;
                nat j = node[in].next;
                do
                {
                    const Vector & v2 = vert[j];
                    const Vector va ( v2 - a );
                    const Vector vb ( v2 - b );
                    const Vector vc ( v2 - c );
                    const Scalar a2 = ba % vb;
                    const Scalar b2 = cb % vc;
                    const Scalar c2 = ac % va;
                    if ( ( c1 > 0 || c2 > 0 ) && ( b1 > 0 || b2 > 0 ) && ( a1 > 0 || a2 > 0 ) )
                    {
                        const Vector dv = v2 - v1;
                        const Scalar da = dv % va;
                        const Scalar db = dv % vb;
                        const Scalar dc = dv % vc;
                        Scalar min, max;
                        if ( da < db )
                        {
                            min = da;
                            max = db;
                        }
                        else
                        {
                            min = db;
                            max = da;
                        }
                        if ( min >= dc )
                        {
                            min = dc;
                        }
                        else
                        if ( max < dc )
                        {
                            max = dc;
                        }
                        if ( min < 0 && max > 0 ) break;
                    }
                    a1 = a2;
                    b1 = b2;
                    c1 = c2;
                    v1 = v2;
                    j = node[j].next;
                }
                while ( j != ic );
                if ( j != ic )
                {
                    ip = ic;
                    continue;
                }
            }
            const nat na = 3 * k;
            const nat nb = na + 1;
            const nat nc = nb + 1;
            SemiRib & ra = res[na];
            ra.next = nb;
            ra.twin = node[ip].info;
            ra.vert = ip;
            SemiRib & rb = res[nb];
            rb.next = nc;
            rb.twin = node[ic].info;
            rb.vert = ic;
            SemiRib & rc = res[nc];
            rc.next = na;
            rc.twin = nr;
            rc.vert = in;
            ++k;
            node[ip].next = in;
            node[ip].info = nc;
            --nv;
            goto m2;
        }
        break;
m2:;}
// Строим неотрицательные треугольники без проверки на пересечения с другими рёбрами
// Этот блок для простого многоугольника выполняться не должен
    while ( nv > 3 )
    {
        for ( i = 0; i < nv; ++i )
        {
            nat ic = node[ip].next;
            nat in = node[ic].next;
            const Vector & a = vert[ip];
            const Vector & b = vert[ic];
            const Vector & c = vert[in];
            const Vector ba ( b - a );
            const Vector cb ( c - b );
            const Scalar a1 = ba % cb;
            if ( a1 > 0 || ( a1 == 0 && cb * ba <= 0 ) )
            {
                const nat na = 3 * k;
                const nat nb = na + 1;
                const nat nc = nb + 1;
                SemiRib & ra = res[na];
                ra.next = nb;
                ra.twin = node[ip].info;
                ra.vert = ip;
                SemiRib & rb = res[nb];
                rb.next = nc;
                rb.twin = node[ic].info;
                rb.vert = ic;
                SemiRib & rc = res[nc];
                rc.next = na;
                rc.twin = nr;
                rc.vert = in;
                ++k;
                node[ip].next = in;
                node[ip].info = nc;
                --nv;
                goto m3;
            }
            ip = ic;
        }
        break;
m3:;}
// Строим произвольные треугольники
    while ( nv > 2 )
    {
        nat ic = node[ip].next;
        nat in = node[ic].next;
        const nat na = 3 * k;
        const nat nb = na + 1;
        const nat nc = nb + 1;
        SemiRib & ra = res[na];
        ra.next = nb;
        ra.twin = node[ip].info;
        ra.vert = ip;
        SemiRib & rb = res[nb];
        rb.next = nc;
        rb.twin = node[ic].info;
        rb.vert = ic;
        SemiRib & rc = res[nc];
        rc.next = na;
        rc.twin = nv == 3 ? node[in].info : nr;
        rc.vert = in;
        ++k;
        node[ip].next = in;
        node[ip].info = nc;
        --nv;
    }
    return nr;
}

//**************************** 18.07.2007 *********************************//
//
//      Триангуляция многоугольника алгоритмами tempTrianNat1 и maxL1
//
//**************************** 27.08.2011 *********************************//

template <class Scalar1, class Vector, class Scalar2> 
SuiteRef<Set3<nat> > & tempTrianNat1L1 ( const Func3a<Scalar2,nat> & quality,
                                         const Func2a<Scalar2,Scalar2> & merge, 
                                         CCArrRef<Vector> & vert, SuiteRef<Set3<nat> > & res )
{
    res.resize(0);
    const nat nv = vert.size();
    if ( nv <= 2 ) return res;
    const nat nr = 3 * nv - 6;
    DynArray<SemiRib> rib ( nr );
    tempTrianNat1<Scalar1, Vector> ( vert, rib );
    maxL1<Scalar2> ( quality, merge, rib );
    for ( nat i = 0; i < nr; ++i )
    {
        SemiRib & a = rib[i];
        if ( a.vert >= nv ) continue;
        SemiRib & b = rib[a.next];
        SemiRib & c = rib[b.next];
        Set3<nat> & t = res.inc();
        t.a = a.vert;
        t.b = b.vert;
        t.c = c.vert;
        a.vert = b.vert = c.vert = nv;
    }
    return res;
}

//**************************** 01.08.2007 *********************************//
//
//      Проверка на наличие триангуляции с положительными треугольниками
//
//**************************** 27.08.2011 *********************************//

template <class Scalar1, class Vector, class Scalar2> 
bool tempTrianTestNat1L1 ( const Func3a<Scalar2, nat> & quality, CCArrRef<Vector> & vert )
{
    const nat nv = vert.size();
    if ( nv <= 2 ) return false;
    const nat nr = 3 * nv - 6;
    DynArray<SemiRib> rib ( nr );
    tempTrianNat1<Scalar1, Vector> ( vert, rib );
    const Min2a<Scalar2> merge;
    maxL1<Scalar2> ( quality, merge, rib );
    for ( nat i = 0; i < nr; ++i )
    {
        SemiRib & a = rib[i];
        if ( a.vert >= nv ) continue;
        SemiRib & b = rib[a.next];
        SemiRib & c = rib[b.next];
        if ( quality ( a.vert, b.vert, c.vert ) <= 0 ) return false;
        a.vert = b.vert = c.vert = nv;
    }
    return true;
}


//*************************************************************************//
//
//                  Удвоенная площадь треугольника
//
//*************************************************************************//

template <class Scalar, class Vector> 
class TQ_Area : public Func3a<Scalar,nat>
{
    CArrRef<Vector> vert;
public:
    explicit TQ_Area ( CCArrRef<Vector> & v ) : vert(v) {}
    virtual Scalar operator () ( nat a, nat b, nat c ) const
    {
        return ( vert[a] - vert[c] ) % ( vert[b] - vert[a] );
    }
};

//*************************************************************************//
//
//  Отношение удвоенной площади треугольника к сумме квадратов длин сторон
//
//*************************************************************************//

template <class Scalar, class Vector> 
class TQ_AdivR2 : public Func3a<Scalar,nat>
{
    CArrRef<Vector> vert;
public:
    explicit TQ_AdivR2 ( CCArrRef<Vector> & v ) : vert(v) {}
    virtual Scalar operator () ( nat a, nat b, nat c ) const
    {
        const Vector ab = vert[b] - vert[a];
        const Vector bc = vert[c] - vert[b];
        const Vector ca = vert[a] - vert[c];
        const Scalar p ( ab * ab + bc * bc + ca * ca );
        return p > 0 ? Scalar ( ca % ab ) / p : 0;
    }
};

//**************************** 17.09.2007 *********************************//
//
//                  Минимальный угол треугольника
//
//**************************** 17.09.2007 *********************************//

template <class Scalar, class Vector> 
class TQ_MinAngle : public Func3a<Scalar,nat>
{
    CArrRef<Vector> vert;
public:
    explicit TQ_MinAngle ( CCArrRef<Vector> & v ) : vert(v) {}
    virtual Scalar operator () ( nat a, nat b, nat c ) const
    {
        const Vector ab = vert[b] - vert[a];
        const Vector bc = vert[c] - vert[b];
        const Vector ca = vert[a] - vert[c];
        const Scalar aa = ca * ab;
        const Scalar bb = ab * bc;
        const Scalar cc = bc * ca;
        double min = 0.;
        if ( aa <= bb )
        {
            if ( aa <= cc )
            {
                if ( aa < 0 ) min = ( ca % bc ) / aa;
            }
            else
            {
                if ( cc < 0 ) min = ( bc % ab ) / cc;
            }
        }
        else
        {
            if ( bb <= cc )
            {
                if ( bb < 0 ) min = ( ca % bc ) / bb;
            }
            else
            {
                if ( cc < 0 ) min = ( ab % ca ) / cc;
            }
        }
        return Scalar ( atan ( min ) );
    }
};

//**************************** 17.09.2007 *********************************//
//
//              Минимальный тангенс угла треугольника
//
//**************************** 17.09.2007 *********************************//

template <class Scalar, class Vector> 
class TQ_MinTan : public Func3a<Scalar,nat>
{
    CArrRef<Vector> vert;
public:
    explicit TQ_MinTan ( CCArrRef<Vector> & v ) : vert(v) {}
    virtual Scalar operator () ( nat a, nat b, nat c ) const
    {
        const Vector ab = vert[b] - vert[a];
        const Vector bc = vert[c] - vert[b];
        const Vector ca = vert[a] - vert[c];
        const Scalar aa = ca * ab;
        const Scalar bb = ab * bc;
        const Scalar cc = bc * ca;
        if ( aa <= bb )
        {
            if ( aa <= cc )
            {
                if ( aa < 0 ) return ( ca % bc ) / aa;
            }
            else
            {
                if ( cc < 0 ) return ( bc % ab ) / cc;
            }
        }
        else
        {
            if ( bb <= cc )
            {
                if ( bb < 0 ) return ( ca % bc ) / bb;
            }
            else
            {
                if ( cc < 0 ) return ( ab % ca ) / cc;
            }
        }
        return 0;
    }
};
