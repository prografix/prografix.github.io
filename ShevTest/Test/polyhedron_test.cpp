
#include "math.h"

#include "PolyhedronErrorRecipient.h"
#include "../Shev/AVL_TreeList.h"
#include "../Shev/ShevArray2.h"
#include "../Shev/Polyhedron.h"
#include "../Shev/intersect3d.h"
#include "../Shev/RealFile.h"
#include "../Shev/filePYH.h"
#include "../Shev/LinAlg.h"
#include "../Shev/mathem.h"
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

bool sluGaussRowNS ( nat n, Suite<SortItem<nat, double>> * data, double * b )
{
    Suite<SortItem<nat, double> > temp;
// Прямой ход
    nat i, j, k;
    for ( k = 0; k < n; ++k )
    {
        Suite<SortItem<nat, double> > & rk = data[k];
        const nat nk = rk.size();
        if ( ! nk || rk[0].head != k )
            return false;
        SortItem<nat, double> & rkm = rk[0];
// Нормализация строки
        const double p = 1. / rkm.tail;
        for ( i = 0; i < nk; ++i ) rk[i].tail *= p;
        rkm.tail = 1;
        b[k] *= p;
// Вычитание строк
        for ( j = k+1; j < n; ++j )
        {
            Suite<SortItem<nat, double> > & rj = data[j];
            if ( rj[0].head != k ) continue;
            const double t = rj[0].tail;
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
                rj.del(0);
                rj.addAftLas ( temp );
                insertSort123 ( rj );
                temp.resize();
            }
            else
                rj.delAndShift(0);
            b[j] -= t * b[k];
        }
    }
//return true;
// Обратная подстановка
    for ( k = n; --k > 0; )
    {
        const double d = b[k];
        for ( j = 0; j < k; ++j )
        {
            Suite<SortItem<nat, double> > & rj = data[j];
            const SortItem<nat, double> & si = rj.las();
            if ( si.head != k ) continue;
            b[j] -= si.tail * d;
            rj.dec();
        }
    }
    return true;
}

bool sluGaussRowS2 ( nat n, Suite<SortItem<nat, double>> * data, double * b )
{
    Suite<SortItem<nat, double> > temp;
// Прямой ход
    nat i, j, k;
    for ( k = 0; k < n; ++k )
    {
        Suite<SortItem<nat, double> > & rk = data[k];
        const nat nk = rk.size();
        if ( ! nk || rk[0].head != k )
            return false;
        SortItem<nat, double> & rkm = rk[0];
// Нормализация строки
        const double p = 1. / rkm.tail;
        for ( i = 0; i < nk; ++i ) rk[i].tail *= p;
        rkm.tail = 1;
        b[k] *= p;
// Вычитание строк
        for ( j = k+1; j < n; ++j )
        {
            Suite<SortItem<nat, double> > & rj = data[j];
            if ( rj[0].head != k ) continue;
            const nat nj = rj.size();
            const double t = rj[0].tail;
            temp.resize();
            nat l = 1;
            for ( i = 1;; )
            {
                if ( i == nk )
                {
                    while ( l < nj ) temp.inc() = rj[l++];
                    break;
                }
                if ( l == nj )
                {
                    while ( l < nj ) temp.inc() = rj[l++];
                    break;
                }
                const SortItem<nat, double> & rki = rk[i];
                const double tt = - rki.tail * t;
                SortItem<nat, double> & rjl = rj[l];
                if ( rjl.head < rki.head )
                {
                    temp.inc() = rjl;
                    ++l;
                }
                else
                if ( rjl.head > rki.head )
                {
                    if ( fabs ( tt ) > 1e-290 ) temp.inc() = SortItem<nat, double> ( rki.head, tt );
                    ++i;
                }
                else
                {
                    if ( fabs ( rjl.tail += tt ) > 1e-14 * fabs ( tt ) ) temp.inc() = rjl;
                    ++i;
                    ++l;
                }
            }
            temp.swap ( rj );
            b[j] -= t * b[k];
        }
    }
//return true;
// Обратная подстановка
    for ( k = n; --k > 0; )
    {
        const double d = b[k];
        for ( j = 0; j < k; ++j )
        {
            Suite<SortItem<nat, double> > & rj = data[j];
            const SortItem<nat, double> & si = rj.las();
            if ( si.head != k ) continue;
            b[j] -= si.tail * d;
            rj.dec();
        }
    }
    return true;
}

bool calcSLU2 ( nat k, nat nf, CCArrRef<Set2<DynArray<nat>, Vector3d> > & facet, 
               CCArrRef<Vector3d> & vertex, CCArrRef<Suite<Set2<nat, Vector3d> > > & vp, double * x )
{
    nat i, l;
    DynArray<Suite<SortItem<nat, double>>> data ( k );
    nat n = 0;
    for ( i = 0, k = 0; i < nf; ++i )
    {
        const Set2<DynArray<nat>, Vector3d> & f = facet[i];
        if ( f.a.size() < 4 ) continue;
        const nat i0 = f.a[0];
        CCArrRef<Set2<nat, Vector3d> > & s0 = vp[i0];
        for ( nat j = 1; j < f.a.size(); ++j )
        {
            Suite<SortItem<nat, double>> & r = data[k];
            const nat ii = f.a[j];
            CCArrRef<Set2<nat, Vector3d> > & si = vp[ii];
            for ( l = 0; l < si.size(); ++l )
            {
                const Set2<nat, Vector3d> & sl = si[l];
                const double p = f.b * sl.b;
                if ( fabs ( p ) < 1e-15 )
                    continue;
                SortItem<nat, double> & si = r.inc();
                si.head = sl.a;
                si.tail = p;
            }
            for ( l = 0; l < s0.size(); ++l )
            {
                const Set2<nat, Vector3d> & sl = s0[l];
                const double p = f.b * sl.b;
                if ( fabs ( p ) < 1e-15 )
                    continue;
                const nat m = firEqu ( r, SortItem<nat, double> ( sl.a ) );
                if ( m < r.size() )
                {
                    if ( fabs ( r[m].tail -= p ) < 1e-15 )
                        r.del ( m );
                }
                else
                {
                    SortItem<nat, double> & si = r.inc();
                    si.head = sl.a;
                    si.tail = - p;
                }
            }
            insertSort123 ( r );
n+= r.size();
//display << r.size() << NL;
            x[k] = f.b * ( vertex[ii] - vertex[i0] );
            ++k;
        }
    }
 //display << k << n/double(k) << NL; return false;
    if ( ! sluGaussRowNS ( k, data(), x ) ) return false;
    return true;
}

bool sluGaussRowS ( ArrRef<Suite<SortItem<nat, double>>> & data, ArrRef<nat> & index )
{
    const nat nRow = data.size();
    if ( ! nRow ) return false;
    Suite<SortItem<nat, double> > temp;
    index.fill ( nRow );
    DynArray<nat> index2 ( nRow );
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
        SortItem<nat, double> & rkm = rk[im];
        index[rkm.head] = k;
        index2[k] = rkm.head;
// Нормализация строки
        const double p = 1. / rkm.tail;
        for ( i = 0; i < nk; ++i ) rk[i].tail *= p;
        rkm.tail = 1;
// Вычитание строк
        for ( j = k+1; j < nRow; ++j )
        {
            Suite<SortItem<nat, double> > & rj = data[j];
            const nat ij = lasEqu123 ( rj, rkm );
            if ( ij == rj.size() ) continue;
            const double t = rj[ij].tail;
            rj.delAndShift(ij);
            for ( i = 0; i < nk; ++i )
            {
                if ( i == im ) continue;
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
        const nat ik = index2[k];
        const SortItem<nat, double> si ( ik );
        for ( j = 0; j < k; ++j )
        {
            Suite<SortItem<nat, double> > & rj = data[j];
            const nat ij = lasEqu123 ( rj, si );
            if ( ij == rj.size() ) continue;
            const double t = rj[ij].tail;
            rj.delAndShift(ij);
            for ( i = 0; i < nk; ++i )
            {
                const SortItem<nat, double> & rki = rk[i];
                if ( rki.head == ik ) continue;
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

bool sluGaussRowT ( ArrRef<AVL_TreeList<nat, double>> & data, SuiteRef<Set2<nat> > & index )
{
    index.resize();
    const nat nRow = data.size();
    if ( ! nRow ) return false;
// Прямой ход
    nat j, k;
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
        index.inc() = Set2<nat> ( k, rkm->key );
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
            node = rk.first();
            while ( node )
            {
                const double tt = - node->data * t;
                double * ii = rj.find ( node->key );
                if ( ii )
                {
                    if ( fabs ( *ii += tt ) < 1e-14 * fabs ( tt ) ) rj.del ( node->key );
                }
                else
                {
                    if ( fabs ( tt ) > 1e-290 ) rj.add ( node->key, tt );
                }
                node = rk.next();
            }
        }
    }
//return true;
// Обратная подстановка
    if ( ! index.size() ) return false;
    for ( k = index.size(); --k > 0; )
    {
        const Set2<nat> sk = index[k];
        AVL_TreeList<nat, double> & rk = data[sk.a];
        for ( j = 0; j < k; ++j )
        {
            AVL_TreeList<nat, double> & rj = data[index[j].a];
            const double * ij = rj.find ( sk.b );
            if ( ! ij ) continue;
            const double t = * ij;
            AVL_TreeList<nat, double>::Node * node = rk.first();
            while ( node )
            {
                const double tt = - node->data * t;
                double * ii = rj.find ( node->key );
                if ( ii )
                {
                    if ( fabs ( *ii += tt ) < 1e-14 * fabs ( tt ) ) rj.del ( node->key );
                }
                else
                {
                    if ( fabs ( tt ) > 1e-290 ) rj.add ( node->key, tt );
                }
                node = rk.next();
            }
        }
    }
    return true;
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
//poly.makeSphere ( 1, n );
    poly *= getRandOrtho3d ( rand(), rand(), rand() );
    poly += Vector3d ( rand(), rand(), rand() );
    for ( nat i = 0; i < poly.vertex.size(); ++i ) poly.vertex[i] += 1e-9 * vrand();
}

double maxDif ( CCArrRef<Vector3d> & vert, CCArrRef<Set2<DynArray<nat>, Vector3d> > & facet )
{
    double dif = 0;
    const nat nf = facet.size();
    for ( nat i = 0; i < nf; ++i )
    {
        const Set2<DynArray<nat>, Vector3d> & f = facet[i];
        if ( f.a.size() < 4 ) continue;
        double max = f.b * vert[f.a[0]];
        double min = max;
        for ( nat j = 1; j < f.a.size(); ++j )
        {
            const double t = f.b * vert[f.a[j]];
            _maxa ( max, t );
            _mina ( min, t );
        }
        _maxa ( dif, max - min );
    }
    return dif;
}

bool calcSLU ( nat k, nat nf, CCArrRef<Set2<DynArray<nat>, Vector3d> > & facet, 
               CCArrRef<Vector3d> & vertex, CCArrRef<Suite<Set2<nat, Vector3d> > > & vp, double * x )
{
    nat i, l;
    HMatrix<double> a ( k, k );
    a.fill(0);
    DynArray<double> b ( k );
    for ( i = 0, k = 0; i < nf; ++i )
    {
        const Set2<DynArray<nat>, Vector3d> & f = facet[i];
        if ( f.a.size() < 4 ) continue;
        const nat i0 = f.a[0];
        CCArrRef<Set2<nat, Vector3d> > & s0 = vp[i0];
        for ( nat j = 1; j < f.a.size(); ++j )
        {
            double * r = a[k];
            const nat ii = f.a[j];
            CCArrRef<Set2<nat, Vector3d> > & si = vp[ii];
            for ( l = 0; l < si.size(); ++l )
            {
                const Set2<nat, Vector3d> & sl = si[l];
                r[sl.a] = f.b * sl.b;
            }
            for ( l = 0; l < s0.size(); ++l )
            {
                const Set2<nat, Vector3d> & sl = s0[l];
                r[sl.a] -= f.b * sl.b;
            }
            b[k] = f.b * ( vertex[ii] - vertex[i0] );
            ++k;
        }
    }
    if(0)for ( nat i = 0; i < k; ++i )
    {
        for ( nat j = 0; j < k; ++j ) display << a[i][j];
        display << NL;
    }
    SM_LDLt slu ( k, a );
    return slu.solve ( b(), x );
}

bool normalizePolyhedron ( CCArrRef<Set2<DynArray<nat>, Vector3d> > & facet, ArrRef<Vector3d> & vertex )
{
    const double dif1 = maxDif ( vertex, facet );
    const nat nv = vertex.size();
    const nat nf = facet.size();
    DynArray<Suite<Set2<nat, Vector3d> > > vp ( nv );
    nat i, k = 0;
    for ( i = 0; i < nf; ++i )
    {
        const Set2<DynArray<nat>, Vector3d> & f = facet[i];
        if ( f.a.size() < 4 ) continue;
        const nat i0 = f.a[0];
        for ( nat j = 1; j < f.a.size(); ++j )
        {
            const nat ii = f.a[j];
            vp[ii].inc() = Set2<nat, Vector3d> ( k, f.b );
            vp[i0].inc() = Set2<nat, Vector3d> ( k, -f.b );
            ++k;
        }
    }
double t1 = timeInSec();
    DynArray<double> x ( k );
    if ( ! calcSLU2 ( k, nf, facet, vertex, vp, x() ) ) return false;
double t2 = timeInSec();
    if(0)for ( i = 0; i < k; ++i ) display << x[i] << NL;
    for ( i = 0; i < nv; ++i )
    {
        Vector3d & v = vertex[i];
        CCArrRef<Set2<nat, Vector3d> > & si = vp[i];
        for ( nat l = 0; l < si.size(); ++l )
        {
            const Set2<nat, Vector3d> & sl = si[l];
            v -= sl.b * x[sl.a];
        }
    }
    const double dif2 = maxDif ( vertex, facet );
display << k << dif1 << dif2 << t2-t1 << NL;
    return dif1 > dif2;
}

bool normalizePolyhedron ( Polyhedron & poly )
{
    DynArray<Vector3d> vertex ( * poly.vertex );
    const nat nf = poly.facet.size();
    DynArray<Set2<DynArray<nat>, Vector3d> > facet ( nf );
    for ( nat i = 0; i < nf; ++i )
    {
        const Facet & f = poly.facet[i];
        if ( f.nv < 3 ) continue;
        Set2<DynArray<nat>, Vector3d> & s = facet[i];
        s.b = f.plane.norm;
        s.a.resize ( f.nv );
        for ( nat j = 0; j < f.nv; ++j ) s.a[j] = f.index[j];
    }
    if ( ! normalizePolyhedron ( facet, vertex ) )
        return false;
    for ( nat i = 0; i < nf; ++i )
    {
        Facet & f = poly.facet[i];
        if ( f.nv < 3 ) continue;
        Set2<DynArray<nat>, Vector3d> & s = facet[i];
        if ( f.nv == 3 )
        {
            const Vector3d v1 = vertex[s.a[1]] - vertex[s.a[0]];
            const Vector3d v2 = vertex[s.a[2]] - vertex[s.a[0]];
            SLU2<double> slu;
            slu.aa = v1 * v1; slu.ab = v1 * v2; slu.ac = v1 * s.b;
            slu.ba =  slu.ab; slu.bb = v2 * v2; slu.bc = v2 * s.b;
            double la1, la2;
            if ( slu.gauss (la1, la2) )
            {
                s.b -= la1 * v1 + la2 * v2;
                f.plane.norm = s.b.setNorm2();
            }
        }
        double max = s.b * vertex[s.a[0]];
        double min = max;
        for ( nat j = 1; j < s.a.size(); ++j )
        {
            const double t = s.b * vertex[s.a[j]];
            _maxa ( max, t );
            _mina ( min, t );
        }
        f.plane.dist = -0.5 * ( min + max );
    }
    vertex.swap ( poly.vertex );
    return true;
}

void normalizePolyhedron_test()
{
    Polyhedron poly;
    initPolyhedron ( poly, 10 ); //poly.makeOctahedron(1);
    draw ( poly, 0, 1, 1, 1, VM_WIRE );
    normalizePolyhedron ( poly );
}

void normalizePolyhedron_test2()
{
    Polyhedron poly;
    for ( nat k = 0; k < 9; ++k )
    {
        initPolyhedron ( poly, k );
        normalizePolyhedron ( poly );
    }
    display << "end" << NL;
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
    normalizePolyhedron_test2();
    endNewList ();
}