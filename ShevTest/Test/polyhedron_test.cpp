
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

void initPolyhedron ( Polyhedron & poly, nat n )
{
    static PRand rand;
    static QRand2Vector3d vrand;
    Polyhedron temp;
    //poly.makeCube ( 1 );
    //poly.makeTetrahedron ( 1 );
    poly.makeOctahedron ( 1 );
    //poly.makePyramid ( 1 );
        cut ( poly, Plane3d ( Vector3d(0,0,1), -0.6), temp); _swap (temp, poly);
    for ( nat i = 0; i < n; ++i )
    {
        cut ( poly, Plane3d ( vrand(), -0.6), temp ); _swap ( temp, poly );
    }
    poly *= getRandOrtho3d ( rand(), rand(), rand() );
    poly += Vector3d ( 0.2*rand(), 0.2*rand(), 0.2*rand() );
    for ( nat i = 0; i < poly.vertex.size(); ++i ) poly.vertex[i] += 1e-2 * vrand();
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

double stdDif ( CCArrRef<Vector3d> & vert1, CCArrRef<Vector3d> & vert2 )
{
    double sum = 0;
    for ( nat i = 0; i < vert1.size(); ++i )
    {
        sum += qmod ( vert1[i] - vert2[i] );
    }
    return sum / vert1.size();
}

void normalizePolyhedron_test()
{
    Polyhedron poly;
    initPolyhedron ( poly, 40 ); //poly.makeOctahedron(1);
    double d1 = maxDif ( poly );
    DynArray<Vector3d> vert1 ( *poly.vertex );
    draw ( poly, 0, 1, 1, 1, VM_WIRE );
    normalizeV2 ( poly );
    draw ( poly, 1, 1, 0, 1, VM_WIRE );
    double s = stdDif ( vert1, poly.vertex );
    double d2 = maxDif ( poly );
    display << d1 << d2 << s << NL;
}

void normalizePolyhedron_test2()
{
    Polyhedron poly;
    for ( nat k = 10; k < 19; ++k )
    {
        initPolyhedron ( poly, 100*k );
        double d1 = maxDif ( poly );
double t1 = timeInSec();
        normalizeV2 ( poly );
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
    normalizePolyhedron_test();
    endNewList ();
}