
#include <math.h>
#include "../Shev/func1t.h"
#include "../Shev/func2d.h"
#include "../Shev/func3d.h"
#include "../Shev/trans.h"
#include "../Shev/trian2d.h"
#include "../Shev/Vector2d.h"
#include "../Shev/Polyhedron.h"
#include "../Shev/intersect2d.h"
#include "../Shev/intersect3d.h"

#include "../test/display.h"
#include "../draw.h"


//**************************** 28.09.2013 *********************************//
//
//                  Пересечение простых многогранников
//
//**************************** 28.05.2014 *********************************//

void func ( const Polyhedron & poly1, const Polyhedron & poly2, double shift,
            List<ListItem<Set2<Plane3d, DynArray<Vector2d> > > > & glist )
{
    nat i, j;
    CmbSuite<DynArray<Vector3d>, 9> sect3;
    CmbSuite<DynArray<Vector2d>, 9> sect2;
    CmbSuite<Vector2d, 80> temp2;
    CmbSuite<SortItem<double, nat>, 9> plus, minus;
    CmbSuite<nat, 9> minus1, minus2;
    Suite< Suite<Vector2d> > res, res1, res2, res3;
    for ( nat ii = 0; ii < poly1.facet.size(); ++ii )
    {
        const Facet & facet = poly1.facet[ii];
        if ( facet.nv < 3 ) continue;
        Plane3d plane = facet.plane;
        plane.dist += shift;
        intersection ( poly2, plane, sect3 );
        sect2.resize ( sect3.size() );
        plus.resize();
        minus.resize();
        for ( j = 0; j < sect3.size(); ++j )
        {
            trans ( sect3[j], plane.norm, sect2[j].resize ( sect3[j].size() ) );
            const double a = area ( sect2[j] );
if ( a > 0 && ! trianTestNat1L1MinTan ( sect2[j] ) )
{
    Suite<Set3<nat> > res1;
    trianG1MaxMinArea ( sect2[j], res1 );
    double min = 0;
    for ( nat i = 0; i < res1.size(); ++i )
    {
        const Vector2d & a = sect2[j][res1[i].a];
        const Vector2d & b = sect2[j][res1[i].b];
        const Vector2d & c = sect2[j][res1[i].c];
        double s = ( b - a ) % ( c - b );
        if ( min > s ) min = s;
//if ( s < -0.01 && ii == 330 ) display << a << NL << b << NL << c << NL;
    }
    if ( min < -0.01 )
    {
        display << "tr =" << min << ii << NL;
if ( ii == 330 )
{
    drawPolygon ( sect3[j], 1, 0, 0 );
    //for ( nat i = 0; i < sect2[j].size(); ++i ) display << sect2[j][i] << NL;
}
    }
}
            if ( a > 0 ) plus .inc() = SortItem<double, nat> ( a, j );
            if ( a < 0 ) minus.inc() = SortItem<double, nat> ( a, j );
        }
        if ( plus.size() == 0 ) continue;
        temp2.resize ( facet.nv );
        Func3to2 transFunc = getTrans ( plane.norm );
        for ( j = 0; j < facet.nv; ++j ) temp2[j] = transFunc ( poly1.vertex[facet.index[j]] );
        if ( minus.size() == 0 )
        {
            for ( i = 0; i < plus.size(); ++i )
            {
                if ( ! intersection ( sect2[plus[i].tail], temp2, res ) )
                {
//                    intersection ( sect2[plus[i].tail], temp2, res );
                    continue;
                }
                for ( j = 0; j < res.size(); ++j )
                {
                    Suite<Vector2d> & p = res[j];
                    if ( area ( p ) <= 0 ) continue;
                    ListItem<Set2<Plane3d, DynArray<Vector2d> > > * gran = new ListItem<Set2<Plane3d, DynArray<Vector2d> > >;
                    gran->a = plane;
                    gran->b = p;
                    glist.addAftCur ( gran );
                }
            }
        }
        else
        {
            insertSort123 ( plus );
            insertSort321 ( minus );
            for ( i = 0; i < plus.size(); ++i )
            {
                const SortItem<double, nat> & p = plus[i];
                CArrRef<Vector2d> vert1 = sect2[p.tail];
                // Перенесём в minus1 отрицательные многоугольники принадлежащие vert1
                minus1.resize();
                for ( j = 0; j < minus.size(); )
                {
                    const SortItem<double, nat> & m = minus[j];
                    if ( p.head + m.head < 0 ) break;
                    CArrRef<Vector2d> vert2 = sect2[m.tail];
                    if ( isIntersect ( 0.5 * ( vert2[0] + vert2[1] ), vert1 ) )
                    {
                        minus1.inc() = m.tail;
                        minus.delAndShift ( j );
                    }
                    else ++j;
                }
                // Пересекаем положительные многоугольники
                if ( ! intersection ( vert1, temp2, res ) )
                    continue;
                // Пересекаем результат с отрицательными многоугольниками
                for ( nat k = 0; k < res.size(); ++k )
                {
                    res1.resize ( 1 );
                    _swap ( res[k], res1[0] );
                    minus2.resize();
                    for ( j = 0; j < minus1.size(); ++j )
                    {
                        CArrRef<Vector2d> vert2 = sect2[minus1[j]];
                        res3.resize();
                        for ( nat k1 = 0; k1 < res1.size(); ++k1 )
                        {
                            if ( ! intersection ( vert2, res1[k1], res2 ) )
                            {
//                                intersection ( vert2, res1[k1], res2 );
                                continue;
                            }
                            for ( nat k2 = 0; k2 < res2.size(); ++k2 )
                            {
                                if ( area ( res2[k2] ) < 0 )
                                {
                                    minus2.inc() = minus1[j];
                                }
                                else
                                {
                                    res3.inc();
                                    _swap ( res3.las(), res2[k2] );
                                }
                            }
                        }
                        _swap ( res1, res3 );
                    }
                    for ( j = 0; j < res1.size(); ++j )
                    {
                        Suite<Vector2d> & p = res1[j];
                        ListItem<Set2<Plane3d, DynArray<Vector2d> > > * gran = new ListItem<Set2<Plane3d, DynArray<Vector2d> > >;
                        gran->a = plane;
                        gran->b = p;
                        glist.addAftCur ( gran );
                    }
                }
            }
        }
    }
}

typedef ListItem<Vector3d> Vert;
typedef ListItem<List<Vert> > VertList;
typedef List<VertList> VertListList;

static nat getIndex ( VertListList & dlist, nat k, const Vector3d & v, double eps )
{
    Vert * p;
    VertList * vl;
    if ( ! dlist.top() )
    {
        p = new Vert ( v, k );
        vl = new VertList;
        vl->addAftCur ( p );
        dlist.addAftCur ( vl );
        return k;
    }
    const double min = v.x - eps;
    const double max = v.x + eps;
    do
    {
        vl = dlist.cur();
        if ( vl->las()->x <= min ) continue;
        if ( vl->top()->x >= max ) break;
        do
        {
            const Vector3d & p = * vl->cur();
            if ( p.x <= min ) continue;
            if ( p.x >= max ) break;
            if ( fabs ( v.y - p.y ) < eps && fabs ( v.z - p.z ) < eps )
            {
                return vl->cur()->info;
            }
        }
        while ( vl->next() );
    }
    while ( dlist.next() );
    p = new Vert ( v, k );
    dlist.top();
    do
    {
        vl = dlist.cur();
        if ( vl->las()->x < v.x ) continue;
        vl->top();
        do
        {
            if ( vl->cur()->x >= v.x )
            {
                vl->addBefCur ( p );
                goto m1;
            }
        }
        while ( vl->next() );
    }
    while ( dlist.next() );
    vl->addAftLas ( p );
m1: if ( vl->size() > dlist.size() )
    {
        VertList * t = new VertList;
        vl->movFirBefFir ( vl->size()/2, *t );
        dlist.addBefCur ( t );
    }
    return k;
}

class EItem : public ListItem<Set3<nat, nat, ListItem<Facet> *> > { public: typedef nat Par; };
typedef List<EItem> EList;

inline bool delVert ( nat a, nat b, nat c, Facet * facet )
{
    if ( facet->nv < 3 ) return false;
    nat k = facet->index[facet->nv-1];
    for ( nat i = 0; i < facet->nv; ++i )
    {
        if ( a == k && b == facet->index[i] && c == facet->index[i+1] )
        {
            for ( nat j = i; j < facet->nv; ++j ) facet->index[j] = facet->index[j+1];
            --facet->nv;
            facet->index[facet->nv] = facet->index[0];
            return true;
        }
        k = facet->index[i];
    }
    return false;
}

inline void addVert ( nat iv, ListItem<Set3<nat, nat, ListItem<Facet> *> > * p )
{
    ListItem<Facet> * facet = p->c;
    DynArray<nat> index ( 3 * facet->nv + 4 );
//if ( facet->Facet::info == 705 )
//    facet->Facet::info = facet->Facet::info;
    for ( nat i = 0; i < facet->nv; ++i )
    {
        index[i] = facet->index[i];
        if ( index[i] == p->a && facet->index[i+1] == p->b )
        {
            nat i2 = i + 2;
            if ( i2 >= facet->nv ) i2 -= facet->nv;
            if ( iv == facet->index[i2] )
            {
                if ( i2 == 1 )
                {
                    for ( nat j = 1; j < facet->nv; ++j ) facet->index[j-1] = facet->index[j];
                facet->index[--facet->nv] = facet->index[0];
                break;
                }
            }
            index[++i] = iv;
            for ( nat j = i; j <= facet->nv; ++j ) index[j+1] = facet->index[j];
            _swap ( index, facet->index );
            ++facet->nv;
            break;
        }
    }
}

void divide ( Facet * facet, List< ListItem<Facet> > & flist )
{
    nat i, j, k;
m1: if ( facet->nv < 5 ) return;
    for ( i = 2; i < facet->nv; ++i )
    {
        for ( j = 0; j + 1 < i; ++j )
        {
            if ( facet->index[i] != facet->index[j] ) continue;
            const nat n = i - j;
            if ( n > 2 )
            {
                Facet * f = flist.addAftLas ( new ListItem<Facet> ).las();
                f->plane = facet->plane;
                f->resize ( n );
                for ( k = 0; k < n; ++k ) f->index[k] = facet->index[j+k];
                f->index[n] = f->index[0];
                f->info = 111114;
            }
            for ( k = j+1; k < facet->nv; ++k ) facet->index[k] = facet->index[n+k];
            facet->nv -= n;
            goto m1;
        }
    }
}

bool twoFacets ( EList * p )
{
    ListItem<Facet> * f1 = p->top()->c;
    ListItem<Facet> * f2;
    while ( p->next() )
    {
        ListItem<Facet> * f = p->cur()->c;
        if ( f1 == f ) continue;
        f2 = f;
        break;
    }
    while ( p->next() )
    {
        ListItem<Facet> * f = p->cur()->c;
        if ( f1 == f || f2 == f ) continue;
        return false;
    }
    return true;
}

const double q2c = 9e-18;

double coef ( Facet * facet, CArrRef<Vector3d> & vertex )
{
    const Vector3d v1 = vertex[facet->index[0]] - vertex[facet->index[1]];
    const Vector3d v2 = vertex[facet->index[1]] - vertex[facet->index[2]];
    const Vector3d v3 = vertex[facet->index[2]] - vertex[facet->index[0]];
    const double a = v1 % v2 * facet->plane.norm;
    return a * a / _pow2 ( _max ( qmod ( v1 ), qmod ( v2 ), qmod ( v3 ) ) );
}

void addFacets ( EList & list, List< ListItem<Facet> > & flist, double eps, CArrRef<Vector3d> vertex )
{
    List<ListItem<EList> > itog;
    Suite<Facet *> fd;
    makeClosed ( list, itog );
    int info1 = 1000000;
    int info2 = 2000000;
    if ( itog.top() )
    do
    {
        EList * p = itog.cur();
        ListItem<Facet> * item = p->top()->c;
        while ( p->next() )
        {
            ListItem<Facet> * item2 = p->cur()->c;
            if ( item != item2 )
            {
                while ( p->next() )
                {
                    ListItem<Facet> * item3 = p->cur()->c;
                    if ( item3 != item2 && item3 != item )
                        goto m2;
                }
                goto m1;
            }
        }
        flist.del ( p->cur()->c );
        continue;
m1:     //display << p->size() << NL;
        // Выпрямление
        p->top();
        do
        {
            EItem * cur = p->cur();
            EItem * nex = p->cnext();
            while ( cur->c == nex->c && delVert ( cur->a, cur->b, nex->b, cur->c ) )
            {
                cur->b = nex->b;
                p->del ( nex );
                nex = p->jump ( cur ).cnext();
            }
            p->jump ( cur );
        }
        while ( p->next() );

        if ( p->size() > 3 )
        {/*
            p->top();
            Facet & facet = *p->cur()->c;
            for ( nat i = 0; i <= facet.nv; ++i )
            {
                display << vertex[facet.index[i]] << facet.index[i] << NL;
            }
            /*do
            {
                EItem * e = p->cur();
                display << vertex[e->a] << e->a << e->c->Facet::info << NL;
            }
            while ( p->next() );*/
        }
        if ( p->size() == 3 )
        {
            EItem * e1 = p->top();
            EItem * e2 = p->next();
            EItem * e3 = p->next();
            if ( e1->c == e2->c )
                addVert ( e2->a, e3 );
            else
            if ( e2->c == e3->c )
                addVert ( e3->a, e1 );
            else
                addVert ( e1->a, e2 );
            continue;
        }
        if ( p->size() == 2 )
            continue;
m2:     /*display << p->size() << NL;
        if ( p->size() == 8 )
        {
            //p=p;display << p->size() << NL;
            p->top();
            Facet & facet = *p->next()->c;
            for ( nat i = 0; i <= facet.nv; ++i )
            {
                //display << vertex[facet.index[i]] << facet.index[i] << NL;
            }
        }
        /*p->top();
        do
        {
            EItem * e = p->cur();
            display << e->a << e->b << e->c->Facet::info << NL;
        }
        while ( p->next() );*/
        // Удаление вырожденных граней
        nat gg = p->size();
        for(;;)
        {
            const nat n = p->size();
            p->top();
            do
            {
                EItem * cur = p->cur();
                EItem * nex = p->cnext();
                if ( cur->a == nex->b )
                {
                    if ( p->size() < 5 ) goto m3;
                    p->delCur().del ( cur );
                    continue;
                }
                p->jump ( cur );
                const Vector3d v1 = vertex[cur->b] - vertex[cur->a];
                const Vector3d v2 = vertex[nex->b] - vertex[nex->a];
                if ( v1 * v2 >= 0 ) continue;
                const double q1 = v1 * v1;
                const double q2 = v2 * v2;
                if ( q1 > q2 )
                {
                    if ( qmod ( v1 % v2 ) > q2c * q1 * q1 ) continue;
                    addVert ( nex->b, cur );
                    fd.inc() = cur->c;
                    cur->b = nex->b;
                    p->del ( nex );
                    if ( p->size() == 2 ) goto m3;
                    if ( twoFacets ( p ) ) 
                        goto m1;
                    p->jump ( cur );
                }
                else
                {
                    if ( qmod ( v1 % v2 ) > q2c * q2 * q2 ) continue;
                    addVert ( cur->a, nex );
                    fd.inc() = nex->c;
                    nex->a = cur->a;
                    p->del ( cur );
                    if ( p->size() == 2 ) goto m3;
                    if ( twoFacets ( p ) ) 
                        goto m1;
                    p->jump ( nex );
                }
            }
            while ( p->next() );
            if ( p->size() == n ) break;
        }
m4:
        /*if ( gg == 11 )
        {
            p->top();
            do
            {
                EItem * cur = p->cur();
                display << cur->a << cur->b << cur->c->Facet::info << NL;
            }
            while ( p->next() );
        }*/
        {
            Vector3d norm ( 0, 0, 0 );
            const Vector3d & o = vertex[p->top()->b];
            p->next();
            while ( p->next() )
            {
                norm += ( vertex[p->cur()->b] - o ) % ( vertex[p->cur()->a] - o );
            }
            norm.setNorm2();
            double min = norm * vertex[p->top()->a];
            double max = min;
            double avg = min;
            if ( p->size() > 3 )
            {
                while ( p->next() )
                {
                    double t = norm * vertex[p->cur()->a];
                    if ( max < t ) max = t; else
                    if ( min > t ) min = t;
                    avg += t;
                }
            }
            if ( max - min <= eps + eps )
            {
                EItem * pre = p->las();
                p->top();
                while ( p->size() > 2 )
                {
                    EItem * cur = p->cur();
                    if ( pre->c == cur->c && delVert ( pre->a, cur->a, cur->b, cur->c ) )
                    {
                        cur->a = pre->a;
                        p->del ( pre );
                        pre = p->jump ( cur ).cprev();
                        if ( p->jump ( cur ).curIsLas() ) break;
                    }
                    else
                    {
                        if ( ! p->next() ) break;
                        pre = cur;
                    }
                }
                if ( p->size() == 3 )
                {
                    EItem * e1 = p->top();
                    EItem * e2 = p->next();
                    EItem * e3 = p->next();
                    const Vector3d & v1 = vertex[e3->a];
                    const Vector3d & v2 = vertex[e1->a];
                    const Vector3d & v3 = vertex[e2->a];
                    const double q1 = qmod ( v2 - v3 );
                    const double q2 = qmod ( v3 - v1 );
                    const double q3 = qmod ( v1 - v2 );
                    if ( q1 >= q2 && q1 >= q3 )
                    {
                        const double d = fabs ( e1->c->plane % v1 );
 //display << d << NL;
                        if ( d < eps )
                        {
                            addVert ( e2->b, e1 );
                            continue;
                        }
                    }
                    else
                    if ( q2 >= q3 )
                    {
                        const double d = fabs ( e2->c->plane % v2 );
//display << d << NL;
                        if ( d < eps )
                        {
                            addVert ( e3->b, e2 );
                            continue;
                        }
                    }
                    else
                    {
                        const double d = fabs ( e3->c->plane % v3 );
//display << d << norm2 ( ( v2 - v1 ) % ( v1 - v3 ) ) << NL;
                        if ( d < eps )
                        {
                            addVert ( e1->b, e3 );
                            continue;
                        }
                    }
                    //display << qmod ( ( v2 - v1 ) % ( v1 - v3 ) ) / ( q * q ) << NL;
                }
                if ( p->size() > 2 )
                {
                    Facet & facet = * flist.addAftLas ( new ListItem<Facet> ).las();
                    facet.plane.norm = norm;
                    facet.plane.dist = -0.5 * ( min + max );
                    facet.resize ( p->size() );
                    facet.info = ++info1;
                    p->end();
                    for ( nat i = 0; i < facet.nv; ++i )
                    {
//if ( facet.info == 1000001 ) display <<  p->cur()->c->Facet::info << NL;
                        facet.index[i] = p->cur()->a;
                        p->prev();
                    }
                    facet.index[facet.nv] = facet.index[0];
//if ( facet.info == 1000008 ) for ( nat i = 0; i < facet.nv; ++i ) display << vertex[facet.index[i]] << NL;
//++nn;
                }
            }
            else
            {
                avg /= p->size();
                EItem * pm = p->top();
                max = fabs ( norm * vertex[pm->a] - avg );
                while ( p->next() )
                {
                    double t = fabs ( norm * vertex[p->cur()->a] - avg );
                    if ( max < t ) max = t, pm = p->cur();
                }
                EItem * pre = p->jump(pm).cprev();
                if ( pm->c == pre->c && delVert ( pre->a, pm->a, pm->b, pm->c ) )
                {
                }
                else
                {
                    const Vector3d & v1 = vertex[pre->a];
                    const Vector3d & v2 = vertex[pm->b];
                    const Vector3d & v3 = vertex[pm->a];
                    if ( qmod ( v1 - v3 ) > qmod ( v2 - v3 ) )
                    {
//display << norm2 ( ( v1 - v3 ) % ( v2 - v3 ) ) << pre->c->plane % v2 << NL;
                        if ( fabs ( pre->c->plane % v2 ) < eps )
                        {
                            addVert ( pm->b, pre );
                            pm->c = pre->c;
                            goto m5;
                        }
                    }
                    else
                    {
//display << norm2 ( ( v1 - v3 ) % ( v2 - v3 ) ) << pm->c->plane % v1 << NL;
                        if ( fabs ( pm->c->plane % v1 ) < eps )
                        {
                            addVert ( pre->a, pm );
                            goto m5;
                        }
                    }
                    ListItem<Facet> * fi = new ListItem<Facet>;
                    Facet & facet = * flist.addAftLas ( fi ).las();
                    facet.resize ( 3 );
                    facet.index[0] = facet.index[3] = pre->a;
                    facet.index[1] = pm->b;
                    facet.index[2] = pm->a;
                    facet.initPlane ( vertex );
/*                    double t0 = fabs ( facet.plane % vertex[facet.index[0]] );
                    double t1 = fabs ( facet.plane % vertex[facet.index[1]] );
                    double t2 = fabs ( facet.plane % vertex[facet.index[2]] );
                    if ( _max ( t0, t1, t2 ) > 5e-9 )
                    {
                        Vector3d v1 = vertex[facet.index[1]] - vertex[facet.index[0]];
                        Vector3d v2 = vertex[facet.index[2]] - vertex[facet.index[0]];
        Vector3d norm = ( v1 % v2 ).setNorm2();
        double d0 = norm * vertex[facet.index[0]];
        double d1 = norm * vertex[facet.index[1]];
        double d2 = norm * vertex[facet.index[2]];
        Plane3d plane ( norm, -0.5 * ( _max ( d0, d1, d2 ) + _min ( d0, d1, d2 ) ) );
                    double s0 = fabs ( plane % vertex[facet.index[0]] );
                    double s1 = fabs ( plane % vertex[facet.index[1]] );
                    double s2 = fabs ( plane % vertex[facet.index[2]] );
                    double max = _max ( s0, s1, s2 );
                    max = max;
        }*/
                    facet.info = ++info2;
if ( info2 == 2000001 )
info1 = info1;
                    pm->c = flist.las();
                }
m5:             pm->a = pre->a;
                p->del ( pre );
                goto m4;
            }
        }
m3:;
    }
    while ( itog.next() );
    //
    /*EList stor;
    if ( flist.top() )
    do
    {
        ListItem<Facet> * facet = flist.cur();
        for ( nat i = 0; i < facet->nv; ++i )
        {
            put ( Set3<nat, nat, ListItem<Facet> *> ( facet->index[i], facet->index[i+1], facet ), list, stor );
        }
    }
    while ( flist.next() );
    if ( list.top() )
    do
    {
        display << list.cur()->a << list.cur()->b << list.cur()->c->Facet::info << NL;
    }
    while ( list.next() );*/
    for ( nat j = 0; j < fd.size(); ++j ) divide ( fd[j], flist );
}

void doctor ( List< ListItem<Facet> > & flist, double eps, DynArray<Vector3d> & vertex )
{
    nat fi = flist.size();
// Проверка граней на самопересечения
    if ( ! flist.top() ) return;
    CmbSuite<Vector2d, 80> vert2d;
    CmbSuite<Set3<nat>, 80 > set3;
    for(;;)
    {
        ListItem<Facet> * facet = flist.cur();
        if ( facet->nv < 4 )
        {
            if ( ! flist.next() ) break;
            continue;
        }
        vert2d.resize ( facet->nv );
        Func3to2 func = getTrans ( facet->plane.norm );
        for ( nat j = 0; j < facet->nv; ++j ) vert2d[j] = func ( vertex[facet->index[j]] );
if ( facet->Facet::info == 783 )
fi=fi;
//for ( j = 0; j < facet->nv; ++j ) display << vert2d[j] << NL;
        if ( loopNumber ( vert2d ) == 1 && trianTestNat1L1MinTan ( vert2d ) )
        {
            if ( ! flist.next() ) break;
            continue;
        }
        trianNat1L1MaxMinTan ( vert2d, set3 );
        for ( nat i = 0; i < set3.size(); ++i )
        {
            const Set3<nat> & s = set3[i];
            const Vector3d v1 = vertex[facet->index[s.b]] - vertex[facet->index[s.a]];
            const Vector3d v2 = vertex[facet->index[s.c]] - vertex[facet->index[s.b]];
            const double a = v1 % v2 * facet->plane.norm;
            if ( a <= 0 ) continue;
            const Vector3d v3 = vertex[facet->index[s.a]] - vertex[facet->index[s.c]];
            if ( a * a < q2c * _pow2 ( _max ( qmod ( v1 ), qmod ( v2 ), qmod ( v3 ) ) ) ) continue;
            ListItem<Facet> * tr = new ListItem<Facet>;
            tr->resize ( 3 );
            tr->index[0] = facet->index[s.a];
            tr->index[1] = facet->index[s.b];
            tr->index[2] = facet->index[s.c];
            tr->index[3] = tr->index[0];
            tr->plane = facet->plane;
            tr->Facet::info = fi++;
            flist.addBefFir ( tr );
        }
        if ( flist.curIsLas() )
        {
            flist.delCur();
            break;
        }
        flist.delCur();
    }
// Удаление "выростов" на гранях
    if ( flist.top() == 0 ) return;
    for(;;)
    {
m1:     ListItem<Facet> * facet = flist.cur();
        for ( nat cur = 0; cur < facet->nv; ++cur )
        {
            const nat pre = cur > 0 ? cur - 1 : facet->nv - 1;
            const nat nex = cur + 1;
            const Vector3d v1 = vertex[facet->index[cur]] - vertex[facet->index[pre]];
            const Vector3d v2 = vertex[facet->index[nex]] - vertex[facet->index[cur]];
            if ( v1 * v2 >= 0 ) continue;
            const double q1 = v1 * v1;
            const double q2 = v2 * v2;
//if(facet->Facet::info == 494)
//for ( nat j = 0; j < facet->nv; ++j ) display << 1000*vertex[facet->index[j]] << NL;
//double t = qmod ( v1 % v2 ) / _pow2 ( _max ( q1, q2 ) );*/
            if ( qmod ( v1 % v2 ) > q2c * _pow2 ( _max ( q1, q2 ) ) ) continue;
            if ( facet->nv == 3 )
            {
                if ( flist.curIsLas() )
                {
                    flist.delCur();
                    goto m2;
                }
                else
                {
                    flist.delCur();
                    goto m1;
                }
            }
            else
            {
                for ( nat j = cur; j < facet->nv; ++j ) facet->index[j] = facet->index[j+1];
                --facet->nv;
                facet->index[facet->nv] = facet->index[0];
                goto m1;
            }
        }
        if ( ! flist.next() ) break;
    }
m2:;
// Заполнение гранями "дырок"
    EList list, stor;
    if ( flist.top() == 0 ) return;
    do
    {
        ListItem<Facet> * facet = flist.cur();
        for ( nat i = 0; i < facet->nv; ++i )
        {
//if ( facet->Facet::info == 705 ) display << facet->index[i] << 1000* vertex[facet->index[i]] << NL;
            put ( Set3<nat, nat, ListItem<Facet> *> ( facet->index[i], facet->index[i+1], facet ), list, stor );
        }
    }
    while ( flist.next() );
    if ( list.size() > 0 ) 
        addFacets ( list, flist, eps, vertex );
/*    list.delAll();
    if ( flist.top() == 0 ) return;
    do
    {
        ListItem<Facet> * facet = flist.cur();
        for ( nat i = 0; i < facet->nv; ++i )
        {
            put ( Set3<nat, nat, ListItem<Facet> *> ( facet->index[i], facet->index[i+1], facet ), list, stor );
        }
    }
    while ( flist.next() );
    if ( list.top() ) do display << list.cur()->a << list.cur()->b << NL; while ( list.next() );*/
// Удаление лишних вершин
    nat i;
    DynArray<nat> narr ( vertex.size() );
    narr.fill(0);
    flist.top();
    do
    {
        ListItem<Facet> * facet = flist.cur();
        for ( i = 0; i < facet->nv; ++i ) ++narr[facet->index[i]];
    }
    while ( flist.next() );
    CmbSuite<nat, 24> bad;
    for ( i = 0; i < narr.size(); ++i )
    {
        if ( narr[i] < 3 && narr[i] > 0 ) bad.inc() = i;
    }
    while ( bad.size() > 0 )
    {
        const nat iv = bad.las();
        bad.dec();
        if ( flist.top() )
        do
        {
m3:         ListItem<Facet> * facet = flist.cur();
            for ( i = 0; i < facet->nv; ++i )
            {
                if ( iv == facet->index[i] )
                {
                    if ( facet->nv == 3 )
                    {
                        if ( ++i == 3 ) i = 0;
                        nat jv = facet->index[i];
                        if ( --narr[jv] < 3 ) bad.inc() = jv;
                        if ( ++i == 3 ) i = 0;
                        jv = facet->index[i];
                        if ( --narr[jv] < 3 ) bad.inc() = jv;
                        flist.delCur();
                        if ( --narr[iv] == 0 ) goto m4;
                        goto m3;
                    }
                    for ( nat j = i; j < facet->nv; ++j ) facet->index[j] = facet->index[j+1];
                    --facet->nv;
                    facet->index[facet->nv] = facet->index[0];
                    if ( --narr[iv] == 0 ) goto m4;
                    break;
                }
            }
        }
        while ( flist.next() );
m4:;}
// Удаление нулевых рёбер
    if ( flist.top() )
    for(;;)
    {
        ListItem<Facet> * facet = flist.cur();
        if ( facet->nv < 3 )
        {
m5:         if ( flist.curIsLas() )
            {
                flist.delCur();
                break;
            }
            else
            {
                flist.delCur();
                continue;
            }
        }
        for ( nat i = 1; i <= facet->nv; ++i )
        {
            if ( facet->index[i] == facet->index[i-1] )
            {
                for ( nat j = i; j < facet->nv; ++j ) facet->index[j] = facet->index[j+1];
                if ( --facet->nv < 3 ) goto m5;
            }
        }
        if ( ! flist.next() ) break;
    }
// Пересчитываем индексы вершин
    nat nv = 0;
    for ( i = 0; i < narr.size(); ++i ) narr[i] = narr[i] > 0 ? nv++ : narr.size();
    if ( nv == narr.size() ) return;
    if ( flist.top() )
    do
    {
        ListItem<Facet> * facet = flist.cur();
        for ( i = 0; i <= facet->nv; ++i ) facet->index[i] = narr[facet->index[i]];
/*
        for ( i = 0; i < facet->nv; ++i ) 
            for ( nat j = i+1; j < facet->nv; ++j )
                if ( facet->index[i] == facet->index[j] )
                    j=j;
*/
    }
    while ( flist.next() );
    DynArray<Vector3d> temp ( nv );
    nv = 0;
    for ( i = 0; i < narr.size(); ++i )
    {
        if ( narr[i] == narr.size() ) continue;
        temp[nv++] = vertex[i];
    }
    _swap ( vertex, temp );
// Коррекция граней
    int info3 = 3000000;
    List< ListItem<ShevList> > plist;
    if ( flist.top() )
    do
    {
        ListItem<Facet> * facet = flist.cur();
        if ( facet->getArea ( vertex ) < 0 ) 
            facet->plane = - facet->plane;
        if ( facet->nv < 4 ) continue;
        vert2d.resize ( facet->nv );
        Func3to2 func = getTrans ( facet->plane.norm );
        for ( nat j = 0; j < facet->nv; ++j ) vert2d[j] = func ( vertex[facet->index[j]] );
        if ( trianTestNat1L1MinTan ( vert2d ) ) continue;
// Разбиваем грань на выпуклые части
        convexParts ( vert2d, plist );
//display << vert2d.size() << plist.size() << NL;
        if ( ! plist.top() ) continue;
        do
        {
            ShevList * p = plist.cur();
//display << p->size() << NL;
            if ( plist.curIsLas() )
            {
                facet->nv = p->size();
                DynArray<nat> index ( 3 * facet->nv + 1 );
                i = 0;
                p->top();
                do index[i++] = facet->index[p->cur()->info]; while ( p->next() );
                index[facet->nv] = index[0];
                _swap ( facet->index, index );
                if ( facet->getArea ( vertex ) < 0 ) 
                    facet->plane = - facet->plane;
            }
            else
            {
                ListItem<Facet> * tr = new ListItem<Facet>;
                tr->resize ( p->size() );
                i = 0;
                p->top();
                do tr->index[i++] = facet->index[p->cur()->info]; while ( p->next() );
                tr->index[tr->nv] = tr->index[0];
                tr->plane = facet->plane;
                if ( tr->getArea ( vertex ) < 0 ) 
                    tr->plane = - tr->plane;
                tr->Facet::info = ++info3;
                flist.addBefFir ( tr );
            }
        }
        while ( plist.next() );
    }
    while ( flist.next() );
}

void makeFacets ( List<ListItem<Set2<Plane3d, DynArray<Vector2d> > > > & glist, double eps, 
                  DynArray<Vector3d> & vertex, List< ListItem<Facet> > & flist )
{
    Suite<nat> index;
    VertListList vll;
    nat nverts = 0;
    if ( glist.top () )
    do
    {
        const Set2<Plane3d, DynArray<Vector2d> > * g = glist.cur();
        const nat n = g->b.size();
        if ( n < 3 ) continue;
        Func2to3 transFunc ( g->a );
        index.resize(0);
        nat i;
        for ( i = 0; i < n; ++i )
        {
            const nat j = getIndex ( vll, nverts, transFunc ( g->b[i] ), eps );
            if ( j == nverts ) ++nverts;
            if ( index.size() == 0 || j != index.las() ) index.inc() = j;
        }
        if ( index.las() == index[0] ) index.dec();
m1:     if ( index.size() < 3 ) continue;
        for ( i = 2; i < index.size(); ++i )
        {
            const nat i1 = i - 1;
            for ( nat j = 0; j < i1; ++j )
            {
                if ( index[i] == index[j] )
                {
                    const nat k = i - j;
                    if ( k > 2 )
                    {
                        ListItem<Facet> * fi = new ListItem<Facet>;
                        Facet & facet = * fi;
                        facet.plane = g->a;
                        facet.info = flist.size();
                        facet.resize ( k );
                        for ( nat l = 0; l < k; ++l ) facet.index[l] = index[l+j];
                        facet.index[facet.nv] = index[j];
                        flist.addAftLas ( fi );
                    }
                    for ( ++i; i+k < index.size(); ++i ) index[i] = index[i+k];
                    index.resize ( index.size() - k );
                    goto m1;
                }
            }
        }
        ListItem<Facet> * fi = new ListItem<Facet>;
        Facet & facet = * fi;
        facet.plane = g->a;
        facet.info = flist.size();
        facet.resize ( index.size() );
        for ( i = 0; i < facet.nv; ++i ) facet.index[i] = index[i];
        facet.index[facet.nv] = index[0];
        flist.addAftLas ( fi );
    }
    while ( glist.next() );
    // Заполняем массив вершин
    vertex.resize ( nverts );
    if ( vll.top () )
    do
    {
        VertList * vl = vll.cur();
        vl->top ();
        do
        {
            const Vert * v = vl->cur();
            vertex[v->info] = *v;
        }
        while ( vl->next() );
    }
    while ( vll.next() );
    // Исправляем ошибки, если они есть
    doctor ( flist, eps, vertex );
}

bool inline near_ ( const Plane3d & plane1, const Plane3d & plane2, double eps )
{
    return fabs ( plane1.dist - plane2.dist ) < eps && normU ( plane1.norm - plane2.norm ) < eps;
}

void checkintersect ( List<ListItem<Set2<Plane3d, DynArray<Vector2d> > > > & b1, 
                      List<ListItem<Set2<Plane3d, DynArray<Vector2d> > > > & b2, double eps )
{
    b1.for_each123 ( InfoAP(0,1) );
    Suite< Suite<Vector2d> > res;
    if ( b1.top() )
    for(;;)
    {
        const Plane3d & plane = b1.cur()->a;
        ArrRef<Vector2d> a1 = b1.cur()->b;
        if ( b2.top() )
        do
        {
            if ( ! near_ ( b2.cur()->a, plane, eps ) ) continue;
            DynArray<Vector2d> & a2 = b2.cur()->b;
            intersection ( a1, a2, res );
            if ( res.size() == 0 ) continue;
            if ( ! intersection ( a1.reverse(), a2.reverse(), res ) )
            {/*
                intersection ( a1, a2, res );
                nat j;
                for ( j = 0; j < a1.size(); ++j ) display << a1[j] << isIntersect ( a1[j], a2 ) << NL;
                display << NL;
                for ( j = 0; j < a2.size(); ++j ) display << a2[j] << isIntersect ( a2[j], a1 ) << NL;
                display << NL;*/
            }
            if ( res.size() == 1 )
            {
                a2 = res[0].reverse();
                for (;;)
                {
                    if ( ! b2.next() ) break;
                    if ( ! near_ ( b2.cur()->a, plane, eps ) ) continue;
                    ArrRef<Vector2d> a3 = b2.cur()->b;
                    intersection ( a3, a2, res );
                    if ( res.size() == 0 ) continue;
                    intersection ( a3.reverse(), a2.reverse(), res );
                    if ( res.size() == 1 )
                    {
                        a2 = res[0].reverse();
                        if ( b2.curIsLas() )
                        {
                            b2.delCur();
                            break;
                        }
                        else
                        {
                            b2.delCur();
                            continue;
                        }
                    }
                    else
                    {
                        a3.reverse();
                        a2.reverse();
                    }
                }
                goto m1;
            }
            else
            {
                a1.reverse();
                a2.reverse();
            }
        }
        while ( b2.next() );
        if ( ! b1.next() ) break;
        continue;
m1:     if ( b1.curIsLas() )
        {
            b1.delCur();
            break;
        }
        else
        {
            b1.delCur();
            continue;
        }
    }
    b2.movAllAftLas ( b1 );
}

bool intersect ( const Polyhedron & poly1, const Polyhedron & poly2, double eps, Polyhedron & sect )
{
    if ( isConvex ( poly1 ) )
        return intersect1c ( poly1, poly2, sect );
    if ( isConvex ( poly2 ) )
        return intersect1c ( poly2, poly1, sect );
    const double shift = 1e-4 * eps;
    List<ListItem<Set2<Plane3d, DynArray<Vector2d> > > > glist1, glist2;
    func ( poly1, poly2, shift, glist1 );
    func ( poly2, poly1, shift, glist2 );
    checkintersect ( glist1, glist2, eps );
    List< ListItem<Facet> > flist;
    makeFacets ( glist1, eps, sect.vertex, flist );
    sect.facet.resize ( flist.size() );
    flist.top();
    for ( nat i = 0; i < sect.facet.size(); ++i )
    {
        _swap ( sect.facet[i], *(Facet*) flist.cur() );
        flist.next();
    }
    sect.linkFacets();
    return true;
}