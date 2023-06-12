
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

bool slu_chol2 ( nat n, Suite<SortItem<nat, double> > * data, const double * b, double * x )
{
    nat i;
    HMatrix<double> A ( n, n ), L ( n, n );
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
    }
    for ( nat k = 0; k < n; ++k )
    {
        Suite<SortItem<nat, double> > & ak = data[k];
        const nat nk = ak.size();
        if ( ! nk ) return false;
        double & d = ak[0].tail;
        if ( d <= 0 ) return false;
        d = sqrt ( d );
        for ( i = 1; i < nk; ++i ) ak[i].tail /= d;
        for ( nat j = 1; j < nk; ++j )
        {
            const SortItem<nat, double> & akj = ak[j];
            Suite<SortItem<nat, double> > & aj = data[akj.head];
            for ( i = j; i < nk; ++i )
            {
                const SortItem<nat, double> & aki = ak[i];
                nat l = firEqu ( aj, aki );
                if ( l < aj.size() )
                {
                    aj[l].tail -= aki.tail * akj.tail;
                }
                else
                {
                    aj.inc() = SortItem<nat, double> ( aki.head, - aki.tail * akj.tail );
                }
            }
        }
    }
    L.fill(0);
    for ( nat k = 0; k < n; ++k )
    {
        Suite<SortItem<nat, double> > & ak = data[k];
        const nat nk = ak.size();
        for ( nat j = 0; j < nk; ++j )
        {
            const SortItem<nat, double> & akj = ak[j];
            L[k][akj.head] = akj.tail;
        }
    }
    HMatrix<double> T ( n, n ), C ( n, n );
    trans ( L, T );
    multi ( L, T, C );
    for ( nat k = 0; k < n; ++k )
    {
        double * ak = C[k];
        for ( nat j = 0; j < n; ++j ) display << ak[j];
        display << NL;
    }
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
    if(0)for ( i = 0; i < k; ++i )
    {
        insertSort123 ( data[i] );
        Suite<SortItem<nat, double> > & r = data[i];
        for ( l = 0; l < r.size(); ++l ) display << r[l].head;
        display << NL;
    }
    return slu_chol2 ( k, data(), b(), x );
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
    poly.makeTetrahedron ( 1 );
    for ( nat i = 0; i < n; ++i )
    {
        cut ( poly, Plane3d ( vrand(), -0.9), temp ); _swap ( temp, poly );
    }
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

void normalizePolyhedron_test()
{
    Polyhedron poly;
    initPolyhedron ( poly, 1 ); //poly.makeOctahedron(1);
    draw ( poly, 0, 1, 1, 1, VM_WIRE );
    normalize ( poly );
}

void normalizePolyhedron_test2()
{
    Polyhedron poly;
    for ( nat k = 0; k < 9; ++k )
    {
        initPolyhedron ( poly, k );
        normalize ( poly );
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
    normalizePolyhedron_test();
    endNewList ();
}