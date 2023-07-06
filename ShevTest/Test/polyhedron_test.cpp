
#include "math.h"

#include "PolyhedronErrorRecipient.h"
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

void insert ( Suite<SortItem<nat, double> > & a, SortItem<nat, double> & b )
{
    nat n = a.size();
    a.inc();
    while ( n > 0 )
    {
        const nat i = n - 1;
        if ( a[i] <= b ) break;
        a[n] = a[i];
        n = i;
    }
    a[n] = b;
    /*for ( nat i = 1; i < a.size(); ++i )
    {
        if ( a[i-1] > a[i] )
            i=i;
    }*/
}

bool slu_chol ( nat n, Suite<SortItem<nat, double> > * data, const double * b, double * x )
{
//    display << "a" << NL;
    nat i;
    /*
    HMatrix<double> A (n, n), L (n, n);
    A.fill(0);
    for ( nat k = 0; k < n; ++k )
    {
        Suite<SortItem<nat, double> > & ak = data[k];
        const nat nk = ak.size();
        for ( nat j = 0; j < nk; ++j )
        {
            const SortItem<nat, double> & akj = ak[j];
            A[k][akj.head] = A[akj.head][k] = akj.tail;
        }
    }
    if(0)for ( nat k = 0; k < n; ++k )
    {
        double * ak = A[k];
        for ( nat j = 0; j < n; ++j ) display << ak[j];
        display << NL;
    }//*/
    if(0)for ( nat k = 0; k < n; ++k )
    {
        Suite<SortItem<nat, double> > & ak = data[k];
        const nat nk = ak.size();
        for ( nat j = 0; j < nk; ++j )
        {
            const SortItem<nat, double> & akj = ak[j];
            display << akj.head;
        }
        display << NL;
    }
    nat nn1 = 0; for ( nat k = 0; k < n; ++k ) nn1 += data[k].size();
double t1 = timeInSec();
    //display << "b" << NL;
    for ( nat k = 0; k < n; ++k )
    {
        Suite<SortItem<nat, double> > & ak = data[k];
        const nat nk = ak.size();
        if ( ! nk )
            return false;
        double & d = ak[0].tail;
        if ( d <= 0 )
            return false;
        d = sqrt ( d );
        const double t = 1 / d;
        for ( i = 1; i < nk; ++i ) ak[i].tail *= t;
        for ( nat j = 1; j < nk; ++j )
        {
            const SortItem<nat, double> & akj = ak[j];
            for ( i = j; i < nk; ++i )
            {
                const SortItem<nat, double> & aki = ak[i];
                const double s = aki.tail * akj.tail;
                Suite<SortItem<nat, double> > & aj = data[akj.head];
                const nat l = firEqu123 ( aj, aki );
                if ( l < aj.size() )
                {
                    //if ( fabs ( 
                    aj[l].tail -= s;//) < 1e-14 * fabs ( s ) )
                       // aj.delAndShift(l);
                }
                else
                {
                    //aj.inc() = SortItem<nat, double> ( aki.head, - s );
                    //insertSort123 ( aj );
                    insert ( aj, SortItem<nat, double> ( aki.head, - s ) );
                }
            }
        }
    }
double t2 = timeInSec();
    nat nn2 = 0; for ( nat k = 0; k < n; ++k ) nn2 += data[k].size();
    display << n << nn1/double(n) << nn2/double(n) << t2-t1 << NL;
    /*
    L.fill(0);
    for ( nat k = 0; k < n; ++k )
    {
        Suite<SortItem<nat, double> > & ak = data[k];
        const nat nk = ak.size();
        for ( nat j = 0; j < nk; ++j )
        {
            const SortItem<nat, double> & akj = ak[j];
            L[k][akj.head] = akj.tail;
          //  display << akj.head;
        }
       // display << NL;
    }
    if(0)for ( nat k = 0; k < n; ++k )
    {
        double * ak = L[k];
        for ( nat j = 0; j < n; ++j ) display << ak[j];
        display << NL;
    }
    HMatrix<double> T ( n, n ), C ( n, n );
    trans ( L, T );
    multi ( T, L, C );
    if(0)for ( nat k = 0; k < n; ++k )
    {
        double * ak = A[k];
        double * ck = C[k];
        for ( nat j = 0; j < n; ++j ) if ( fabs ( ak[j] - ck[j] ) > 1e-14 )
        display << fabs ( ak[j] - ck[j] ) << NL;
    }
    if(0)for ( nat k = 0; k < n; ++k )
    {
        double * ak = C[k];
        for ( nat j = 0; j < n; ++j ) display << ak[j];
        display << NL;
    }//*/
    return false;
}

bool calcSLU2 ( nat k, nat nf, CCArrRef<Set2<DynArray<nat>, Plane3d> > & facet, 
    CCArrRef<Vector3d> & vertex, CCArrRef<Suite<Set2<nat, Vector3d> > > & vp, double * x )
{
    nat i, l;
    DynArray<Suite<SortItem<nat, double> > > data ( k );
    DynArray<double> b ( k );
    for ( i = 0, k = 0; i < nf; ++i )
    {
        const Set2<DynArray<nat>, Plane3d> & f = facet[i];
        if ( f.a.size() < 4 ) continue;
        const nat i0 = f.a[0];
        CCArrRef<Set2<nat, Vector3d> > & s0 = vp[i0];
        for ( nat j = 1; j < f.a.size(); ++j )
        {
            Suite<SortItem<nat, double> > & r = data[k];
            const nat ii = f.a[j];
            CCArrRef<Set2<nat, Vector3d> > & si = vp[ii];
            for ( l = 0; l < si.size(); ++l )
            {
                const Set2<nat, Vector3d> & sl = si[l];
                if ( sl.a < k ) continue;
                const double p = f.b.norm * sl.b;
                if ( fabs ( p ) < 1e-15 )
                    continue;
                SortItem<nat, double> & si = r.inc();
                si.head = sl.a;
                si.tail = p;
            }
            for ( l = 0; l < s0.size(); ++l )
            {
                const Set2<nat, Vector3d> & sl = s0[l];
                if ( sl.a < k ) continue;
                const double p = f.b.norm * sl.b;
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
            b[k] = f.b.norm * ( vertex[ii] - vertex[i0] );
            ++k;
        }
    }
    for ( i = 0; i < k; ++i ) insertSort123 ( data[i] );
    return slu_chol ( k, data(), b(), x );
}

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

void initPolyhedron ( Polyhedron & poly, nat n )
{
    static PRand rand;
    static QRand2Vector3d vrand;
    Polyhedron temp;
    poly.makeCube ( 1 );
    //poly.makeTetrahedron ( 1 );
    for ( nat i = 0; i < n; ++i )
    {
        cut ( poly, Plane3d ( vrand(), -0.9), temp ); _swap ( temp, poly );
    }
    poly *= getRandOrtho3d ( rand(), rand(), rand() );
    poly += Vector3d ( rand(), rand(), rand() );
    for ( nat i = 0; i < poly.vertex.size(); ++i ) poly.vertex[i] += 1e-3 * vrand();
}

double maxDif ( const Polyhedron & poly )
{
    double dif = 0;
    const nat nf = poly.facet.size();
    for ( nat i = 0; i < nf; ++i )
    {
        const Facet & f = poly.facet[i];
        if ( ! f.nv ) continue;
        double max = fabs ( f.plane % poly.vertex[f.index[0]] );
        for ( nat j = 1; j < f.nv; ++j )
        {
            const double t = fabs ( f.plane % poly.vertex[f.index[j]] );
            _maxa ( max, t );
        }
        _maxa ( dif, max );
    }
    return dif;
}

void normalizePolyhedron_test()
{
    Polyhedron poly;
    initPolyhedron ( poly, 2 ); //poly.makeOctahedron(1);
    draw ( poly, 0, 1, 1, 1, VM_WIRE );
    normalize ( poly );
}

void normalizePolyhedron_test2()
{
    Polyhedron poly;
    for ( nat k = 300; k < 500; ++k )
    {
        initPolyhedron ( poly, k );
        double d1 = maxDif ( poly );
double t1 = timeInSec();
        normalize ( poly );
double t2 = timeInSec();
        double d2 = maxDif ( poly );
    display << k << poly.facet.size() + poly.vertex.size() << d1 << d2 << t2-t1 << NL;
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