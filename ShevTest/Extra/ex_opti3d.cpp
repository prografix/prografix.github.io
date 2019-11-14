
#include <math.h>

#include "../Shev/func1t.h"
#include "../Shev/func3d.h"
#include "../Shev/opti3d.h"
#include "../Shev/Vector3d.h"
#include "../Shev/Vector4d.h"
#include "../Shev/WireModel.h"
#include "../Shev/Polyhedron.h"

//**************************** 03.05.2014 *********************************//
//
//       ћаксимальный многогранник вписанный в выпуклый многогранник 
//
//**************************** 03.05.2014 *********************************//

Def<Spin3d> maxPolyhedronInConvexPolyhedron ( CArrRef<Vector3d> vertex, CArrRef<Plane3d> plane, 
                                              WireModel<7> & model, List< Vertex<7> > & stor )
{
    Def<Spin3d> res;
    for ( nat k = 0; k < 100; ++k )
    {
// ѕоиск максимального решени€
        Vertex<7> * vert = model.vlist.top();
        if ( ! vert ) return res;
        double max = vert->coor.d0;
        while ( model.vlist.next() )
        {
            Vertex<7> * v = model.vlist.cur();
            const double & d = v->coor.d0;
            if ( max < d ) max = d, vert = v;
        }
        const Double<7> a = vert->coor;
// ѕоиск максимального нарушени€ ограничений дл€ выбранного решени€
        nat im, jm;
        max = 0.;
        for ( nat i = 0; i < vertex.size(); ++i )
        {
            const Vector3d & v = vertex[i];
            const Vector3d p ( a.d0 * v.x - a.d3 * v.y + a.d2 * v.z + a.d4,
                               a.d0 * v.y - a.d1 * v.z + a.d3 * v.x + a.d5,
                               a.d0 * v.z - a.d2 * v.x + a.d1 * v.y + a.d6 );
            for ( nat j = 0; j < plane.size(); ++j )
            {
                const double t = plane[j] % p;
                if ( max < t ) max = t, im = i, jm = j;
            }
        }
// ≈сли нарушение мало, то завершение программы
        if ( max < 1e-5 )
        {
            const double ro = sqrt ( 0.5 * ( a.d0 + sqrt ( a.d0 * a.d0 + ( a.d1 * a.d1 + a.d2 * a.d2 + a.d3 * a.d3 ) / 3 ) ) );
            const double la = 0.5 * a.d1 / ro;
            const double mu = 0.5 * a.d2 / ro;
            const double nu = 0.5 * a.d3 / ro;
            return Spin3d ( ro, la, mu, nu );
        }
// ѕрименение ограничени€ к области допустимых преобразований
        const Plane3d & p = plane[jm];
        const Vector3d & v = vertex[im];
        const Vector3d & n = p.norm;
        Double<8> cor;
        cor.d0 = n * v;
        cor.d1 = n.z * v.y - n.y * v.z;
        cor.d2 = n.x * v.z - n.z * v.x;
        cor.d3 = n.y * v.x - n.x * v.y;
        cor.d4 = n.x;
        cor.d5 = n.y;
        cor.d6 = n.z;
        cor.d7 = p.dist;
        model.cut ( cor, stor );
    }
    return res;
}

Def<Conform3d> maxPolyhedronInConvexPolyhedronNR ( const Vector4d & v0, double eps, CArrRef<Plane4d> plane );

Def<Conform3d> maxPolyhedronInConvexPolyhedronNR ( CArrRef<Vector3d> vertex, CArrRef<Plane3d> plane )
{
    DynArray<Plane4d> plane4 ( plane.size() );
    for ( nat i = 0; i < plane.size(); ++i )
    {
        const Plane3d & p = plane[i];
        double max = p.norm * vertex[0];
        for ( nat j = 1; j < vertex.size(); ++j )
        {
            double t = p.norm * vertex[j];
            if ( max < t ) max = t;
        }
        plane4[i].norm = Vector4d ( p.norm.x, p.norm.y, p.norm.z, max );
        plane4[i].dist = p.dist;
    }
    const Vector4d v0 ( 1, 1, 1, 1 );
    return maxPolyhedronInConvexPolyhedronNR ( v0, 1e-9, plane4 );
}

Def<Conform3d> maxPolyhedronInConvexPolyhedron0 ( const Polyhedron & inner, const Polyhedron & outer )
{
    Def<Conform3d> res;
    if ( inner.facet.size() < 4 || inner.vertex.size() < 4 ||
         outer.facet.size() < 4 || outer.vertex.size() < 4 ) return res;
    const Segment3d dim1 = dimensions ( inner.vertex );
    const Segment3d dim2 = dimensions ( outer.vertex );
    const double d1 = normU ( dim1 );
    const double d2 = normU ( dim2 );
    if ( ! d1 || ! d2 ) return res;
    const double coef1 = 2. / d1;
    const Conform3d conf1 ( Spin3d(), -0.5 * coef1 * ( dim1.a + dim1.b ), coef1 );
    const Similar3d sim1 ( conf1 );
    const double coef2 = 2. / d2;
    const Conform3d conf2 ( Spin3d(), -0.5 * coef2 * ( dim2.a + dim2.b ), coef2 );
    const Similar3d sim2 ( conf2 );
    nat i;
    DynArray<Vector3d> vertex ( inner.vertex.size() );
    for ( i = 0; i < vertex.size(); ++i )
    {
        vertex[i] = sim1 ( inner.vertex[i] );
    }
    DynArray<Plane3d> plane ( outer.facet.size() );
    for ( i = 0; i < plane.size(); ++i )
    {
        plane[i] = sim2 ( outer.facet[i].plane );
    }
    Spin3d spin;
    List< Vertex<7> > stor;
    WireModel<7> model, sample;
    DynArray< SortItem <const Vertex<7> *, Vertex<7> *> > buf;
    sample.simplex ( 2*(7+1), stor ).vlist -= Double<7>().fill(1);
    Double<8> cor;
    cor.fill ( 0 );
    cor.d0 = -1;
    const double a = 2;
    cor.d1 = -a;
    sample.cut ( cor, stor );
    cor.d1 = a;
    sample.cut ( cor, stor );
    cor.d1 = 0;
    cor.d2 = -a;
    sample.cut ( cor, stor );
    cor.d2 = a;
    sample.cut ( cor, stor );
    cor.d2 = 0;
    cor.d3 = -a;
    sample.cut ( cor, stor );
    cor.d3 = a;
    sample.cut ( cor, stor );
    Def<Spin3d> at;
    for ( i = 0; i < 20; ++i )
    {
        copy ( sample, model, stor, buf );
        at = maxPolyhedronInConvexPolyhedron ( vertex, plane, model, stor );
        if ( ! at.isDef ) return res;
        spin = at * spin;
        vertex *= Ortho3d ( at );
        if ( at.getAngle() < 1e-3 ) break;
    }
    res = maxPolyhedronInConvexPolyhedronNR ( vertex, plane );
    if ( ! res.isDef ) return res;
    return ~conf2 * res * Conform3d ( spin ) * conf1;
}



