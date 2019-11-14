
#include "math.h"
// Shev
#include "../shev/Vector2d.h"
#include "../shev/Vector3d.h"
#include "../shev/func1t.h"
#include "../shev/Polyhedron.h"
#include "Body2.h"

DynArrRef<Vector2d> & copy ( const Side & side, DynArrRef<Vector2d> & p )
{
    const double ax = fabs ( side.plane.norm.x );
    const double ay = fabs ( side.plane.norm.y );
    const double az = fabs ( side.plane.norm.z );
    Show<BonePtr> g ( side );
    p.resize ( side.size() );
    nat i = 0;

    if ( ax >= ay && ax >= az )
    {
        if ( side.plane.norm.x > 0 )
        {
            if ( g.top() )
            do
            {
                const BonePtr * bp = g.cur();
                const Bone * b = bp->bone;
                const Vector3d & t = & b->b0 == bp ? b->v0->point : b->v1->point;
                p[i].x = t.y;
                p[i].y = t.z;
                ++i;
            }
            while ( g.next() );
        }
        else
        {
            if ( g.top() )
            do
            {
                const BonePtr * bp = g.cur();
                const Bone * b = bp->bone;
                const Vector3d & t = & b->b0 == bp ? b->v0->point : b->v1->point;
                p[i].x = t.z;
                p[i].y = t.y;
                ++i;
            }
            while ( g.next() );
        }
    }
    else
    if ( ay >= ax && ay >= az )
    {
        if ( side.plane.norm.y > 0 )
        {
            if ( g.top() )
            do
            {
                const BonePtr * bp = g.cur();
                const Bone * b = bp->bone;
                const Vector3d & t = & b->b0 == bp ? b->v0->point : b->v1->point;
                p[i].x = t.z;
                p[i].y = t.x;
                ++i;
            }
            while ( g.next() );
        }
        else
        {
            if ( g.top() )
            do
            {
                const BonePtr * bp = g.cur();
                const Bone * b = bp->bone;
                const Vector3d & t = & b->b0 == bp ? b->v0->point : b->v1->point;
                p[i].x = t.x;
                p[i].y = t.z;
                ++i;
            }
            while ( g.next() );
        }
    }
    else
    {
        if ( side.plane.norm.z > 0 )
        {
            if ( g.top() )
            do
            {
                const BonePtr * bp = g.cur();
                const Bone * b = bp->bone;
                const Vector3d & t = & b->b0 == bp ? b->v0->point : b->v1->point;
                p[i].x = t.x;
                p[i].y = t.y;
                ++i;
            }
            while ( g.next() );
        }
        else
        {
            if ( g.top() )
            do
            {
                const BonePtr * bp = g.cur();
                const Bone * b = bp->bone;
                const Vector3d & t = & b->b0 == bp ? b->v0->point : b->v1->point;
                p[i].x = t.y;
                p[i].y = t.x;
                ++i;
            }
            while ( g.next() );
        }
    }
    return p;
}

///////////////////////////////////////////////////////////////////////////////

Body & copy ( const Polyhedron & poly, BodyStor & stor, Body & body )
{
    nat i;
    body.makeVoid ( stor );
    DynArray<Side *> sarr ( poly.facet.size() );
    DynArray<Vert *> varr ( poly.vertex.size() );

    body.verts.resize ( varr.size(), stor.verts );
    body.verts.top();
    for ( i = 0; i < poly.vertex.size(); ++i )
    {
        ( varr[i] = body.verts.cur() )->point = poly.vertex[i];
        body.verts.next();
    }

    body.sides.resize ( sarr.size(), stor.sides );
    body.sides.top();
    for ( i = 0; i < poly.facet.size(); ++i )
    {
        ( sarr[i] = body.sides.cur() )->plane = poly.facet[i].plane;
        body.sides.next();
    }

    for ( i = 0; i < poly.facet.size(); ++i )
    {
        const Facet & facet = poly.facet[i];
        sarr[i]->info = facet.info;
        for ( nat j = 0; j < facet.nv; ++j )
        {
            const nat g = facet.nv + 1 + j;
            const nat l = facet.index[g];
            const nat m = facet.index[g+facet.nv];
            if ( l >= poly.facet.size() || m >= poly.facet[l].nv )
            {
                return body.makeVoid ( stor );
            }
            if ( l > i || ( l == i && j < m ) )
            {
                Bone * b;
                if ( stor.sides.size() > 0 )
                {
                    stor.bones.movLasAftLas ( body.bones );
                    b = body.bones.las();
                    b->v0 = varr[facet.index[j]];
                    b->v1 = varr[facet.index[j+1]];
                    b->s0 = sarr[i];
                    b->s1 = sarr[l];
                }
                else
                {
                    b = new Bone ( varr[facet.index[j]], varr[facet.index[j+1]], sarr[i], sarr[l] );
                    body.bones.addAftCur ( b );
                }
                sarr[i]->addAftCur ( & b->b0 );
                sarr[l]->addAftCur ( & b->b1 );
                b->b0.info = j;
                b->b1.info = m;
            }
        }
    }

    for ( i = 0; i < sarr.size(); ++i ) sarr[i]->sort123();

    return body;
}

Body & copy ( const Polyhedron & poly, Body & body )
{
    return copy ( poly, BodyStor(), body );
}

Polyhedron & copy ( Body & body, Polyhedron & poly )
{
    poly.vertex.resize ( body.verts.size() );
    poly.facet .resize ( body.sides.size() );

    nat i;
    body.verts.top();
    for ( i = 0; i < poly.vertex.size(); ++i )
    {
        poly.vertex[i] = body.verts.cur()->point;
        body.verts.cur()->info = i;
        body.verts.next();
    }

    body.sides.top();
    for ( i = 0; i < poly.facet.size(); ++i )
    {
        Facet & facet = poly.facet[i];
        Side * s = body.sides.cur();
        facet.plane = s->plane;
        facet.info = s->info;
        if ( s->top() )
        {
            facet.resize ( s->size() );
            for ( nat j = 0; j < facet.nv; ++j )
            {
                facet.index[j] = s->vert0()->info;
                s->next();
            }
            facet.index[facet.nv] = facet.index[0];
        }
        body.sides.next ();
    }

    return poly.linkFacets();
}

typedef SortItem<const void *, nat> PtrNat;

bool copy ( const Body & body, Polyhedron & poly )
{
    Show<Vert> verts ( body.verts );
    Show<Side> sides ( body.sides );
    Polyhedron temp ( verts.size(), sides.size() );

    DynArray<PtrNat> pna ( verts.size() );
    nat i;
    verts.top();
    for ( i = 0; i < temp.vertex.size(); ++i )
    {
        temp.vertex[i] = verts.cur()->point;
        pna[i].head = verts.cur();
        pna[i].tail = i;
        verts.next();
    }
    quickSort123 ( pna );

    sides.top();
    for ( i = 0; i < temp.facet.size(); ++i )
    {
        Facet & facet = temp.facet[i];
        const Side * s = sides.cur();
        facet.plane = s->plane;
        facet.info = s->info;
        Show<BonePtr> side ( *s );
        if ( side.top() )
        {
            facet.resize ( side.size() );
            for ( nat j = 0; j < facet.nv; ++j )
            {
                const nat k = lasEqu123 ( pna, PtrNat ( side.cur()->vert0() ) );
                if ( k >= pna.size() ) return false;
                facet.index[j] = pna[k].tail;
                side.next();
            }
        }
        sides.next ();
    }

    temp.linkFacets();
    _swap ( temp, poly );
    return true;
}
