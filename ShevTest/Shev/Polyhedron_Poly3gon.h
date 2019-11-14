
#pragma once

#include "trian2d.h"
#include "Vector2d.h"
#include "Poly3gon.h"
#include "Polyhedron.h"

//**************************** 25.01.2006 ****************************
//
//             Копирование Polyhedron в Poly3gon и обратно
//
//**************************** 18.05.2018 ****************************

template <class T>
inline void setInfo ( Facet &, const Trigon<T> & ) {}

template <>
inline void setInfo ( Facet & f, const Trigon<int> & t )
{
    f.info = t.info;
}

template <class Vector, class Info>
void copy ( const Poly3gon<Vector, Info> & poly1, Polyhedron & poly2 )
{
    poly2.vertex.resize ( poly1.vertex.size() );
    poly2.facet.resize ( poly1.side.size() );
    nat i;
    for ( i = 0; i < poly2.vertex.size(); ++i )
    {
        assignXYZ ( poly2.vertex[i], poly1.vertex[i] );
    }
    for ( i = 0; i < poly2.facet.size(); ++i )
    {
        const Trigon<Info> & t = poly1.side[i];
        Facet & facet = poly2.facet[i];
        facet.resize ( 3 );
        facet.index[0] = t.a;
        facet.index[1] = t.b;
        facet.index[2] = t.c;
        makePlane ( t, poly1.vertex(), facet.plane );
        setInfo ( facet, t );
    }
    poly2.linkFacets();
}

template <class T>
inline void setInfo ( Trigon<T> & t, const Facet & f )
{
    assignXYZ ( t.info, f.plane.norm );
}

template <>
inline void setInfo ( Trigon<void> &, const Facet & ) {}

template <>
inline void setInfo ( Trigon<int> & g, const Facet & f )
{
    g.info = f.info;
}

template <class Vector, class Info>
void copy ( const Polyhedron & poly1, Poly3gon<Vector, Info> & poly2 )
{
    poly2.vertex.resize ( poly1.vertex.size() );
    nat i;
    for ( i = 0; i < poly1.vertex.size(); ++i )
    {
        assignXYZ ( poly2.vertex[i], poly1.vertex[i] );
    }
    Suite< Trigon<Info> > temp ( poly1.facet.size() );
    Suite<Vector2d> vbuf;
    Suite<Set3<nat> > tbuf;
    for ( i = 0; i < poly1.facet.size(); ++i )
    {
        const Facet & facet = poly1.facet[i];
        if ( facet.nv < 3 ) continue;
        if ( facet.nv == 3 )
        {
            Trigon<Info> & g = temp.inc();
            g.a = facet.index[0];
            g.b = facet.index[1];
            g.c = facet.index[2];
            setInfo ( g, facet );
            continue;
        }
        ortho_trans ( poly1.vertex, facet, vbuf );
        tbuf.resize ( facet.nv - 2 );
        const nat n = trianNat1L1MaxMinTan ( vbuf, tbuf ).size();
        for ( nat j = 0; j < n; ++j )
        {
            const Set3<nat> & t = tbuf[j];
            Trigon<Info> & g = temp.inc();
            g.a = facet.index[t.a];
            g.b = facet.index[t.b];
            g.c = facet.index[t.c];
            setInfo ( g, facet );
        }
    }
    poly2.side = temp;
}