
#include <math.h>
#include "../draw.h"
#include "../Shev/tune.h"
#include "../Shev/lists.h"
#include "../Shev/func3d.h"
#include "../Shev/trian3d.h"
#include "../Shev/AVL_Tree.h"
#include "../Shev/Poly3gon.h"
#include "../Shev/VertFunc.h"
#include "../Shev/Polyhedron.h"
#include "../Shev/Polyhedron_Poly3gon.h"
#include "../Shev/MarchingCubes.h"
#include "../Shev/Vector3f.h"
#include "../Shev/ShevArray2.h"
#include "../Shev/realfile.h"
#include "../Shev/fileSTL.h"
//#include "../Shev/ioSTL.h"
#include "PolyhedronErrorRecipient.h"

#include "display.h"

double timeInSec();
void simplify ( Poly3gon<Vector3d, void> & poly3gon );
bool loadSTL ( const char * filename, Polyhedron & poly );
bool saveSTLB ( const char * filename, const Polyhedron & poly );

namespace
{

void testMarchingCubes()
{
    bool node[4][4][4];
    for ( nat i1 = 0; i1 < 4; ++i1 )
    {
        for ( nat i2 = 0; i2 < 4; ++i2 )
        {
            for ( nat i3 = 0; i3 < 4; ++i3 )
            {
                node[i1][i2][i3] = false;
            }
        }
    }
    Vector3d vert[12];
    Suite<Vector3d> vtemp; 
    Suite<Set3<nat> > ntemp;
    List<ListItem<Segment3d> > list, stor;
    MarchingCubes1 mc;
    for ( nat code = 1; code < 255; ++code )
    {
        node[1][1][1] = ( code & 1 ) != 0;
        node[2][1][1] = ( code & 2 ) != 0;
        node[2][2][1] = ( code & 4 ) != 0;
        node[1][2][1] = ( code & 8 ) != 0;
        node[1][1][2] = ( code & 16 ) != 0;
        node[2][1][2] = ( code & 32 ) != 0;
        node[2][2][2] = ( code & 64 ) != 0;
        node[1][2][2] = ( code & 128 ) != 0;
        for ( nat k = 0; k < 3; ++k )
        {
            const nat z0 = k;
            const nat z1 = k + 1;
            for ( nat j = 0; j < 3; ++j )
            {
                const nat y0 = j;
                const nat y1 = j + 1;
                for ( nat i = 0; i < 3; ++i )
                {
                    const nat x0 = i;
                    const nat x1 = i + 1;
//         4-----7        +-7---+        +-----+
//        /|    /|       4|    6|       /8    /1
//       5-----6 |      +---5-+ |      +-----+ 1
//       | 0---|-3      | +-3-|-+      9 +---1-+
//       |/    |/       |0    |2       |/    0/
//       1-----2        +---1-+        +-----+
                    vert[0] = Vector3d ( x0 + 0.5, y0, z0 );
                    vert[1] = Vector3d ( x1, y0 + 0.5, z0 );
                    vert[2] = Vector3d ( x0 + 0.5, y1, z0 );
                    vert[3] = Vector3d ( x0, y0 + 0.5, z0 );
                    vert[4] = Vector3d ( x0 + 0.5, y0, z1 );
                    vert[5] = Vector3d ( x1, y0 + 0.5, z1 );
                    vert[6] = Vector3d ( x0 + 0.5, y1, z1 );
                    vert[7] = Vector3d ( x0, y0 + 0.5, z1 );
                    vert[8] = Vector3d ( x0, y0, z0 + 0.5 );
                    vert[9] = Vector3d ( x1, y0, z0 + 0.5 );
                    vert[10] = Vector3d ( x1, y1, z0 + 0.5 );
                    vert[11] = Vector3d ( x0, y1, z0 + 0.5 );
                    nat code2 = 0;
                    if ( node[x0][y0][z0] ) code2 |= 1;
                    if ( node[x1][y0][z0] ) code2 |= 2;
                    if ( node[x1][y1][z0] ) code2 |= 4;
                    if ( node[x0][y1][z0] ) code2 |= 8;
                    if ( node[x0][y0][z1] ) code2 |= 16;
                    if ( node[x1][y0][z1] ) code2 |= 32;
                    if ( node[x1][y1][z1] ) code2 |= 64;
                    if ( node[x0][y1][z1] ) code2 |= 128;
                    CArrRef<DynArray<nat> > itog = mc[code2];
//if(code2) display << x0 << y0 << z0 << NL;
                    for ( nat l = 0; l < itog.size(); ++l )
                    {
						nat m;
                        CArrRef<nat> index = itog[l];
                        vtemp.resize ( index.size() );
                        for ( m = 0; m < vtemp.size(); ++m ) vtemp[m] = vert[index[m]];
                        trian ( vtemp, ntemp );
                        for ( m = 0; m < ntemp.size(); ++m )
                        {
                            const Set3<nat> & sj = ntemp[m];
                            Set3<Vector3d> sv;
                            sv.a = vtemp[sj.a];
                            sv.b = vtemp[sj.b];
                            sv.c = vtemp[sj.c];
                            put ( Segment3d ( sv.a, sv.b ), list, stor );
                            put ( Segment3d ( sv.b, sv.c ), list, stor );
                            put ( Segment3d ( sv.c, sv.a ), list, stor );
                        }
                    }
                }
            }
        }
        display << code << list.size() << NL;
        if ( list.top() )
        do
        {
            display << list.cur()->a << list.cur()->b << NL;
        }
        while ( list.next() );
        list.movAllAftCur ( stor );
    }
        display << "end" << NL;
}

void testMarchingCubes2()
{
    bool node[4][4][4];
    for ( nat i1 = 0; i1 < 4; ++i1 )
    {
        for ( nat i2 = 0; i2 < 4; ++i2 )
        {
            for ( nat i3 = 0; i3 < 4; ++i3 )
            {
                node[i1][i2][i3] = false;
            }
        }
    }
    Vector3d vert[12];
    Vector3d vtemp[3]; 
    List<ListItem<Segment3d> > list, stor;
    MarchingCubes2 mc;
    for ( nat code = 1; code < 255; ++code )
    {
        node[1][1][1] = ( code & 1 ) != 0;
        node[2][1][1] = ( code & 2 ) != 0;
        node[2][2][1] = ( code & 4 ) != 0;
        node[1][2][1] = ( code & 8 ) != 0;
        node[1][1][2] = ( code & 16 ) != 0;
        node[2][1][2] = ( code & 32 ) != 0;
        node[2][2][2] = ( code & 64 ) != 0;
        node[1][2][2] = ( code & 128 ) != 0;
        for ( nat k = 0; k < 3; ++k )
        {
            const nat z0 = k;
            const nat z1 = k + 1;
            for ( nat j = 0; j < 3; ++j )
            {
                const nat y0 = j;
                const nat y1 = j + 1;
                for ( nat i = 0; i < 3; ++i )
                {
                    const nat x0 = i;
                    const nat x1 = i + 1;
//         4-----7        +-7---+        +-----+
//        /|    /|       4|    6|       /8    /1
//       5-----6 |      +---5-+ |      +-----+ 1
//       | 0---|-3      | +-3-|-+      9 +---1-+
//       |/    |/       |0    |2       |/    0/
//       1-----2        +---1-+        +-----+
                    vert[0] = Vector3d ( x0 + 0.5, y0, z0 );
                    vert[1] = Vector3d ( x1, y0 + 0.5, z0 );
                    vert[2] = Vector3d ( x0 + 0.5, y1, z0 );
                    vert[3] = Vector3d ( x0, y0 + 0.5, z0 );
                    vert[4] = Vector3d ( x0 + 0.5, y0, z1 );
                    vert[5] = Vector3d ( x1, y0 + 0.5, z1 );
                    vert[6] = Vector3d ( x0 + 0.5, y1, z1 );
                    vert[7] = Vector3d ( x0, y0 + 0.5, z1 );
                    vert[8] = Vector3d ( x0, y0, z0 + 0.5 );
                    vert[9] = Vector3d ( x1, y0, z0 + 0.5 );
                    vert[10] = Vector3d ( x1, y1, z0 + 0.5 );
                    vert[11] = Vector3d ( x0, y1, z0 + 0.5 );
                    nat code2 = 0;
                    if ( node[x0][y0][z0] ) code2 |= 1;
                    if ( node[x1][y0][z0] ) code2 |= 2;
                    if ( node[x1][y1][z0] ) code2 |= 4;
                    if ( node[x0][y1][z0] ) code2 |= 8;
                    if ( node[x0][y0][z1] ) code2 |= 16;
                    if ( node[x1][y0][z1] ) code2 |= 32;
                    if ( node[x1][y1][z1] ) code2 |= 64;
                    if ( node[x0][y1][z1] ) code2 |= 128;
                    CArrRef<Set3<nat> > itog = mc[code2];
//if(code2) display << x0 << y0 << z0 << NL;
                    for ( nat l = 0; l < itog.size(); ++l )
                    {
                        const Set3<nat> index = itog[l];
                        Set3<Vector3d> sv;
                        sv.a = vert[index.a];
                        sv.b = vert[index.b];
                        sv.c = vert[index.c];
                        put ( Segment3d ( sv.a, sv.b ), list, stor );
                        put ( Segment3d ( sv.b, sv.c ), list, stor );
                        put ( Segment3d ( sv.c, sv.a ), list, stor );
                    }
                }
            }
        }
        display << code << list.size() << NL;
        /*if ( list.top() )
        do
        {
            display << list.cur()->a << list.cur()->b << NL;
        }
        while ( list.next() );*/
        list.movAllAftCur ( stor );
    }
        display << "end" << NL;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////


void fill ( CArrRef<Vector2d> poly, double r, Suite<Cylinder> & cylinder, Suite<Sphere> & sphere )
{
    Vector2d a = poly.las();
    for ( nat i = 0; i < poly.size(); ++i )
    {
        Vector2d b = poly[i];
        sphere.inc() = Sphere ( Vector3d ( a.x, a.y, 0. ), r );
        cylinder.inc() = Cylinder ( Vector3d ( a.x, a.y, 0. ), Vector3d ( b.x, b.y, 0. ), r );
        a = b;
    }
}

class SceneFunc1 : public IVertFunc
{
    LayerFunc layerX, layerY, layerZ;
    Suite<Cylinder> cylinder, ncylinder;
    Suite<Sphere> sphere, nsphere;
    Suite<Torus> torus;
    AndNotFunc andNot;
    AndFunc and;
    OrFunc or;
public:
    SceneFunc1() : 
    layerX ( Plane3d ( Vector3d ( 1, 0, 0 ), -0.75 ), Plane3d ( Vector3d (-1, 0, 0 ), -0.75 ) ),
    layerY ( Plane3d ( Vector3d ( 0, 1, 0 ), -0.75 ), Plane3d ( Vector3d ( 0,-1, 0 ), -0.75 ) ),
    layerZ ( Plane3d ( Vector3d ( 0, 0, 1 ), 0 ), Plane3d ( Vector3d ( 0, 0,-1 ), -0.1 ) )
    {
        FixArray<Vector2d, 4> quad;
        double a = 0.2, b = 0.4;
        quad[0] = Vector2d ( b+a, b+a );
        quad[1] = Vector2d ( b-a, b+a );
        quad[2] = Vector2d ( b-a, b-a );
        quad[3] = Vector2d ( b+a, b-a );
        Vector2d c ( b, b );
        nat i;
        for ( i = 0; i < 4; ++i )
        {
            if ( i % 2 )
            {
                //fill ( quad, 0.03, cylinder, sphere );
                torus.inc() = Torus ( Vector3d ( c.x, c.y, 0 ), 0.25, 0.03 );
            }
            else
                fill ( quad, 0.03, ncylinder, nsphere );
            quad *= Spin2d::d090;
            c *= Spin2d::d090;
        }
        sphere.inc() = Sphere ( Vector3d ( 0, 0, -0.01 ), 0.07 );
        for ( i = 0; i < nsphere.size(); ++i ) andNot.array.inc() = nsphere(i);
        for ( i = 0; i < ncylinder.size(); ++i ) andNot.array.inc() = ncylinder(i);
        and.array.inc() = &layerX;
        and.array.inc() = &layerY;
        and.array.inc() = &layerZ;
        and.array.inc() = &andNot;
        or.array.inc() = &and;
        for ( i = 0; i < torus.size(); ++i ) or.array.inc() = torus(i);
        for ( i = 0; i < sphere.size(); ++i ) or.array.inc() = sphere(i);
        for ( i = 0; i < cylinder.size(); ++i ) or.array.inc() = cylinder(i);
    }
    bool operator() ( const Vector3d & v )
    {
        return or ( v );
    }
    bool operator() ( const Vector3d & va, const Vector3d & vb, Vector3d & vc )
    {
        return or ( va, vb, vc );
    }
};

class SceneFunc2 : public IVertFunc
{
    LayerFunc layerX, layerY, layerZ;
    Suite<Torus> torus;
    AndFunc and;
    OrFunc or;
public:
    SceneFunc2() : 
    layerX ( Plane3d ( Vector3d ( 1, 0, 0 ), -0.75 ), Plane3d ( Vector3d (-1, 0, 0 ), -0.75 ) ),
    layerY ( Plane3d ( Vector3d ( 0, 1, 0 ), -0.75 ), Plane3d ( Vector3d ( 0,-1, 0 ), -0.75 ) ),
    layerZ ( Plane3d ( Vector3d ( 0, 0, 1 ), 0 ), Plane3d ( Vector3d ( 0, 0,-1 ), -0.1 ) )
    {
        torus.inc() = Torus ( null3d, 0.2, 0.03 );
        and.array.inc() = &layerX;
        and.array.inc() = &layerY;
        and.array.inc() = &layerZ;
        or.array.inc() = &and;
        or.array.inc() = torus();
    }
    bool operator() ( const Vector3d & v )
    {
        return or ( v );
    }
    bool operator() ( const Vector3d & va, const Vector3d & vb, Vector3d & vc )
    {
        return or ( va, vb, vc );
    }
};

nat32 encode ( nat x, nat y, nat z, nat e )
{
    switch ( e )
    {
    case 1: ++x; break;
    case 2: e = 0; ++y; break;
    case 3: e = 1; break;
    case 4: e = 0; ++z; break;
    case 5: e = 1; ++x; ++z; break;
    case 6: e = 0; ++y; ++z; break;
    case 7: e = 1; ++z; break;
    case 8: e = 2; break;
    case 9: e = 2; ++x; break;
    case 10: e = 2; ++x; ++y; break;
    case 11: e = 2; ++y; break;
    }
    nat32 code = x;
    code |= y << 10;
    code |= z << 20;
    code |= e << 30;
    return code;
}

//         4-----7        +-7---+        +-----+
//        /|    /|       4|    6|       /8    /1
//       5-----6 |      +---5-+ |      +-----+ 1
//       | 0---|-3      | +-3-|-+      9 +---1-+
//       |/    |/       |0    |2       |/    0/
//       1-----2        +---1-+        +-----+

nat getCode ( CArrRef2<bool> arr0, CArrRef2<bool> arr1, nat y0, nat z0 )
{
    const nat y1 = y0 + 1;
    const nat z1 = z0 + 1;
    bool flag[8];
    flag[0] = arr0[y0][z0];
    flag[1] = arr1[y0][z0];
    flag[2] = arr1[y1][z0];
    flag[3] = arr0[y1][z0];
    flag[4] = arr0[y0][z1];
    flag[5] = arr1[y0][z1];
    flag[6] = arr1[y1][z1];
    flag[7] = arr0[y1][z1];
    nat code = 0;
    for ( nat i = 0; i < 8; ++i )
    {
        if ( flag[i] ) code |= 1 << i;
    }
    return code;
}

void fillVert ( IVertFunc & func, nat code, const Vector3d & v0, const Vector3d & v6, Vector3d vert[12] )
{
    bool flag[8];
    for ( nat i = 0; i < 8; ++i )
    {
        flag[i] = ( code & ( 1 << i ) ) != 0;
    }
    const Vector3d v1 ( v6.x, v0.y, v0.z );
    const Vector3d v2 ( v6.x, v6.y, v0.z );
    const Vector3d v3 ( v0.x, v6.y, v0.z );
    const Vector3d v4 ( v0.x, v0.y, v6.z );
    const Vector3d v5 ( v6.x, v0.y, v6.z );
    const Vector3d v7 ( v0.x, v6.y, v6.z );
    if ( flag[0] != flag[1] ) flag[0] ? func ( v0, v1, vert[ 0] ) : func ( v1, v0, vert[ 0] );
    if ( flag[1] != flag[2] ) flag[1] ? func ( v1, v2, vert[ 1] ) : func ( v2, v1, vert[ 1] );
    if ( flag[2] != flag[3] ) flag[2] ? func ( v2, v3, vert[ 2] ) : func ( v3, v2, vert[ 2] );
    if ( flag[3] != flag[0] ) flag[3] ? func ( v3, v0, vert[ 3] ) : func ( v0, v3, vert[ 3] );
    if ( flag[4] != flag[5] ) flag[4] ? func ( v4, v5, vert[ 4] ) : func ( v5, v4, vert[ 4] );
    if ( flag[5] != flag[6] ) flag[5] ? func ( v5, v6, vert[ 5] ) : func ( v6, v5, vert[ 5] );
    if ( flag[6] != flag[7] ) flag[6] ? func ( v6, v7, vert[ 6] ) : func ( v7, v6, vert[ 6] );
    if ( flag[7] != flag[4] ) flag[7] ? func ( v7, v4, vert[ 7] ) : func ( v4, v7, vert[ 7] );
    if ( flag[0] != flag[4] ) flag[0] ? func ( v0, v4, vert[ 8] ) : func ( v4, v0, vert[ 8] );
    if ( flag[1] != flag[5] ) flag[1] ? func ( v1, v5, vert[ 9] ) : func ( v5, v1, vert[ 9] );
    if ( flag[2] != flag[6] ) flag[2] ? func ( v2, v6, vert[10] ) : func ( v6, v2, vert[10] );
    if ( flag[3] != flag[7] ) flag[3] ? func ( v3, v7, vert[11] ) : func ( v7, v3, vert[11] );
}

void recon ( IVertFunc & func, Poly3gon<Vector3d, void> & poly3gon )
{
    const MarchingCubes2 mc;
    double dx, dy, dz;
    nat n = 200, l;
    int i, j, k, ni = n, nj = n, nk = n/9;
    dx = dy = dz = ( 1. - 1e-6 ) / n;
    Vector3d vert[12];
    Suite<Vector3d> vertex;
    Suite<Trigon<void> > facet;
    AVL_Tree<SortItem<nat32,nat> > tree;
    DynArray2<bool> arr0 ( nj+nj+1, nk+nk+1 );
    DynArray2<bool> arr1 ( nj+nj+1, nk+nk+1 );
    for ( i = -ni; i <= ni; ++i )
    {
        const int ii = ni + i;
        const double x1 = dx * i;
        for ( j = -nj; j <= nj; ++j )
        {
            const double y = dy * j;
            ArrRef<bool> & arr = arr1[nj+j];
            for ( k = -nk; k <= nk; ++k )
            {
                const double z = dz * k;
                arr[nk+k] = func ( Vector3d ( x1, y, z ) );
            }
        }
        if ( i > -ni )
        {
            const double x0 = x1 - dx;
            for ( j = -nj; j < nj; ++j )
            {
                const int jj = nj + j;
                const double y0 = dy * j;
                const double y1 = dy + y0;
                for ( k = -nk; k < nk; ++k )
                {
                    const int kk = nk + k;
                    const double z0 = dz * k;
                    const double z1 = dz + z0;
                    const nat code = getCode ( arr0, arr1, jj, kk );
                    fillVert ( func, code, Vector3d ( x0, y0, z0 ), Vector3d ( x1, y1, z1 ), vert );
                    CArrRef<Set3<nat> > itog = mc[code];
                    SortItem<nat32,nat> si1, si2;
                    for ( l = 0; l < itog.size(); ++l )
                    {
                        const Set3<nat> index = itog[l];
                        si1.head = encode ( ii, jj, kk, index.a );
                        si1.tail = vertex.size();
                        si2 = tree.add ( si1 );
                        if ( si1.tail == si2.tail ) vertex.inc() = vert[index.a];
                        const nat a = si2.tail;
                        si1.head = encode ( ii, jj, kk, index.b );
                        si1.tail = vertex.size();
                        si2 = tree.add ( si1 );
                        if ( si1.tail == si2.tail ) vertex.inc() = vert[index.b];
                        const nat b = si2.tail;
                        if ( a == b ) continue;
                        si1.head = encode ( ii, jj, kk, index.c );
                        si1.tail = vertex.size();
                        si2 = tree.add ( si1 );
                        if ( si1.tail == si2.tail ) vertex.inc() = vert[index.c];
                        const nat c = si2.tail;
                        if ( b == c ) continue;
                        if ( c == a ) continue;
                        Trigon<void> & tri = facet.inc();
                        tri.a = a;
                        tri.b = b;
                        tri.c = c;
                    }
                }
            }
        }
        arr0.swap ( arr1 );
    }
    poly3gon.vertex = vertex;
    poly3gon.side = facet;
}

void recon ( IVertFunc & func )
{
    /*Vector3d vc;
    Cylinder temp ( Vector3d ( 1, 1, 0 ), Vector3d ( 1, 3, 0 ), 1 );
    //temp ( Vector3d ( 0, 2, 0 ), Vector3d ( 0, 0, 0 ), vc );
    temp ( Vector3d ( 1, 2, 0 ), Vector3d (-2, 2, 0 ), vc );
    return;*/
    Poly3gon<Vector3d, void> poly3gon;
    double t0 = timeInSec();
    recon ( func, poly3gon );
    double t1 = timeInSec();
    display << "vertex =" << poly3gon.vertex.size() << "facet =" << poly3gon.side.size() << t1 - t0 << NL;
    t0 = timeInSec();
    simplify ( poly3gon );
    t1 = timeInSec();
    display << "vertex =" << poly3gon.vertex.size() << "facet =" << poly3gon.side.size() << t1 - t0 << NL;
    Polyhedron poly;
    copy ( poly3gon, poly );
    PolyhedronErrorRecipient rec ( 1e-9, 1e-9 );
    check ( poly, rec );
//    draw ( poly, 0, 1, 1, 1, VM_WIRE );
    draw ( poly, 0, 1, 1, 1, VM_FLAT );
    if(0)
    {
        Poly3gon<Vector3f, Vector3f> poly2;
        copy ( poly, poly2 );
        saveSTLB ( RealFile ( "recon_sample.stl", "wb" ), 0, 0, poly2 );
    }
}

void recon()
{
    recon ( SceneFunc2() );
}

void add ( Polyhedron & poly1, const Polyhedron & poly2 )
{
    const nat nv1 = poly1.vertex.size();
    const nat nv2 = poly2.vertex.size();
    DynArray<Vector3d> vertex ( nv1 + nv2 );
    nat i;
    for ( i = 0; i < nv1; ++i ) vertex[i] = poly1.vertex[i];
    for ( i = 0; i < nv2; ++i ) vertex[i+nv1] = poly2.vertex[i];
    const nat nf1 = poly1.facet.size();
    const nat nf2 = poly2.facet.size();
    DynArray<Facet> facet ( nf1 + nf2 );
    for ( i = 0; i < nf1; ++i ) facet[i] = poly1.facet[i];
    for ( i = 0; i < nf2; ++i )
    {
        Facet & f = facet[i+nf1];
        f = poly2.facet[i];
        for ( nat j = 0; j < f.nv; ++j ) f.index[j] += nv1;
    }
    poly1.vertex.swap ( vertex );
    poly1.facet.swap ( facet );
    poly1.linkFacets();
}

void energia()
{//display << sqrt(_pow2(1800) - _pow2(816.5) ) << NL;
    nat i, j;
    Polyhedron poly1, poly2;
    if ( ! loadSTL ( "data/s1.stl", poly1 ) )
    {
        display << "no load file1" << NL;
        return;
    }
    poly1 *= 2;
    if ( ! loadSTL ( "data/s2.stl", poly2 ) )
    {
        display << "no load file2" << NL;
        return;
    }
    poly2 -= poly2.centerOfMass();
    poly2 *= Spin3d ( Vector3d ( 1,0,0 ), -M_PI/2 );
    poly2 += Vector3d ( 0, 0, 1800+15./2 );
//    ViewMode wm = VM_WIRE;
    ViewMode wm = VM_FLAT;
    Spin3d spin[64], spin0 ( Vector3d ( 0,0,1 ), 360/(RAD*8) );
    if(1)
    {
        spin[0] = Spin3d ( Vector3d ( 1,0,0 ), -86.29/RAD ) * spin0;
        spin[1] = Spin3d ( Vector3d ( 1,0,0 ),  7.474/RAD ) * spin[0];
        spin[2] = Spin3d ( Vector3d (-1,0,1 ),  -5.26/RAD ) * spin[0];
        spin[3] = Spin3d ( Vector3d ( 1,0,1 ),   5.26/RAD ) * spin[0];
        spin[4] = Spin3d ( Vector3d ( 1,0,0 ),  7.68/RAD ) * spin[1];
        spin[5] = Spin3d ( Vector3d (-1,0,1 ),  -5.38/RAD ) * spin[1];
        spin[6] = Spin3d ( Vector3d (-1,0,1 ),  -5.26/RAD ) * spin[2];
        spin[7] = Spin3d ( Vector3d ( 1,0,1 ),   5.38/RAD ) * spin[1];
        spin[8] = Spin3d ( Vector3d ( 1,0,1 ),   5.26/RAD ) * spin[3];
        spin[9] = Spin3d ( Vector3d (-1,0,1 ),  -5.34/RAD ) * spin[5];
        spin[10] = Spin3d ( Vector3d (-1,0,1 ),  -5.26/RAD ) * spin[6];
        spin[11] = Spin3d ( Vector3d ( 1,0,1 ),  5.34/RAD ) * spin[7];
        spin[12] = Spin3d ( Vector3d ( 1,0,1 ),  5.26/RAD ) * spin[8];
        spin[13] = Spin3d ( Vector3d (-1,0,1 ), -5.48/RAD ) * spin[4];
        spin[13] = Spin3d ( Vector3d ( 0,1,0 ),  0.2/RAD ) * spin[13];
        spin[14] = Spin3d ( Vector3d ( 1,0,1 ),  5.48/RAD ) * spin[4];
        spin[14] = Spin3d ( Vector3d ( 0,1,0 ), -0.2/RAD ) * spin[14];
        spin[ 4] = Spin3d ( Vector3d ( 1,0,0 ),  0.05/RAD ) * spin[4];
        spin[15] = Spin3d ( Vector3d ( 1,0,0 ),  7.65/RAD ) * spin[4];
    }
    for ( i = 1; i < 4; ++i )
    {
        Spin3d spinY ( Vector3d ( 0,1,0 ), 360*i/(RAD*4) );
        for ( j = 0; j < 16; ++j )
        {
            spin[j+16*i] = spinY * spin[j];
        }
    }
    /*for ( i = 0; i < 4; ++i )
    {
        for ( j = 0; j < 16; ++j )
        {
            Rectangle3d rect ( Vector3d ( 0, 0, 1815 ), 83, 83 );
            rect *= spin[j+16*i];
            rect *= 1./1800;
            if(j==25)
            draw ( rect, 0, 0, 1, 1, wm );
            else
            draw ( rect, 1, 1, 0, 1, wm );
        }
    }*
    for ( i = 0; i < 4; ++i )
    {
        for ( j = 0; j < 16; ++j )
        {
            Polyhedron poly;
            poly = poly2;
            poly *= spin[j+16*i];
            poly *= 1./1800;
            draw ( poly, 0, 0, 1, 1, wm );
        }
    }*/
    for ( i = 0; i < 4; ++i )
    {
        for ( j = 0; j < 16; ++j )
        {
            Polyhedron poly;
            poly = poly2;
            poly *= spin[j+16*i];
            add ( poly1, poly );
        }
    }
    saveSTLB ( "data/energy.stl", poly1 );
    poly1 *= 1./1800;
    draw ( poly1, 0, 1, 1, 1, VM_FLAT );
    if(0)
    {
        poly1 = poly2;
        poly2 *= Spin3d ( Vector3d ( 1,0,0 ), -5.3/RAD );
        double min = 100;
        for ( nat i = 0; i < poly1.vertex.size(); ++i )
        {
            double dist; 
            nat vi, ei, fi;
            distance ( poly2, poly1.vertex[i], dist, vi, ei, fi );
            if ( min > dist ) min = dist;
        }
        display << min << NL;
    }
    if(0)
    {
        Rectangle3d rect1 ( Vector3d ( 0, 0, 1815 ), 83, 83 );
        Rectangle3d rect2 = rect1;
        rect1 *= spin[5];
        rect2 *= spin[4];
        Vector3d vert[4];
        rect1.getVerts ( vert );
        for ( nat i = 0; i < 4; ++i )
            display << getDistance ( rect2, vert[i] ) << NL;
    }
//display << "end" << NL;
}

} // end of namespace

void recon_test ()
{
    drawNewList2d();
//    trian3dtest();
//    recon();
    energia();
//    testMarchingCubes2();
    endNewList();
}
