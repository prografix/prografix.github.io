
#include "math.h"

#include "../Shev/tune.h"
#include "../Shev/trian3d.h"
#include "../draw.h"

#include "display.h"

double timeInSec();

namespace
{

void trian_test()
{
    const nat n = 114;
    FixArray<Vector3d, n> vert;
    CmbArray<Set3<nat>, n-2> res;
    nat i;
    for ( i = 0; i < n; ++i )
    {
        const double a = i * M_2PI / n;
        Vector3d & v = vert[i];
        v.x = sin(a);
        v.y = cos(a);
        v.z = v.x * v.x - 0.5;
    }
    trian ( vert, res );
    for ( i = 0; i < res.size(); ++i )
    {
        const Set3<nat> & t = res[i];
        draw ( Segment3d ( vert[t.a], vert[t.b] ), 0, 1, 1 );
        draw ( Segment3d ( vert[t.b], vert[t.c] ), 0, 1, 1 );
        draw ( Segment3d ( vert[t.c], vert[t.a] ), 0, 1, 1 );
    }
}

void trian2_test()
{
    const nat n = 55;
    FixArray<Vector3d, n> vert1, vert2;
    CmbArray<Set3<nat>, n+n-2> res;
    nat i;
    for ( i = 0; i < n; ++i )
    {
        const double a = i * M_2PI / ( n - 1 );
        Vector3d & v1 = vert1[i];
        v1.x = ( a - M_PI ) / 3;
        v1.y = 2*(0.5 - sin(0.5*a));
        v1.z = -0.6;
        Vector3d & v2 = vert2[i];
        v2.x = 0.5*v1.x;
        v2.y = 0.5*v1.y;
        v2.z = -v1.z;
    }
    trian ( vert1, vert2, false, res );
    for ( i = 0; i < res.size(); ++i )
    {
        const Set3<nat> & t = res[i];
        const Vector3d & va = t.a >= n ? vert2[t.a-n] : vert1[t.a];
        const Vector3d & vb = t.b >= n ? vert2[t.b-n] : vert1[t.b];
        const Vector3d & vc = t.c >= n ? vert2[t.c-n] : vert1[t.c];
        draw ( Segment3d ( va, vb ), 0, 1, 1 );
        draw ( Segment3d ( vb, vc ), 0, 1, 1 );
        draw ( Segment3d ( vc, va ), 0, 1, 1 );
    }
    for ( i = 1; i < n; ++i )
    {
        draw ( Segment3d ( vert1[i], vert1[i-1] ), 1, 1, 0 );
        draw ( Segment3d ( vert2[i], vert2[i-1] ), 1, 0, 0 );
    }
}

} // namespace

void trian3d_test ()
{
    drawNewList2d();
    trian2_test();
    endNewList ();
}