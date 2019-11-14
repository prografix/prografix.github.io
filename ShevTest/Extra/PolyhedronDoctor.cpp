
#include "math.h"

#include "../shev/template.h"
#include "../shev/ShevList.h"
#include "../shev/Vector2d.h"
#include "../shev/Vector3d.h"
#include "../shev/ShevArray.h"
#include "../shev/func1t.h"
#include "Micro.h"
#include "../shev/Trans.h"
#include "../shev/lists.h"
#include "../shev/trian3d.h"
#include "PolyhedronDoctor.h"
#include "../shev/approx3d.h"
#include "../shev/trian2d.h"

bool correctNullSides ( ArrRef<Set4<nat, nat, nat, Plane3d> > triangle, const Vector3d * vertex )
{
    double max = 0;
    nat im = 0;
    for ( nat i = 0; i < triangle.size(); ++i  )
    {
        Set4<nat, nat, nat, Plane3d> & t = triangle[i];
        const Vector3d & v0 = vertex[t.a];
        const Vector3d & v1 = vertex[t.b];
        const Vector3d & v2 = vertex[t.c];
        const double d01 = qmod ( v0 - v1 );
        const double d12 = qmod ( v1 - v2 );
        const double d20 = qmod ( v2 - v0 );
        if ( d01 >= d12 && d01 >= d20 )
        {
            double f = ( d01 - d12 - d20 ) / sqrt ( d12 * d20 );
            if ( max < f ) max = f, im = i;
            if ( d01 - d12 - d20 < ( 2 - 1e-4 ) * sqrt ( d12 * d20 ) ) continue;
            i=i;
        }
        else
        if ( d12 >= d20 )
        {
            double f = ( d12 - d20 - d01 ) / sqrt ( d20 * d01 );
            if ( max < f ) max = f, im = i;
            if ( d12 - d20 - d01 < ( 2 - 1e-4 ) * sqrt ( d20 * d01 ) ) continue;
            i=i;
        }
        else
        {
            double f = (  d20 - d01 - d12 ) / sqrt ( d01 * d12 );
            if ( max < f ) max = f, im = i;
            if ( d20 - d01 - d12 < ( 2 - 1e-4 ) * sqrt ( d01 * d12 ) ) continue;
            i=i;
        }
    }
    {
        Set4<nat, nat, nat, Plane3d> & t = triangle[im];
        const Vector3d & v0 = vertex[t.a];
        const Vector3d & v1 = vertex[t.b];
        const Vector3d & v2 = vertex[t.c];
        const double d01 = qmod ( v0 - v1 );
        const double d12 = qmod ( v1 - v2 );
        const double d20 = qmod ( v2 - v0 );
        double o = norm2 ( ( v0 - v1 ) % ( v0 - v2 ) );
        im = im;
    }
    return false;
}

// ѕроверка и исправление плоскости грани

inline double maxDev (  const Plane3d & plane, const Vector3d & a, const Vector3d & b, const Vector3d & c )
{
    return _max ( fabs ( plane % a ), fabs ( plane % b ), fabs ( plane % c ) );
}

double maxDev ( const Vector3d * vertex, CArrRef<nat> index, const Plane3d & plane )
{
    double max = 0;
    for ( nat i = 0; i < index.size(); ++i )
    {
        const double t = fabs ( plane % vertex[index[i]] );
        if ( max < t ) max = t;
    }
    return max;
}

bool correctPlane ( Set2<CmbArray<nat, 3>, Plane3d> & s, const Vector3d * vertex )
{
    const nat n = s.a.size();
    if ( n == 3 )
    {
        const Vector3d & v0 = vertex[s.a[0]];
        const Vector3d & v1 = vertex[s.a[1]];
        const Vector3d & v2 = vertex[s.a[2]];
        const double max = maxDev ( s.b, v0, v1, v2 );
        if ( ! micro ( max ) )
        {
            Plane3d plane ( v0, v1, v2 );
            const double max2 = maxDev ( plane, v0, v1, v2 );
            if ( max > max2 )
            {
                s.b = plane;
                return true;
            }
        }
    }
    else
    {
        const double max = maxDev ( vertex, s.a, s.b );
        if ( ! micro ( max ) )
        {
            DynArray<Vector3d> point ( n );
            for ( nat i = 0; i < n; ++i ) point[i] = vertex[s.a[i]];
            const Plane3d plane = getPlane2 ( point );
            const double max2 = maxDev ( vertex, s.a, plane );
            if ( max > max2 )
            {
                s.b = s.b.norm * plane.norm < 0 ? - plane : plane;
                return true;
            }
        }
    }
    return false;
}

Plane3d selectPlane ( const Vector3d & a, const Vector3d & b, const Vector3d & c, const Plane3d & plane1 )
{
    const double max1 = maxDev ( plane1, a, b, c );
    const Plane3d plane2 ( a, b, c );
    const double max2 = maxDev ( plane2, a, b, c );
    return max2 < max1 ? plane2 : plane1;
}

PolyhedronDoctorReturn doctorPolyhedron ( Suite<Vector3d> & vertex, CArrRef<Set2<CmbArray<nat, 3>, Plane3d> > facet, 
                             bool isTrian, Suite<Set2<CmbArray<nat, 3>, Plane3d> > & facet2, IPolyhedronDoctorReport & rep )
{
    nat i, j;
    List2n list, stor;
    bool isGoodTopo = true;
    bool isGoodPlane = true;
    CmbSuite<Vector2d, 9> vert2d;
    CmbSuite<Vector3d, 9> vert3d;
    CmbSuite<Set3<nat>, 9> stmp;
    const nat nv = vertex.size();
    for ( i = 0; i < facet.size(); ++i )
    {
        const Set2<CmbArray<nat, 3>, Plane3d> & side = facet[i];
        CArrRef<nat> index = side.a;
        const nat n = index.size();
        if ( n < 3 )
        {
            isGoodTopo = false;
            rep ( PHD2_VertexNumberInFacetLess3, i, n );
            continue;
        }
        for ( j = 0; j < n; ++j )
        {
            if ( index[j] >= nv )
            {
                rep ( PHD2_VertexIndexOutOfRange, i, index[j] );
                return PDR_FatalError;
            }
            put ( Set2<nat> ( index.cnext(j), index[j] ), list, stor );
        }
        const Plane3d & plane = side.b;
        if ( n == 3 )
        {
            if ( index[0] == index[1] || index[1] == index[2] || index[2] == index[0] )
            {
                rep ( PHD1_FacetHasEqualVertexIndex, i );
                isGoodTopo = false;
                continue;
            }
            Set2<CmbArray<nat, 3>, Plane3d> & side2 = facet2.inc();
            side2.a.resize ( 3 );
            for ( j = 0; j < n; ++j ) side2.a[j] = index[j];
            side2.b = plane;
            if ( correctPlane ( side2, vertex() ) )
            {
                rep ( PHD1_FacetHasBadPlane, i );
                isGoodPlane = false;
            }
        }
        else
        {
            isGoodTopo = false;
            rep ( PHD2_VertexNumberInFacetMore3, i, n );
            const OrthoFunc3to2 ortho ( plane.norm );
            vert2d.resize ( n );
            for ( j = 0; j < n; ++j ) vert2d[j] = ortho ( vertex[index[j]] );
            trianNat1L1MaxMinTan ( vert2d, stmp );
            for ( j = 0; j < stmp.size(); ++j )
            {
                const Set3<nat> & s = stmp[j];
                const nat i0 = index[s.a];
                const nat i1 = index[s.b];
                const nat i2 = index[s.c];
                if ( i0 == i1 || i1 == i2 || i2 == i1 ) continue;
                Set2<CmbArray<nat, 3>, Plane3d> & side2 = facet2.inc();
                side2.a.resize ( 3 );
                side2.a[0] = i0;
                side2.a[1] = i1;
                side2.a[2] = i2;
                side2.b = plane;
                correctPlane ( side2, vertex() );
                Plane3d ( vertex[i0], vertex[i1], vertex[i2] );
            }
        }
    }
    if ( list.size() > 0 )
    {
        isGoodTopo = false;
        rep ( PHD1_ModelHasHoles, list.size() );
        List< ListItem<List2n> > itog;
        makeClosed ( list, itog );
        CmbSuite<nat, 9> index;
        if ( itog.top() )
        do
        {
            List2n * zol = itog.cur();
            const nat n = zol->size();
            if ( n == 3 )
            {
                nat i0 = zol->fir()->a;
                nat i1 = zol->fir()->b;
                nat i2 = zol->las()->a;
                Set2<CmbArray<nat, 3>, Plane3d> & side2 = facet2.inc();
                side2.a.resize ( 3 );
                side2.a[0] = i0;
                side2.a[1] = i1;
                side2.a[2] = i2;
                side2.b = Plane3d ( vertex[i0], vertex[i1], vertex[i2] );
            }
            else
            if ( n > 3 )
            {
                vert3d.resize ( n );
                index.resize ( n );
                nat count = 0;
                zol->top();
                do
                {
                    index[count] = zol->cur()->a;
                    vert3d[count] = vertex[index[count]];
                    ++count;
                }
                while ( zol->next() );
                trian ( vert3d, stmp );
                for ( i = 0; i < stmp.size(); ++i )
                {
                    const Set3<nat> & s = stmp[i];
                    const nat i0 = index[s.a];
                    const nat i1 = index[s.b];
                    const nat i2 = index[s.c];
                    Set2<CmbArray<nat, 3>, Plane3d> & side2 = facet2.inc();
                    side2.a.resize ( 3 );
                    side2.a[0] = i0;
                    side2.a[1] = i1;
                    side2.a[2] = i2;
                    side2.b = Plane3d ( vertex[i0], vertex[i1], vertex[i2] );
                }
            }
        }
        while ( itog.next() );
    }
    return isGoodTopo && isGoodPlane ? PDR_ModelIsGood : PDR_ModelChange;
}
