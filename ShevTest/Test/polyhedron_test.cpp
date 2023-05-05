
#include "math.h"

#include "PolyhedronErrorRecipient.h"
#include "../Shev/AVL_TreeList.h"
#include "../Shev/ShevArray2.h"
#include "../Shev/Polyhedron.h"
#include "../Shev/intersect3d.h"
#include "../Shev/RealFile.h"
#include "../Shev/filePYH.h"
#include "../Shev/LinAlg.h"
#include "../Shev/func3d.h"
#include "../Shev/func1t.h"
#include "../Shev/opti2d.h"
#include "../Shev/opti3d.h"
#include "../Shev/trans.h"
#include "../Shev/rand.h"
#include "../draw.h"
#include "Display.h"

double timeInSec();

namespace
{

void centerOfMass_test()
{
    Polyhedron poly;
//    poly.makeCuboid ( 1, 2, 3 );
//    makeEllipsoid ( 20, 1, 2, 3, poly );
/*
    for ( nat i = 0; i < poly.facet.size(); ++i )
    {
        poly.facet[i].index[3] = poly.facet[i].index[1];
        Vector3d c = centerOfMass ( poly.facet[i], poly.vertex );
        display << c << poly.facet[i].plane % c << NL;
    }
*/
    PRandVector3d rand;
    for ( nat i = 0; i < 9; ++i )
    {
        poly.makeCuboid ( 1, 2, 3 );
        Vector3d v = rand();
        poly += v;
        display << poly.centerOfMass() - v << NL;
    }
}

void makeTetrahedron_test()
{
    Polyhedron poly;
    PolyhedronErrorRecipient rec ( 1e-9, 1e-9 );
    check ( poly.makeTetrahedron ( 0.5 ), rec );
    draw ( poly, 0, 1, 1, 1, VM_WIRE );
    display << volume ( poly ) << NL;
}

void makeOctahedron_test()
{
    Polyhedron poly;
    PolyhedronErrorRecipient rec ( 1e-9, 1e-9 );
    check ( poly.makeOctahedron ( 1 ), rec );
}

void makeModel_test()
{
    Polyhedron poly;
    PolyhedronErrorRecipient rec ( 1e-9, 1e-9 );
    Cone3d fig;
    fig.h = fig.r = 1;
    fig.o = Vector3d(0,0,0);
    check ( poly.makeModel ( fig, 100 ), rec );
}

bool sluGaussRowNS ( ArrRef<Suite<SortItem<nat, double>>> & data )
{
    const nat nRow = data.size();
    if ( ! nRow ) return false;
    Suite<SortItem<nat, double> > temp;
// Прямой ход
    nat i, j, k;
    for ( k = 0; k < nRow; ++k )
    {
// Поиск максимального по модулю члена в k-ой строке
        Suite<SortItem<nat, double> > & rk = data[k];
        const nat nk = rk.size();
        if ( ! nk ) return false;
        nat im = 0;
        double max = fabs ( rk[0].tail );
        for ( i = 1; i < nk; ++i )
        {
            if ( _maxa ( max, fabs ( rk[i].tail ) ) ) im = i;
        }
        _swap ( rk[0], rk[im] );
        SortItem<nat, double> & rkm = rk[0];
// Нормализация строки
        const double p = 1. / rkm.tail;
        for ( i = 1; i < nk; ++i ) rk[i].tail *= p;
        rkm.tail = 1;
// Вычитание строк
        for ( j = k+1; j < nRow; ++j )
        {
            Suite<SortItem<nat, double> > & rj = data[j];
            const nat ij = firEqu ( rj, rkm );
            if ( ij == rj.size() ) continue;
            const double t = rj[ij].tail;
            rj.del(ij);
            for ( i = 1; i < nk; ++i )
            {
                const SortItem<nat, double> & rki = rk[i];
                const double tt = - rki.tail * t;
                const nat ii = firEqu ( rj, rki );
                if ( ii < rj.size() )
                {
                    if ( fabs ( rj[ii].tail += tt ) < 1e-14 * fabs ( tt ) ) rj.del(ii);
                }
                else
                {
                    if ( fabs ( tt ) > 1e-290 ) temp.inc() = SortItem<nat, double> ( rki.head, tt );
                }
            }
            if ( temp.size() > 0 )
            {
                rj.addAftLas ( temp );
                temp.resize();
            }
        }
    }
//return true;
// Обратная подстановка
    for ( k = nRow; --k > 0; )
    {
        Suite<SortItem<nat, double> > & rk = data[k];
        const nat nk = rk.size();
        if ( ! nk ) continue;
        const SortItem<nat, double> si ( rk[0] );
        for ( j = 0; j < k; ++j )
        {
            Suite<SortItem<nat, double> > & rj = data[j];
            const nat ij = firEqu ( rj, si );
            if ( ij == rj.size() ) continue;
            const double t = rj[ij].tail;
            rj.del(ij);
            for ( i = 1; i < nk; ++i )
            {
                const SortItem<nat, double> & rki = rk[i];
                const double tt = - rki.tail * t;
                const nat ii = firEqu ( rj, rki );
                if ( ii < rj.size() )
                {
                    if ( fabs ( rj[ii].tail += tt ) < 1e-14 * fabs ( tt ) ) rj.del(ii);
                }
                else
                {
                    if ( fabs ( tt ) > 1e-290 ) temp.inc() = SortItem<nat, double> ( rki.head, tt );
                }
            }
            if ( temp.size() > 0 )
            {
                rj.addAftLas ( temp );
                temp.resize();
            }
        }
    }
    return true;
}

bool sluGaussRowS ( ArrRef<Suite<SortItem<nat, double>>> & data )
{
    const nat nRow = data.size();
    if ( ! nRow ) return false;
    Suite<SortItem<nat, double> > temp;
// Прямой ход
    nat i, j, k;
    for ( k = 0; k < nRow; ++k ) insertSort123 ( data[k] );
    for ( k = 0; k < nRow; ++k )
    {
// Поиск максимального по модулю члена в k-ой строке
        Suite<SortItem<nat, double> > & rk = data[k];
        const nat nk = rk.size();
        if ( ! nk ) return false;
        nat im = 0;
        double max = fabs ( rk[0].tail );
        for ( i = 1; i < nk; ++i )
        {
            if ( _maxa ( max, fabs ( rk[i].tail ) ) ) im = i;
        }
        if ( im > 0 ) ArrRef<SortItem<nat, double> > ( rk, 0, im+1 ) >>= 1;
        SortItem<nat, double> & rkm = rk[0];
// Нормализация строки
        const double p = 1. / rkm.tail;
        for ( i = 1; i < nk; ++i ) rk[i].tail *= p;
        rkm.tail = 1;
// Вычитание строк
        for ( j = k+1; j < nRow; ++j )
        {
            Suite<SortItem<nat, double> > & rj = data[j];
            const nat ij = lasEqu123 ( rj, rkm );
            if ( ij == rj.size() ) continue;
            const double t = rj[ij].tail;
            rj.delAndShift(ij);
            for ( i = 1; i < nk; ++i )
            {
                const SortItem<nat, double> & rki = rk[i];
                const double tt = - rki.tail * t;
                const nat ii = lasEqu123 ( rj, rki );
                if ( ii < rj.size() )
                {
                    if ( fabs ( rj[ii].tail += tt ) < 1e-14 * fabs ( tt ) ) rj.delAndShift(ii);
                }
                else
                {
                    if ( fabs ( tt ) > 1e-290 ) temp.inc() = SortItem<nat, double> ( rki.head, tt );
                }
            }
            if ( temp.size() > 0 )
            {
                rj.addAftLas ( temp );
                insertSort123 ( rj );
                temp.resize();
            }
        }
    }
//return true;
// Обратная подстановка
    for ( k = nRow; --k > 0; )
    {
        Suite<SortItem<nat, double> > & rk = data[k];
        const nat nk = rk.size();
        if ( ! nk ) continue;
        const SortItem<nat, double> si ( rk[0] );
        for ( j = 0; j < k; ++j )
        {
            Suite<SortItem<nat, double> > & rj = data[j];
            const nat ij = lasEqu123 ( rj, si );
            if ( ij == rj.size() ) continue;
            const double t = rj[ij].tail;
            rj.delAndShift(ij);
            for ( i = 1; i < nk; ++i )
            {
                const SortItem<nat, double> & rki = rk[i];
                const double tt = - rki.tail * t;
                const nat ii = lasEqu123 ( rj, rki );
                if ( ii < rj.size() )
                {
                    if ( fabs ( rj[ii].tail += tt ) < 1e-14 * fabs ( tt ) ) rj.delAndShift(ii);
                }
                else
                {
                    if ( fabs ( tt ) > 1e-290 ) temp.inc() = SortItem<nat, double> ( rki.head, tt );
                }
            }
            if ( temp.size() > 0 )
            {
                rj.addAftLas ( temp );
                insertSort123 ( ArrRef<SortItem<nat, double> > ( rj, 1, rj.size()-1 ) );
                temp.resize();
            }
        }
    }
    return true;
}

bool sluGaussRowT ( ArrRef<AVL_TreeList<nat, double>> & data )
{
    const nat nRow = data.size();
    if ( ! nRow ) return false;
// Прямой ход
    nat i, j, k;
    for ( k = 0; k < nRow; ++k )
    {
// Поиск максимального по модулю члена в k-ой строке
        AVL_TreeList<nat, double> & rk = data[k];
        AVL_TreeList<nat, double>::Node * rkm = rk.first();
        if ( ! rkm ) continue;
        double max = fabs ( rkm->data );
        for (;;)
        {
            AVL_TreeList<nat, double>::Node * node = rk.next();
            if ( ! node ) break;
            if ( _maxa ( max, fabs ( node->data ) ) ) rkm = node;
        }
// Нормализация строки
        const double p = 1. / rkm->data;
        AVL_TreeList<nat, double>::Node * node = rk.first();
        for (;;)
        {
            node->data *= p;
            node = rk.next();
            if ( ! node ) break;
        }
// Вычитание строк
        for ( j = k+1; j < nRow; ++j )
        {
            AVL_TreeList<nat, double> & rj = data[j];
            const double * ij = rj.find ( rkm->key );
            if ( ! ij ) continue;
            const double t = * ij;
            rj.del ( rkm->key );
            for ( i = 1; i < nk; ++i )
            {
                const SortItem<nat, double> & rki = rk[i];
                const double tt = - rki.tail * t;
                const nat ii = lasEqu123 ( rj, rki );
                if ( ii < rj.size() )
                {
                    if ( fabs ( rj[ii].tail += tt ) < 1e-14 * fabs ( tt ) ) rj.delAndShift(ii);
                }
                else
                {
                    if ( fabs ( tt ) > 1e-290 ) temp.inc() = SortItem<nat, double> ( rki.head, tt );
                }
            }
        }
    }
//return true;
// Обратная подстановка
    for ( k = nRow; --k > 0; )
    {
        Suite<SortItem<nat, double> > & rk = data[k];
        const nat nk = rk.size();
        if ( ! nk ) continue;
        const SortItem<nat, double> si ( rk[0] );
        for ( j = 0; j < k; ++j )
        {
            Suite<SortItem<nat, double> > & rj = data[j];
            const nat ij = lasEqu123 ( rj, si );
            if ( ij == rj.size() ) continue;
            const double t = rj[ij].tail;
            rj.delAndShift(ij);
            for ( i = 1; i < nk; ++i )
            {
                const SortItem<nat, double> & rki = rk[i];
                const double tt = - rki.tail * t;
                const nat ii = lasEqu123 ( rj, rki );
                if ( ii < rj.size() )
                {
                    if ( fabs ( rj[ii].tail += tt ) < 1e-14 * fabs ( tt ) ) rj.delAndShift(ii);
                }
                else
                {
                    if ( fabs ( tt ) > 1e-290 ) temp.inc() = SortItem<nat, double> ( rki.head, tt );
                }
            }
            if ( temp.size() > 0 )
            {
                rj.addAftLas ( temp );
                insertSort123 ( ArrRef<SortItem<nat, double> > ( rj, 1, rj.size()-1 ) );
                temp.resize();
            }
        }
    }
    return true;
}

void initPolyhedron ( Polyhedron & poly )
{
    static PRand rand;
    Polyhedron temp;
    //poly.makeTetrahedron ( 1 );
    poly.makeOctahedron ( 1 );
    cut ( poly, Plane3d ( Vector3d (0,0,1), -0.9), temp ); _swap ( temp, poly );
    cut ( poly, Plane3d ( Vector3d (0,0,-1),-0.9), temp ); _swap ( temp, poly );
    cut ( poly, Plane3d ( Vector3d (0,1,0), -0.9), temp ); _swap ( temp, poly );
    cut ( poly, Plane3d ( Vector3d (-1,0,0),-0.9), temp ); _swap ( temp, poly );
    cut ( poly, Plane3d ( Vector3d (1,0,0), -0.9), temp ); _swap ( temp, poly );
    cut ( poly, Plane3d ( Vector3d (0,-1,0),-0.9), temp ); _swap ( temp, poly );
    //poly.makeCube ( 1 );
    poly *= getRandOrtho3d ( rand(), rand(), rand() );
    poly += Vector3d ( rand(), rand(), rand() );
}

void initPolyhedron ( Polyhedron & poly, nat n )
{
    static PRand rand;
    static QRand2Vector3d vrand;
    Polyhedron temp;
    poly.makeCube ( 1 );
    for ( nat i = 0; i < n; ++i )
    {
        cut ( poly, Plane3d ( vrand(), -0.9), temp ); _swap ( temp, poly );
    }
    poly *= getRandOrtho3d ( rand(), rand(), rand() );
    poly += Vector3d ( rand(), rand(), rand() );
}

void fillData ( const Polyhedron & poly, DynArray<Suite<SortItem<nat, double>>> & data, DynArray<double> & x )
{
    const nat nv = poly.vertex.size();
    const nat nf = poly.facet.size();
    nat nr = 0;
    for ( nat i = 0; i < nf; ++i )
    {
        const Facet & f = poly.facet[i];
        nr += f.nv - 1;
    }
    data.resize ( nr );
    for ( nat i = 0, k = 0; i < nf; ++i )
    {
        const Facet & f = poly.facet[i];
        const nat i1 = 3 * f.index[0];
        const nat i2 = i1 + 1;
        const nat i3 = i2 + 1;
        for ( nat j = 1; j < f.nv; ++j )
        {
            Suite<SortItem<nat, double>> & r = data[k++];
            const nat ix = 3 * f.index[j];
            const nat iy = ix + 1;
            const nat iz = iy + 1;
            if ( f.plane.norm.x )
            {
                r.inc() = SortItem<nat, double> ( ix,  f.plane.norm.x );
                r.inc() = SortItem<nat, double> ( i1, -f.plane.norm.x );
            }
            if ( f.plane.norm.y )
            {
                r.inc() = SortItem<nat, double> ( iy,  f.plane.norm.y );
                r.inc() = SortItem<nat, double> ( i2, -f.plane.norm.y );
            }
            if ( f.plane.norm.z )
            {
                r.inc() = SortItem<nat, double> ( iz,  f.plane.norm.z );
                r.inc() = SortItem<nat, double> ( i3, -f.plane.norm.z );
            }
        }
    }
    const nat nc = 3*nv;
    x.resize ( nc );
    for ( nat i = 0; i < nv; ++i )
    {
        const Vector3d & v = poly.vertex[i];
        const nat ix = 3 * i;
        const nat iy = ix + 1;
        const nat iz = iy + 1;
        x[ix] = v.x;
        x[iy] = v.y;
        x[iz] = v.z;
    }
}

void fillData ( const Polyhedron & poly, DynArray2<double> & data, DynArray<double> & x )
{
    const nat nv = poly.vertex.size();
    const nat nf = poly.facet.size();
    nat nr = 0;
    for ( nat i = 0; i < nf; ++i )
    {
        const Facet & f = poly.facet[i];
        nr += f.nv - 1;
    }
    const nat nc = 3*nv;
    DynArray2<double> temp ( nr, nc );
    temp.fill(0);
    for ( nat i = 0, k = 0; i < nf; ++i )
    {
        const Facet & f = poly.facet[i];
        const nat i1 = 3 * f.index[0];
        const nat i2 = i1 + 1;
        const nat i3 = i2 + 1;
        for ( nat j = 1; j < f.nv; ++j )
        {
            ArrRef<double> & r = temp[k++];
            const nat ix = 3 * f.index[j];
            const nat iy = ix + 1;
            const nat iz = iy + 1;
            r[ix] = f.plane.norm.x;
            r[i1] = -f.plane.norm.x;
            r[iy] = f.plane.norm.y;
            r[i2] = -f.plane.norm.y;
            r[iz] = f.plane.norm.z;
            r[i3] = -f.plane.norm.z;
        }
    }
    x.resize ( nc );
    for ( nat i = 0; i < nv; ++i )
    {
        const Vector3d & v = poly.vertex[i];
        const nat ix = 3 * i;
        const nat iy = ix + 1;
        const nat iz = iy + 1;
        x[ix] = v.x;
        x[iy] = v.y;
        x[iz] = v.z;
    }
    temp.swap ( data );
}

void normalizePolyhedron_test()
{
    Polyhedron poly;
    initPolyhedron ( poly, 99 );
    draw ( poly, 0, 1, 1, 1, VM_WIRE );
    DynArray<Suite<SortItem<nat, double>>> data;
    //DynArray2<double> data(0,0);
    DynArray<double> x;
    fillData ( poly, data, x );
    const nat nr = data.size();
    const nat nc = x.size();
    /*if ( 0 )
    {
        for ( nat i = 0; i < nr; ++i )
        {
            Suite<SortItem<nat, double>> & r = data[i];
            double sum = 0;
            for ( nat j = 0; j < r.size(); ++j ) sum += r[j].tail * x[r[j].head];
            //display << i << ( fabs(sum) < 1e-13 ? 0 : sum ) << NL;
            //for ( nat j = 0; j < r.size(); ++j ) display << "["<<r[j].head << "] ="<< r[j].tail; display << NL;
        }
        display << NL;
    }*/
    DynArray<nat> index(nc);
    double t0 = timeInSec();
    //bool ok = sluGaussRow ( data, nr, nc, index(), nr, nc);
    bool ok = sluGaussRowS ( data );
    double t1 = timeInSec();
    /*for ( nat i = 0; i < nr; ++i )
    {
        Suite<SortItem<nat, double>> & r = data[i];
        double sum = 0;
        for ( nat j = 0; j < r.size(); ++j ) sum += r[j].tail * x[r[j].head];
        if ( fabs(sum) > 1e-13 ) display << i << sum << NL;
        //for ( nat j = 0; j < r.size(); ++j ) display << "["<<r[j].head << "] ="<< r[j].tail; display << NL;
    }*/
    display << ok << nr * nc << t1-t0 << NL;
}

void normalizePolyhedron_test2()
{
    Polyhedron poly;
    initPolyhedron ( poly );
    DynArray<Suite<SortItem<nat, double>>> data;
    DynArray<double> x;
    for ( nat k = 0; k < 100; ++k )
    {
        fillData ( poly, data, x );
        const nat nr = data.size();
        const nat nc = x.size();
        bool ok = sluGaussRowNS ( data );
        if (ok) for ( nat i = 0; i < nr; ++i )
        {
            Suite<SortItem<nat, double>> & r = data[i];
            double sum = 0;
            for ( nat j = 0; j < r.size(); ++j ) sum += r[j].tail * x[r[j].head];
            if ( fabs(sum) > 1e-13 ) display << i << sum << NL;
        }
        else display << k << ok << NL;
    }
    display << "end" << NL;
}

void normalizePolyhedron_test3()
{
    Polyhedron poly;
    DynArray2<double> data(0,0);
    DynArray<Suite<SortItem<nat, double>>> data1;
    DynArray<double> x;
    for ( nat k = 0; k < 100; ++k )
    {
        initPolyhedron ( poly, 120 + k );
        fillData ( poly, data1, x );
        fillData ( poly, data, x );
        const nat nr = data1.size();
        const nat nc = x.size();
        DynArray<nat> index(nc);
        double t0 = timeInSec();
        sluGaussRow ( data, nr, nc, index(), nr, nc );
        double t1 = timeInSec();
        sluGaussRowS ( data1 );
        double t2 = timeInSec();
    display << nr * nc << t1-t0 << t2-t1 << (t1-t0) / (t2-t1) << NL;
    }
}

} // end of namespace

void polyhedron_test()
{
    drawNewList2d ();
//    makeWindows_test();
//    makeTetrahedron_test();
//    makeOctahedron_test();
//    centerOfMass_test();
//    makeModel_test();
    normalizePolyhedron_test3();
    endNewList ();
}