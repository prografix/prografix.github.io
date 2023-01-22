/*
#include "../stdafx.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
*/
#include "../shev/WireModel.h"
#include "../shev/Polyhedron.h"
#include "../shev/trian2d.h"
#include "../shev/func1t.h"
#include "../shev/func2d.h"
#include "../shev/func3d.h"
#include "../shev/lists.h"
#include "../shev/trans.h"
//#include "../draw.h"
//#include "../test/display.h"

void section ( List<ListItem<SortItem<double, WireModel<4> > > > & mlist, CArrRef<SortItem<double, Double<5> > > cor,
               List<ListItem<SortItem<double, WireModel<4> > > > & mstor, List< Vertex<4> > & stor )
{
    List<ListItem<SortItem<double, WireModel<4> > > > temp;
    mlist.movFirBefFir ( temp );
    for ( nat j = 0; j < cor.size(); ++j )
    {
        ListItem<SortItem<double, WireModel<4> > > * item = temp.las();
        if ( j + 1 == cor.size() )
        {
            item->tail.cut ( cor[j].tail, stor );
        }
        else
        {
            if ( mstor.cur() ) 
                mstor.movCurAftLas ( temp );
            else
                temp.addAftLas ( new ListItem<SortItem<double, WireModel<4> > > );
            item->tail.cut ( cor[j].tail, stor, temp.las()->tail );
        }
        List< Vertex<4> > & vlist = item->tail.vlist;
        Vertex<4> * vmax = vlist.top();
        if ( ! vmax )
        {
            temp.jump ( item ).movCurAftLas ( mstor );
            continue;
        }
        while ( vlist.next() )
        {
            if ( vmax->coor.d0 < vlist.cur()->coor.d0 ) vmax = vlist.cur();
        }
        vlist.makeFir ( vmax );
        item->head = vmax->coor.d0;
    }
    if ( temp.size() == 0 ) return;
    sort321 ( temp ).top();
    if ( ! mlist.top() )
    {
        temp.movAllAftLas ( mlist );
        return;
    }
    for (;;)
    {
        if ( *(SortItem<double, WireModel<4> >*) mlist.cur() < 
             *(SortItem<double, WireModel<4> >*) temp.fir() )
        {
            if ( temp.movFirBefCur ( mlist ).size() == 0 ) break;
        }
        else
        {
            if ( mlist.curIsLas() )
            {
                temp.movAllAftLas ( mlist );
                break;
            }
        }
        mlist.next();
    }
}

void prism ( WireModel<3> & model, CArrRef<Vector2d> base, double h, 
             List<Vertex<3> > & stor, DynArrRef<Vertex<3> *> & varr )
{
    const nat n = base.size();
    varr.resize ( n + n );
    model.vlist.resize ( n+n, stor ).top();
    nat16 i;
    for ( i = 0; i < varr.size(); ++i )
    {
        varr[i] = model.vlist.cur();
        model.vlist.next();
    }
    for ( i = 0; i < n; ++i )
    {
        const nat i1 = ( i + n - 1 ) % n;
        const nat i2 = ( i + 1 ) % n;
        Double<3> & d1 = varr[i]->coor;
        d1.d0 = base[i].x;
        d1.d1 = base[i].y;
        d1.d2 = h;
        Double<3> & d2 = varr[i+n]->coor;
        d2.d0 = base[i].x;
        d2.d1 = base[i].y;
        d2.d2 = 0;
        Vertex<3> * v1 = varr[i];
        Vertex<3> * v2 = varr[i+n];
        if ( i1 < i )
        {
            v1->vertex[0] = varr[i2];
            v1->vertex[1] = varr[i1];
            v2->vertex[0] = varr[i2+n];
            v2->vertex[1] = varr[i1+n];
            v1->nfacet[0] = v2->nfacet[0] = i1;
            v1->nfacet[1] = v2->nfacet[1] = i;
        }
        else
        {
            v1->vertex[0] = varr[i1];
            v1->vertex[1] = varr[i2];
            v2->vertex[0] = varr[i1+n];
            v2->vertex[1] = varr[i2+n];
            v1->nfacet[0] = v2->nfacet[0] = i;
            v1->nfacet[1] = v2->nfacet[1] = i1;
        }
        v1->vertex[2] = varr[i+n];
        v2->vertex[2] = varr[i];
        v1->nfacet[2] = n;
        v2->nfacet[2] = n + 1;
    }
    model.count = n + 2;
}

double pointIn ( CArrRef<Plane3d> inner, const Set2<DynArray<Vector2d>, Plane3d> & s, Vector3d & p,
                 double h, List< Vertex<3> > & stor, DynArrRef<Vertex<3> *> & varr )
{
    const double ax = fabs ( s.b.norm.x );
    const double ay = fabs ( s.b.norm.y );
    const double az = fabs ( s.b.norm.z );

    Vector3d a, b, c;
    if ( ax >= ay && ax >= az )
    {
        const double t = -1. / s.b.norm.x;
        if ( s.b.norm.x > 0 )
        {
            a.x = t * s.b.norm.y;
            b.x = t * s.b.norm.z;
            a.y = 1;
            b.y = 0;
            a.z = 0;
            b.z = 1;
        }
        else
        {
            a.x = t * s.b.norm.z;
            b.x = t * s.b.norm.y;
            a.y = 0;
            b.y = 1;
            a.z = 1;
            b.z = 0;
        }
        c.x = t * s.b.dist;
        c.y = 0;
        c.z = 0;
    }
    else
    if ( ay >= az )
    {
        const double t = -1. / s.b.norm.y;
        if ( s.b.norm.y > 0 )
        {
            a.x = 0;
            b.x = 1;
            a.y = t * s.b.norm.z;
            b.y = t * s.b.norm.x;
            a.z = 1;
            b.z = 0;
        }
        else
        {
            a.x = 1;
            b.x = 0;
            a.y = t * s.b.norm.x;
            b.y = t * s.b.norm.z;
            a.z = 0;
            b.z = 1;
        }
        c.x = 0;
        c.y = t * s.b.dist;
        c.z = 0;
    }
    else
    {
        const double t = -1. / s.b.norm.z;
        if ( s.b.norm.z > 0 )
        {
            a.x = 1;
            b.x = 0;
            a.y = 0;
            b.y = 1;
            a.z = t * s.b.norm.x;
            b.z = t * s.b.norm.y;
        }
        else
        {
            a.x = 0;
            b.x = 1;
            a.y = 1;
            b.y = 0;
            a.z = t * s.b.norm.y;
            b.z = t * s.b.norm.x;
        }
        c.x = 0;
        c.y = 0;
        c.z = t * s.b.dist;
    }
    DynArray<Double<4> > cor ( inner.size() );
    nat i, j;
    for ( i = 0; i < inner.size(); ++i )
    {
        const Plane3d & p = inner[i];
        Double<4> & d = cor[i];
        d.d0 = p.norm * a;
        d.d1 = p.norm * b;
        d.d2 = 1;
        d.d3 = p % c;
    }
    WireModel<3> model;
    prism ( model, s.a, h, stor, varr );
    for ( i = 0; i < 1000; ++i )
    {
        Vertex<3> * v = model.vlist.top();
        if ( ! v ) break;
        while ( model.vlist.next() )
        {
            if ( v->coor.d2 < model.vlist.cur()->coor.d2 ) v = model.vlist.cur();
        }
        nat jm;
        double max = 0;
        for ( j = 0; j < inner.size(); ++j )
        {
            const double t = cor[j] % v->coor;
            if ( max < t ) max = t, jm = j;
        }
        if ( max < 0.001 )
        {
            p = a * v->coor.d0 + b * v->coor.d1 + c;
            model.vlist.movAllAftLas ( stor );
            return v->coor.d2;
        }
        model.cut ( cor[jm], stor );
    }
    model.vlist.movAllAftLas ( stor );
    return 0;
}

void fillCor1 ( const Polyhedron & inner, const Vector3d & p, const Double<4> & v,
                SuiteRef<SortItem<double, Double<5> > > & pcor )
{
    pcor.resize ( inner.facet.size() );
    for ( nat j = 0; j < pcor.size(); ++j )
    {
        const Plane3d & plane = inner.facet[j].plane;
        Double<5> & d5 = pcor[j].tail;
        d5.d0 = - plane.dist;
        d5.d1 = plane.norm.x;
        d5.d2 = plane.norm.y;
        d5.d3 = plane.norm.z;
        d5.d4 = - ( plane.norm * p );
        pcor[j].head = d5 % v;
    }
    quickSort123 ( pcor );
}

void fillCor2 ( const Polyhedron & inner, const Set2<DynArray<Vector2d>, Plane3d> & p, const Double<4> & v,
                SuiteRef<SortItem<double, Double<5> > > & pcor )
{
    nat j;
    double min = p.b % inner.vertex[0];
    double max = min;
    for ( j = 1; j < inner.vertex.size(); ++j )
    {
        const double t = p.b % inner.vertex[j];
        if ( min > t ) min = t; else
        if ( max < t ) max = t;
    }
    pcor.resize();
    /*    Double<5> & d5 = pcor[j].tail;
        d5.d0 = - plane.dist;
        d5.d1 = p.b.norm.x;
        d5.d2 = p.b.norm.y;
        d5.d3 = p.b.norm.z;
        d5.d4 = - ( p.b.norm * p );
        pcor[j].head = d5 % v;*/
    quickSort123 ( pcor );
}

//**************************** 16.10.2008 *********************************//
//
//          ћаксимальный многогранник вписанный в многогранник
//
//**************************** 01.07.2011 *********************************//

bool maxConvexPolyhedronInPolyhedronNR ( const Polyhedron & inner, CArrRef<Plane3d> chull, 
                                         const Segment3d & dim1, const Segment3d & dim2,
                                         CArrRef<Set2<DynArray<Vector2d>, Plane3d> > inc2d,
                                         CArrRef<Vector3d> inc0d, Conform3d & res )
{
    const double d1 = normU ( dim1 );
    const double d2 = normU ( dim2 );
// »нициализаци€ выпуклых ограничений
    Suite<Double<5> > conv ( chull.size(), chull.size() );
    nat i, j;
    for ( i = 0; i < conv.size(); ++i )
    {
        const Plane3d & p = chull[i];
        double max = p.norm * inner.vertex[0];
        for ( j = 1; j < inner.vertex.size(); ++j )
        {
            const double t = p.norm * inner.vertex[j];
            if ( max < t ) max = t;
        }
        Double<5> & c = conv[i];
        c.d0 = max;
        c.d1 = p.norm.x;
        c.d2 = p.norm.y;
        c.d3 = p.norm.z;
        c.d4 = p.dist;
    }
// »нициализаци€ области допустимых преобразований
    List<ListItem<SortItem<double, WireModel<4> > > > mlist, mstor;
    List< Vertex<4> > stor4;
    List< Vertex<3> > stor3;
    ListItem<SortItem<double, WireModel<4> > > * item = new ListItem<SortItem<double, WireModel<4> > >;
    item->tail.simplex ( 4, stor4 );
    Double<4> d4;
    d4.d0 = d2 / d1;
    d4.d1 = dim2.b.x - dim2.a.x;
    d4.d2 = dim2.b.y - dim2.a.y;
    d4.d3 = dim2.b.z - dim2.a.z;
    item->tail.vlist *= d4;
    item->head = d4.d0;
    d4.d0 = 0;
    d4.d1 = dim2.a.x;
    d4.d2 = dim2.a.y;
    d4.d3 = dim2.a.z;
    item->tail.vlist += d4;
    mlist.addAftLas ( item );
    Suite<SortItem<double, Double<5> > > pcor ( inner.facet.size() );
    DynArray<Plane3d> iplane;
    Suite<Vertex<3> *> varr;
    if ( inc2d.size() > 0 )
    {
        iplane.resize ( inner.facet.size() );
        for ( j = 0; j < inner.facet.size(); ++j ) iplane[j].norm = inner.facet[j].plane.norm;
    }
    double lowLvl = 0;
    const double eps = 1e-3;
    for ( i = 0; i < 1000; ++i )
    {
// ќтбрасываем слишком маленькие решени€
        while ( mlist.size() > 0 )
        {
            if ( mlist.las()->head > lowLvl ) break;
            mlist.las()->tail.vlist.movAllAftLas ( stor4 );
            mlist.movLasAftLas ( mstor );
        }
        if ( ! mlist.top() ) return false;
// ѕоиск максимального нарушени€ среди выпуклых ограничений
        const Double<4> & v = mlist.fir()->tail.vlist.fir()->coor;
        nat k, km;
        double max = 0.;
        for ( k = 0; k < conv.size(); ++k )
        {
            const double t = conv[k] % v;
            if ( max < t ) max = t, km = k;
        }
// ѕрименение выпуклого ограничени€ к области допустимых преобразований
        if ( max > eps )
        {
            for(;;)
            {
                item = mlist.cur();
                WireModel<4> & model = item->tail;
                model.cut ( conv[km], stor4 );
                List< Vertex<4> > & vlist = model.vlist;
                if ( ! vlist.top() )
                {
                    if ( mlist.curIsLas() )
                    {
                        mlist.movCurAftLas ( mstor );
                        break;
                    }
                    else
                    {
                        mlist.movCurAftLas ( mstor );
                        continue;
                    }
                }
                Vertex<4> * vmax = vlist.cur();
                while ( vlist.next() )
                {
                    if ( vmax->coor.d0 < vlist.cur()->coor.d0 ) vmax = vlist.cur();
                }
                vlist.makeFir ( vmax );
                item->head = vmax->coor.d0;
                if ( ! mlist.next() ) break;
            }
            sort321 ( mlist );
            conv.del ( km );
            continue;
        }
        if ( ! lowLvl ) lowLvl = v.d0 / 8;
// ѕоиск максимального нарушени€ среди точечных ограничений
        max = 0.;
        for ( k = 0; k < inc0d.size(); ++k )
        {
            const Vector3d & p = inc0d[k];
            const Vector3d p1 ( ( p.x - v.d1 ) / v.d0, ( p.y - v.d2 ) / v.d0, ( p.z - v.d3 ) / v.d0 );
            double min = - ( inner.facet[0].plane % p1 );
            for ( j = 1; j < inner.facet.size(); ++j )
            {
                const double t = - ( inner.facet[j].plane % p1 );
                if ( min > t ) min = t;
            }
            if ( max < min ) max = min, km = k;
        }
// ѕрименение точечного ограничени€ к области допустимых преобразований
        if ( max * v.d0 > eps )
        {
            fillCor1 ( inner, inc0d[km], v, pcor );
            section ( mlist, pcor, mstor, stor4 );
            continue;
        }
// ѕрименение плоских ограничений к области допустимых преобразований
        if ( inc2d.size() > 0 )
        {
            for ( j = 0; j < inner.facet.size(); ++j )
            {
                const Plane3d & p = inner.facet[j].plane;
                iplane[j].dist = p.dist * v.d0 - p.norm.x * v.d1 - p.norm.y * v.d2 - p.norm.z * v.d3;
            }
            max = 0.;
            Vector3d p, pm;
            for ( k = 0; k < inc2d.size(); ++k )
            {
                const double t = pointIn ( iplane, inc2d[k], p, v.d0 * d1, stor3, varr );
                if ( max < t ) max = t, pm = p;
            }
            if ( max > eps )
            {
 //drawPoint ( pm, 1, 1, 0 );
                fillCor1 ( inner, pm, v, pcor );
                section ( mlist, pcor, mstor, stor4 );
                continue;
            }
        }
// ќтправл€ем найденное решение
        res.magn = v.d0;
        res.trans.x = v.d1;
        res.trans.y = v.d2;
        res.trans.z = v.d3;
        res.spin = Spin3d();
//display << i+1 << mlist.size() << NL;
        return true;
    }
    return false;
}

bool maxConvexPolyhedronInPolyhedronNR ( const Polyhedron & inner, const Polyhedron & outer, 
                                         CArrRef<Vector3d> inc0d, Conform3d & res )
{
    const Segment3d dim1 = dimensions ( inner.vertex );
    const Segment3d dim2 = dimensions ( outer.vertex );
    const double d1 = normU ( dim1 );
    const double d2 = normU ( dim2 );
    if ( ! d1 || ! d2 ) return false;
    nat i, j;
    Suite<Set2<DynArray<Vector2d>, Plane3d> > inc2d; 
    Suite<Vector2d> vert2d;
    for ( i = 0; i < outer.facet.size(); ++i )
    {
        const Facet & facet = outer.facet[i];
        const Plane3d & plane = facet.plane;
        for ( j = 0; j < outer.vertex.size(); ++j )
        {
            if ( plane % outer.vertex[j] < 0.001 ) continue;
            Func3to2 func = getTrans ( plane.norm );
            vert2d.resize ( facet.nv );
            for ( j = 0; j < facet.nv; ++j ) vert2d[j] = func ( outer.vertex[facet.index[j]] );
            if ( isConvex ( vert2d ) )
            {
                Set2<DynArray<Vector2d>, Plane3d> & c = inc2d.inc();
                c.a = vert2d;
                c.b = plane;
            }
            else
            {
                List< ListItem<ShevList> > plist;
//                convexParts ( vert2d, plist );
                if ( plist.top() )
                do
                {
                    ShevList * list = plist.cur();
                    Set2<DynArray<Vector2d>, Plane3d> & c = inc2d.inc();
                    c.a.resize ( list->size() );
                    list->top();
                    for ( j = 0; j < c.a.size(); ++j )
                    {
                        c.a[j] = vert2d[list->cur()->info];
                        list->next();
                    }
                    c.b = plane;
                }
                while ( plist.next() );
            }
            break;
        }
    }
    DynArray<Plane3d> plane;
    convexHull ( outer.vertex, plane );
    return maxConvexPolyhedronInPolyhedronNR ( inner, plane, dim1, dim2, inc2d, inc0d, res );
}
