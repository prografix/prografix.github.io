
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

static double inprod ( nat n, const double * x, const double * y )
{
    double s = 0;
    for ( nat i = 0; i < n; ++i ) s += x[i] * y[i];
    return s;
}

static void op ( nat n, const Suite<SortItem<nat, double>> * data, const double * x, double * r )
{
    for ( nat i = 0; i < n; ++i )
    {
        CCArrRef<SortItem<nat, double> > & ri = data[i];
        const nat ni = ri.size();
        double & s = r[i];
        s = 0;
        for ( nat j = 0; j < ni; ++j )
        {
             const SortItem<nat, double> & rij = ri[j];
             s += rij.tail * x[rij.head];
        }
    }
}

bool calcSLU2 ( nat k, nat nf, CCArrRef<Set2<DynArray<nat>, Vector3d> > & facet, 
               CCArrRef<Vector3d> & vertex, CCArrRef<Suite<Set2<nat, Vector3d> > > & vp, double * x )
{
    nat i, l;
    DynArray<Suite<SortItem<nat, double>>> data ( k );
    DynArray<double> b ( k );
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
            b[k] = f.b * ( vertex[ii] - vertex[i0] );
            ++k;
        }
    }
    slu_cg ( k, data(), b(), x );
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
        double max = f.plane % poly.vertex[f.index[0]];
        double min = max;
        for ( nat j = 1; j < f.nv; ++j )
        {
            const double t = f.plane % poly.vertex[f.index[j]];
            _maxa ( max, t );
            _mina ( min, t );
        }
        _maxa ( dif, max - min );
    }
    return dif;
}

bool calcSLU1 ( nat k, nat nf, CCArrRef<Set2<DynArray<nat>, Vector3d> > & facet, 
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

void normalizePolyhedron_test()
{
    Polyhedron poly;
    initPolyhedron ( poly, 1 ); //poly.makeOctahedron(1);
    draw ( poly, 0, 1, 1, 1, VM_WIRE );
    double d1 = maxDif ( poly );
    double t1 = timeInSec();
    normalize ( poly );
    double t2 = timeInSec();
    double d2 = maxDif ( poly );
    display << d1 << d2 << t2-t1 << NL;
}

void normalizePolyhedron_test2()
{
    Polyhedron poly;
    for ( nat k = 100; k < 199; ++k )
    {
        initPolyhedron ( poly, k );
        double d1 = maxDif ( poly );
        double t1 = timeInSec();
        normalize ( poly );
        double t2 = timeInSec();
        double d2 = maxDif ( poly );
        display << k << d1 << d2 << t2-t1 << NL;
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